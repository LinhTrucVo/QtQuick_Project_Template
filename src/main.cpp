#include <QGuiApplication>
#include <QDebug>
#include <iostream>
#include "Client_Code/Task1/Task1.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    app.setQuitOnLastWindowClosed(false);
    Bico_QUIThread::setMainApp(&app);

    // // Print all available resource paths
    // QDirIterator qrc(":", QDirIterator::Subdirectories);
    // while (qrc.hasNext()) {
    //     qDebug() << (qrc.next());
    // }

// Thread with UI --------------------------------------------
// --------------------------------------------------------
    Task1* ui0 = Bico_QUIThread::create<Task1>
            (
                new Bico_DataQueue,
                1,
                new Bico_DataQueue,
                1,
                "task_0",
                "qrc:/Client_Code/Task1/UI/Task1Content/App.qml"
            );
    ui0->start();

    // Task1* ui1 = Bico_QUIThread::create<Task1>
    //         (
    //             new Bico_DataQueue,
    //             1,
    //             new Bico_DataQueue,
    //             1,
    //             "task_1",
    //             "qrc:/Client_Code/Task1/UI/Task1Content/App.qml"
    //         );
    // ui1->start();
// --------------------------------------------------------
    
// Thread without UI --------------------------------------------
// --------------------------------------------------------
//    Task1* ui0 = Bico_QUIThread::create<Task1>
//            (
//                new Bico_DataQueue,
//                1,
//                new Bico_DataQueue,
//                1,
//                "task_0"
//            );
//    ui0->start();

//    Task1* ui1 = Bico_QUIThread::create<Task1>
//            (
//                new Bico_DataQueue,
//                1,
//                new Bico_DataQueue,
//                1,
//                "task_1"
//            );
//    ui1->start();
// --------------------------------------------------------

    return app.exec();
}
