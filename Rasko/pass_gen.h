#ifndef PASS_GEN_H
#define PASS_GEN_H

#include <QDialog>
namespace Ui {
class pass_gen;
}

class pass_gen : public QDialog
{
    Q_OBJECT

public:
    explicit pass_gen(QWidget *parent = 0);
    ~pass_gen();
    QString pass_return();
    void set_FileLocation();
private slots:
    void on_exec_2_clicked();
    void mouseMoveEvent(QMouseEvent *event);
    void on_exec_clicked();
    void mousePressEvent(QMouseEvent *event);

    void on_pushButton_close_gen_clicked();

private:
    Ui::pass_gen *ui;
};

#endif // PASS_GEN_H
