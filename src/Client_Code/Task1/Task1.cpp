#include "Task1.h"
#include <QRandomGenerator>


void Task1::cleanupChildren()
{
    // Get all children from this->children() and terminate them
    QObjectList child_list = this->children();
    for (QObject* child : child_list)
    {
        Bico_QUIThread* thread = qobject_cast<Bico_QUIThread*>(child);
        if (thread != nullptr)
        {
            QString child_name = thread->objectName();
            Bico_QMessData mess_data("terminate", "");
            thread->qinEnqueue(mess_data);
            qDebug() << "Sent terminate message to child thread:" << child_name;
            
            // Wait for child thread to finish, otherwise, the app will be failed and terminated
            if (thread->isRunning())
            {
                thread->wait(5000); // Wait up to 5 seconds
            }
        }
    }
}

uint8_t Task1::MainTask()
{
    // continue_to_run is used to terminate the thread by reset it to 0
    uint8_t continue_to_run = 1;

    Bico_QMessData input;
    if (qinDequeue(input))
    {
        QString mess = input.mess();
        QVariant data = input.data();
        // Received message is handled here - begin -------------------------------------------------------------------------
        if (mess == QString("terminate"))
        {
            continue_to_run = 0;
            cleanupChildren();
        }
        else if (mess == QString("num1"))
        {
            qDebug() << objectName() << mess << data.value<int>() << "\t" << ex_data_obj.getData_1();
        }
        else if (mess == QString("num2"))
        {
            qDebug() << objectName() << mess << data.value<int>() << "\t" << ex_data_obj.getData_2();
        }
        else if (mess == QString("text"))
        {
            qDebug() << objectName() << mess << data.value<QString>();
            emit toUI(mess, data);
        }
        else if (mess == QString("create"))
        {
            qDebug() << objectName() << mess << data.value<QString>();
            QString random_id = QString::number(QRandomGenerator::global()->bounded(1000000));
            QString thread_name = "task_" + random_id;
            Bico_QUIThread::create<Task1>
                    (
                        new Bico_DataQueue,
                        1,
                        new Bico_DataQueue,
                        1,
                        thread_name,
                        "qrc:/Client_Code/Task1/UI/Task1Content/App.qml"
                    );
            getThreadHash().value(thread_name)->start();
        }
        else if (mess == QString("create_child"))
        {
            qDebug() << objectName() << mess << data.value<QString>();
            QString random_id = QString::number(QRandomGenerator::global()->bounded(1000000));
            QString thread_name = "task_" + random_id;
            Bico_QUIThread::create<Task1>
                    (
                        new Bico_DataQueue,
                        1,
                        new Bico_DataQueue,
                        1,
                        thread_name,
                        "qrc:/Client_Code/Task1/UI/Task1Content/App.qml",
                        getThreadHash().value(this->objectName())  // set parent
                    );
            getThreadHash().value(thread_name)->start();
        }
        else if (mess == QString("size"))
        {
            qDebug() << objectName() << mess << data.value<QSizeF>();
            emit toUI(mess, data);
        }
        else if (mess == QString("from_another_thread"))
        {
            qDebug() << objectName() << mess << ": "  << input.src() << data.value<int>();
        }
    }


    // Default work - begin --------------------------------------------------------------
    qDebug() << QString("Hello from ") + objectName();
    qDebug() << "Num of running thread: " << getThreadHash().count();
    msleep(100);

    if ((objectName() == "task_1") && (getThreadHash().value("task_0") != nullptr))
    {
        static int i = 0;
        Bico_QMessData mess_data("from_another_thread", i++);
        mess_data.setSrc(objectName());
        getThreadHash().value("task_0")->qinEnqueue(mess_data);
        msleep(236);
    }

    qDebug() << Qt::endl;
    // Default work - end--------------------------------------------------------------

    // This block of code is to test the remove method
    // Test the remove method - begin --------------------------------------------------------------
    // if (objectName() == "task_0")
    // {
    //     Bico_QUIThread::remove(objectName());
    // }
    // Test the remove method - end --------------------------------------------------------------

    return continue_to_run;
}

