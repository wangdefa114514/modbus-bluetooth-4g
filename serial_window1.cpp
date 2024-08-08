#include "serial_window1.h"
#include "ui_serial_window1.h"


serial_window1::serial_window1(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::serial_window1)
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

    mp_serial_port2=new serial_port2();
    mp_serial_port2->moveToThread(&m_serial_thread);
    connect(&m_serial_thread,&QThread::started,mp_serial_port2,&serial_port2::satrt);
    connect(mp_serial_port2,&serial_port2::sig_set_com_names,this,&serial_window1::set_com_names);
    connect(mp_serial_port2,&serial_port::sig_send_log,this,&serial_window1::send_log);
    connect(mp_serial_port2,&serial_port::sig_receive_log,this,&serial_window1::receive_log);
    connect(this,&serial_window1::sig_open_serialport,mp_serial_port2,&serial_port2::open_serial_port);
    connect(ui->close_port,&QPushButton::clicked,mp_serial_port2,&serial_port2::close_serial_port);
    connect(mp_serial_port2,&serial_port::sig_log,this,&serial_window1::log);

    generator=QRandomGenerator::global();
    setWindowTitle("芝士雪豹");
   // lay->setSizeConstraint(QLayout::SetFixedSize);
   // lay->addWidget(ui->reg_table,0);

    // 创建一个表格小部件

    ui->reg_table->setRowCount(53);
    ui->reg_table->setColumnCount(4);
    ui->reg_table->setHorizontalHeaderLabels(QStringList() << "寄存器地址" << "功能" << "值" << "备注");

    ui->table_06->setRowCount(28);
    ui->table_06->setColumnCount(3);
    ui->table_06->setHorizontalHeaderLabels(QStringList()<<"寄存器地址" << "功能" << "值");
    ui->switch_table_05->setRowCount(26);
    ui->switch_table_05->setColumnCount(4);
    ui->switch_table_05->setHorizontalHeaderLabels(QStringList()<<"开关地址" << "功能" << "值"<<"备注");
    ui->reg_table_05->setRowCount(50);
    ui->reg_table_05->setColumnCount(3);
    ui->reg_table_05->setHorizontalHeaderLabels(QStringList()<<"寄存器地址" << "功能" << "值");
    // 调整表格的大小策略
    ui->reg_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->switch_quantity_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->table_06->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->switch_table_05->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->reg_table_05->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //ui->reg_table->verticalHeader()->setVisible(false); // 可选：隐藏垂直标题
    //ui->switch_quantity_table->verticalHeader()->setVisible(false);
    // 将表格添加到中央小部件

    const QStringList functions{"电池包实际电电压","电芯数量","SOC","剩余容量","SOH","充电电流","电芯芯温度2","电芯芯温度1","主板温度","电芯1电压","电芯2电压","电芯3电压","电芯4电压","电芯5电压","电芯6电压","电芯7电压","电芯8电压","电芯9电压","电芯10电压","电芯11电压","电芯12电压","电芯13电压","电芯14电压","电芯15电压","电芯16电压","电芯17电压","电芯18电压","电芯19电压","电芯20电压","电芯21电压","电芯22电压","电芯23电压","电芯24电压","充电开关","放电开关","标称电压","电池类型","使用循环次数数","SOH电池健康度","故障码I","故障码II","扩展温度1","扩展温度2","均衡状态","单体最大压差","低温加热开关状态","电池标称容量","充电MOS电流","放电MOS电流","设备PN","设备密钥","固件版号","硬件版号"};
    const QStringList addrs{"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","21","22","23","24","25","26","27","28","29","30","31","32","100","101","102","103","104","105","106","107","108","109","110","111","112","113","114","115","1000","1008","1016","1018"};
    const QStringList functions2{"保留位","电芯压差过大","充电过流","放电过流","短路保护","充电高温保护","放电高温保护","充电低温保护","放电低温保护","充电MOS损坏","放电MOS损坏","内部通讯异常","过充电压保护1","过充电压保护2","过充电压保护3","过充电压保护4","过充电压保护5","过充电压保护6","过充电压保护7","过充电压保护8","过充电压保护9","过充电压保护10","过充电压保护11","过充电压保护12","过充电压保护13","过充电压保护14","过充电压保护15","过充电压保护16","过充电压保护17","过充电压保护18","过充电压保护19","过充电压保护20","过放电压保护1","过放电压保护2","过放电压保护3","过放电压保护4","过放电压保护5","过放电压保护6","过放电压保护7","过放电压保护8","过放电压保护9","过放电压保护10","过放电压保护11","过放电压保护12","过放电压保护13","过放电压保护14","过放电压保护15","过放电压保护16","过放电压保护17","过放电压保护18","过放电压保护19","过放电压保护20","过充电压保护21","过充电压保护22","过充电压保护23","过充电压保护24","过放电压保护21","过放电压保护22","过放电压保护23","过放电压保护24"};
    const QStringList annotations{"默认值为0","1表示压差过大","1表示过流","1表示过流","1表示短路保护","1表示充电高温保护","1表示放电高温保护","1表示充电低温保护","1表示放电低温保护","1表示损坏","1表示损坏","1表示异常","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过充电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护","1表示过放电压保护"};
    const QStringList functions3{"器件ID","APP版本号","BOOT版本号","硬件版本号","产品序列号","标定序列表","ABS状态指示","ABS使能控制","状态表1","状态表2","前轮轮速","后轮轮速","整车速度","后轮轮速传感器低边电压","前轮轮速传感器低边电压","功率电源电压","受控功率电源电压","12V电压","5V电压","ABS使能电压","刹车信号电压","电磁阀/电机高边电压","MCU内部温度","抽真空时间","注油时间","自动抽真空/注油控制","手动抽真空","手动注油"};
    const QStringList addrs3{"0","2","4","6","8","16","24","25","26","28","30","32","34","36","37","38","39","40","41","42","43","44","45","46","47","48","49","50"};
    const QStringList sfunction5{"转把故障","刹车故障","霍尔故障","控制器故障","欠压保护","过压保护","堵转保护","过流保护","巡航状态","防盗启动","刹车状态","保留","保留","保留","保留","保留","锂电模式","里程模式","TCS开关","HDC开关","HHC开关","P档开关","能量回收","远程修复","恢复出厂设置","里程清零"};
    const QStringList sannotation5{"0:无故障 1:故障","0:无故障 1:故障","0:无故障 1:故障","0:无故障 1:故障","0:正常 1:欠压","0:正常 1:过压","0:正常 1:堵转","0:正常 1:过流","0:未启动 1:启动","0:未启动 1:启动","0:未刹车 1:刹车中","保留","保留","保留","保留","保留","0:关闭 1:启用","0:关闭 1:启用","0:关闭 1:启用","0:关闭 1:启用","0:关闭 1:启用","0:关闭 1:启用","0:关闭 1:启用","0:关闭 1:启用","0:关闭 1:启用","0:关闭 1:启用"};
    const QStringList rfunction05{"保留","保留","保留","保留","保留","保留","保留","保留","保留","保留","母线电压","母线电流","电机转速","电控温度","车速","单次里程","总里程","保留","保留","保留","保留","控制器硬件版本号","控制器硬件版本号(固件版本)","电池设置-额定电压","电池设置-软欠压值","速度比例设置-1档","速度比例设置-2档","速度比例设置-3档","速度比例设置-4档","速度比例设置-5档","1档限流比例","2档限流比例","3档限流比例","4档限流比例","5档限流比例","相电流限流比例","降流温度（此功能不做）","停机温度（此功能不做）","低电量提醒","油门加速设置","推车速度比例","倒车速度比例","电刹力度","限速比例","刹车能量回收等级","速度显示比例","轮胎周长","保留","保留","电机极对数"};
    const QStringList raddr05{"0","1","2","3","4","5","6","7","8","9","10","11","12","13","14","15","16","17","18","19","20","25","26","50","51","52","53","54","55","56","57","58","59","60","61","62","63","64","65","66","67","68","69","70","71","72","73","74","75","76"};

    // 填充表格数据
    for (int row = 0; row < 53; ++row) {
        QTableWidgetItem *item1 = new QTableWidgetItem(functions[row]);
        QTableWidgetItem *item0 = new QTableWidgetItem(addrs[row]);
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(generator->bounded(255),10));
        QTableWidgetItem *item3 = new QTableWidgetItem(" ");
        ui->reg_table->setItem(row, 1, item1);
        ui->reg_table->setItem(row, 0, item0);
        ui->reg_table->setItem(row, 2, item2);
        ui->reg_table->setItem(row, 3, item3);
        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        item3->setFlags(item3->flags() & ~Qt::ItemIsEditable);
        item0->setToolTip(item0->text());
        item1->setToolTip(item1->text());
        item2->setToolTip(item2->text());
        item3->setToolTip(item3->text());
    }
    for (int row = 0; row < 50; ++row) {
        QTableWidgetItem *item1 = new QTableWidgetItem(rfunction05[row]);
        QTableWidgetItem *item0 = new QTableWidgetItem(raddr05[row]);
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(generator->bounded(255),10));
        ui->reg_table_05->setItem(row, 1, item1);
        ui->reg_table_05->setItem(row, 0, item0);
        ui->reg_table_05->setItem(row, 2, item2);

        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);

        item0->setToolTip(item0->text());
        item1->setToolTip(item1->text());
        item2->setToolTip(item2->text());

    }

    for (int row = 0; row < 26; ++row) {
        QTableWidgetItem *item1 = new QTableWidgetItem(sfunction5[row]);
        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(row));
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(generator->bounded(255),10));
        QTableWidgetItem *item3 = new QTableWidgetItem(sannotation5[row]);
        ui->switch_table_05->setItem(row, 1, item1);
        ui->switch_table_05->setItem(row, 0, item0);
        ui->switch_table_05->setItem(row, 2, item2);
        ui->switch_table_05->setItem(row, 3, item3);
        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        item3->setFlags(item3->flags() & ~Qt::ItemIsEditable);
        item0->setToolTip(item0->text());
        item1->setToolTip(item1->text());
        item2->setToolTip(item2->text());
        item3->setToolTip(item3->text());
    }


    for (int row = 0; row < 28; ++row) {
        QTableWidgetItem *item1 = new QTableWidgetItem(functions3[row]);
        QTableWidgetItem *item0 = new QTableWidgetItem(addrs3[row]);
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(generator->bounded(255),10));
        ui->table_06->setItem(row, 1, item1);
        ui->table_06->setItem(row, 0, item0);
        ui->table_06->setItem(row, 2, item2);

        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);

        item0->setToolTip(item0->text());
        item1->setToolTip(item1->text());
        item2->setToolTip(item2->text());

    }
    ui->switch_quantity_table->setRowCount(60);
    ui->switch_quantity_table->setColumnCount(4);
    ui->switch_quantity_table->setHorizontalHeaderLabels(QStringList() << "开关地址" << "功能" << "值" << "备注");
    for(int row=0;row<60;row++){
        QTableWidgetItem *item1 = new QTableWidgetItem(functions2[row]);
        QTableWidgetItem *item3 = new QTableWidgetItem(annotations[row]);
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(generator->bounded(2)));
        QTableWidgetItem *item0 = new QTableWidgetItem(QString::number(row));
        ui->switch_quantity_table->setItem(row, 1, item1);
        ui->switch_quantity_table->setItem(row, 0, item0);
        ui->switch_quantity_table->setItem(row, 2, item2);
        ui->switch_quantity_table->setItem(row, 3, item3);
        item0->setFlags(item0->flags() & ~Qt::ItemIsEditable);
        item1->setFlags(item1->flags() & ~Qt::ItemIsEditable);
        item3->setFlags(item3->flags() & ~Qt::ItemIsEditable);
        item0->setToolTip(item0->text());
        item1->setToolTip(item1->text());
        item2->setToolTip(item2->text());
        item3->setToolTip(item3->text());
    }

    m_p_parse=new parse1();
    m_p_parse->moveToThread(&m_parse_thread);



    connect(&m_parse_thread,&QThread::started,m_p_parse,&parse1::start);
    connect(mp_serial_port2,&serial_port::sig_log,this,&serial_window1::log);
    connect(m_p_parse,&parse1::sig_log_parse1,this,&serial_window1::log);
    connect(mp_serial_port2,&serial_port2::sig_parse_reveive,m_p_parse,&parse1::parse1_receive);

    connect(m_p_parse,&parse1::sig_send_reg,mp_serial_port2,&serial_port::send_data);
    connect(m_p_parse,&parse1::sig_send_switch,mp_serial_port2,&serial_port::send_data);
    connect(mp_serial_port2,&serial_port2::sig_get_table,this,&serial_window1::get_table);
    connect(this->ui->test,&QPushButton::clicked,mp_serial_port2,&serial_port2::show_table);
    connect(m_p_parse,&parse1::sig_get_table_parse1,this,&serial_window1::get_table_parse);
    connect(this->ui->addr_set_button,&QPushButton::clicked,this,&serial_window1::addr_set_handler);
    connect(this,&serial_window1::sig_set,m_p_parse,&parse1::set);


    m_serial_thread.start();
    m_parse_thread.start();
}

