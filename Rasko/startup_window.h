#ifndef STARTUP_WINDOW_H
#define STARTUP_WINDOW_H

#include <QDialog>
#include <QMouseEvent>

extern bool pass_startup;

namespace Ui {
class startup_window;
}

class startup_window : public QDialog
{
    Q_OBJECT

public:
    explicit startup_window(QWidget *parent = 0);
    ~startup_window();
    void file_edited_warning();
    void set_FileLocation(QString l);
    void set_ky(qint64 ky);

private slots:
    void on_pushButton_cancel_clicked();
    void on_checkBox_show_pass_stateChanged(int arg1);
    void on_checkBox_pass_onstartup_stateChanged(int arg1);
    void on_pushButton_create_clicked();

    void on_pass_field_textChanged(const QString &arg1);

private:
    Ui::startup_window *ui;
    //window mouse movement
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    int m_nMouseClick_X_Coordinate;
    int m_nMouseClick_Y_Coordinate;

};



#endif // STARTUP_WINDOW_H
