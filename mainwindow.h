#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h>
#include <winperf.h>
#include <tchar.h>
#include <sigar.h>
#include <iostream>
#include <qdebug.h>
extern "C"{
#include <sigar_format.h>
}
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

private:
    Ui::MainWindow *ui;
};

class CPU{

public:
    CPU();
    double cpuUsage, cpuSpeed;
    int cpuTemp;
    void getUsage(void);
   // void getTemp(void);
   // void getSpeed(void);
};


#endif // MAINWINDOW_H
