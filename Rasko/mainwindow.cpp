#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "startup_window.h"
#include "passwordprompt.h"
#include "setting.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QList>
#include <QMouseEvent>
#include <QtNetwork/QNetworkInterface>
#include "accountnew_edit.h"
#include <QClipboard>
#include <QDesktopServices>
#include <QStandardPaths>
#include <QDir>
#include <QTimer>
#include <QTime>
#include "simplecrypt.h"
#include <QHotkey>
#include <windows.h>

QTimer *timer;
QTimer *timer_exit;
QPoint mpos ; // mouse position
int c_last_indx = 0  ;
bool opened_data = false;
QString full_data_location ;
QString pass_prompt_status = false ;
bool status_timer = false ;
int time_min ;
bool status_exit_Timer = false;
int time_exit_min ;
bool check_hw = true;
bool one_click = false;
bool move_window = true ;
SimpleCrypt crypto1;
qint64 kymain;
MainWindow::MainWindow(qint64 ky, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    QString st_id ;
    startup_window Startup ;
    check_DirData_exists(); // check if File and dir exists and assign a keyto the files.
    kymain  = ky ; // getting key for the encryption
    crypto1.setKey(ky); // setting  the key
    ui->setupUi(this);
    //Check for timer idle Time

    timer= new QTimer(this);
    timer_exit= new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(activate_pass_prompt()));
    connect(timer_exit, SIGNAL(timeout()), this, SLOT(Autoexit()));
    //------------------------------------------------------------
    st_id = get_mac();

    QStringList words = readdataFile_returnArray(1);

    if(words.at(0) == "") {
        Startup.setModal(true);
        Startup.set_ky(kymain);
        Startup.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
        Startup.set_FileLocation(full_data_location);
        Startup.exec();
    }
    else {
        //Checking hwid
        if(words.at(2) != st_id && check_hw){
            del_data();
            Startup.setModal(true);
            Startup.set_ky(kymain);
            Startup.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
            Startup.set_FileLocation(full_data_location);
            Startup.exec();
        }
        //Checking Password prompt
        if(words.at(0) == "false"){
            //Succesful login without password prompt
        }
        else if(words.at(0) == "true"){
            pass_prompt_status = "true" ;
            activate_pass_prompt();
        }
        else if(words.at(0) != "false" &&  words.at(0) != "" && words.at(0) != "true") {
            del_data();
            Startup.file_edited_warning();
            Startup.setModal(true);
            Startup.set_ky(kymain);
            Startup.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
            Startup.set_FileLocation(full_data_location);
            Startup.exec();
        }
        //getting th info for the exit timer
        if(words.at(3) == "true"){
            status_timer = true ;
            time_min = words.at(4).toInt() *60000;
            ui->label_timer->setText("Timer: <font color=green>ON");
            timer->start(time_min);
        }

        else{
            status_timer = false;
            time_min = words.at(4).toInt()*60000;
        }

        //getting th info for the exit timer ;
        if(words.at(5) == "true"){
            status_exit_Timer = true;
            time_exit_min = words.at(6).toInt() *60000 ;
            timer_exit->start(time_exit_min);
            ui->label_4_exit_timer->setText("AutoExit: <font color=green>ON");
        }

        else {
            status_exit_Timer = false;
            time_exit_min = words.at(6).toInt()* 60000;
        }
    }



    //Populating ComboBox From data file
    populate_combo();
    //  ui->checkBox->setChecked(true);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setFixedSize(323,221);
    this->show();
}



MainWindow::~MainWindow()
{
    delete ui;
}


//Generate a random key uniqu key for the encryption of this program
unsigned MainWindow::rand256() {
    static unsigned const limit = RAND_MAX - RAND_MAX % 256;
    unsigned result = rand();
    while ( result >= limit ) {
        result = rand();
    }
    return result % 256;
}

unsigned long long
MainWindow::rand64bits()
{
    unsigned long long results = 0ULL;
    for ( int count = 8; count > 0; -- count ) {
        results = 256U * results + rand256();
    }
    return results;
}




