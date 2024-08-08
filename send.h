#ifndef SEND_H
#define SEND_H

#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QLineEdit>
#include <QEvent>
#include "setting_text.h"
#include "utils.h"
#include <QSettings>
#include <QCoreApplication>
#include <QFileInfo>
namespace Ui {
class send;
}

class send : public QWidget
{
    Q_OBJECT

public:
    explicit send(QWidget *parent = nullptr);
    void loadTableData();
    ~send();
    int row=0;
public slots:
    void set(int i);
    void sending(int i);
    void set_text_handler(int i,QString text);

signals:
    void sig_set(int row);
    void sig_send(int row);
    void sig_send_single(const QByteArray &send);
    void sig_cycle_send(const QList<QByteArray> &send,const QList<int> &time);
    void sig_stop_cycle();
private slots:
    void on_start_clicked();

    void on_stop_clicked();

    void on_send_destroyed();

    void on_pushButton_clicked();

private:
    Ui::send *ui;
    setting_text *mp_text=nullptr;

    bool done=false;
};
class QLineEditWithDoubleClick : public QLineEdit
{
    Q_OBJECT
public:
    QLineEditWithDoubleClick(QWidget *parent = nullptr) : QLineEdit(parent)
    {
        installEventFilter(this);
    }

protected:
    bool eventFilter(QObject *obj, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonDblClick && obj == this) {
            emit doubleClicked();
            return true;
        } else {
            return QLineEdit::eventFilter(obj, event);
        }
    }

signals:
    void doubleClicked();
private:

};
#endif // SEND_H
