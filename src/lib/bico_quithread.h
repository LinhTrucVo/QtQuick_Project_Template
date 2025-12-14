#ifndef BICO_QUITHREAD_H
#define BICO_QUITHREAD_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QDirIterator>
#include <QMetaObject>
#include <functional>

#include "bico_qthread.h"

class Bico_QUIThread;

/**
 * @brief Helper class that lives in the main thread to create new threads safely.
 */
class ThreadFactory : public QObject
{
    Q_OBJECT

public:
    explicit ThreadFactory(QObject* parent = nullptr);
    
    Bico_QUIThread* created_thread;
    
    // Pending parameters to avoid Q_ARG type issues
    struct PendingParams {
        Bico_DataQueue* qin;
        uint8_t qin_owner;
        Bico_DataQueue* qout;
        uint8_t qout_owner;
        QString obj_name;
        QString ui_path;
        QObject* parent;
        std::function<Bico_QUIThread*(Bico_DataQueue*, uint8_t, Bico_DataQueue*, uint8_t, QString, QString, QObject*)> creator;
    };
    
    PendingParams* pending_params;

public slots:
    void createThread();
};

/**
 * @brief Helper class that lives in the main thread to load QML engines safely.
 */
class EngineFactory : public QObject
{
    Q_OBJECT

public:
    explicit EngineFactory(QObject* parent = nullptr);
    
    // Pending parameters for engine loading
    struct PendingParams {
        Bico_QUIThread* thread;
        QString ui_path;
        QThread::Priority priority;
    };
    
    PendingParams* pending_params;

public slots:
    void loadEngine();
};


class Bico_QUIThread : public QThread, public Bico_QThread
{
    Q_OBJECT
    friend class ThreadFactory;
    friend class EngineFactory;

public:
    Bico_QUIThread
        (
            Bico_DataQueue *qin = nullptr,
            uint8_t qin_owner = 0,
            Bico_DataQueue *qout = nullptr,
            uint8_t qout_owner = 0,
            QString obj_name = "",
            QString ui_path = "",
            QObject *parent = nullptr
        );
    ~Bico_QUIThread();

    void start(QThread::Priority priority = InheritPriority);
    void run();

    // This method is implemented by subclass
    virtual uint8_t MainTask() = 0;

    QObject* getRootFirstObj();
    QQmlApplicationEngine* getEngine();


public:
    template<typename T>
    static T* create
    (
            Bico_DataQueue *qin = nullptr,
            uint8_t qin_owner = 0,
            Bico_DataQueue *qout = nullptr,
            uint8_t qout_owner = 0,
            QString obj_name = "",
            QString ui_path = "",
            QObject *parent = nullptr
    )
    {
        if (thread_hash.find(obj_name) == thread_hash.end())
        {
            // Check if we're in the main thread
            QThread* main_thread = main_app->thread();
            QThread* current_thread = QThread::currentThread();
            
            if (current_thread == main_thread)
            {
                // We're in the main thread, create directly
                T* thread = new T(qin, qin_owner, qout, qout_owner, obj_name, ui_path, parent);
                return thread;
            }
            else
            {
                // We're in a worker thread, use factory to create in main thread
                thread_factory->created_thread = nullptr;
                thread_factory->pending_params = new ThreadFactory::PendingParams{
                    qin, qin_owner, qout, qout_owner, obj_name, ui_path, parent,
                    [](Bico_DataQueue* qin, uint8_t qin_owner, Bico_DataQueue* qout, uint8_t qout_owner, 
                       QString obj_name, QString ui_path, QObject* parent) -> Bico_QUIThread* {
                        return new T(qin, qin_owner, qout, qout_owner, obj_name, ui_path, parent);
                    }
                };
                
                // Check if the factory is in the same thread (would cause deadlock with BlockingQueued)
                QThread* factory_thread = thread_factory->thread();
                Qt::ConnectionType connection_type = (current_thread == factory_thread) 
                    ? Qt::DirectConnection 
                    : Qt::BlockingQueuedConnection;
                
                // Use BlockingQueuedConnection to wait for the thread to be created
                QMetaObject::invokeMethod(
                    thread_factory,
                    "createThread",
                    connection_type
                );
                
                T* result = static_cast<T*>(thread_factory->created_thread);
                return result;
            }
        }
        else
        {
            return nullptr;
        }
    }

    static void remove(QString thread_name)
    {
        if (thread_hash.contains(thread_name))
        {
            Bico_QMessData mess_data("terminate", "");
            getThreadHash().value(thread_name)->qinEnqueue(mess_data);
        }
    }

    static QHash<QString, Bico_QUIThread *> getThreadHash();
    static QGuiApplication* getMainApp();
    static void setMainApp(QGuiApplication* app);
    static void initializeFactories();  // Must be called from main thread
    static ThreadFactory* getThreadFactory();
    static EngineFactory* getEngineFactory();

public slots:
    static void selfRemove(QString obj_name);

protected:
    QString _ui_path;
    QQmlApplicationEngine* _engine;

    static QHash<QString, Bico_QUIThread*> thread_hash;
    static QMutex thread_hash_mutex;
    static QGuiApplication* main_app;
    static ThreadFactory* thread_factory;
    static EngineFactory* engine_factory;
    static QStringList qml_import_paths;  // Cached QML import paths from QRC



signals:
    void toUI(QString mess, QVariant data);

public slots:
    void fromUI(QString mess, QVariant data);
};

#endif // BICO_QUITHREAD_H
