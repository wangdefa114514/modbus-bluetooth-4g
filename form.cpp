#include "form.h"
#include "ui_form.h"
#include <QSerialPort>
Form::Form(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Form)
{
    ui->setupUi(this);
    ui->baud_rate->addItem("1200", 1200);
    ui->baud_rate->addItem("2400", 2400);
    ui->baud_rate->addItem("4800", 4800);
    ui->baud_rate->addItem("9600", 9600);
    ui->baud_rate->addItem("14400", 14400);
    ui->baud_rate->addItem("19200", 19200);
    ui->baud_rate->addItem("38400", 38400);
    ui->baud_rate->addItem("57600", 57600);
    ui->baud_rate->addItem("115200", 115200);
    ui->baud_rate->addItem("230400", 230400);
    ui->baud_rate->addItem("460800", 460800);
    ui->baud_rate->addItem("921600", 921600);
    ui->check_bit->addItem("none",0);
    ui->check_bit->addItem("even",2);
    ui->check_bit->addItem("odd",3);
    ui->stop_bit->addItem("one",1);
    ui->stop_bit->addItem("two",2);
    ui->data_length->addItem("8",8);
    ui->data_length->addItem("7",7);
}

Form::~Form()
{
    delete ui;
}

void Form::on_switch_2_clicked()
{
    int baud=ui->baud_rate->currentText().toInt();
    int parity=ui->check_bit->currentData().toInt();
    int stop=ui->stop_bit->currentData().toInt();
    int data=ui->data_length->currentData().toInt();
    emit sig_switch(baud,parity,stop,data);
}
