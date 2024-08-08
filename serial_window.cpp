#include "serial_window.h"
#include "ui_serial_window.h"

serial_window::serial_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serial_window)
{
    ui->setupUi(this);
    ui->modbus_option->addItem("读线圈寄存器",1);
    ui->modbus_option->addItem("读保持寄存器",2);
    ui->modbus_option->addItem("写单个线圈寄存器",3);
    ui->modbus_option->addItem("写单个保持寄存器",4);
    ui->modbus_option->addItem("写多个保持寄存器",5);
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

    ui->other1->addItem("启动车辆",1);
    ui->other1->addItem("熄火车辆",2);
    ui->other1->addItem("车辆设防",3);
    ui->other1->addItem("车辆撤防",4);
    ui->other1->addItem("开座鞍锁",5);
    ui->other1->addItem("鸣笛寻车",6);
    ui->other1->addItem("开龙头锁",7);
    ui->other1->addItem("关龙头锁",8);
    ui->other1->addItem("进入输密码状态",9);
    ui->other1->addItem("退出输密码状态",10);
    ui->other1->addItem("进入学卡状态",11);
    ui->other1->addItem("退出学卡状态",12);
    ui->other1->addItem("开启扫卡",13);
    ui->other1->addItem("关闭扫卡",14);
    ui->other2->addItem("数据",1);
    ui->other2->addItem("时间",2);

    ui->move_box->addItem("动作下发",1);
    ui->move_box->addItem("参数下发",2);
    ui->move_box->addItem("查询",3);


    mp_file=new file();
    m_p_parse=new parse();
    mp_serial_port1=new serial_port1();
    mp_serial_port1->moveToThread(&m_serial_thread);
    connect(&m_serial_thread,&QThread::started,mp_serial_port1,&serial_port1::satrt);
    connect(mp_serial_port1,&serial_port1::sig_set_com_names,this,&serial_window::set_com_names);
    connect(mp_serial_port1,&serial_port1::sig_send_log,this,&serial_window::send_log);
    connect(mp_serial_port1,&serial_port1::sig_receive_log,this,&serial_window::receive_log);
    connect(this,&serial_window::sig_open_serialport,mp_serial_port1,&serial_port1::open_serial_port);
    connect(ui->close_port,&QPushButton::clicked,mp_serial_port1,&serial_port1::close_serial_port);
    connect(mp_serial_port1,&serial_port1::sig_log,this,&serial_window::log);
    connect(this,&serial_window::sig_send_modbus,mp_serial_port1,&serial_port1::send_data);

    connect(mp_file,&file::sig_file_serialport_setting,mp_serial_port1,&serial_port1::file_settings);
    connect(m_p_parse,&parse::sig_send_modbus,mp_serial_port1,&serial_port1::send_modbus);
    connect(mp_file,&file::sig_open_file,m_p_parse,&parse::open_file);
    connect(m_p_parse,&parse::sig_sendHandshakesRequest,mp_serial_port1,&serial_port1::on_sendHandshakesRequest);
    connect(mp_serial_port1,&serial_port1::sig_parseHandshakesResponse,m_p_parse,&parse::on_parseHandshakesResponse);
    connect(m_p_parse,&parse::sig_downloadWrapper,m_p_parse,&parse::on_downloadWrapper);
    connect(m_p_parse,&parse::sig_sendDownloadRequestByPackageIndex,m_p_parse,&parse::on_sendDownloadRequestByPackageIndex);
    connect(m_p_parse,&parse::sig_sendDownloadResultRequest,mp_serial_port1,&serial_port1::on_sendDownloadResultRequest);
    connect(mp_serial_port1,&serial_port1::sig_parseDownloadResultResponse,m_p_parse,&parse::on_parseDownloadResultResponse);
    connect(m_p_parse,&parse::sig_sendDownloadRequest,mp_serial_port1,&serial_port1::on_sendDownloadRequest);
    connect(mp_serial_port1,&serial_port1::sig_parseDownloadResponse,m_p_parse,&parse::on_parseDownloadResponse);
    connect(mp_file,&file::sig_set_parse,m_p_parse,&parse::set);
    connect(mp_file,&file::sig_close_bin,this,&serial_window::close_bin);
    connect(this,&serial_window::sig_reconnect,mp_serial_port1,&serial_port1::reconnect);
    connect(m_p_parse,&parse::sig_download_bin,m_p_parse,&parse::on_downloadBin);
    connect(this,&serial_window::sig_disconnect,mp_serial_port1,&serial_port1::disconnect);
    connect(m_p_parse,&parse::sig_progressBarDownload,mp_file,&file::on_progressBarDownload);
    connect(m_p_parse,&parse::sig_downloadResultResponseSuccessed,mp_file,&file::log_succeed);
    connect(mp_serial_port1,&serial_port::sig_flog,mp_file,&file::log);
    connect(m_p_parse,&parse::sig_flog,mp_file,&file::log);
    connect(mp_serial_port1,&serial_port1::sig_parse_rec,this,&serial_window::parse_rec);
    m_serial_thread.start();
}

