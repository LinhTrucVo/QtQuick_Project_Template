#include "Task1.h"
#include <QRandomGenerator>

// Initialize static member
int Task1::count = 0;

Task1::Task1
(
    Bico_DataQueue *qin,
    uint8_t qin_owner,
    Bico_DataQueue *qout,
    uint8_t qout_owner,
    QString obj_name,
    QString ui_path,
    QObject *parent
) : Bico_QUIThread(qin, qin_owner, qout, qout_owner, obj_name, ui_path, parent)
{
    // Initialize message handlers
    message_handlers["terminate"] = &Task1::_handle_terminate;
    message_handlers["num1"] = &Task1::_handle_num1;
    message_handlers["num2"] = &Task1::_handle_num2;
    message_handlers["text"] = &Task1::_handle_text;
    message_handlers["create"] = &Task1::_handle_create;
    message_handlers["create_child"] = &Task1::_handle_create_child;
    message_handlers["size"] = &Task1::_handle_size;
    message_handlers["from_another_thread"] = &Task1::_handle_from_another_thread;
}

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

        // Use the message handler map
        auto handler_it = message_handlers.find(mess);
        if (handler_it != message_handlers.end())
        {
            continue_to_run = (this->*handler_it.value())(data, input);
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

uint8_t Task1::_handle_terminate(QVariant& data, Bico_QMessData& input)
{
    cleanupChildren();
    return 0;  // Signal to stop running
}

uint8_t Task1::_handle_num1(QVariant& data, Bico_QMessData& input)
{
    qDebug() << objectName() << "num1" << data.value<int>() << "\t" << ex_data_obj.getData_1();
    return 1;
}

uint8_t Task1::_handle_num2(QVariant& data, Bico_QMessData& input)
{
    qDebug() << objectName() << "num2" << data.value<int>() << "\t" << ex_data_obj.getData_2();
    return 1;
}

uint8_t Task1::_handle_text(QVariant& data, Bico_QMessData& input)
{
    qDebug() << objectName() << "text" << data.value<QString>();
    emit toUI("text", data);
    return 1;
}

uint8_t Task1::_handle_create(QVariant& data, Bico_QMessData& input)
{
    qDebug() << objectName() << "create" << data.value<QString>();
    QString thread_name = "task_" + QString::number(++count);
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
    return 1;
}

uint8_t Task1::_handle_create_child(QVariant& data, Bico_QMessData& input)
{
    qDebug() << objectName() << "create_child" << data.value<QString>();
    QString thread_name = "task_" + QString::number(++count);
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
    return 1;
}

uint8_t Task1::_handle_size(QVariant& data, Bico_QMessData& input)
{
    qDebug() << objectName() << "size" << data.value<QSizeF>();
    emit toUI("size", data);
    return 1;
}

uint8_t Task1::_handle_from_another_thread(QVariant& data, Bico_QMessData& input)
{
    qDebug() << objectName() << "from_another_thread: " << input.src() << data.value<int>();
    return 1;
}

