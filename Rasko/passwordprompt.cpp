#include "passwordprompt.h"
#include "ui_passwordprompt.h"
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDir>
#include <QDesktopServices>
#include "simplecrypt.h"
#include <QMouseEvent>

SimpleCrypt crypto3;
QPoint mpos10 ;
bool opened_data1 = false;
QString  full_data_location1 ;
passwordPrompt::passwordPrompt(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::passwordPrompt)
{
    ui->setupUi(this);
    ui->label_wrong_pass->setText("");
    ui->pass_prompt_field->setPlaceholderText("Password");
}

passwordPrompt::~passwordPrompt()
{
    delete ui;
}

void passwordPrompt::set_ky(qint64 ky){
crypto3.setKey(ky); ;
}
void passwordPrompt::set_FileLocation(QString l){
    full_data_location1 = l ;
}

void passwordPrompt::mousePressEvent(QMouseEvent *event){
    mpos10 = event->pos();
}

void passwordPrompt::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        QPoint diff = event->pos() - mpos10;
        QPoint newpos = this->pos() + diff;
        this->move(newpos);
    }
}

void passwordPrompt::on_pushButton_2_cancel_prompt_clicked()
{
    exit(0);
}

void passwordPrompt::on_pushButton_enter_prompt_clicked()
{
    QString pass_account = ui->pass_prompt_field->text();
    QFile file(full_data_location1);
                if(!file.open(QIODevice::ReadOnly)) {
                    QMessageBox::information(0, "error", file.errorString());
                }
                else {
                        QTextStream in(&file);
                        QString line = crypto3.decryptToString(in.readLine());
                        QStringList words = line.split(",");
                         if(words.at(1) == pass_account){
                            this->close();
                         }

                       else {
                            ui->label_wrong_pass->setText("<font color=red> Incorrect Password, try again </font>");
                            ui->pass_prompt_field->setText("");
                           }

                        }
}

