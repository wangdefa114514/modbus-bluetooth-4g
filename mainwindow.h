#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "serial_port.h"
#include <QThread>
#include "utils.h"
#include "parse.h"
#include <QTime>
#include "form.h"
#include "send.h"
#include "file.h"
#include "tables.h"
#include "serial_window.h"
#include "serial_window1.h"
#include "r_window.h"
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
public slots:
    void set_com_names(QVector<QString> names);
    void turn_on_serial_handler();
    void clear_send_handler();
    void clear_receive_handler();
    void send_button_handler();
    void log(QString s);
    void send_log(QString s);
    void receive_log(QString s);
    void send_other_handler();
    void set_serial_handler();
signals:
    void sig_open_serialport(QString port_name,int baudrate,int parity,int stop,int databits);
    void sig_parse_modbus(QByteArray send);
    void sig_parse_other(QByteArray pay,bool requires_receive,quint8 type);
    void sig_set(int,int);
    void sig_reconnect();
    void sig_disconnect();

private slots:
    void on_set_serial_button_clicked();
    void switch_(int,int,int,int);

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();
    void close_bin();

    void on_pushButton_5_clicked();


    void on_pushButton_6_clicked();

private:
    Ui::MainWindow *ui;
    QVector<QString> m_serialPort_names{};
    serial_port* m_pserial_port=nullptr;
    QThread m_serial_port_thread;
    parse* m_p_parse=nullptr;
    QThread m_parse_thread;
    QByteArray getCurrentTimeInCustomFormat();
    void appendLittleEndian(QByteArray &array, quint16 value);
    Form* mp_form=nullptr;
    int serial_settings[4]{115200,0,1,8};
    send* mp_send=nullptr;
    file* mp_file=nullptr;
    tables* mp_table=nullptr;
    serial_window* mp_serial_window=nullptr;
    serial_window1* mp_serial_window1=nullptr;
    r_window* mp_rwindow=nullptr;
};
#endif // MAINWINDOW_H
