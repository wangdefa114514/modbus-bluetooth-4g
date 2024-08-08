#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->modbus_option->addItem("读线圈寄存器",1);
    ui->modbus_option->addItem("读保持寄存器",2);
    ui->modbus_option->addItem("写单个线圈寄存器",3);
    ui->modbus_option->addItem("写单个保持寄存器",4);
    ui->modbus_option->addItem("写多个保持寄存器",5);
    ui->other1->addItem("启动车辆",1);
    ui->other1->addItem("熄火车辆",2);
    ui->other1->addItem("车辆设防",3);
    ui->other1->addItem("车辆撤防",4);
    ui->other1->addItem("开座鞍锁",5);
    ui->other1->addItem("鸣笛寻车",6);
    ui->other2->addItem("时间日期",1);
    ui->other3->addItem("启动",1);
    ui->other3->addItem("熄火",2);
    ui->other4->addItem("车辆已启动",1);
    ui->other4->addItem("车辆已熄火",2);
    ui->other4->addItem("单击",3);
    ui->other4->addItem("双击",4);
    ui->other4->addItem("长按",5);
    ui->other4->addItem("轮动",6);
    ui->other4->addItem("钥匙启动",7);
    ui->other4->addItem("钥匙熄火",8);
    ui->other5->addItem("手动输入",1);
    ui->other5->addItem("nfc密码",2);
    ui->move_box->addItem("动作下发",1);
    ui->move_box->addItem("参数下发",2);
    ui->move_box->addItem("查询",3);
    ui->move_box->addItem("事件上报",4);
    ui->move_box->addItem("数据上报",5);
    mp_form=new Form();
    mp_table=new tables();
    m_pserial_port=new serial_port();
    mp_send=new send();
    m_p_parse=new parse();
    mp_file=new file();
    mp_serial_window1=new serial_window1;
    qRegisterMetaType<QVector<QString>>("QVector<QString>&");
    qRegisterMetaType<QList<int>>("QList<int>&");
    m_pserial_port->moveToThread(&m_serial_port_thread);
    m_p_parse->moveToThread(&m_parse_thread);
    mp_serial_window=new serial_window();
    connect(&m_serial_port_thread,&QThread::started,m_pserial_port,&serial_port::satrt);
    connect(m_pserial_port,&serial_port::sig_set_com_names,this,&MainWindow::set_com_names);
    connect(this->ui->open_port,&QPushButton::clicked,this,&MainWindow::turn_on_serial_handler);
    connect(this,&MainWindow::sig_open_serialport,m_pserial_port,&serial_port::open_serial_port);
    connect(this->ui->close_port,&QPushButton::clicked,m_pserial_port,&serial_port::close_serial_port);
    connect(this->ui->clear_send_info,&QPushButton::clicked,this,&MainWindow::clear_send_handler);
    connect(this->ui->clear_receive_info,&QPushButton::clicked,this,&MainWindow::clear_receive_handler);
    connect(ui->send_button,&QPushButton::clicked,this,&MainWindow::send_button_handler);
    connect(this,&MainWindow::sig_parse_modbus,m_p_parse,&parse::parse_modbus);
    connect(m_p_parse,&parse::sig_send,m_pserial_port,&serial_port::send_data);
    connect(m_p_parse,&parse::sig_send_receive,m_pserial_port,&serial_port::send_receive_data);
    connect(m_pserial_port,&serial_port::sig_send_log,this,&MainWindow::send_log);
    connect(m_pserial_port,&serial_port::sig_receive_log,this,&MainWindow::receive_log);
    connect(ui->send_other_button,&QPushButton::clicked,this,&MainWindow::send_other_handler);
    connect(this,&MainWindow::sig_parse_other,m_p_parse,&parse::parse_other);
    connect(m_pserial_port,&serial_port::sig_log,this,&MainWindow::log);
    connect(ui->set_serial,&QPushButton::clicked,this,&MainWindow::set_serial_handler);
    connect(this,&MainWindow::sig_set,m_pserial_port,&serial_port::set);
    connect(mp_form,&Form::sig_switch,this,&MainWindow::switch_);
    connect(mp_send,&send::sig_send_single,m_pserial_port,&serial_port::send_data);
    connect(m_pserial_port,&serial_port::sig_send_next,m_pserial_port,&serial_port::send_next);
    connect(mp_send,&send::sig_cycle_send,m_pserial_port,&serial_port::cycle_send);
    connect(mp_send,&send::sig_stop_cycle,m_pserial_port,&serial_port::stop_send);
    connect(m_p_parse,&parse::sig_log,this,&MainWindow::log);
    connect(m_pserial_port,&serial_port::sig_parse_rec,m_p_parse,&parse::parse_rec);
    connect(mp_file,&file::sig_file_serialport_setting,m_pserial_port,&serial_port::file_settings);
    connect(m_p_parse,&parse::sig_send_modbus,m_pserial_port,&serial_port::send_modbus);
    connect(m_pserial_port,&serial_port::sig_update_table,mp_table,&tables::update_table);
    connect(mp_file,&file::sig_open_file,m_p_parse,&parse::open_file);

    connect(m_p_parse,&parse::sig_sendHandshakesRequest,m_pserial_port,&serial_port::on_sendHandshakesRequest);
    connect(m_pserial_port,&serial_port::sig_parseHandshakesResponse,m_p_parse,&parse::on_parseHandshakesResponse);
    connect(m_p_parse,&parse::sig_downloadWrapper,m_p_parse,&parse::on_downloadWrapper);
    connect(m_p_parse,&parse::sig_sendDownloadRequestByPackageIndex,m_p_parse,&parse::on_sendDownloadRequestByPackageIndex);
    connect(m_p_parse,&parse::sig_sendDownloadResultRequest,m_pserial_port,&serial_port::on_sendDownloadResultRequest);
    connect(m_pserial_port,&serial_port::sig_parseDownloadResultResponse,m_p_parse,&parse::on_parseDownloadResultResponse);
    connect(m_p_parse,&parse::sig_sendDownloadRequest,m_pserial_port,&serial_port::on_sendDownloadRequest);
    connect(m_pserial_port,&serial_port::sig_parseDownloadResponse,m_p_parse,&parse::on_parseDownloadResponse);
    connect(mp_file,&file::sig_set_parse,m_p_parse,&parse::set);
    connect(mp_file,&file::sig_close_bin,this,&MainWindow::close_bin);
    connect(this,&MainWindow::sig_reconnect,m_pserial_port,&serial_port::reconnect);
    connect(m_p_parse,&parse::sig_download_bin,m_p_parse,&parse::on_downloadBin);
    connect(this,&MainWindow::sig_disconnect,m_pserial_port,&serial_port::disconnect);
    connect(m_p_parse,&parse::sig_progressBarDownload,mp_file,&file::on_progressBarDownload);
    connect(m_p_parse,&parse::sig_downloadResultResponseSuccessed,mp_file,&file::log_succeed);
    connect(m_pserial_port,&serial_port::sig_flog,mp_file,&file::log);
    connect(m_p_parse,&parse::sig_flog,mp_file,&file::log);


    m_serial_port_thread.start();
    m_parse_thread.start();
