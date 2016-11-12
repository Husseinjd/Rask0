#ifndef ACCOUNTNEW_EDIT_H
#define ACCOUNTNEW_EDIT_H

#include <QDialog>

namespace Ui {
class accountNew_edit;
}

class accountNew_edit : public QDialog
{
    Q_OBJECT

public:
    explicit accountNew_edit(QWidget *parent = 0);
    ~accountNew_edit();
    void setLabel(QString s);
    QString getItem();
    bool item_empty();
    void setFields(QString acname, QString acusername ,QString pass,QString indx);
    void clear_old_data();
    bool hasBeenEdited();
    void set_FileLocation(QString l);
    void set_ky(qint64 ky);

private slots:
    void on_pushButton_cancelAdd_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    QStringList readdataFile_returnArray();
    void add_acc(QString ac_name, QString ac_email , QString ac_pas);
    bool check_fields(QString ac_name, QString ac_email , QString ac_pas);
    void on_pushButton_add_edit_clicked();
    void on_checkBox_pass_show_stateChanged(int arg1);
    void on_account_name_textChanged(const QString &arg1);
    void on_username_email_textChanged(const QString &arg1);
    void on_new_account_password_textChanged(const QString &arg1);
    void on_pushButton_generate_clicked();
    bool check_if_already_exists(QString acname);
private:
    Ui::accountNew_edit *ui;

};

#endif // ACCOUNTNEW_EDIT_H
