#include "Task_1.h"

uint8_t Task_1::MainTask()
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