void MainWindow::click_pass_button(){
    on_pushButton_copy_passsword_clicked();
}
void MainWindow::click_username_button(){
    on_pushButton_copy_username_clicked();
}

void MainWindow::Autoexit(){
    QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setText("Stay Safe :)");
    close();
}

void MainWindow::activate_pass_prompt(){
    passwordPrompt Promptwindow;
    Promptwindow.setModal(true);
    Promptwindow.set_ky(kymain);
    Promptwindow.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    Promptwindow.set_FileLocation(full_data_location);
    Promptwindow.exec();
}
void MainWindow::delay( int millisecondsToWait )
{
    QTime dieTime = QTime::currentTime().addMSecs( millisecondsToWait );
    while( QTime::currentTime() < dieTime )
    {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100 );
    }
}



void MainWindow::check_DirData_exists(){
    //--------------------trying out file creation
    QString path_usb = QDir::currentPath() + "/dataFiles";
    if(QDir(path_usb).exists()){
        check_hw = false;
        full_data_location = path_usb + "/data.txt" ;
    }
    else {
        QString path = QStandardPaths::standardLocations(QStandardPaths::AppLocalDataLocation).at(0);
        full_data_location = path + "/data.txt";
        if(!QDir(path).exists()){
            QDir().mkdir(path);
            opened_data = false;
            full_data_location = path + "/data.txt" ;

            //-----------------------------------------------------------
            QFile myFile(full_data_location);
            if(!myFile.exists()){
                if(!myFile.open(QIODevice::WriteOnly | QIODevice::Text ))
                {

                    QMessageBox::information(this,"Data Error", " Failed to create data file");
                }
            }
        }
    }
}

void MainWindow::del_data(){
    QMessageBox::warning(this, "Data Error", "Data Corrupt or New Hardware detected, Data Deleted :)");
    QFile file(full_data_location);
    if(!file.open(QFile::WriteOnly | QFile::Text)){
        QMessageBox::information(this,"Error","File Does not exist");
    }
    QTextStream out(&file);
    out << "" ;
    file.flush();
    file.close();
}


void MainWindow::on_pushButton_add_clicked()
{
    status_reset();
    transpost_text("Add");
}

void MainWindow::on_pushButton_edit_clicked()
{
    status_reset();
    transpost_text("Edit");
}

void MainWindow::populate_combo(){
    int n = file_nbLines();
    for(int i = 2 ; i <= n ; i ++ ){
        ui->comboBox->addItem(readdataFile_returnArray(i).at(0));
    }
}

void MainWindow::transpost_text(QString  s){
    //Giving the name of the button to the next form to know which function to use
    accountNew_edit Anew ;
    Anew.setLabel(s);
    Anew.setModal(true);
    Anew.set_ky(kymain);
    Anew.set_FileLocation(full_data_location);
    Anew.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    if(s == "Add"){
        Anew.exec();
        if(!Anew.item_empty()){
            ui->comboBox->addItem(Anew.getItem());
            ui->label_status->setText("Account added successfuly :)");
        }
    }
    if(s == "Edit"){
        int inx = ui->comboBox->currentIndex();
        if(ui->comboBox->currentText() == "" || ui->comboBox->currentText() == " " ){
            QMessageBox *message ;
            message->information(this, "Data", "<font color=white>No Account data exist to Edit, Please add an account first");

        }
        else {
            Anew.setFields(readdataFile_returnArray(inx+2).at(0),readdataFile_returnArray(inx+2).at(1),readdataFile_returnArray(inx+2).at(2),QString::number(inx)) ;
            Anew.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
            Anew.set_FileLocation(full_data_location);
            Anew.set_ky(kymain);
            Anew.exec();
            if(Anew.hasBeenEdited()){
                ui->comboBox->removeItem(inx);
                if(!Anew.item_empty()){
                    ui->comboBox->addItem(Anew.getItem());}
                ui->label_status->setText("Accounts updated successfuly :)");
            }
        }
    }
}

void MainWindow::status_reset(){
    ui->label_status->setText("");
}