serial_window::~serial_window()
{
    delete ui;
    m_serial_thread.quit();
    m_serial_thread.wait();
    mp_file->deleteLater();
    mp_serial_port1->deleteLater();
}
serial_port1::serial_port1(){

}
void serial_window::set_com_names(QVector<QString> port_names){
    if(port_names!=m_serialPort_names){
        this->m_serialPort_names=port_names;
        ui->port_names->clear();
        for(auto name : port_names){
            ui->port_names->addItem(name);
        }
    }
}

void serial_window::on_open_port_clicked()
{
    QString port_name{};
    port_name=this->ui->port_names->currentText();
    int baud=ui->baud_rate->currentData().toInt();
    int check=ui->check_bit->currentData().toInt();
    int stop=ui->stop_bit->currentData().toInt();
    int data=ui->data_length->currentData().toInt();
    //serial_settings[4]{115200,0,1,8};
    emit sig_open_serialport(port_name,baud,check,stop,data);
}

void serial_window::send_log(QString s){
    ui->send_info->append(s);
}
void serial_window::receive_log(QString s){
    ui->receive_info->append(s);
}

void serial_window::log(QString s){
    this->ui->log_info->append(s);
}

void serial_window::on_clear_send_info_clicked()
{
    ui->send_info->clear();
}

void serial_window::on_clear_receive_info_clicked()
{
    ui->receive_info->clear();
}
void serial_port1::send_data(const QByteArray &data_to_send){
    emit sig_send_log(data_to_send.toHex(' '));
    send_queue.push(data_to_send);
    int numberOfWriteBytes = 0;
    int totalRequestDataSize = data_to_send.size();

    qDebug() << "requestData:" << data_to_send.toHex(' ').toUpper();

    for (int retryIndex = 0; retryIndex <= 0; ++retryIndex)
    {
        // Request
        numberOfWriteBytes = m_pQserialport->write(data_to_send);
        if (numberOfWriteBytes != totalRequestDataSize)
        {
            qDebug() << "requestData:" << data_to_send.left(numberOfWriteBytes).toHex(' ').toUpper();
            break;
        }
        break;

    }

}

void serial_window::on_send_button_clicked()
{
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

    }
    emit sig_send_modbus(send);
}

void serial_window::on_pushButton_2_clicked()
{
    ui->log_info->clear();
}

void serial_window::on_pushButton_3_clicked()
{
    mp_file->show();
    mp_serial_port1->bin_mode=true;
    emit sig_disconnect();
}
void serial_window::close_bin(){
    mp_serial_port1->bin_mode=false;
    emit sig_reconnect();
}
bool serial_port1::sendRequest(const QByteArray &requestData, QByteArray& responseData)
{
    qDebug() <<"enter send request";
    bool result = false;
    quint32 singleStep = 500; // ms
    quint32 responseTimeout = m_responseTimeout;
    quint32 numberOfWriteBytes = 0;
    quint32 totalRequestDataSize = requestData.size();

    qDebug() << "requestData:" << requestData.toHex(' ').toUpper();

    for (quint32 retryIndex = 0; retryIndex <= m_numberOfRetries; ++retryIndex)
    {
        // Request
        QByteArray send=requestData;
        emit sig_flog("发送"+send.toHex(' '));
        totalRequestDataSize=send.size();
        numberOfWriteBytes = m_pQserialport->write(send);
        if (numberOfWriteBytes != totalRequestDataSize)
        {
            qDebug() << "requestData:" << requestData.left(numberOfWriteBytes).toHex(' ').toUpper();
        }

        // waitForReadyRead
        if (!m_pQserialport->waitForReadyRead(responseTimeout))
        {
            if (QSerialPort::TimeoutError == m_pQserialport->error())
            {
                qWarning() << QString(tr("Response timeout: %1 ms. retryIndex: %2"))
                              .arg(responseTimeout)
                              .arg(retryIndex);
                responseTimeout += singleStep; // 1s, 1.5s, 2s, 2.5s, 3s, 3.5s
            }
            else
            {
                qWarning() << QString(tr("Response error: %1. retryIndex: %2"))
                              .arg(m_pQserialport->errorString())
                              .arg(retryIndex);
            }

            continue;
        }

        // Response

        responseData = m_pQserialport->readAll();
        while (m_pQserialport->waitForReadyRead(m_responseInterval))
        {
            responseData += m_pQserialport->readAll();
        }

        qDebug() << "responseData:" << responseData.toHex(' ').toUpper();
        emit sig_flog("接收"+responseData.toHex(' '));

        result = true;
        break;
    }

    return result;
}

