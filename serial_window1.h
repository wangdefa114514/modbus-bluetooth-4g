#ifndef serial_window11_H
#define serial_window11_H


#include <QWidget>
#include "serial_port.h"
#include <QThread>
#include "parse1.h"
#include <QRandomGenerator>
namespace Ui {
class serial_window1;
}



class serial_port2: public serial_port{
    Q_OBJECT
public:
    serial_port2();
    QTableWidget* reg_table=nullptr;
    QTableWidget* switch_table=nullptr;
    bool table_loaded=false;
    void show_table();
signals:
    void sig_parse_reveive(const QByteArray &receive);
    void sig_get_table();
private:
    std::queue<QByteArray> send_queue;

public slots:
    void send_data(const QByteArray &data_to_send);
    void satrt();
    void parse_receive_handler();

};






class serial_window1 : public QWidget
{
    Q_OBJECT

public:
    explicit serial_window1(QWidget *parent = nullptr);
    ~serial_window1();
public slots:
    void set_com_names(QVector<QString> port_names);
    void send_log(QString s);
    void receive_log(QString s);
    void log(QString s);
    void get_table();
    void get_table_parse();
    void addr_set_handler();
signals:
    void sig_open_serialport(QString port_name,int baudrate,int parity,int stop,int databits);
    void sig_set(QStringList sl);
private slots:
    void on_open_port_clicked();


    void on_clear_send_info_clicked();

    void on_clear_receive_info_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

private:
    Ui::serial_window1 *ui;
    QThread m_serial_thread;
    serial_port2* mp_serial_port2=nullptr;
    QVector<QString> m_serialPort_names{};
    parse1* m_p_parse=nullptr;
    QRandomGenerator *generator = nullptr;
    QThread m_parse_thread;

};
#endif // serial_window11_H
