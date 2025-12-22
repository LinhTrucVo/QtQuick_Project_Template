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
    );
    
    virtual uint8_t MainTask();

private:
    void cleanupChildren();
    Task1_Data ex_data_obj;
    static int count;  // Counter for thread naming
    
    // Message handler function type
    typedef uint8_t (Task1::*MessageHandler)(QVariant&, Bico_QMessData&);
    QMap<QString, MessageHandler> message_handlers;
    
    // Message handler methods
    uint8_t _handle_terminate(QVariant& data, Bico_QMessData& input);
    uint8_t _handle_num1(QVariant& data, Bico_QMessData& input);
    uint8_t _handle_num2(QVariant& data, Bico_QMessData& input);
    uint8_t _handle_text(QVariant& data, Bico_QMessData& input);
    uint8_t _handle_create(QVariant& data, Bico_QMessData& input);
    uint8_t _handle_create_child(QVariant& data, Bico_QMessData& input);
    uint8_t _handle_size(QVariant& data, Bico_QMessData& input);
    uint8_t _handle_from_another_thread(QVariant& data, Bico_QMessData& input);
};

#endif // TASK1_H
