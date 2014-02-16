#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "Memory\Memory.h"
#include "CPU\CPUusage.h"
#include "CPU\CPUusagethread.h"
#include "CPU\CPUspeed.h"
#include "CPU\CPUspeedthread.h"
#include "Settings\settings.h"

//local function prototypes
std::string intToString(int i);
std::string doubleToString(double i);
//prototypes

//variable stubs
std::string cpuUse;
std::string memUsage;
std::string cpuSpeed;
Memory mem;
Settings set;
QString qcpuUse;
QString qmemUse;
QString qcpuSpeed;
CPUusagethread cpuUthread;
CPUspeedthread cpuSthread;
QTimer *timer;

//variables

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //if metric is on start thread
    if(set.cpuUse)
        cpuUthread.start();
    if(set.cpuSpeed)
        cpuSthread.start();
    //end threads

    ui->setupUi(this);
    //setStyleSheet("background-color: black;");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProg()));
    timer->start(1000);

    updateProg();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProg() {
    ui->mainList->clear();
    if (set.collecting) {
        //memory
        if(set.memUse) {
            mem.getUsage();
            memUsage = "Memory Usage: " + intToString(mem.memoryUsage) + "%";
            qmemUse = QString::fromStdString(memUsage);
            ui->mainList->addItem(qmemUse);
            //ui->MemoryUse->setText();
        }
        //cpu use
        if(set.cpuUse) {
            cpuUse = "CPU Usage: " + intToString((int)cpuUthread.cpuUsage+.5) + "%";
            qcpuUse = QString::fromStdString(cpuUse);
            ui->mainList->addItem(qcpuUse);
        }
        //cpuspeed
        if(set.cpuSpeed) {
            if(cpuSthread.cpuSpeed == 0)
                cpuSpeed = "Calculating...(cpu usage will spike)";
            else
                cpuSpeed = "CPU Speed: " + doubleToString(cpuSthread.cpuSpeed) + "Ghz";
            qcpuSpeed = QString::fromStdString(cpuSpeed);
            ui->mainList->addItem(qcpuSpeed);
        }
        //cputemp
        if(set.cpuTemp) {
            ui->mainList->addItem("CPU temp: updated");
        }
    }
}

std::string intToString(int i)
{
    char* temp = new char[20];
    sprintf(temp,"%d",i);
    return temp;
}

std::string doubleToString(double i){
    char* temp = new char[20];
    sprintf(temp,"%.3lf",i);
    return temp;
}

void MainWindow::on_actionNumeric_Display_triggered()
{
    if (set.memUse) {
        set.memUse = false;
    }
    else {
        set.memUse = true;
    }
    updateProg();
}

void MainWindow::on_actionNumeric_Display_2_triggered()
{
    if (set.cpuUse) {
        set.cpuUse = false;
    }
    else {
        set.cpuUse = true;
    }
    updateProg();
}

void MainWindow::on_actionNumeric_Display_3_triggered()
{
    if (set.cpuSpeed) {
        set.cpuSpeed = false;
    }
    else {
        set.cpuSpeed = true;
    }
    updateProg();
}

void MainWindow::on_actionNumeric_Display_4_triggered()
{
    if (set.cpuTemp) {
        set.cpuTemp = false;
    }
    else {
        set.cpuTemp = true;
    }
    updateProg();
}

void MainWindow::on_actionStart_triggered()
{
    if (!set.collecting) {
        set.collecting = true;
    }
    updateProg();
}

void MainWindow::on_actionStop_triggered()
{
    if (set.collecting) {
        set.collecting = false;
    }
    updateProg();
}

void MainWindow::on_action5_Low_triggered()
{
    timer->start(1500);
}

void MainWindow::on_action4_triggered()
{
    timer->start(1250);
}

void MainWindow::on_action3_Medium_triggered()
{
    timer->start(1000);
}

void MainWindow::on_action2_triggered()
{
    timer->start(750);
}

void MainWindow::on_action1_High_triggered()
{
    timer->start(500);
}
