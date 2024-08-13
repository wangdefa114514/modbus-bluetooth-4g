#include "r_window.h"
#include "ui_r_window.h"
#include "send.h"



r_window::r_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::r_window)
{
    ui->setupUi(this);
    this->mp_form=new serial_set();
    scanner=new Serialport_scanner();
    manager=new Serialport_manager();
    scanner->moveToThread(&scanner_thread);
    mp_parser=new parser();
    mp_parser->moveToThread(&m_parse_thread);
    connect(&scanner_thread,&QThread::started,scanner,&Serialport_scanner::start);
    connect(scanner,&Serialport_scanner::sig_set_com_names,mp_form,&serial_set::set_com_names);
    connect(mp_form,&serial_set::sig_add_serial,manager,&Serialport_manager::make_serial);
    connect(manager,&Serialport_manager::sig_log_interface,this,&r_window::log_handler);
    connect(manager,&Serialport_manager::sig_add_port,this,&r_window::update_port);
    connect(manager,&Serialport_manager::sig_send_next,manager,&Serialport_manager::send_next);
    connect(manager,&Serialport_manager::sig_send_parse,mp_parser,&parser::parse_wrapper);
    connect(mp_parser,&parser::sig_log,this,&r_window::log);
    connect(mp_parser,&parser::sig_rec_log,this,&r_window::rec_log);
    connect(mp_parser,&parser::sig_send_log,this,&r_window::send_log);
    connect(mp_parser,&parser::sig_log1,this,&r_window::log1);
    scanner_thread.start();
    //mp_form->show();


    mp_text=new setting_text();
    mp_text->i=&row;
    ui->tableWidget ->setRowCount(100);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setHorizontalHeaderLabels(QStringList() << "HEX" << "点击发送" << "顺序" << "延时");
    QPushButton* b=new QPushButton(ui->tableWidget);
    b->setText("asdf");
    ui->tableWidget->setCellWidget(1,1,b);
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        QPushButton* b=new QPushButton(ui->tableWidget);
        b->setText("注释"+QString::number(i));
        ui->tableWidget->setCellWidget(i,1,b);
        connect(b,&QPushButton::clicked,this,[this,i]{emit sig_send(i);});
        QLineEditWithDoubleClick* l1=new QLineEditWithDoubleClick(ui->tableWidget);
        ui->tableWidget->setCellWidget(i,0,l1);
        connect(l1,&QLineEditWithDoubleClick::doubleClicked,this,[this,i]{emit sig_set(i);});

        QLineEditWithDoubleClick* l2=new QLineEditWithDoubleClick(ui->tableWidget);
        l2->setText("0");
        ui->tableWidget->setCellWidget(i,2,l2);
        QLineEditWithDoubleClick* l3=new QLineEditWithDoubleClick(ui->tableWidget);
        l3->setText("1000");
        ui->tableWidget->setCellWidget(i,3,l3);

        QComboBox* l4=new QComboBox(ui->tableWidget);
        ui->tableWidget->setCellWidget(i,4,l4);
    }
    ui->tableWidget->setColumnWidth(0,300);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,50);
    ui->tableWidget->setColumnWidth(3,50);
    ui->tableWidget->setColumnWidth(4,300);
    connect(this,&r_window::sig_set,this,&r_window::set);
    connect(this,&r_window::sig_send,this,&r_window::sending);
    connect(mp_text,&setting_text::sig_set,this,&r_window::set_text_handler);
    connect(this,&r_window::sig_cycle_send,manager,&Serialport_manager::cycle_send);
    connect(this,&r_window::sig_stop_cycle,manager,&Serialport_manager::stop_send);
    loadTableData();
    manager->p_parser=mp_parser;
    m_parse_thread.start();


}
void r_window::update_port_col(){
    QComboBox* box;
    qDebug()<<"start updating"<<ports;
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        box = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,4));
        box->clear();
        box->addItems(ports);
        qDebug()<<"set done";
    }
}
void r_window::update_port(QString name){
    if(ports.indexOf(name)==-1){
        ports.append(name);
    }
    update_port_col();
}
void r_window::log_handler(const QString &send,int opcode){
    if(opcode==0){
        ui->receive_log->append(send);
    }
}
r_window::~r_window()
{
    delete ui;
}

Serialport_scanner::Serialport_scanner(){

}
void Serialport_scanner::scan_port(){
    //qDebug()<<"the scan port is running";
    QVector<QString> temp={};
    for(const QSerialPortInfo &port :QSerialPortInfo::availablePorts()){
        //qDebug()<<port.portName();

        temp.push_back(port.portName());
    }
    qSort(temp.begin(),temp.end());
    if(temp!=port_names){
        this->port_names=temp;

    }
    emit sig_set_com_names(temp);

}
void Serialport_scanner::start(){
    timer=new QTimer();
    QObject::connect(this->timer,&QTimer::timeout,this,&Serialport_scanner::scan_port);
    timer->start(1000);
}

