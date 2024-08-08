#ifndef TABLES_H
#define TABLES_H

#include <QWidget>
#include "utils.h"
namespace Ui {
class tables;
}

class tables : public QWidget
{
    Q_OBJECT

public:
    explicit tables(QWidget *parent = nullptr);
    ~tables();

public slots:
    void on_addr_set_button_clicked();
    void update_table(QByteArray rec,quint16 start);
    QString byteToBinaryString(quint8 byte);

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::tables *ui;
};

#endif // TABLES_H
