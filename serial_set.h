#ifndef SERIAL_SET_H
#define SERIAL_SET_H

#include <QWidget>

namespace Ui {
class serial_set;
}

class serial_set : public QWidget
{
    Q_OBJECT

public:
    explicit serial_set(QWidget *parent = nullptr);
    ~serial_set();
public slots:
    void set_com_names(QVector<QString> names);
private slots:
    void on_switch_2_clicked();
signals:
    void sig_add_serial(QString port_name,int baudrate,int parity,int stop,int databits);
private:
    Ui::serial_set *ui;
    QVector<QString>    m_serialPort_names;
};

#endif // SERIAL_SET_H
