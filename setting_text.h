#ifndef SETTING_TEXT_H
#define SETTING_TEXT_H

#include <QWidget>

namespace Ui {
class setting_text;
}

class setting_text : public QWidget
{
    Q_OBJECT

public:
    explicit setting_text(QWidget *parent = nullptr);
    ~setting_text();
    QString text;
    int* i;
signals:
    void sig_set(int i,QString text);

private:
    Ui::setting_text *ui;
};

#endif // SETTING_TEXT_H
