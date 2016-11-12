#include "startup_window.h"
#include "ui_startup_window.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QtNetwork/QNetworkInterface>
#include <QDir>
#include <QDesktopServices>
#include "simplecrypt.h"




QString full_data_location2 ;
bool opened_data2 ;
bool pass_on_startup = false ;
bool file_corrupt_edited = false;
QString uniq_string ;

SimpleCrypt crypto2;
startup_window::startup_window(QWidget *parent) :

    QDialog(parent),
    ui(new Ui::startup_window)
{


    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        // Return only the first non-loopback MAC Address
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
            uniq_string = netInterface.hardwareAddress();
    }

    ui->setupUi(this);

}


void startup_window::set_ky(qint64 ky){
    crypto2.setKey(ky); ;
}

startup_window::~startup_window()
{
    delete ui;
}

void startup_window::set_FileLocation(QString l){
    full_data_location2 = l ;

}

void startup_window::on_pushButton_cancel_clicked()
{
    exit(1);
}

void startup_window::file_edited_warning(){
    file_corrupt_edited = true;
}

//frameless mouse movemenet code
void startup_window::mousePressEvent(QMouseEvent *event) {
    m_nMouseClick_X_Coordinate = event->x();
    m_nMouseClick_Y_Coordinate = event->y();
}

void startup_window::mouseMoveEvent(QMouseEvent *event) {
    move(event->globalX()-m_nMouseClick_X_Coordinate,event->globalY()-m_nMouseClick_Y_Coordinate);
}


void startup_window::on_checkBox_show_pass_stateChanged(int arg1)
{
    if(arg1)
        ui->pass_field->setEchoMode(QLineEdit::Normal);
    else
        ui->pass_field->setEchoMode(QLineEdit::Password);
}

void startup_window::on_checkBox_pass_onstartup_stateChanged(int arg1)
{
    if(arg1){
        pass_on_startup = true;
    }
}

void startup_window::on_pushButton_create_clicked()
{
    QMessageBox::StandardButton reply;
    if(ui->pass_field->text() != "" && ui->pass_field->text() != ""){

        QString pass = ui->pass_field->text()+"," ;
        QString pass_prompt ;
        if(pass_on_startup)
            pass_prompt = ("true,");
        else {
            pass_prompt = ("false,");
            reply = QMessageBox::question(this, "<font color=white>Password", "<font color=white>Are you sure you don't a password on start-up?",
                                          QMessageBox::Yes|QMessageBox::No);
        }
        //Adding Information to data File
        if (reply == QMessageBox::Yes || pass_prompt == "true,"){
            QFile file(full_data_location2);
            if(!file.open(QFile::WriteOnly | QFile::Text)){
                QMessageBox::information(this,"Error","File Does not exist");
            }
            QTextStream out(&file);
            QString idl_time_autoExit = ",false,1,false,1";
            QString enc =  crypto2.encryptToString(pass_prompt + pass + uniq_string + idl_time_autoExit);
            out << enc << "\n" ;
            file.flush();
            file.close();
            QMessageBox::information(this,"Login","<font color=white>Account Created Succesfully and has been linked to this PC</font> ");
            close();
        }
    }

    else {
        ui->label_pass_empty->setText("<font color=red>Please Enter a Password");
    }
}







void startup_window::on_pass_field_textChanged(const QString &arg1)
{
    ui->label_pass_empty->setText("");
}
