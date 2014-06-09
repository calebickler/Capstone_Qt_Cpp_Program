#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "displaysettings.h"

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

    void updateList();
    bool eventFilter(QObject *t, QEvent *e);
    void keyPressEvent(QKeyEvent *event);
    void mousePressEvent(QMouseEvent *event);

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

    void on_actionDisplay_Settings_triggered();

    void on_actionSession_High_Low_triggered();

    void on_actionSession_High_Low_2_triggered();

    void on_actionSession_High_Low_3_triggered();

    void on_actionSession_High_Low_4_triggered();

    void on_actionSession_High_Low_5_triggered();

    void on_actionNumeric_Display_5_triggered();

    void on_actionNumeric_Display_6_triggered();

    void on_actionKeyboard_Log_triggered();

    void on_KeyboardTime2_triggered();

    void on_KeyboardTime4_triggered();

    void on_KeyboardTime6_triggered();

    void on_KeyboardTime8_triggered();

    void on_KeyboardTime10_triggered();

    void on_actionSet_Highlight_Color_triggered();

    void on_actionGraph_Display_triggered();

    void on_actionGraph_Display_2_triggered();

    void on_actionGraph_Display_3_triggered();

    void on_actionGraph_Display_4_triggered();

    void on_actionGraph_Display_5_triggered();

    void on_actionAdd_Blank_Line_triggered();

    void on_actionMacro_Recorder_triggered();

    void on_actionRemove_Blank_Line_triggered();

    void on_actionSet_Activation_Key_triggered();

    void on_actionSet_Delay_triggered();

    void help();

    void on_actionHelp_triggered();

    int myInt(QString, int, int);

    void writeFile();

private:
    Ui::MainWindow *ui;
    displaysettings *display;
};


#endif // MAINWINDOW_H
