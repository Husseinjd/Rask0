#ifndef PASSWORDPROMPT_H
#define PASSWORDPROMPT_H

#include <QDialog>

namespace Ui {
class passwordPrompt;
}

class passwordPrompt : public QDialog
{
    Q_OBJECT

public:
    explicit passwordPrompt(QWidget *parent = 0);
    ~passwordPrompt();
    void set_FileLocation(QString l);
    void set_ky(qint64 ky);
void set_FileLocation();
private slots:
    void on_pushButton_2_cancel_prompt_clicked();
    void on_pushButton_enter_prompt_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private:
    Ui::passwordPrompt *ui;
};

#endif // PASSWORDPROMPT_H
