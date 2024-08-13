#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = nullptr);
    ~Form();


private slots:
    void on_switch_2_clicked();
signals:
    void sig_switch(int baud,int parity,int stop,int data);

protected:
    Ui::Form *ui;

};

#endif // FORM_H
