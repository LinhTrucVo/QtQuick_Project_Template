#ifndef TASK1_H
#define TASK1_H

#include <QJsonDocument>
#include <QJsonObject>
#include <QStringList>

#include "bico_quithread.h"
#include "Data_Object/Task1_Data.h"


class Task1 : public Bico_QUIThread
{
    Q_OBJECT

public:
    Task1
    (
        Bico_DataQueue *qin = nullptr,
        uint8_t qin_owner = 0,
        Bico_DataQueue *qout = nullptr,
        uint8_t qout_owner = 0,
        QString obj_name = "",
        QString ui_path = "",
        QObject *parent = nullptr
    ) : Bico_QUIThread(qin, qin_owner, qout, qout_owner, obj_name, ui_path, parent)
    {}

    virtual ~Task1();

    virtual uint8_t MainTask();

private:
    Task1_Data ex_data_obj;
};

#endif // TASK1_H
