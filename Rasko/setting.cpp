#include "setting.h"
#include "ui_setting.h"
#include <QMouseEvent>
#include <QFile>
#include "simplecrypt.h"
#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QFileDialog>
#include <QDesktopServices>


QString full_data_location5 ;
QPoint mpos3 ;
bool enter_change = false;
bool pass_change1 = false;
bool state_Idle_change = false;
bool state_autoExit_change = false;
int min_autoExit;
int min ;
bool spin_box_one_change = false;
bool spin_box_two_change = false ;
bool retore_but_isclicked = false ;
SimpleCrypt crypto6;
setting::setting(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setting)
{
    ui->setupUi(this);
    ui->spinBox_min_idle->setMinimum(1);


}

setting::~setting()
{
    delete ui;
}



void setting::set_ky(qint64 ky){
    crypto6.setKey(ky); ;
}


bool setting::pass_changed(){
    return pass_change1 ;
}

bool setting::pass_prompt_changed(){
    return  enter_change ;
}

bool setting::return_last_status_pass_prompt(){
    return ui->checkBox_password_prompt->isChecked();
}

void setting::on_pushButton_cancel_clicked()
{
    pass_change1 = false;
    enter_change = false;
    close();
}

void setting::set_FileLocation(QString l){
    full_data_location5 = l;
    if(full_data_location5 == (QDir::currentPath() + "/dataFiles")){
        ui->label_6_portable->setText("Portable version");
        ui->pushButton_backup->setEnabled(false);
        ui->pushButton_export->setEnabled(false);

    }
}

QString setting::get_fullDataLocation(){
    return full_data_location5 ;
}

void setting::mousePressEvent(QMouseEvent *event){
    mpos3 = event->pos();

}

void setting::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        QPoint diff = event->pos() - mpos3;
        QPoint newpos = this->pos() + diff;
        this->move(newpos);
    }
}



void setting::check_fields( QString ac_pas ,QString con_pass){

    //For password Change
    QString radio_but;
    QString state_checked_autoExit ;
    min = ui->spinBox_min_idle->value();
    min_autoExit = ui->spinBox_autoExit->value();

    if(ui->checkBox_idle->isChecked()){
        radio_but = "true";
    }

    else
    {
        radio_but = "false";
    }


    if(ui->checkBox_autoExit->isChecked()){
        state_checked_autoExit = "true";
    }
    else
    {
        state_checked_autoExit = "false";
    }


    //------------------------------------



    if(ac_pas == con_pass && ac_pas != "") {
        //change the data file to add the new pass
        check_prompt_change(true,radio_but,min,state_checked_autoExit,min_autoExit);
        close();
    }

    else if(ac_pas == con_pass && ac_pas == ""){
        check_prompt_change(false,radio_but,min,state_checked_autoExit,min_autoExit);
        if(enter_change || state_Idle_change || state_autoExit_change ||spin_box_one_change || spin_box_two_change)
            close();
        else {
            ui->label_pass_message->setText("<font color=red> Passwords are Different </font>");
            ui->label_con_pas_mess->setText("<font color=red> Passwords are Different </font>");
        }
    }

    else if (ac_pas != con_pass) {
        ui->label_pass_message->setText("<font color=red> Passwords are Different </font>");
        ui->label_con_pas_mess->setText("<font color=red> Passwords are Different </font>");

    }


}

void setting::on_pushButton_save_clicked()
{

    check_fields(ui->lineEdit_new_pass->text() , ui->lineEdit_confrim_new->text());
}

