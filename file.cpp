#include "file.h"
#include "ui_file.h"

file::file(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::file)
{
    ui->setupUi(this);
    ui->progressBar->setValue(0);
}

file::~file()
{
    delete ui;
}

void file::on_response_intervals_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    int retry=ui->retry_times->text().toInt();
    int timeout=ui->response_timeout->text().toInt();
    int intervals=ui->response_intervals->text().toInt();
    emit sig_file_serialport_setting(retry,timeout,intervals);
}

void file::on_response_timeout_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    int retry=ui->retry_times->text().toInt();
    int timeout=ui->response_timeout->text().toInt();
    int intervals=ui->response_intervals->text().toInt();
    emit sig_file_serialport_setting(retry,timeout,intervals);
}

void file::on_retry_times_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    int retry=ui->retry_times->text().toInt();
    int timeout=ui->response_timeout->text().toInt();
    int intervals=ui->response_intervals->text().toInt();
    emit sig_file_serialport_setting(retry,timeout,intervals);
}

void file::on_import_button_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "打开bin文件", QCoreApplication::applicationDirPath(),  "Allfile(*.*);;");
    qDebug()<<fileName;
    ui->filepath_edit->setText(fileName);
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        qWarning() << QString(tr("[%1] Failed to open file: %2 "))
                      .arg(metaObject()->className())
                      .arg(fileName);
        return;
    }


    QByteArray m_file = file.readAll();
    quint32 m_fileCrc32=utils::instance().crc32(m_file);
    quint32 size=m_file.size();
    log("CRC32:"+QString::number(m_fileCrc32,16)+",字节数："+QString::number(size));


}

void file::on_send_button_clicked()
{

    emit sig_open_file(ui->filepath_edit->text());
}

void file::on_slave_id_editingFinished()
{
    quint8 u8=ui->slave_id->text().toUInt();
    quint32 mcode=ui->manufacture->text().toUInt();
    quint32 version=ui->version->text().toUInt();
    emit sig_set_parse(u8,mcode,version);
}

void file::on_manufacture_editingFinished()
{
    quint8 u8=ui->slave_id->text().toUInt();
    quint32 mcode=ui->manufacture->text().toUInt();
    quint32 version=ui->version->text().toUInt();
    emit sig_set_parse(u8,mcode,version);
}

void file::on_version_editingFinished()
{
    quint8 u8=ui->slave_id->text().toUInt();
    quint32 mcode=ui->manufacture->text().toUInt();
    quint32 version=ui->version->text().toUInt();
    emit sig_set_parse(u8,mcode,version);
}

void file::on_pushButton_clicked()
{
    emit sig_close_bin();
    log("bin 模式已关闭");
}
void file::on_progressBarDownload(int value){
    ui->progressBar->setValue(value);
}
void file::log_succeed(){
    ui->download_info->append("下载成功");
}
void file::log(QString s){
    QDateTime currentDateTime = QDateTime::currentDateTimeUtc();
    QString formattedDateTime = currentDateTime.toString("hh:mm:ss.zzz");

    ui->download_info->append(formattedDateTime+"--"+s);
}

void file::on_pushButton_2_clicked()
{
    ui->download_info->clear();
}