void r_window::on_pushButton_2_clicked()
{
    this->mp_form->show();
}
void Serialport_manager::make_serial(QString port_name,int baudrate,int parity,int stop,int databits){
    QThread* serial_thread=new QThread;
    thread_pool.append(serial_thread);
    serial_port* pserial=new serial_port();
    serial_pool.append(pserial);
    pserial->moveToThread(serial_thread);
    connect(serial_thread,&QThread::started,pserial,&serial_port::satrt);
    connect(this,&Serialport_manager::sig_open,pserial,&serial_port::open_serial_port);
    connect(pserial,&serial_port::sig_receive_log,this,[this](const QString &logger){emit sig_log_interface(logger,0);});
    connect(pserial,&serial_port::sig_parse_rec,p_parser,&parser::rec_parse_wrapper);
    serial_thread->start();
    //QMetaObject::invokeMethod(pserial, "disconnect",Qt::QueuedConnection);

    emit sig_open(port_name,baudrate,parity,stop,databits);
    emit sig_add_port(port_name);
}



void r_window::set(int i){
    qDebug()<<"setting column "<<i;
    row=i;
    mp_text->show();
}
void r_window::sending(int i){
    qDebug()<<"sending "<<i;
    QByteArray send;
    QLineEdit *line = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(i,0));
    QString s=line->text();
    utils::instance().Hexstring_to_bytes(s,send);
    emit sig_send_single(send);
}
void r_window::set_text_handler(int i,QString s){
    QPushButton *button = qobject_cast<QPushButton*>(ui->tableWidget->cellWidget(i,1));
    button->setText(s);
}




void r_window::loadTableData()
{

    QString filePath=QCoreApplication::applicationDirPath()+"/sb2.ini";
    QFileInfo checkFile(filePath);
    if(!(checkFile.exists() && checkFile.isDir())){

    }
    QSettings settings(filePath, QSettings::IniFormat);
    QLineEdit *line3 ;
    QPushButton *button;
    QStringList groups = settings.childGroups();
    foreach (QString group, groups) {
        settings.beginGroup(group);
        QStringList keys = settings.allKeys();
        foreach (QString key, keys) {
            QStringList indices = key.split('/');
            if (indices.size() == 2) {
                int row = indices.at(0).toInt();
                int col = indices.at(1).toInt();
                QString value = settings.value(key).toString();
                if(col==1){
                    button = qobject_cast<QPushButton*>(ui->tableWidget->cellWidget(row,1));
                    button->setText(value);
                    //ui->tableWidget->setCellWidget(row,col,button);
                }
                else{
                    line3 = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(row,col));
                    line3->setText(value);
                    // ui->tableWidget->setCellWidget(row,col,line3);

                }
            }
        }
        settings.endGroup();
    }
}

void r_window::on_pushButton_clicked()
{
    QSettings settings(QCoreApplication::applicationDirPath()+"/sb2.ini", QSettings::IniFormat);
    qDebug()<<QCoreApplication::applicationDirPath();
    int rowCount = ui->tableWidget->rowCount();

    //settings.remove("");
    QLineEdit *line1 ;
    QLineEdit *line2 ;
    QLineEdit *line3 ;
    QPushButton *button;
    // 保存表格数据
    for (int row = 0; row < rowCount; ++row) {
        line1 = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(row,0));
        line2 = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(row,2));
        line3 = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(row,3));
        button = qobject_cast<QPushButton*>(ui->tableWidget->cellWidget(row,1));
        settings.setValue(QString("table/%1/%2").arg(row).arg(0), line1->text());
        settings.setValue(QString("table/%1/%2").arg(row).arg(1), button->text());
        settings.setValue(QString("table/%1/%2").arg(row).arg(2), line2->text());
        settings.setValue(QString("table/%1/%2").arg(row).arg(3), line3->text());

    }
}