void setting::check_prompt_change(bool pass_change,QString idle_Time_pass,int timer_min, QString exit_time_value,int exit_min){
    QString prompt ;
    QString pass_new ;
    QString fix_idle ;
    QString time_exit_print ;
    int count = 0 ;
    QList<QString> list ;
    QString first_line ;

    QFile file(full_data_location5);
    if(!file.open(QIODevice::ReadOnly)) {
        QMessageBox::information(0, "error", "Error in File location or existence");
        exit(0);
    }
    QTextStream in(&file);
    while(!in.atEnd()) {
        QString line = in.readLine();
        if(count == 0){
            first_line = line ;
        }
        list.append(line);
        count++;
    }
    file.flush();
    file.close();
    QStringList lineList = crypto6.decryptToString(first_line).split(",");
    QString valuePrompt = lineList.at(0) ; // value for pass prompt in the file
    QString valueIdle = lineList.at(3);
    fix_idle = valueIdle ;
    QString valueExit = lineList.at(5);
    pass_new = lineList.at(1) ;
    prompt = lineList.at(0);
    time_exit_print = valueExit;
    if(valuePrompt == "false" && ui->checkBox_password_prompt->isChecked()){
        prompt = "true";
        enter_change = true ;
    }
    else if(valuePrompt == "true" && !(ui->checkBox_password_prompt->isChecked())){
        prompt = "false";
        enter_change = true ;
    }
    if(pass_change){
        pass_change1 = true;
        pass_new =  ui->lineEdit_confrim_new->text() ;
    }
    //PROMPT FOR IDLE TIME CHECKING
    if(idle_Time_pass == "true"  && valueIdle == "false"){
        state_Idle_change = true;
        fix_idle = "true" ;
    }
    if(idle_Time_pass == "false"  && valueIdle == "true"){
        state_Idle_change = true;
        fix_idle = "false" ;
    }
    //_------------------------------------------------------------------------------------

    //PROMPT FOR AutoExit TIME CHECKING
    if(exit_time_value == "true"  && valueExit == "false"){
        state_autoExit_change = true;
        time_exit_print = "true" ;
    }

    else if (exit_time_value == "false"  && valueExit == "true"){
        state_autoExit_change = true;
        time_exit_print = "false" ;

    }
    QString replaceexit = crypto6.encryptToString(prompt+","+pass_new+"," +lineList.at(2) +"," + fix_idle +"," +
                                                  QString::number(timer_min)+"," +time_exit_print+
                                                  "," + QString::number(exit_min));
    list[0] = replaceexit;
    QFile file1(full_data_location5);
    if (!file1.open(QIODevice::WriteOnly | QFile::Text)) {
        QMessageBox::information(this,"Error","Data File Error");
    }
    QTextStream out(&file1);
    for(int i = 0 ; i < list.size()  ; i++){
        out<<list.at(i) <<"\n" ;
    }
    file1.flush();
    file1.close();

    //_------------------------------------------------------------------------------------

}

void setting::set_checked_prompt(bool check){
    if(check)
        ui->checkBox_password_prompt->setChecked(true);
}
void setting::on_lineEdit_new_pass_textChanged(const QString &arg1)
{
    ui->label_pass_message->setText("");
}
void setting::on_lineEdit_confrim_new_textChanged(const QString &arg1)
{
    ui->label_con_pas_mess->setText("");
}
void setting::on_checkBox_show_pass_toggled(bool checked)
{
    if(checked){
        ui->lineEdit_confrim_new->setEchoMode(QLineEdit::Normal);
        ui->lineEdit_new_pass->setEchoMode(QLineEdit::Normal);
    }
    else {
        ui->lineEdit_new_pass->setEchoMode(QLineEdit::Password);
        ui->lineEdit_confrim_new->setEchoMode(QLineEdit::Password);
    }
}

void setting::on_pushButton_export_clicked()
{
    ui->label_6_please_wait->setText("Please wait....");
    QMessageBox::information(this,"Choose Location" ,"Please choose the usb drive file to Export to");
    QString path = QDir::currentPath() + "/dataFiles" ;

    QDir dir(path);
    if(!dir.exists())
        dir.mkpath(path);

    QString full_files_data = QDir::currentPath();
    QFileInfo fi(full_data_location5);
    QString fileName= fi.fileName();
    QString destinationPath= QFileDialog::getExistingDirectory(this,QObject::tr("Export To File"))  + "/Rasko";

    QDir dir1(destinationPath);
    if (dir1.exists()){
        QMessageBox::StandardButton reply = QMessageBox::question(this, "<font color=white>Exists", "<font color=white>File Already exists do you wish to overwrite it?",
                                                                  QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes )
            removeDir(destinationPath);
    }

    QString data_file_destination = destinationPath +"/dataFiles" + QDir::separator()+ fileName;
    if( copyRecursively(full_files_data,destinationPath) && QFile::copy(full_data_location5,data_file_destination)){
        QMessageBox::information(this,"Export" , "<font color=white>Data file has been Exported");
        close();
    }
    else
        QMessageBox::warning(this,"Export" , "<font color=white> Failed to export Data file ");
    //remove the created directory after the export has been done
    removeDir(path);
    ui->label_6_please_wait->setText("");
}


