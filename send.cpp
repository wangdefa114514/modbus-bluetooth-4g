#include "send.h"
#include "ui_send.h"

send::send(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::send)
{
    ui->setupUi(this);
    mp_text=new setting_text();
    mp_text->i=&row;
    ui->tableWidget ->setRowCount(100);
    ui->tableWidget->setColumnCount(4);
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




    }
    ui->tableWidget->setColumnWidth(0,300);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,50);
    ui->tableWidget->setColumnWidth(3,50);
    connect(this,&send::sig_set,this,&send::set);
    connect(this,&send::sig_send,this,&send::sending);
    connect(mp_text,&setting_text::sig_set,this,&send::set_text_handler);
    loadTableData();
}

send::~send()
{
    delete ui;
}
void send::set(int i){
    qDebug()<<"setting column "<<i;
    row=i;
    mp_text->show();
}
void send::sending(int i){
    qDebug()<<"sending "<<i;
    QByteArray send;
    QLineEdit *line = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(i,0));
    QString s=line->text();
    utils::instance().Hexstring_to_bytes(s,send);
    emit sig_send_single(send);
}
void send::set_text_handler(int i,QString s){
    QPushButton *button = qobject_cast<QPushButton*>(ui->tableWidget->cellWidget(i,1));
    button->setText(s);
}


void send::on_start_clicked()
{
    QList<QByteArray> send;
    QList<QList<QString>> sll;
    QList<QString> sl;
    QString s;
    QByteArray temp;
    QList<int> time;
    QLineEdit *line1 ;
    QLineEdit *line2 ;
    QLineEdit *line3 ;
    for(int i=0;i<ui->tableWidget->rowCount();i++){
        line1 = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(i,0));
        line2 = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(i,2));
        line3 = qobject_cast<QLineEdit*>(ui->tableWidget->cellWidget(i,3));
        if(line2->text().toInt()!=0){
            sl.clear();
            sl.append(line1->text());
            sl.append(line2->text());
            sl.append(line3->text());
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
    }
    qDebug()<<send;
    qDebug()<<time;
    emit sig_cycle_send(send,time);

}

void send::on_stop_clicked()
{
    emit sig_stop_cycle();
}

void send::on_send_destroyed()
{
    qDebug()<<"ready to save settings";
}

void send::on_pushButton_clicked()
{
    QSettings settings(QCoreApplication::applicationDirPath()+"/sb.ini", QSettings::IniFormat);
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

void send::loadTableData()
{

    QString filePath=QCoreApplication::applicationDirPath()+"/sb.ini";
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
