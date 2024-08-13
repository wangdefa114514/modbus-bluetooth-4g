#ifndef R_WINDOW_H
#define R_WINDOW_H
#include <QPushButton>
#include <QWidget>
#include "form.h"
#include "serial_set.h"
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QTimer>
#include <QThread>
#include "serial_port.h"
#include "setting_text.h"
#include <QComboBox>
#include "utils.h"
#include <QQueue>
namespace Ui {
class r_window;
}

class parser:public QObject{
    Q_OBJECT
public:
    enum message_type{
        BLUETOOTH,
        MODBUS
    };
    ~parser();
signals:
    void sig_log(QString);
    void sig_rec_log(QString);
    void sig_send_log(QString);
    void sig_log1(QString);
public slots:
    message_type type_judge(QByteArray array);
    void parse_modbus(QByteArray array);
    void parse_bluetooth(QByteArray array);
    void parse_wrapper(QByteArray rec);
    void parse_bluetooth_res(QByteArray send,QByteArray rec);
    void parse_modbus_res(QByteArray send,QByteArray rec);
    void rec_parse_wrapper(QByteArray rec);
    bool need_push(QByteArray arr);
private:
    QQueue<QByteArray> send_queue;
};



class Serialport_scanner:public QObject{
    Q_OBJECT
public:
    Serialport_scanner();
    QVector<QString> port_names;
public slots:
    void scan_port();
    void start();
signals:
    void sig_set_com_names(QVector<QString>);


private:
    QTimer* timer=nullptr;


};


class Serialport_manager:public QObject{
    Q_OBJECT
public:
    Serialport_manager();
    ~Serialport_manager();
    parser* p_parser=nullptr;
public slots:

    void make_serial(QString port_name,int baudrate,int parity,int stop,int databits);
    void cycle_send(const QList<QByteArray> &send,const QList<int> &time,const QList<int> &port_no);
    void send_next();
    void stop_send();
signals:
    void sig_open(QString port_name,int baudrate,int parity,int stop,int databits);
    void sig_log_interface(const QString &send,int opcode);
    void sig_add_port(QString name);
    void sig_send_next();
    void sig_send_parse(QByteArray send);

private:
    QVector<serial_port*> serial_pool;
    QVector<QThread*> thread_pool;
    QList<QByteArray> m_datalist;
    QList<int> m_delay_list;
    QList<int> m_port_no_list;
    QTimer* mp_send_timer=nullptr;
    int m_index;
    QQueue<QByteArray> rec_queue;


};


class r_window : public QWidget
{
    Q_OBJECT

public:
    explicit r_window(QWidget *parent = nullptr);
    ~r_window();
    int row=0;

public slots:
    void on_pushButton_2_clicked();
    void log_handler(const QString &send,int opcode);
    void update_port(QString name);
    void set(int i);
    void sending(int i);
    void set_text_handler(int i,QString text);
    void loadTableData();

signals:
    void sig_set(int row);
    void sig_send(int row);
    void sig_send_single(const QByteArray &send);
    void sig_cycle_send(const QList<QByteArray> &send,const QList<int> &time,const QList<int> &port_no);
    void sig_stop_cycle();



private slots:
    void on_pushButton_clicked();

    void on_start_clicked();
    void update_port_col();
    void on_stop_clicked();
    void log(QString);
    void rec_log(QString);
    void send_log(QString);
    void log1(QString);

private:
    Ui::r_window *ui;
    serial_set* mp_form=nullptr;
    Serialport_scanner* scanner=nullptr;
    QThread scanner_thread;
    Serialport_manager* manager=nullptr;
    QStringList ports;
    setting_text *mp_text=nullptr;
    parser* mp_parser=nullptr;
    QThread m_parse_thread;

};

#endif // R_WINDOW_H
