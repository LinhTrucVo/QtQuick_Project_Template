#ifndef BICO_QUITHREAD_SAMPLE_H
#define BICO_QUITHREAD_SAMPLE_H

#include <QJsonDocument>
#include <QJsonObject>

#include "../../Template_Material/bico_quithread.h"
#include "Data_Object/Bico_QUIThread_Sample_Data.h"


class Bico_QUIThread_Sample : public Bico_QUIThread
{
    Q_OBJECT

public:
    Bico_QUIThread_Sample
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
    Bico_QUIThread_Sample_Data ex_data_obj;
};

#endif // BICO_QUITHREAD_SAMPLE_H