void serial_window::on_send_other_button_clicked()
{
    int op1=ui->move_box->currentData().toInt();
    int op2;
    QByteArray payload;
    QByteArray send;
    if(op1==1){
        op2=ui->other1->currentData().toInt();
        quint16 pay=op2;
        utils::instance().append(payload,pay);
        send=pack_4g(payload,1);
        log(ui->other1->itemText(op2-1));
        emit sig_send_modbus(send);
    }
    else if(op1==2){
        QString text=ui->password_edit->text();

        op2=ui->other2->currentData().toInt();
        log(ui->other2->itemText(op2-1));
        if(op2==1){
            text.remove(' ');
        utils::instance().Hexstring_to_bytes(text,payload);
        send=pack_4g(payload,2);}
        else{
            payload.append(getCurrentTimeInCustomFormat());
            utils::instance().append(payload,(quint8)(0x00));
            send=pack_4g(payload,3);
        }
        emit sig_send_modbus(send);

    }
    else if(op1==3){
        QByteArray temp;
        QString temp1=ui->addredit->text();
        temp1.remove(' ');
        log("查询");
        utils::instance().Hexstring_to_bytes(temp1,temp);
        payload.append(temp);
        temp.clear();
        temp1=ui->numedit->text();
        temp1.remove(' ');
        utils::instance().Hexstring_to_bytes(temp1,temp);
        payload.append(temp);
        send=pack_4g(payload,4);
        emit sig_send_modbus(send);
    }
    else if(op1==4){

        quint8 pay=op2;
        utils::instance().append(payload,pay);
        send=pack_4g(payload,5);
        emit sig_send_modbus(send);
    }
    else if(op1==5){

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


        send=pack_4g(payload,5);
        emit sig_send_modbus(send);
    }
}
QByteArray serial_window::getCurrentTimeInCustomFormat() {
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
QByteArray serial_window::pack_4g(QByteArray payload,int op){
    QByteArray send;
    quint16 crc;
    if(op==1){
        utils::instance().append(send,(quint8)0x02);
        utils::instance().append(send,(quint8)0x06);
        utils::instance().append(send,(quint8)0x00);
        utils::instance().append(send,(quint8)0x02);
        send.append(payload);
        crc=utils::instance().calc_crc16(send);
        utils::instance().append(send,crc);
    }
    else if(op==2){
        utils::instance().append(send,(quint8)0x02);
        utils::instance().append(send,(quint8)0x10);
        utils::instance().append(send,(quint8)0x00);
        utils::instance().append(send,(quint8)0x00);
        utils::instance().append(send,(quint8)0x00);
        utils::instance().append(send,(quint8)0x08);
        utils::instance().append(send,(quint8)(payload.size()));
        send.append(payload);
        crc=utils::instance().calc_crc16(send);
        utils::instance().append(send,crc);
    }
    else if(op==3){
        utils::instance().append(send,(quint8)0x02);
        utils::instance().append(send,(quint8)0x10);
        utils::instance().append(send,(quint8)0x00);
        utils::instance().append(send,(quint8)0x08);
        utils::instance().append(send,(quint8)0x00);
        utils::instance().append(send,(quint8)0x04);
        utils::instance().append(send,(quint8)(payload.size()));
        send.append(payload);
        crc=utils::instance().calc_crc16(send);
        utils::instance().append(send,crc);
    }
    else if(op==4){
        utils::instance().append(send,(quint8)0x02);
        utils::instance().append(send,(quint8)0x03);
        send.append(payload);
        crc=utils::instance().calc_crc16(send);
        utils::instance().append(send,crc);
    }
    return send;
}
void serial_port1::satrt(){
    this->m_pQserialport=new QSerialPort();
    this->m_pPort_scan_timer=new QTimer();
    this->mp_send_timer=new QTimer();
    QObject::connect(this->m_pPort_scan_timer,&QTimer::timeout,this,&serial_port::scan_port);
    connect(mp_send_timer,&QTimer::timeout,this,&serial_port::send_next);

    m_connection=QObject::connect(m_pQserialport, &QSerialPort::readyRead, this, [this](){

        if(!addrs.empty()){
            quint16 addr=addrs.front();
            addrs.pop();
            QByteArray rec=this->m_pQserialport->readAll();
//            while (m_pQserialport->waitForReadyRead(200))
//            {
//                rec += m_pQserialport->readAll();
//            }
            emit sig_update_table(rec,addr);
            emit sig_receive_log(rec.toHex(' '));
            return;
        }
        QByteArray rec=this->m_pQserialport->readAll();
        emit sig_parse_rec(rec);
        emit sig_receive_log(rec.toHex(' '));
    });
    m_pPort_scan_timer->start(1000);
}
void serial_window::parse_rec(QByteArray rec){
    quint8 opcode;
    QByteArray temp;

    utils::instance().number(opcode,rec,1);
    if(opcode==0x03 ||opcode==0x10||opcode==0x06 ){
        log("成功");
    }
    else if(opcode==0x13||opcode==0x90||opcode==0x86){
        utils::instance().number(opcode,rec,2);
        if(opcode==0x02){
            log("无效地址");
        }
        else{
            log("无效值");
        }
    }
}

