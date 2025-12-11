#include "bico_quithread.h"

// Global engine loader instance
EngineLoader* engine_loader = new EngineLoader();

QHash<QString, Bico_QUIThread*> Bico_QUIThread::thread_hash;
QMutex Bico_QUIThread::thread_hash_mutex;
QGuiApplication* Bico_QUIThread::main_app = nullptr;

void EngineLoader::createEngine(QObject* thread_obj, QString ui_path, QThread::Priority priority)
{
    Bico_QUIThread* thread = qobject_cast<Bico_QUIThread*>(thread_obj);
    if (thread)
    {
        thread->_engine = new QQmlApplicationEngine();
        
        QDirIterator qrc(":", QDirIterator::Subdirectories);
        while (qrc.hasNext()) {
            thread->_engine->addImportPath(qrc.next());
        }
        thread->_engine->load(ui_path);

        QObject* root_object = thread->getRootFirstObj();
        if (root_object)
        {
            QObject::connect(thread, SIGNAL(toUI(QString,QVariant)), root_object, SIGNAL(fromThread(QString,QVariant)));
            QObject::connect(root_object, SIGNAL(toThread(QString,QVariant)), thread, SLOT(fromUI(QString,QVariant)));
        }

        // Call QThread::start directly to bypass the overridden start() method
        thread->QThread::start(priority);
    }
}

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
    // Move this thread object to the main thread for proper signal handling
    if (main_app != nullptr)
    {
        moveToThread(main_app->thread());
    }

    // Set parent after moveToThread to ensure proper thread affinity
    // Otherwise, the parent-child relationship is not correctly established in mainapp_context
    // -> in that case, parent will have no children
    if (parent != nullptr)
    {
        setParent(parent);
    }
    
    setObjectName(obj_name);
    thread_hash_mutex.lock();
    thread_hash.insert(obj_name, this);
    thread_hash_mutex.unlock();

    _ui_path = ui_path;
    _engine = nullptr;
}

Bico_QUIThread::~Bico_QUIThread()
{
    if (_ui_path != "" && _engine != nullptr)
    {
        QObject* root_object = getRootFirstObj();
        if (root_object)
        {
            QObject::disconnect(this, SIGNAL(toUI(QString,QVariant)), root_object, SIGNAL(fromThread(QString,QVariant)));
            QObject::disconnect(root_object, SIGNAL(toThread(QString,QVariant)), this, SLOT(fromUI(QString,QVariant)));
        }
        delete _engine;
        _engine = nullptr;
    }

    thread_hash_mutex.lock();
    thread_hash.remove(this->objectName());
    thread_hash_mutex.unlock();

    if (thread_hash.count() < 1)
    {
        if (main_app != nullptr)
        {
            main_app->exit(0);
        }
    }
}

void Bico_QUIThread::start(QThread::Priority priority)
{
    if (_ui_path != "")
    {
        // Can not create QQmlApplicationEngine directly if this "start" func
        // is called by worker Thread (not main thread), because
        // QQmlApplicationEngine must be created in main thread.
        // That's why using QMetaObject::invokeMethod to call createEngine
        // which is a slot in EngineLoader object living in main thread (globally).
        QMetaObject::invokeMethod(
            engine_loader,              // object in main thread
            "createEngine",             // method to call (must be slot or invokable)
            Qt::QueuedConnection,       // QueuedConnection to avoid freezing
            Q_ARG(QObject*, this),      // pass thread object
            Q_ARG(QString, _ui_path),   // pass UI path
            Q_ARG(QThread::Priority, priority)  // pass priority enum
        );
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

    deleteLater();
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


void Bico_QUIThread::fromUI(QString mess, QVariant data)
{
    Bico_QMessData mess_data;
    mess_data.setMess(mess);
    mess_data.setData(data);

    qinEnqueue(mess_data);
}