serial_window1::~serial_window1()
{
    delete ui;
}
serial_port2::serial_port2(){

}
void serial_window1::set_com_names(QVector<QString> port_names){
    if(port_names!=m_serialPort_names){
        this->m_serialPort_names=port_names;
        ui->port_names->clear();
        for(auto name : port_names){
            ui->port_names->addItem(name);
        }
    }
}

void serial_window1::on_open_port_clicked()
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

void serial_window1::send_log(QString s){
    ui->send_info->append(s);
}
void serial_window1::receive_log(QString s){
    ui->receive_info->append(s);
}

void serial_window1::log(QString s){
    this->ui->log_info->append(s);
}

void serial_window1::on_clear_send_info_clicked()
{
    ui->send_info->clear();
}

void serial_window1::on_clear_receive_info_clicked()
{
    ui->receive_info->clear();
}
void serial_port2::send_data(const QByteArray &data_to_send){
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
void serial_window1::get_table(){
    mp_serial_port2->reg_table=this->ui->reg_table;
    mp_serial_port2->switch_table=this->ui->switch_quantity_table;
    mp_serial_port2->table_loaded=true;
}
void serial_window1::get_table_parse(){
    m_p_parse->reg_table=this->ui->reg_table;
    m_p_parse->switch_table=this->ui->switch_quantity_table;
    m_p_parse->table_06=this->ui->table_06;
    m_p_parse->stable_05=ui->switch_table_05;
    m_p_parse->rtable_05=ui->reg_table_05;

}
void serial_window1::addr_set_handler(){
    this->ui->battery_addr_label->setText(ui->lineEdit_2->text());
    this->ui->label_4->setText(ui->lineEdit_6->text());
    QStringList sl;
    sl.append(ui->lineEdit_2->text());
    sl.append(ui->lineEdit_3->text());
    sl.append(ui->lineEdit_4->text());
    sl.append(ui->lineEdit_5->text());
    sl.append(ui->lineEdit_6->text());
    sl.append(ui->lineEdit_7->text());
    emit sig_set(sl);
}

void serial_port2::show_table(){
    qDebug()<<"show table running";
    table_loaded=false;
    emit sig_log( reg_table->item(3,2)->text());
}
void serial_port2::satrt(){
    m_pQserialport=new QSerialPort();
    m_pPort_scan_timer=new QTimer();
    mp_send_timer=new QTimer();
    QObject::connect(this->m_pPort_scan_timer,&QTimer::timeout,this,&serial_port::scan_port);
    connect(mp_send_timer,&QTimer::timeout,this,&serial_port::send_next);
    QObject::connect(m_pQserialport, &QSerialPort::readyRead, this, &serial_port2::parse_receive_handler);
    m_pPort_scan_timer->start(1000);
}
void serial_port2::parse_receive_handler(){
    QByteArray receive;
    receive=m_pQserialport->readAll();
    emit sig_receive_log(receive.toHex(' '));
    emit sig_parse_reveive(receive);
}

void serial_window1::on_pushButton_3_clicked()
{
    for (int row = 0; row < ui->reg_table->rowCount(); ++row) {
       ui->reg_table->item(row,2)->setText(QString::number(generator->bounded(255),10));
    }

    for (int row = 0; row < ui->switch_quantity_table->rowCount(); ++row) {
       ui->switch_quantity_table->item(row,2)->setText(QString::number(generator->bounded(2)));
    }
}

void serial_window1::on_pushButton_4_clicked()
{
    for (int row = 0; row < ui->reg_table->rowCount(); ++row) {
       ui->reg_table->item(row,2)->setText(QString::number(0));
    }

    for (int row = 0; row < ui->switch_quantity_table->rowCount(); ++row) {
       ui->switch_quantity_table->item(row,2)->setText(QString::number(0));
    }
}

void serial_window1::on_pushButton_5_clicked()
{
    for (int row = 0; row < ui->table_06->rowCount(); ++row) {
       ui->table_06->item(row,2)->setText(QString::number(generator->bounded(255),10));
    }
}

void serial_window1::on_pushButton_6_clicked()
{
    for (int row = 0; row < ui->table_06->rowCount(); ++row) {
       ui->table_06->item(row,2)->setText(QString::number(0));
    }
}

void serial_window1::on_pushButton_7_clicked()
{
    for (int row = 0; row < ui->reg_table_05->rowCount(); ++row) {
       ui->reg_table_05->item(row,2)->setText(QString::number(generator->bounded(255),10));
    }

    for (int row = 0; row < ui->switch_table_05->rowCount(); ++row) {
       ui->switch_table_05->item(row,2)->setText(QString::number(generator->bounded(2)));
    }
}

void serial_window1::on_pushButton_8_clicked()
{
    for (int row = 0; row < ui->reg_table_05->rowCount(); ++row) {
       ui->reg_table_05->item(row,2)->setText(QString::number(0));
    }

    for (int row = 0; row < ui->switch_table_05->rowCount(); ++row) {
       ui->switch_table_05->item(row,2)->setText(QString::number(0));
    }
}
