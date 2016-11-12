#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "startup_window.h"
#include <QMouseEvent>
#include "accountnew_edit.h"



namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(qint64 kymain,QWidget *parent = 0);
    ~MainWindow();
    void click_pass_button();
     void click_username_button() ;
    void set_one_Click(bool set);
    bool get_one_Click();
    QString get_user();
    QString get_pass();

private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_edit_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void transpost_text(QString s);
    QStringList readdataFile_returnArray(int n );
    QString get_mac();
    int file_nbLines();
    void status_reset();
    void populate_combo();
    void on_pushButton_close_clicked();
    void on_pushButton_min_clicked();
    QString get_acc_info(QString info);
    void on_pushButton_delete_clicked();
    void on_pushButton_copy_username_clicked();
    void on_pushButton_copy_passsword_clicked();
    void check_DirData_exists();
    void on_comboBox_currentIndexChanged(int index);
    void delay( int millisecondsToWait );
    void activate_pass_prompt();
    void on_checkBox_stateChanged(int arg1);
    void Autoexit();
    void on_pushButton_setting_clicked();
    void on_pushButton_information_clicked();
    void on_comboBox_currentTextChanged(const QString &arg1);
    void on_comboBox_activated(const QString &arg1);
    unsigned rand256();
    unsigned long long rand64bits();


private:
    Ui::MainWindow *ui;
    startup_window *Startup ;
    void del_data();

};

#endif // MAINWINDOW_H
