#include "setting_text.h"
#include "ui_setting_text.h"

setting_text::setting_text(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::setting_text)
{
    ui->setupUi(this);
    connect(ui->set_button,&QPushButton::clicked,this,[this]{text=ui->lineEdit->text();
        this->hide();
        ui->lineEdit->clear();

        emit sig_set(*i,text);
    });
}

setting_text::~setting_text()
{
    delete ui;
}