void r_window::on_start_clicked()
{
    QList<QByteArray> send;
    QList<QList<QString>> sll;
    QList<int> port_no;
    QList<QString> sl;
    QString s;
    QByteArray temp;
    QList<int> time;
    QLineEdit *line1 ;
    QLineEdit *line2 ;
    QLineEdit *line3 ;
    QComboBox *box;
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        line1 = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(i,0));
        line2 = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(i,2));
        line3 = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(i,3));
        box = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i,4));
        if(line2->text().toInt()!=0){
            sl.clear();
            sl.append(line1->text());
            sl.append(line2->text());
            sl.append(line3->text());
            sl.append(box->currentText());
            sll.append(sl);
        }
    }
    std::sort(sll.begin(),sll.end(),[](QList<QString> a,QList<QString> b){
        return a[1].toUInt()<b[1].toUInt();
    });
    for(int i=0;i<sll.size();i++){
        s=sll[i][0];
        temp.clear();
        s.remove(' ');
        utils::instance().Hexstring_to_bytes(s,temp);
        send.append(temp);
        time.append(sll[i][2].toInt());
        port_no.append(ports.indexOf(sll[i][3]));
    }
    qDebug()<<send;
    qDebug()<<time;
    qDebug()<<port_no;
    emit sig_cycle_send(send,time,port_no);
}