bool setting::copyRecursively(const QString &srcFilePath, const QString &tgtFilePath)
{
    QFileInfo srcFileInfo(srcFilePath);
    if (srcFileInfo.isDir()) {
        QDir targetDir(tgtFilePath);
        targetDir.cdUp();
        if (!targetDir.mkdir(QFileInfo(tgtFilePath).fileName()))
            return false;
        QDir sourceDir(srcFilePath);
        QStringList fileNames = sourceDir.entryList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);
        foreach (const QString &fileName, fileNames) {
            const QString newSrcFilePath
                    = srcFilePath + QLatin1Char('/') + fileName;
            const QString newTgtFilePath
                    = tgtFilePath + QLatin1Char('/') + fileName;
            if (!copyRecursively(newSrcFilePath, newTgtFilePath))
                return false;
        }
    } else {
        if (!QFile::copy(srcFilePath, tgtFilePath))
            return false;
    }
    return true;
}

bool setting::removeDir(const QString & dirName)
{
    bool result = true;
    QDir dir(dirName);

    if (dir.exists()) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot | QDir::System | QDir::Hidden  | QDir::AllDirs | QDir::Files, QDir::DirsFirst)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = QDir().rmdir(dirName);
    }
    return result;
}



void setting::set_radio(bool ac){
    ui->checkBox_idle->setChecked(ac);
}

bool setting::get_radio(){
    return ui->checkBox_idle->isChecked() ;
}

int setting::get_minutes(){
    return ui->spinBox_min_idle->value();
}

int setting::get_exit_min(){
    return ui->spinBox_autoExit->value();
}

void setting::on_checkBox_idle_toggled(bool checked)
{
    ui->spinBox_min_idle->setEnabled(checked);
}


void setting::on_pushButton_backup_clicked()
{
    QString  loc = QFileDialog::getExistingDirectory(this,QObject::tr("Backup location"))  ;
    QFileInfo check_file(loc + QDir::separator() + "data.txt");
    if (check_file.exists() && check_file.isFile()){
        QFile file1(loc + QDir::separator() + "data.txt");
        QMessageBox::StandardButton reply = QMessageBox::question(this, "<font color=white>Exists", "<font color=white>File Already exists do you wish to overwrite it?",
                                                                  QMessageBox::Yes|QMessageBox::No);

        if (reply == QMessageBox::Yes )
            file1.remove();
    }

    if(QFile::copy(full_data_location5,loc + QDir::separator() + "data.txt"))
    {
        QMessageBox::information(this,"Backup" , "<font color=white>Backed-up to specified location succesfully ! ");
        close();
    }

    else
        QMessageBox::warning(this,"Backup" , "<font color=white> Error during Backup ! , Please check file location,or try another folder");
}

void setting::set_idle_time_min(int idle_min){

    ui->spinBox_min_idle->setValue(idle_min);
}


void setting::set_exit(bool ac){
    ui->checkBox_autoExit->setChecked(ac);
}

void setting::set_exit_time_min(int exit_min){
    ui->spinBox_autoExit->setValue(exit_min);
}

bool setting::get_exit(){
    return ui->checkBox_autoExit->isChecked() ;
}

void setting::on_checkBox_autoExit_toggled(bool checked1)
{
    ui->spinBox_autoExit->setEnabled(checked1);
}

void setting::on_spinBox_min_idle_editingFinished()
{
    spin_box_one_change = true ;
}

void setting::on_spinBox_autoExit_editingFinished()
{
    spin_box_two_change = true ;
}

void setting::on_pushButton_restorbackup_clicked()
{
    QMessageBox::information(this,"Backup" ,"<font color=white> Please Back up current data file before proceeding!");
    QFileDialog qdialog ;
    QString  loc = qdialog.getOpenFileName(this,tr("Choose File"),QDir::currentPath(),tr("Text files (*.txt)")) ;
    QString path_usb = QDir::currentPath() + "/dataFiles" ;
    if(loc != ""){
        QFile file(full_data_location5);
        file.remove();}

    if(QDir(path_usb).exists()){
        if(QFile::copy(loc,path_usb + QDir::separator()+ "data.txt"))
        {
            QMessageBox::information(this,"<font color=white> Restore Backup" ,"Data retored succesfully, please restart the App! ");
            close();
        }

        else
            QMessageBox::warning(this,"<font color=white> Backup" , "Error during restore ! , Please check file location");
    }

    else {
        if(QFile::copy(loc,full_data_location5))
        {
            QMessageBox::information(this,"<font color=white> Restore Backup" ,"Data retored succesfully, please restart the App! ");
            close();
        }
        else
            QMessageBox::warning(this,"<font color=white> Backup" , "Error during restore main ! , Please check file location");
    }


}