QStringList MainWindow::readdataFile_returnArray(int n){

    QString line ;
    QFile file(full_data_location);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", "Error in File location or existence");
        exit(0);
    }
    QTextStream in(&file);
    for(int i = 0 ; i < n ;i++){
        line = crypto1.decryptToString(in.readLine());}
    QStringList words = line.split(",");
    file.flush();
    file.close();
    return words;
}

int MainWindow::file_nbLines(){
    QString line ;
    int count = 0 ;
    QFile file(full_data_location);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", "Error in File location or existence");
        exit(0);
    }
    QTextStream in(&file);
    while (!in.atEnd())
    {
        line = in.readLine();
        count++;
    }

    file.flush();
    file.close();
    return count ;
}

QString MainWindow::get_mac(){
    //getting the mac
    QString st_id ;
    foreach(QNetworkInterface netInterface, QNetworkInterface::allInterfaces())
    {
        if (!(netInterface.flags() & QNetworkInterface::IsLoopBack))
            st_id = netInterface.hardwareAddress();
    }

    return st_id ;

}


void MainWindow::mousePressEvent(QMouseEvent *event){

    mpos = event->pos();
    if(status_timer)
        timer->start(time_min);

    if(status_exit_Timer)
        timer_exit->start(time_exit_min);

    status_reset();
}

void MainWindow::mouseMoveEvent(QMouseEvent *event){

    QRect rec(16,89,251,25);
    if(!rec.contains(mpos)){
        if (event->buttons() & Qt::LeftButton) {
            QPoint diff = event->pos() - mpos;
            QPoint newpos = this->pos() + diff;
            status_reset();
            this->move(newpos);
            if(status_exit_Timer)
                timer_exit->start(time_exit_min);
            if(status_timer)
                timer->start(time_min);
        }
    }
}

void MainWindow::on_pushButton_close_clicked()
{
    QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setText("Stay Safe :)");
    close();
}

void MainWindow::on_pushButton_min_clicked()
{
    status_reset();
    showMinimized();
}

void MainWindow::on_pushButton_delete_clicked()
{
    QString acc = ui->comboBox->currentText();
    if(ui->comboBox->currentText() == "" || ui->comboBox->currentText() == " " ){
        QMessageBox *message ;
        message->information(this, "Data", "<font color=white>No Account data exist to Edit, Please add an account first");

    }
    else {
        QMessageBox::StandardButton reply = QMessageBox::question(this,"Delete"," <font color=white >Are you sure you want to Delete " + acc + " ?",
                                                                  QMessageBox::Yes | QMessageBox::No);
        if(reply == QMessageBox::Yes) {
            int count = 0 ;
            QList<QString> list ;
            int current_index = ui->comboBox->currentIndex();
            QFile file(full_data_location);
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
            QFile file1(full_data_location);
            if (!file1.open(QIODevice::WriteOnly | QFile::Text)) {
                QMessageBox::information(this,"Error","Data File Error");
            }
            QTextStream out(&file1);
            for(int i = 0 ; i < list.size()  ; i++){
                out<<list.at(i) <<"\n" ;
            }
            file1.flush();
            file1.close();
            ui->comboBox->removeItem(current_index);
            ui->label_status->setText("Account has been deleted ");
        }
    }

}

void MainWindow::on_pushButton_copy_username_clicked()
{
    QClipboard *p_Clipboard = QApplication::clipboard();
    p_Clipboard->setText(get_acc_info("user/email"));
    ui->label_status->setText("Account Username/Email copied to Clipboard ");
}

QString MainWindow::get_user(){

    return get_acc_info("user/email");
}

QString MainWindow::get_pass(){

    return get_acc_info("pass");
}


void MainWindow::on_pushButton_copy_passsword_clicked()
{
    QClipboard *p_Clipboard = QApplication::clipboard();

    p_Clipboard->setText(get_acc_info("pass"));
    ui->label_status->setText("Account Password copied to Clipboard ");

}