void r_window::on_stop_clicked()
{
    emit sig_stop_cycle();
}
void Serialport_manager::cycle_send(const QList<QByteArray> &send,const QList<int> &time,const QList<int> &port_no){
    m_datalist=send;
    m_delay_list=time;
    m_index=0;
    m_port_no_list=port_no;
    emit sig_send_next();
}
void Serialport_manager::send_next(){
    if(m_index>=m_datalist.size()){
        m_index=0;
    }
    if (m_index < m_datalist.size()) {
        // 发送数据
        QByteArray data = m_datalist[m_index];

        serial_port* m_pQserialport=serial_pool[m_port_no_list[m_index]];
        emit sig_send_parse(data);
        QMetaObject::invokeMethod(m_pQserialport, "send_data",
                                  Qt::QueuedConnection, Q_ARG(QByteArray, data));

        qDebug() <<"com"<<m_port_no_list[m_index]<< "Sent:" << data;

        // 设置下一个延时
        if (m_index < m_delay_list.size()) {
            mp_send_timer->start(m_delay_list[m_index]);
            //emit sig_send_log(data.toHex(' '));
        }

        m_index++;

    }
}
Serialport_manager::Serialport_manager(){
    mp_send_timer=new QTimer();
    connect(mp_send_timer,&QTimer::timeout,this,&Serialport_manager::send_next);

}
Serialport_manager::~Serialport_manager(){
    for(auto pthread:thread_pool){
        pthread->quit();
        pthread->wait();

    }
    for(auto serial:serial_pool){
        serial->deleteLater();
    }
}
void Serialport_manager::stop_send(){
    this->mp_send_timer->stop();
    m_datalist.clear();
    m_delay_list.clear();
    m_port_no_list.clear();

}
parser::message_type parser::type_judge(QByteArray array){
    quint16 crc;
    utils::instance().number(crc,array,array.size()-2);
    if(crc==utils::instance().calc_crc16(array.mid(0,array.size()-2))){
        return parser::MODBUS;
    }
    return parser::BLUETOOTH;
}
void parser::parse_modbus(QByteArray array) {
    emit sig_log("modbus:");
    quint8 opcode;

    utils::instance().number(opcode, array, 5);
    switch (opcode) {
    case 1:
        emit sig_log("启动车辆"); // 使用新的信号名
        break;
    case 2:
        emit sig_log("熄火车辆");
        break;
    case 3:
        emit sig_log("车辆设防");
        break;
    case 4:
        emit sig_log("车辆撤防");
        break;
    case 5:
        emit sig_log("开启桶锁");
        break;
    case 6:
        emit sig_log("鸣笛寻车");
        break;
    case 7:
        emit sig_log("开龙头锁");
        break;
    case 8:
        emit sig_log("关龙头锁");
        break;
    case 9:
        emit sig_log("进入输密码状态");
        break;
    case 10:
        emit sig_log("退出输密码状态");
        break;
    case 11:
        emit sig_log("进入学卡");
        break;
    case 12:
        emit sig_log("退出学卡");
        break;
    case 13:
        emit sig_log("开启扫卡");
        break;
    case 14:
        emit sig_log("关闭扫卡1");
        break;
    default:
        emit sig_log("未知指令");
        break;
    }

};
void parser::parse_bluetooth(QByteArray array) {
    quint8 type;
    utils::instance().number(type, array, 2);
    emit sig_log("蓝牙:");

    if (type == 1) {
        emit sig_log("嵌套");
        parse_modbus(array.mid(3, array.size() - 5));
    } else {
        if (type == 2) {
            emit sig_log("动作下发");
            quint8 op2;
            utils::instance().number(op2, array, 3);
            switch (op2) {
            case 0x01:
                emit sig_log("启动车辆");
                break;
            case 0x02:
                emit sig_log("熄火车辆");
                break;
            case 0x03:
                emit sig_log("车辆设防");
                break;
            case 0x04:
                emit sig_log("车辆撤防");
                break;
            case 0x05:
                emit sig_log("开鞍座锁");
                break;
            case 0x06:
                emit sig_log("鸣笛寻车");
                break;
            default:
                emit sig_log("未知的动作码");
                break;
            }
        } else if (type == 3) {
            emit sig_log("参数下发");
        } else if (type == 4) {
            emit sig_log("查询");
        } else if (type == 5) {
            emit sig_log("事件上报");
            quint8 op2;
            utils::instance().number(op2, array, 3);
            switch (op2) {
            case 0x01:
                emit sig_log("车辆已启动");
                break;
            case 0x02:
                emit sig_log("车辆已熄火");
                break;
            case 0x03:
                emit sig_log("用户单击了车辆的一键启动按钮");
                break;
            case 0x04:
                emit sig_log("用户双击了车辆的一键启动按钮");
                break;
            case 0x05:
                emit sig_log("用户长按了车辆的一键启动按钮");
                break;
            case 0x06:
                emit sig_log("车辆有轮动");
                break;
            case 0x07:
                emit sig_log("机械钥匙插入车辆，并处于启动车辆位置");
                break;
            case 0x08:
                emit sig_log("机械钥匙插入车辆，并处于车辆熄火位置");
                break;
            default:
                emit sig_log("未知的状态码");
                break;
            }
        } else if (type == 6) {
            emit sig_log("数据上报");
            quint8 op2;
            utils::instance().number(op2, array, 3);
            switch (op2) {
            case 0x01:
                emit sig_log("手动输入密码");
                break;
            case 0x02:
                emit sig_log("NFC密码");
                break;
            default:
                emit sig_log("未知的数据上报类型");
                break;
            }
        } else {
            emit sig_log("未知的蓝牙类型");
        }
    }
}
bool parser::need_push(QByteArray arr){
    return true;
}
void parser::parse_wrapper(QByteArray rec){

    emit sig_send_log(rec.toHex(' '));
    emit sig_log(utils::instance().current_time());
    if(need_push(rec)){
        send_queue.push_back(rec);
    }
    parser::message_type type=type_judge(rec);
    if(type==parser::MODBUS){
        parse_modbus(rec);
    }
    else{
        parse_bluetooth(rec);
    }
}
void r_window::log(QString s){
    ui->send_log->append(s);
}
void r_window::rec_log(QString s){
    ui->receive_log->append(s);
}
void r_window::send_log(QString s){
    ui->log_info->append(s);
}
parser::~parser(){
    send_queue.clear();
}
void parser::parse_bluetooth_res(QByteArray send,QByteArray rec){
    quint8 func1,func2;
    sig_log1("蓝牙");
    utils::instance().number(func1,send,2);
    utils::instance().number(func2,rec,2);
    if((func1+128)!=func2){
        sig_log1("类型不匹配");
    }
    utils::instance().number(func1,rec,2);
    if(func1==0x82){
        utils::instance().number(func2,rec,4);
        if(func2==0x00){
            sig_log1("成功");
        }
        else{
            sig_log1("失败");
        }
        return;
    }
    else if(func1==0x83){
        utils::instance().number(func2,rec,4);
        if(func2==0x00){
            sig_log1("成功");
        }
        else{
            sig_log1("失败");
        }
        return;
    }
    else if(func1==0x84){
        utils::instance().number(func2,rec,4);
        if(func2==0x00){
            sig_log1("启动");
        }
        else{
            sig_log1("熄火");
        }
        return;
    }
    else if(func1==0x86){
        utils::instance().number(func2,rec,4);
        if(func2==0x00){
            sig_log1("成功");
        }
        else{
            sig_log1("失败");
        }
        return;
    }

};
void parser::parse_modbus_res(QByteArray send,QByteArray rec){
    quint8 func1,func2;
    utils::instance().number(func1,send,1);
    utils::instance().number(func2,rec,1);
    if(func1!=func2){
        if(func2==0x86){
            sig_log1("动作1下发失败");
            return;
        }
        if(func2==0x90){
            sig_log1("参数下发失败");
            return;
        }
        if(func2==0x13){
            sig_log1("查询失败");
            return;
        }
        sig_log1("功能码不匹配");
        return;
    }
    else{
        sig_log1("成功");
    }

};
void parser::rec_parse_wrapper(QByteArray rec){
    if(send_queue.empty()){
        return;
    }
    QByteArray send=send_queue.front();
    send_queue.pop_front();

    if(!(type_judge(send)==type_judge(rec))){
        sig_log1("类型不匹配");
        return;
    }
    if(parser::MODBUS==type_judge(send)){
        parse_modbus_res(send,rec);
    }
    else{
        parse_bluetooth_res(send,rec);
    }
};
void r_window::log1(QString s){
    ui->log1->append(s);
}
