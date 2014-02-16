#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:
    void updateProg();

    void on_actionNumeric_Display_triggered();

    void on_actionNumeric_Display_2_triggered();

    void on_actionNumeric_Display_3_triggered();

    void on_actionNumeric_Display_4_triggered();

    void on_actionStart_triggered();

    void on_actionStop_triggered();

    void on_action5_Low_triggered();

    void on_action4_triggered();

    void on_action3_Medium_triggered();

    void on_action2_triggered();

    void on_action1_High_triggered();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
