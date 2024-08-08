#ifndef parse1_H
#define parse1_H

#include <QObject>
#include <QDataStream>
#include <QDebug>
#include <QTableWidget>
#include "utils.h"

class parse1:public QObject
{
    Q_OBJECT
public:
    parse1();
    void struct_receive(const QByteArray &receive,g_receive_data_t &data);
    uint16_t calc_modbus_crc(const QByteArray &array);
    void readAndAdjustItems(int start_idx, int item_count, int desiredSize,QByteArray &temp_byte_array);
    void readAndAdjustItems_hex(int start_idx, int item_count, int desiredSize,QByteArray &temp_byte_array);
    void readAndAdjustItems_06(int start_idx, int item_count, int desiredSize,QByteArray &temp_byte_array);
    void adjustByteArraySize(QByteArray &byteArray, int desiredSize);
    QTableWidget* reg_table;
    QTableWidget* switch_table;
    QTableWidget* table_06;
    QTableWidget* stable_05;
    QTableWidget* rtable_05;

public slots:
    void parse1_receive(const QByteArray &receive);
    void start();
    void read_switch(quint8 addr,quint16 start,quint16 nums);
    void read_reg(quint8 addr,quint16 start,quint16 nums);
    void read_switch_05(quint8 addr,quint16 start,quint16 nums);
    void read_reg_05(quint8 addr,quint16 start,quint16 nums);
    void read_06(quint8 addr,quint16 start,quint16 nums);
    void set(QStringList sl);
signals:
    void sig_log_parse1(QString log);
    void sig_read_switch(quint8 addr,quint16 start,quint16 nums);
    void sig_read_reg(quint8 addr,quint16 start,quint16 nums);

    void sig_get_table_parse1();
    void sig_send_switch(QByteArray send);
    void sig_send_reg(QByteArray send);

private:
    quint8 m_bms_protection_board=0x01;
    quint8 m_instrument;
    quint8 m_intelligent_central_control;
    quint8 m_motor_controller=0x05;
    quint8 m_helmet_lock=0x06;
    quint8 m_rear_wheel_lock_hub_lock;
};

#endif // parse1_H
