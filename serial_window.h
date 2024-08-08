#ifndef SERIAL_WINDOW_H
#define SERIAL_WINDOW_H

#include <QWidget>
#include "serial_port.h"
#include <QThread>
#include "file.h"
#include "parse.h"
namespace Ui {
class serial_window;
}
class serial_port1: public serial_port{
    Q_OBJECT
public:
    serial_port1();
private:
    std::queue<QByteArray> send_queue;
public slots:
    void send_data(const QByteArray &data_to_send);
    bool sendRequest(const QByteArray &requestData, QByteArray& responseData) override;
    void satrt() override;

};







class serial_window : public QWidget
{
    Q_OBJECT

public:
    explicit serial_window(QWidget *parent = nullptr);
    ~serial_window();
    QByteArray getCurrentTimeInCustomFormat();
public slots:
    void set_com_names(QVector<QString> port_names);
    void send_log(QString s);
    void receive_log(QString s);
    void log(QString s);
    void close_bin();
    QByteArray pack_4g(QByteArray payload,int op);
signals:
    void sig_open_serialport(QString port_name,int baudrate,int parity,int stop,int databits);
    void sig_send_modbus(QByteArray send);
    void sig_reconnect();
    void sig_disconnect();

private slots:
    void on_open_port_clicked();

    void parse_rec(QByteArray rec);

    void on_clear_send_info_clicked();

    void on_clear_receive_info_clicked();

    void on_send_button_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

    void on_send_other_button_clicked();

private:
    Ui::serial_window *ui;
    QThread m_serial_thread;
    serial_port1* mp_serial_port1=nullptr;
    QVector<QString> m_serialPort_names{};
    file* mp_file=nullptr;
    parse* m_p_parse=nullptr;
};



#endif // SERIAL_WINDOW_H
