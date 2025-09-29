#ifndef BICO_QUITHREAD_H
#define BICO_QUITHREAD_H

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QThread>
#include <QDirIterator>

#include "bico_qthread.h"


class Bico_QUIThread : public QThread, public Bico_QThread
{
    Q_OBJECT

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


public:
    template<typename T>
    static T* create
    (
            Bico_DataQueue *qin = nullptr,
            uint8_t qin_owner = 0,
            Bico_DataQueue *qout = nullptr,
            uint8_t qout_owner = 0,
            QString obj_name = "",
            QString ui_path = ""
    )
    {
        if (thread_hash.find(obj_name) == thread_hash.end())
        {
            T* thread = new T(qin, qin_owner, qout, qout_owner, obj_name, ui_path);
            return thread;
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

protected:
    QString _ui_path;
    QQmlApplicationEngine _engine;

    static QHash<QString, Bico_QUIThread*> thread_hash;
    static QMutex thread_hash_mutex;
    static QGuiApplication* main_app;



signals:
    void toUI(QString mess, QVariant data);

public slots:
    void fromUI(QString mess, QVariant data);
};

#endif // BICO_QUITHREAD_H
