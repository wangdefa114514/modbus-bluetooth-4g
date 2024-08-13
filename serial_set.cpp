#include "serial_set.h"
#include "ui_serial_set.h"

serial_set::serial_set(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serial_set)
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

serial_set::~serial_set()
{
    delete ui;
}
void serial_set::set_com_names(QVector<QString> port_names){
    if(port_names!=m_serialPort_names){
        this->m_serialPort_names=port_names;
        ui->port_names->clear();
        for(auto name : port_names){
            ui->port_names->addItem(name);
        }
    }
}

void serial_set::on_switch_2_clicked()
{   QString port_name=ui->port_names->currentText();
    int baudrate=ui->baud_rate->currentText().toInt();
    int parity=ui->check_bit->currentText().toInt();
    int stop=ui->stop_bit->currentText().toInt();
    int databits=ui->data_length->currentText().toInt();
    emit sig_add_serial(port_name,baudrate,parity,stop,databits);
}
