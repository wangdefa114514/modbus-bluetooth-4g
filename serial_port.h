#ifndef SERIAL_PORT_H
#define SERIAL_PORT_H

#include <QObject>
#include <QSerialPort>
#include <QTimer>
#include <QVector>
#include <QSerialPortInfo>
#include <QDebug>
#include <queue>
#include "utils.h"
class serial_port:public QObject
{
    Q_OBJECT
public:
    serial_port();
    bool is_open();
    bool bin_mode=false;
    QMetaObject::Connection m_connection;
    QSerialPort* m_pQserialport=nullptr;
public slots:
    void scan_port();
    void open_serial_port(QString port_name,int baudrate,int parity,int stop,int databits);
    void close_serial_port();
    virtual void satrt();
    void send_data(const QByteArray &data_to_send);
    void set(int retry,int timeout);
    void send_receive_data(const QByteArray &data_to_send);
    void cycle_send(const QList<QByteArray> &send,const QList<int> &time);
    void send_next();
    void stop_send();
    void file_settings(int retry,int timeout,int interval);
    void on_sendDownloadRequest(const QByteArray downloadRequestData, quint32 pkgIdx);
    void on_sendDownloadResultRequest(const QByteArray downloadResultRequestData);
    virtual bool sendRequest(const QByteArray &requestData, QByteArray& responseData);
    void send_modbus(QByteArray requestData,quint16 start);
    void on_sendHandshakesRequest(const QByteArray handshakesRequestData);
    void clearReadBuffer() const;
    void reconnect();
    void disconnect();
signals:
    void sig_set_com_names(QVector<QString> port_names);
    void sig_show_info(QByteArray info);
    void sig_log(const QString &info);
    void sig_send_log(const QString &info);
    void sig_receive_log(const QString &info);
    void sig_send_next();
    void sig_parse_rec(QByteArray rec);
    void sig_downloadResultResponseFailed();
    void sig_parseHandshakesResponse(const QByteArray handshakesResponseData);
    void sig_parseDownloadResponse(const QByteArray downloadResponseData, quint32 pkgIdx);
    void sig_parseDownloadResultResponse(const QByteArray downloadResultResponseData);
    void sig_update_table(QByteArray rec,quint16 start);
    void sig_flog(QString s);
protected:
    QTimer* m_pPort_scan_timer=nullptr;

    QVector<QString> port_names={};
    quint32 m_numberOfRetries=0;
    quint32 m_responseInterval=1000;
    quint32 m_responseTimeout=10;
    QList<QByteArray> m_datalist;
    QList<int> m_delay_list;
    QTimer* mp_send_timer=nullptr;
    int m_index;
    QByteArray m_rec;
    bool modbus=false;
    std::queue<quint16> addrs;
};
#endif // SERIAL_PORT_H
