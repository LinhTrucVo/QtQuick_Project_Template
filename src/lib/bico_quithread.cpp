#include "bico_quithread.h"

// Avoid runntime issue: "QMetaMethod::invoke: Unable to handle unregistered datatype 'QThread::Priority'" 
// (this issue happen when testing on linux)
// Register QThread::Priority for QMetaObject::invokeMethod
Q_DECLARE_METATYPE(QThread::Priority)

namespace {
    struct MetaTypeRegistration {
        MetaTypeRegistration() {
            qRegisterMetaType<QThread::Priority>("QThread::Priority");
        }
    };
    static MetaTypeRegistration registration;
}

// ThreadFactory implementation
ThreadFactory::ThreadFactory(QObject* parent)
    : QObject(parent), created_thread(nullptr), pending_params(nullptr)
{
}

void ThreadFactory::createThread()
{
    if (pending_params == nullptr)
        return;
    
    // Create new thread directly (we're already in main thread)
    created_thread = pending_params->creator(
        pending_params->qin,
        pending_params->qin_owner,
        pending_params->qout,
        pending_params->qout_owner,
        pending_params->obj_name,
        pending_params->ui_path,
        pending_params->parent
    );
    
    // Clear pending params
    delete pending_params;
    pending_params = nullptr;
}

// EngineFactory implementation
EngineFactory::EngineFactory(QObject* parent)
    : QObject(parent), pending_params(nullptr)
{
}

void EngineFactory::loadEngine()
{
    if (pending_params == nullptr)
        return;
    
    Bico_QUIThread* thread = pending_params->thread;
    if (thread == nullptr)
    {
        delete pending_params;
        pending_params = nullptr;
        return;
    }
    
    // Create the QML engine
    thread->_engine = new QQmlApplicationEngine();
    
    // Use cached import paths instead of iterating QRC every time
    for (const QString& path : Bico_QUIThread::qml_import_paths) {
        thread->_engine->addImportPath(path);
    }
    
    // Load the QML file (but window visibility is controlled by QML visible property)
    thread->_engine->load(pending_params->ui_path);

    QObject* root_object = thread->getRootFirstObj();
    if (root_object)
    {
        // Connect signals between thread and UI
        QObject::connect(thread, SIGNAL(toUI(QString,QVariant)), root_object, SIGNAL(fromThread(QString,QVariant)));
        QObject::connect(root_object, SIGNAL(toThread(QString,QVariant)), thread, SLOT(fromUI(QString,QVariant)));
    }
    
    // Start the thread with the specified priority
    thread->QThread::start(pending_params->priority);
    
    // Clear pending params
    delete pending_params;
    pending_params = nullptr;
}

// Static member initialization
QHash<QString, Bico_QUIThread*> Bico_QUIThread::thread_hash;
QMutex Bico_QUIThread::thread_hash_mutex;
QGuiApplication* Bico_QUIThread::main_app = nullptr;
ThreadFactory* Bico_QUIThread::thread_factory = nullptr;
EngineFactory* Bico_QUIThread::engine_factory = nullptr;
QStringList Bico_QUIThread::qml_import_paths;  // Cached import paths

Bico_QUIThread::Bico_QUIThread
(
    Bico_DataQueue *qin,
    uint8_t qin_owner,
    Bico_DataQueue *qout,
    uint8_t qout_owner,
    QString obj_name,
    QString ui_path,
    QObject *parent
) : Bico_QThread(qin, qin_owner, qout, qout_owner), QThread(parent)
{    
    setObjectName(obj_name);
    
    thread_hash_mutex.lock();
    thread_hash.insert(obj_name, this);
    thread_hash_mutex.unlock();

    // Connect finished signal to selfRemove slot for automatic cleanup
    QObject::connect(this, &QThread::finished, [obj_name]() {
        Bico_QUIThread::selfRemove(obj_name);
    });

    _ui_path = ui_path;
    _engine = nullptr;
}

Bico_QUIThread::~Bico_QUIThread()
{
    // Destructor is now minimal - cleanup is handled by selfRemove slot
    // which is called when the 'finished' signal is emitted
}