//    mp_serial_window->show();
//    mp_serial_window1->show();
//    mp_send->show();
//    mp_file->show();

}

MainWindow::~MainWindow()
{
    m_serial_port_thread.quit();
    m_serial_port_thread.wait();
    m_parse_thread.quit();
    m_parse_thread.wait();
    m_pserial_port->deleteLater();
    m_p_parse->deleteLater();
    delete ui;
}

void MainWindow::set_com_names(QVector<QString> port_names){
    if(port_names!=m_serialPort_names){
        this->m_serialPort_names=port_names;
        ui->port_names->clear();
        for(auto name : port_names){
            ui->port_names->addItem(name);
        }
    }
}
void MainWindow::turn_on_serial_handler(){
    QString port_name{};
    port_name=this->ui->port_names->currentText();

    emit sig_open_serialport(port_name,serial_settings[0],serial_settings[1],serial_settings[2],serial_settings[3]);
}
void MainWindow::clear_send_handler(){
    this->ui->send_info->clear();
}
void MainWindow::clear_receive_handler(){
    this->ui->receive_info->clear();
}
void MainWindow::send_button_handler(){
    QByteArray send;
    quint8 addr=ui->addr->text().toUShort();
    quint16 reg_addr=ui->reg_addr->text().toUShort();
    quint16 reg_nums=ui->reg_num->text().toUShort();
    quint8 func_code=0;
    int option=ui->modbus_option->currentData().toInt();
    if(option==1){
        func_code=0x01;
    }
    else if(option==2){
        func_code=0x03;
    }
    else if(option==3){
        func_code=0x05;
    }
    else if(option==4){
        func_code=0x06;
    }
    else if(option==5){
        func_code=0x10;
    }
    if(option<=4){
        utils::instance().append(send,addr);
        utils::instance().append(send,func_code);
        utils::instance().append(send,reg_addr);
        utils::instance().append(send,reg_nums);
        quint16 crc=utils::instance().calc_crc16(send);

        utils::instance().append(send,crc);
        emit log("modbus:"+send.toHex(' '));
        emit sig_parse_modbus(send);
    }
    else{
        utils::instance().append(send,addr);
        utils::instance().append(send,func_code);
        utils::instance().append(send,reg_addr);
        QString s=ui->reg_num->text();
        QByteArray temp;
        reg_nums=(int)((s.size()+3)/4);
        quint8 byte_num=(int)(s.size()/2);

        utils::instance().append(send,reg_nums);
        utils::instance().append(send,byte_num);
        for(int i=0;i<s.size();i+=2){
            if(i+1<s.size()){
                temp.clear();
                temp.append(s[i].toLatin1());
                temp.append(s[i+1].toLatin1());
                func_code= temp.toUShort(nullptr,16);
                utils::instance().append(send,func_code);
            }
        }
        quint16 crc=utils::instance().calc_crc16(send);

        utils::instance().append(send,crc);
        emit log("modbus:"+send.toHex(' '));
        emit sig_parse_modbus(send);
    }

}
void MainWindow::log(QString s){
    this->ui->log_info->append(s);
}
void MainWindow::send_log(QString s){
    ui->send_info->append(s);
}
void MainWindow::receive_log(QString s){
    ui->receive_info->append(s);
}
void MainWindow::send_other_handler(){
    int op1=ui->move_box->currentData().toInt();
    int op2;
    QByteArray payload;
    if(op1==1){
        op2=ui->other1->currentData().toInt();
        quint8 pay=op2;
        utils::instance().append(payload,pay);
        emit sig_parse_other(payload,true,0x02);
    }
    else if(op1==2){
        utils::instance().append(payload,(quint8)0x01);
        payload.append(getCurrentTimeInCustomFormat());
        emit sig_parse_other(payload,true,0x03);
    }
    else if(op1==3){
        utils::instance().append(payload,(quint8)0x01);
        emit sig_parse_other(payload,true,0x04);
    }
    else if(op1==4){
        op2=ui->other4->currentData().toInt();
        quint8 pay=op2;
        utils::instance().append(payload,pay);
        emit sig_parse_other(payload,false,0x05);
    }
    else if(op1==5){
        op2=ui->other5->currentData().toInt();
        quint8 pay=op2;
        utils::instance().append(payload,pay);
        QString pass_s=ui->password_edit->text();
        quint32 password=pass_s.toUInt();
        if(0<=password && password<=9999){
            for(int i=0;i<4;i++){
                quint8 num=pass_s.mid(i,1).toUShort();
                utils::instance().append(payload,num);
            }
        }


        emit sig_parse_other(payload,true,0x06);
    }

}
QByteArray MainWindow::getCurrentTimeInCustomFormat() {
    QByteArray array;

    QString text= ui->time_edit->text();
    text.remove(' ');
    quint16 u16;
    quint8 u8;
    u16=text.mid(0,4).toUShort();
    utils::instance().append(array,u16);
    for(int i=0,p=4;i<5;i++,p+=2){
        u8=text.mid(p,2).toUShort();
        utils::instance().append(array,u8);
    }
    char c=array[0];
    array[0]=array[1];
    array[1]=c;
    return array;
}
void MainWindow::appendLittleEndian(QByteArray &array, quint16 value) {
    array.append(static_cast<char>(value & 0xFF));
    array.append(static_cast<char>((value >> 8) & 0xFF));
}
void MainWindow::set_serial_handler(){
    int retry=this->ui->max_retry->toPlainText().toInt();
    int timeout=this->ui->response_time_out->toPlainText().toInt();
    emit sig_set(retry,timeout);

}

void MainWindow::on_set_serial_button_clicked()
{

    mp_form->show();
}
void MainWindow::switch_(int baud,int parity,int stop,int data){
    this->show();
    mp_form->hide();
    serial_settings[0]=baud;
    serial_settings[1]=parity;
    serial_settings[2]=stop;
    serial_settings[3]=data;
}

void MainWindow::on_pushButton_clicked()
{
    mp_send->show();
}

void MainWindow::on_pushButton_2_clicked()
{
    ui->log_info->clear();
}

void MainWindow::on_pushButton_3_clicked()
{
    this->mp_table->show();
}

void MainWindow::on_pushButton_4_clicked()
{
    this->mp_file->show();
    m_pserial_port->bin_mode=true;
    emit sig_disconnect();
}
void MainWindow::close_bin(){
    m_pserial_port->bin_mode=false;
    emit sig_reconnect();
}

void MainWindow::on_pushButton_5_clicked()
{
    this->mp_serial_window->show();
}

void MainWindow::on_pushButton_6_clicked()
{
    this->mp_serial_window1->show();
}
