#include "tables.h"
#include "ui_tables.h"

tables::tables(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tables)
{
    ui->setupUi(this);
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
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(0));
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
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(0));
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
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(0));
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
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(0));
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
        QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(0));
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

}

tables::~tables()
{
    delete ui;
}

void tables::on_addr_set_button_clicked()
{
    this->ui->battery_addr_label->setText(ui->lineEdit_2->text());
    this->ui->label_4->setText(ui->lineEdit_6->text());
    this->ui->label_11->setText(ui->lineEdit_5->text());
}
void tables::update_table(QByteArray rec,quint16 start){
    rec=utils::instance().decode(rec);
    quint8 addr;
    utils::instance().number(addr,rec,0);
    quint8 op_code;
    utils::instance().number(op_code,rec,1);
    quint8 byte_nums;
    utils::instance().number(byte_nums,rec,2);
    QTableWidget* table=nullptr;
    QByteArray temp;
    temp=rec.mid(3,rec.size()-5);
    if(addr==ui->battery_addr_label->text().toUInt()){
        if(op_code==0x01){
            table=ui->switch_quantity_table;
        }
        else{
            table=ui->reg_table;
        }

    }
    if(addr==ui->label_4->text().toUInt()){
        if(op_code==0x03){
            table=ui->table_06;
        }

    }
    if(addr==ui->label_11->text().toUInt()){
        if(op_code==0x01){
            table=ui->switch_table_05;
        }
        else{
            table=ui->reg_table_05;
        }
    }
    if(table==nullptr){
        return;
    }
    if(op_code==0x01){
        int start_idx=0;
        for(int i=0;i<table->rowCount();i++,start_idx++){
            if(table->item(i,0)->text().toUInt()==start){
                break;
            }
        }
        QString s;
        for(int i=0;i<byte_nums;i++){
            s+=byteToBinaryString(temp[i]);
        }
        for(int i=0;i<byte_nums*8;i++){
            table->item(start_idx+i,2)->setText(QString(s[i]));
        }

    }
    if(op_code==0x03){
        int start_idx=0;
        for(int i=0;i<table->rowCount();i++,start_idx++){
            if(table->item(i,0)->text().toUInt()==start){
                break;
            }

        }
        int reg_nums=byte_nums/2;
        int reg_len=0;
        int i=start_idx;//col
        int front =0;//front of byte array
        while(reg_nums){

            if(i+1<table->rowCount()){
                reg_len=table->item(i+1,0)->text().toInt()-table->item(i,0)->text().toInt();
                if(reg_len==1){
                    quint16 v16;
                    utils::instance().number(v16,temp,front);
                    table->item(i,2)->setText(QString::number(static_cast<qint16>(v16)));

                    front+=2;
                    reg_nums-=1;

                }
                else if(reg_len==2){
                    quint32 v32;
                    utils::instance().number(v32,temp,front);
                    table->item(i,2)->setText(QString::number(static_cast<qint32>(v32)));

                    reg_nums-=2;
                    front+=4;
                }
                else{
                    table->item(i,2)->setText(temp.mid(front,reg_len*2).toHex(' '));

                    reg_nums-=reg_len;
                    front+=reg_len*2;
                }

            }
            else{
                table->item(i,2)->setText("HEX:"+temp.mid(front,temp.size()-front).toHex(' '));

                return;
            }
            i+=1;
        }
    }
}
QString tables::byteToBinaryString(quint8 byte) {
    QString binaryString;
    for (int i = 7; i >= 0; --i) {
        binaryString.append((byte & (1 << i)) ? '1' : '0');
    }
    std::reverse(binaryString.begin(),binaryString.end());
    return binaryString;
}

void tables::on_pushButton_clicked()
{
    QTableWidget* table;
    table=ui->reg_table;
    for(int i=0;i<table->rowCount();i++){
        table->item(i,2)->setText(QString::number(0));
    }
    table=ui->switch_quantity_table;
    for(int i=0;i<table->rowCount();i++){
        table->item(i,2)->setText(QString::number(0));
    }
}

void tables::on_pushButton_2_clicked()
{
    QTableWidget* table;
    table=ui->table_06;
    for(int i=0;i<table->rowCount();i++){
        table->item(i,2)->setText(QString::number(0));
    }
}

void tables::on_pushButton_3_clicked()
{
    QTableWidget* table;
    table=ui->reg_table_05;
    for(int i=0;i<table->rowCount();i++){
        table->item(i,2)->setText(QString::number(0));
    }
    table=ui->switch_table_05;
    for(int i=0;i<table->rowCount();i++){
        table->item(i,2)->setText(QString::number(0));
    }
}