void Bico_QUIThread::start(QThread::Priority priority)
{
    if (_ui_path != "")
    {
        // Can not create QQmlApplicationEngine directly if this "start" func
        // is called by worker Thread (not main thread), because
        // QQmlApplicationEngine must be created in main thread.
        // That's why using EngineFactory to handle engine creation in main thread.

        // Check if we're in the main thread
        QThread* main_thread = main_app->thread();
        QThread* current_thread = QThread::currentThread();
        
        if (current_thread == main_thread)
        {
            // We're in the main thread, create engine directly
            _engine = new QQmlApplicationEngine();
            
            // Use cached import paths instead of iterating QRC every time
            for (const QString& path : qml_import_paths) {
                _engine->addImportPath(path);
            }
            
            // Load the QML file (but window visibility is controlled by QML visible property)
            _engine->load(_ui_path);

            QObject* root_object = getRootFirstObj();
            if (root_object)
            {
                // Connect signals between thread and UI
                QObject::connect(this, SIGNAL(toUI(QString,QVariant)), root_object, SIGNAL(fromThread(QString,QVariant)));
                QObject::connect(root_object, SIGNAL(toThread(QString,QVariant)), this, SLOT(fromUI(QString,QVariant)));
            }
            
            // Start the thread with the specified priority
            QThread::start(priority);
        }
        else
        {
            // We're in a worker thread, must use factory to create engine in main thread
            engine_factory->pending_params = new EngineFactory::PendingParams{
                this,
                _ui_path,
                priority
            };
            // Use DirectConnection if already in main thread, else QueuedConnection
            Qt::ConnectionType connection_type = (current_thread == main_thread)
                ? Qt::DirectConnection
                : Qt::QueuedConnection;
            QMetaObject::invokeMethod(
                engine_factory,
                "loadEngine",
                connection_type
            );
        }
    }
    else
    {
        QThread::start(priority);
    }
}


void Bico_QUIThread::run()
{
    do
    {

    } while (MainTask());

    // No deleteLater() here - cleanup is handled by selfRemove() 
    // which is called when the 'finished' signal is emitted
}


QObject *Bico_QUIThread::getRootFirstObj()
{
    if (_engine && !_engine->rootObjects().isEmpty())
    {
        return _engine->rootObjects().at(0);
    }
    return nullptr;
}

QQmlApplicationEngine *Bico_QUIThread::getEngine()
{
    return _engine;
}


QHash<QString, Bico_QUIThread *> Bico_QUIThread::getThreadHash()
{
    return thread_hash;
}

QGuiApplication *Bico_QUIThread::getMainApp()
{
    return main_app;
}

void Bico_QUIThread::setMainApp(QGuiApplication* app)
{
    main_app = app;
}

void Bico_QUIThread::initializeFactories()
{
    // Initialize global factory instances (they live in main thread)
    // This MUST be called from the main thread before any worker threads are created
    if (thread_factory == nullptr)
    {
        thread_factory = new ThreadFactory();
    }
    if (engine_factory == nullptr)
    {
        engine_factory = new EngineFactory();
    }
    
    // Cache QML import paths from QRC (only scan once at startup)
    // without this cache, every thread creation would scan QRC which is inefficient
    // and we can see the delay on every time a thread's qml engine is created
    if (qml_import_paths.isEmpty())
    {
        QDirIterator qrc(":", QDirIterator::Subdirectories);
        while (qrc.hasNext()) {
            qml_import_paths.append(qrc.next());
        }
    }
}

ThreadFactory* Bico_QUIThread::getThreadFactory()
{
    return thread_factory;
}

EngineFactory* Bico_QUIThread::getEngineFactory()
{
    return engine_factory;
}


void Bico_QUIThread::fromUI(QString mess, QVariant data)
{
    Bico_QMessData mess_data;
    mess_data.setMess(mess);
    mess_data.setData(data);

    qinEnqueue(mess_data);
}

void Bico_QUIThread::selfRemove(QString obj_name)
{
    Bico_QUIThread* thread = thread_hash.value(obj_name);
    if (thread == nullptr)
        return;
    
    // Clean up engine connection
    if (thread->_ui_path != "" && thread->_engine != nullptr)
    {
        QObject* root_object = thread->getRootFirstObj();
        if (root_object)
        {
            QObject::disconnect(thread, SIGNAL(toUI(QString,QVariant)), root_object, SIGNAL(fromThread(QString,QVariant)));
            QObject::disconnect(root_object, SIGNAL(toThread(QString,QVariant)), thread, SLOT(fromUI(QString,QVariant)));
        }
        
        // SAFE: Schedule deletion in the main thread (where _engine was created)
        // The engine will be deleted by Qt's event loop in the main thread
        // All root QML objects will be automatically deleted as children of the engine
        thread->_engine->deleteLater();
        thread->_engine = nullptr;  // Clear our pointer after scheduling deletion
    }
    
    // Remove from hash and schedule for deletion
    thread_hash_mutex.lock();
    thread_hash.remove(obj_name);
    thread_hash_mutex.unlock();
    
    thread->deleteLater();
    
    // Exit app if no more threads
    if (thread_hash.count() < 1)
    {
        if (main_app != nullptr)
        {
            main_app->exit(0);
        }
    }
}
