#include <QGuiApplication>
#include <QDebug>
#include <iostream>
#include "Client_Code/Bico_QUIThread_Sample/Bico_QUIThread_Sample.h"
#include "Client_Code/Task_1/Task_1.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    Bico_QUIThread::setMainApp(&app);

    Task_1* ui0 = Bico_QUIThread::create<Task_1>
            (
                new Bico_DataQueue,
                1,
                new Bico_DataQueue,
                1,
                "task_0",
                "qrc:/Client_Code/Task_1/UI/Task_1Content/App.qml"
            );
    ui0->start();

// -----------------------------------------------------------------------------------------------------------------------------------------------
    // Bico_QUIThread_Sample* ui0 = Bico_QUIThread::create<Bico_QUIThread_Sample>
    //         (
    //             new Bico_DataQueue,
    //             1,
    //             new Bico_DataQueue,
    //             1,
    //             "task_0",
    //             "qrc:/Client_Code/Bico_QUIThread_Sample/Bico_QUIThread_Sample.qml"
    //         );
    // ui0->start();


    // Bico_QUIThread_Sample* ui1 = Bico_QUIThread::create<Bico_QUIThread_Sample>
    //         (
    //             new Bico_DataQueue,
    //             1,
    //             new Bico_DataQueue,
    //             1,
    //             "task_1",
    //             "qrc:/Client_Code/Bico_QUIThread_Sample/Bico_QUIThread_Sample.qml"
    //         );
    // ui1->start();

// -----------------------------------------------------------------------------------------------------------------------------------------------

// // -----------------------------------------------------------------------------------------------------------------------------------------------
//     Task_1* ui0 = Bico_QUIThread::create<Task_1>
//         (
//             new Bico_DataQueue,
//             1,
//             new Bico_DataQueue,
//             1,
//             "task_0",
//             "qrc:/Client_Code/Task_1/Task_1.qml"
//             );
//     ui0->start();


//     Task_1* ui1 = Bico_QUIThread::create<Task_1>
//         (
//             new Bico_DataQueue,
//             1,
//             new Bico_DataQueue,
//             1,
//             "task_1",
//             "qrc:/Client_Code/Task_1/Task_1.qml"
//             );
//     ui1->start();

// // -----------------------------------------------------------------------------------------------------------------------------------------------


// -----------------------------------------------------------------------------------------------------------------------------------------------
//    Bico_QUIThread_Sample* ui0 = Bico_QUIThread::create<Bico_QUIThread_Sample>
//            (
//                new Bico_DataQueue,
//                1,
//                new Bico_DataQueue,
//                1,
//                "task_0"
//            );
//    ui0->start();


//    Bico_QUIThread_Sample* ui1 = Bico_QUIThread::create<Bico_QUIThread_Sample>
//            (
//                new Bico_DataQueue,
//                1,
//                new Bico_DataQueue,
//                1,
//                "task_1"
//            );
//    ui1->start();
// -----------------------------------------------------------------------------------------------------------------------------------------------

    return app.exec();
}
