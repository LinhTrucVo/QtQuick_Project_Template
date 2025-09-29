#include "bico_quithread.h"

QHash<QString, Bico_QUIThread*> Bico_QUIThread::thread_hash;
QMutex Bico_QUIThread::thread_hash_mutex;
QGuiApplication* Bico_QUIThread::main_app = nullptr;

Bico_QUIThread::Bico_QUIThread
(
    Bico_DataQueue *qin,
    uint8_t qin_owner,
    Bico_DataQueue *qout,
    uint8_t qout_owner,
    QString obj_name,
    QString ui_path,
    QObject *parent
) : QThread(parent), Bico_QThread(qin, qin_owner, qout, qout_owner)
{
    setObjectName(obj_name);
    thread_hash_mutex.lock();
    thread_hash.insert(obj_name, this);
    thread_hash_mutex.unlock();

    _ui_path = ui_path;
}

Bico_QUIThread::~Bico_QUIThread()
{
    if (_ui_path != "")
    {
        QObject* root_object = getRootFirstObj();
        QObject::disconnect(this, SIGNAL(toUI(QString,QVariant)), root_object, SIGNAL(fromThread(QString,QVariant)));
        QObject::disconnect(root_object, SIGNAL(toThread(QString,QVariant)), this, SLOT(fromUI(QString,QVariant)));
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
        QDirIterator qrc(":", QDirIterator::Subdirectories);
        while (qrc.hasNext()) {
            _engine.addImportPath(qrc.next());
        }
        _engine.load(_ui_path);

        QObject* root_object = getRootFirstObj();
        QObject::connect(this, SIGNAL(toUI(QString,QVariant)), root_object, SIGNAL(fromThread(QString,QVariant)));
        QObject::connect(root_object, SIGNAL(toThread(QString,QVariant)), this, SLOT(fromUI(QString,QVariant)));
    }
    QThread::start(priority);
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
    return _engine.rootObjects().at(0);
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
