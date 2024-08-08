#ifndef FILE_H
#define FILE_H

#include <QWidget>
#include <QDebug>
#include <QFileDialog>
#include <QDateTime>
#include "utils.h"
namespace Ui {
class file;
}

class file : public QWidget
{
    Q_OBJECT

public:
    explicit file(QWidget *parent = nullptr);
    ~file();
signals:
    void sig_file_serialport_setting(int retry,int timeout,int interval);
    void sig_open_file(QString file_path);


public slots:
    void on_response_intervals_valueChanged(int arg1);

    void on_response_timeout_valueChanged(int arg1);

    void on_retry_times_valueChanged(int arg1);

    void on_import_button_clicked();

    void on_send_button_clicked();

    void on_slave_id_editingFinished();

    void on_manufacture_editingFinished();

    void on_version_editingFinished();
    void on_pushButton_clicked();
    void on_progressBarDownload(int value);
    void log_succeed();
    void log(QString s);

signals:
    void sig_set_parse(quint8,quint32,quint32);
    void sig_close_bin();

private slots:
    void on_pushButton_2_clicked();

private:
    Ui::file *ui;
};

#endif // FILE_H
