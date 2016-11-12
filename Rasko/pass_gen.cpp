#include "pass_gen.h"
#include "ui_pass_gen.h"
#include <QMessageBox>
#include <stdio.h>
#include <sstream>
#include <QDebug>
#include <QMouseEvent>
#include <QClipboard>

QString pass_list ;
QPoint  mpos4 ;
pass_gen::pass_gen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pass_gen)
{
    ui->setupUi(this);
}

pass_gen::~pass_gen()
{
    delete ui;
}

void pass_gen::on_exec_2_clicked() // Generate automatic
{
        std::string Passwort_Architecture = "", PasswordList = "";
        if(ui->lowerLetter->isChecked())
            Passwort_Architecture += ui->lowerLetters->text().toStdString();
        if(ui->upperLetter->isChecked())
            Passwort_Architecture += ui->upperLetters->text().toStdString();
        if(ui->numerus->isChecked())
            Passwort_Architecture += ui->digits->text().toStdString();
        if(ui->specialChars->isChecked())
            Passwort_Architecture += ui->specialCharacters->text().toStdString();

        int Architecture_Length = static_cast<int>(Passwort_Architecture.length()),  Lnge_Val = ui->lnge->value();

            for(int j = 0; j < Lnge_Val; j++)
                PasswordList += Passwort_Architecture[rand() % Architecture_Length];

            pass_list = QString::fromUtf8(PasswordList.c_str());
            QClipboard *p_Clipboard = QApplication::clipboard();
            p_Clipboard->setText(pass_list);
            QMessageBox::information(this,"Password" ,"<font color=white> Generated Password has been added to ClipBoard");
            close();

}

void pass_gen::on_exec_clicked()
{
        std::string PasswordStructure = ui->strct->text().toStdString(), PasswordList = "";

        std::string digits = ui->digits->text().toStdString();
        int digitsLength = digits.length()-1;

        std::string lowerLetters = ui->lowerLetters->text().toStdString();
        int lowerLettersLength = lowerLetters.length()-1;

        std::string upperLetters = ui->upperLetters->text().toStdString();
        int upperLettersLength = upperLetters.length()-1;

        std::string specialChars = ui->specialCharacters->text().toStdString();
        int specialCharsLength =  specialChars.length()-1;

        int Password_Length = PasswordStructure.length();

            for(int i = 0; i < Password_Length; i++)
            {
                switch(PasswordStructure[i])
                {
                case 'n':
                    PasswordList += digits[rand() % digitsLength];
                    break;
                case 'b':
                    PasswordList += lowerLetters[rand() % lowerLettersLength];
                    break;
                case 'B':
                    PasswordList += upperLetters[rand() % upperLettersLength];
                    break;
                case 'c':
                    PasswordList += specialChars[rand() % specialCharsLength];
                    break;
                default:
                    PasswordList += PasswordStructure[i];
                    break;
                }
        }

            pass_list = QString::fromUtf8(PasswordList.c_str());
            QClipboard *p_Clipboard = QApplication::clipboard();
            p_Clipboard->setText(pass_list);
            QMessageBox::information(this,"Password" ,"<font color=white> Generated Password has been added to ClipBoard");
            close();
}



QString pass_gen::pass_return(){
return pass_list;
}


void pass_gen::mousePressEvent(QMouseEvent *event){
    mpos4 = event->pos();

}

void pass_gen::mouseMoveEvent(QMouseEvent *event){
    if (event->buttons() & Qt::LeftButton) {
        QPoint diff = event->pos() - mpos4;
        QPoint newpos = this->pos() + diff;
        this->move(newpos);
    }
}

void pass_gen::on_pushButton_close_gen_clicked()
{
    close();
}
