#ifndef TASK_1_H
#define TASK_1_H

#include <QJsonDocument>
#include <QJsonObject>

#include "../../Template_Material/bico_quithread.h"
#include "Data_Object/Task_1_Data.h"


class Task_1 : public Bico_QUIThread
{
    Q_OBJECT

public:
    Task_1
    (
        Bico_DataQueue *qin = nullptr,
        uint8_t qin_owner = 0,
        Bico_DataQueue *qout = nullptr,
        uint8_t qout_owner = 0,
        QString obj_name = "",
        QString ui_path = "",
        QThread *parent = nullptr
    ) : Bico_QUIThread(qin, qin_owner, qout, qout_owner, obj_name, ui_path, parent)
    {}

    virtual uint8_t MainTask();

private:
    Task_1_Data ex_data_obj;
};

#endif // TASK_1_H
