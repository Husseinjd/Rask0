#include "accountnew_edit.h"
#include "ui_accountnew_edit.h"
#include <QMouseEvent>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>
#include <QDir>
#include "simplecrypt.h"
#include "pass_gen.h"

QPoint mpos1;
int curr_del_indx ;
bool edited_acc = false;
QString full_data_location3 ;
bool opened_data3 ;
bool acc_name_check = true ;
SimpleCrypt crypto;
accountNew_edit::accountNew_edit(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::accountNew_edit)
{
    ui->setupUi(this);
    ui->label_emptyName->setText("");
    ui->label_emptyPassword->setText("");
    ui->label_emptyUserName->setText("");
    edited_acc = false;
}

accountNew_edit::~accountNew_edit()
{
    delete ui;
}

void accountNew_edit::set_ky(qint64 ky){
crypto.setKey(ky); ;
}

void accountNew_edit::on_pushButton_add_edit_clicked()
{
    QString ac_name = ui->account_name->text() ;
    QString ac_email = ui->username_email->text();
    QString ac_pas = ui->new_account_password->text();


    if(ui->pushButton_add_edit->text() == "Add"){
        add_acc(ac_name,ac_email,ac_pas);
    }
    else {
        if(check_fields(ac_name,ac_email,ac_pas))
            clear_old_data();
    }

}

 void accountNew_edit::set_FileLocation(QString l){

full_data_location3 = l ;
 }


void accountNew_edit::clear_old_data(){
    int count = 0 ;
    QList<QString> list ;
    int current_index = ui->label_hidden_indx->text().toInt();
    ui->label_hidden_indx->setText("");
    QFile file(full_data_location3);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", "Error in File location or existence");
        exit(0);
    }
    QTextStream in(&file);

    while(!in.atEnd()) {
        if(count != current_index +1){
            QString line = in.readLine();
            list.append(line);
        }
        else
            QString line = in.readLine();
        count++;
    }
    file.flush();
    file.close();

    //-------------------------------------------Paste back to data file
    QFile file1(full_data_location3);
    if (!file1.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::information(this,"Error","Data File Error");
    }
    QTextStream out(&file1);
    for(int i = 0 ; i < list.size()  ; i++){
        out<<list.at(i) <<"\n" ;
    }
    file1.flush();
    file1.close();


    QString ac_name = ui->account_name->text() ;
    QString ac_email = ui->username_email->text();
    QString ac_pas = ui->new_account_password->text();
    add_acc( ac_name,  ac_email ,  ac_pas);
    edited_acc = true ;
    close();


}

void accountNew_edit::add_acc(QString ac_name, QString ac_email , QString ac_pas){

    if(check_if_already_exists(ac_name) && check_fields(ac_name,ac_email,ac_pas)  ){
        //add the information to the data file
        QFile file(full_data_location3);
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append)) {
            QMessageBox::information(this,"Error","Data File Error");
        }
        QTextStream out(&file);
        QString enc = crypto.encryptToString(ac_name + "," + ac_email+  "," + ac_pas);
        out << enc << "\n" ;
        file.flush();
        file.close();
        close();
    }

}

bool accountNew_edit::check_fields(QString ac_name, QString ac_email , QString ac_pas){
    if(ac_name == "" || ac_name ==" ")
        ui->label_emptyName->setText("<font color=red> Name field empty </font>");
    else
        ui->label_emptyName->setText("");

    if ( ac_email == "" || ac_email == " ")
        ui->label_emptyUserName->setText("<font color=red> Username/Email field empty </font>");
    else
        ui->label_emptyUserName->setText("");

    if (ac_pas == "" || ac_email == " ")
        ui->label_emptyPassword->setText("<font color=red> Password field empty </font>");
    else
        ui->label_emptyPassword->setText("");

    if( ac_name != "" && ac_name != " "  && ac_email != "" && ac_email != " " && ac_pas != "" && ac_email != " ") {
        return true;
    }
    return false ;
}

bool accountNew_edit::hasBeenEdited(){
    return edited_acc ;
}


void accountNew_edit::setFields(QString acname, QString acusername ,QString pass,QString indx){
    ui->account_name->setText((acname));
    ui->username_email->setText((acusername));
    ui->new_account_password->setText((pass));
    ui->label_hidden_indx->setText(indx);

}


void accountNew_edit::setLabel(QString s){
    ui->pushButton_add_edit->setText(s) ;
}

QString accountNew_edit::getItem(){
    return ui->account_name->text() ;
}

bool accountNew_edit::item_empty(){
    if(!acc_name_check)
            return true;

    if(ui->account_name->text().isEmpty())
        return true ;

    return false;
}

void accountNew_edit::on_pushButton_cancelAdd_clicked()
{
    close();
}

QStringList accountNew_edit::readdataFile_returnArray(){
    QFile file(full_data_location3);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", "Error in File location or existence");
        exit(0);
    }
    QTextStream in(&file);
    QString line = crypto.decryptToString(in.readLine());
    QStringList words = line.split(",");
    return words;
}


void accountNew_edit::mousePressEvent(QMouseEvent *event){
    mpos1 = event->pos();
}

void accountNew_edit::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        QPoint diff = event->pos() - mpos1;
        QPoint newpos = this->pos() + diff;
        this->move(newpos);
    }
}

void accountNew_edit::on_checkBox_pass_show_stateChanged(int arg1)
{
    if(arg1)
        ui->new_account_password->setEchoMode(QLineEdit::Normal);
    else
        ui->new_account_password->setEchoMode(QLineEdit::Password);
}

void accountNew_edit::on_account_name_textChanged(const QString &arg1)
{
    acc_name_check = true;
    ui->label_emptyName->setText("");
}

void accountNew_edit::on_username_email_textChanged(const QString &arg1)
{
    ui->label_emptyUserName->setText("");
}

void accountNew_edit::on_new_account_password_textChanged(const QString &arg1)
{
    ui->label_emptyPassword->setText("");
}

void accountNew_edit::on_pushButton_generate_clicked()
{
    int x = this->x();
    int y = this->y();
    pass_gen p ;
    p.move(x-450,y-250);
    p.setModal(true);
    p.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    p.exec();
    ui->new_account_password->setText(p.pass_return());
}


bool accountNew_edit::check_if_already_exists(QString acname){
    QFile file(full_data_location3);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", "Error in File location or existence");
        exit(0);
    }

    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = crypto.decryptToString(in.readLine());
        QString file_acname =  line.split(",").at(0);
        if(acname == file_acname){
            ui->label_emptyName->setText("<font color=red> Account name already exists");
            acc_name_check = false;
            return false;
        }

    }
    file.flush();
    file.close();
    return true;
}
