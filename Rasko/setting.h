#ifndef SETTING_H
#define SETTING_H
#include <QDialog>

namespace Ui {
class setting;
}

class setting : public QDialog
{
    Q_OBJECT

public:
    explicit setting(QWidget *parent = 0);
    ~setting();
     void set_checked_prompt(bool check);
    bool pass_changed();
    bool pass_prompt_changed();
    void set_FileLocation(QString l);
    void set_radio(bool ac);
    bool get_radio();
    void set_exit(bool ac);
    bool get_exit();
    int get_minutes();
    bool return_last_status_pass_prompt();
    void set_idle_time_min(int idle_min);
    void set_exit_time_min(int exit_min);
    int get_exit_min();
    void set_ky(qint64 ky);
private slots:
    void on_pushButton_cancel_clicked();
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void check_fields( QString ac_pas ,QString con_pass);
    void on_pushButton_save_clicked();
    void check_prompt_change(bool pass_change,QString idle_Time_pass,int timer_min,QString exit_time_value,int exit_min);
    static bool copyRecursively(const QString &srcFilePath, const QString &tgtFilePath);
    void on_lineEdit_new_pass_textChanged(const QString &arg1);
bool removeDir(const QString & dirName);
    void on_lineEdit_confrim_new_textChanged(const QString &arg1);
    void on_checkBox_show_pass_toggled(bool checked);
    void on_pushButton_export_clicked();
    void on_checkBox_idle_toggled(bool checked);
    QString get_fullDataLocation();
    void on_pushButton_backup_clicked();
    void on_checkBox_autoExit_toggled(bool checked1);


    void on_spinBox_min_idle_editingFinished();

    void on_spinBox_autoExit_editingFinished();

    void on_pushButton_restorbackup_clicked();

private:
    Ui::setting *ui;
};

#endif // SETTING_H