QString MainWindow::get_acc_info(QString info){

    QStringList words ;
    QString line ;
    QString acc_user ;
    QString acc_pass ;
    QString acc_n = ui->comboBox->currentText();
    QFile file(full_data_location);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", "Error in File location or existence");
        exit(0);
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        line = crypto1.decryptToString(in.readLine());
        words = line.split(",");
        if(words.at(0) == acc_n){
            acc_user = words.at(1);
            acc_pass = words.at(2);
            break;
        }
    }
    if(info == "user/email")
        return acc_user;

    if(info == "pass")
        return acc_pass  ;

    return "Something went wrong" ;

}

void MainWindow::on_comboBox_currentIndexChanged(int index)
{
    status_reset();
}



void MainWindow::on_checkBox_stateChanged(int arg1)
{
    Qt::WindowFlags flags = this->windowFlags();
    if (arg1 == 1)
    {
        this->setWindowFlags(flags | Qt::CustomizeWindowHint | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
        this->show();
    }
    else
    {
        this->setWindowFlags(flags ^ (Qt::WindowStaysOnTopHint));
        this->show();
    }
}

void MainWindow::on_pushButton_setting_clicked()
{
    passwordPrompt Promptwindow;
    Promptwindow.setModal(true);
    Promptwindow.set_ky(kymain);
    Promptwindow.setWindowFlags(Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    Promptwindow.set_FileLocation(full_data_location);
    Promptwindow.exec();

    int x = this->x();
    int y = this->y();
    setting s ;

    //set value for the check in the setting
    if( pass_prompt_status == "true")
        s.set_checked_prompt(true);
    else
        s.set_checked_prompt(false);
    //-------------------------------------------------
    s.move(x-300,y);
    s.setModal(true);
    s.set_ky(kymain);
    s.setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    s.set_FileLocation(full_data_location);
    s.set_radio(status_timer);
    s.set_idle_time_min(time_min/60000);
    s.set_exit(status_exit_Timer);
    s.set_exit_time_min(time_exit_min/60000);
    s.exec();

    //to get the last set value of the prompt in the last session
    if(s.return_last_status_pass_prompt())
        pass_prompt_status = "true";
    else
        pass_prompt_status = "false" ;
    //-------------------------------------------------------


    //Setup the timers to on or off depending on exit from setting
    if(s.get_radio()) {
        time_min = s.get_minutes() * 60000 ;
        timer->start(time_min);
        status_timer = true;
        ui->label_timer->setText("Timer:<font color=green> ON");
    }
    else {
        timer->stop();
        status_timer = false;
        ui->label_timer->setText("Timer: <font color=red>OFF");
    }
    if(s.get_exit()) {
        time_exit_min = s.get_exit_min() * 60000;
        timer_exit->start(time_exit_min);
        status_exit_Timer = true;
        ui->label_4_exit_timer->setText("AutoExit: <font color=green>ON");
    }
    else {
        timer_exit->stop();
        status_exit_Timer = false;
        ui->label_4_exit_timer->setText("AutoExit:<font color=red> OFF");
    }

    if(s.pass_changed() && s.pass_prompt_changed() )
        ui->label_status->setText("Password and Prompt for Password Changed!");
    else if(s.pass_changed())
        ui->label_status->setText("Password Changed Succesfully !");
    else if(s.pass_prompt_changed())
        ui->label_status->setText("Prompt for Password Changed Succesfully !");
}

void MainWindow::on_pushButton_information_clicked()
{

    QString info = "<font color=white> Copyright (C) 2016  Hussein Jundi <br>"
                   "\n This is free software, and you are welcome to redistribute this version of RaSk0";
    QMessageBox::about(this,"RaSk0",info);
}


void MainWindow::set_one_Click(bool set){
    one_click = set;
}

bool MainWindow::get_one_Click()
{
    return one_click ;
}

void MainWindow::on_comboBox_currentTextChanged(const QString &arg1)
{
    ui->label_status->setText("Press \"Ctrl + X\" On Login Field for autoPaste ");
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    ui->label_status->setText("Press \"Ctrl + X\" On Login Field for autoPaste ");
}


