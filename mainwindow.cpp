#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "Memory\Memory.h"
#include "CPU\CPUusage.h"
#include "CPU\CPUusagethread.h"
#include "CPU\CPUspeed.h"
#include "CPU\CPUspeedthread.h"
#include "Settings\settings.h"
#include "displaysettings.h"
#include "GPU\GPUtemp.h"


//local function prototypes
std::string intToString(int i);
std::string doubleToString(double i);
//prototypes

//variable stubs
std::string cpuUse;
std::string memUsage;
std::string cpuSpeed;
std::string cpuTemp;
std::string gpuTemp;
int cpuTempValue;
Memory mem;
Settings set;
GPUtemp gpu;
QString qcpuUse;
QString qmemUse;
QString qcpuSpeed;
QString qcpuTemp;
QString qgpuTemp;
CPUusagethread cpuUthread;
CPUspeedthread cpuSthread;
QTimer *timer;
displaysettings *display;
//highlows
int HcpuUse;
int LcpuUse;
int HmemUsage;
int LmemUsage;
double HcpuSpeed;
double LcpuSpeed;
int HcpuTemp;
int LcpuTemp;
int HgpuTemp;
int LgpuTemp;
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
    display = new displaysettings(this);
    ui->setupUi(this);
    //setStyleSheet("background-color: black;");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProg()));
    timer->start(500);
    updateProg();
    LcpuUse = 1000;
    LmemUsage = 1000;
    LcpuSpeed = 1000000;
    LcpuTemp = 1000;
    LgpuTemp = 1000;
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProg() {
    //highlows
    std::string high;
    std::string low;

    ui->mainList->clear();
    if (set.collecting) {
        //memory
        if(set.memUse) {
            mem.getUsage();
            memUsage = "Memory Usage: " + intToString(mem.memoryUsage) + "%";
            qmemUse = QString::fromStdString(memUsage);
            ui->mainList->addItem(qmemUse);
        }
        //memory HighLow
        if(set.HLmemUsage) {
            high = "Memory Usage High: " + intToString(HmemUsage) + "%";
            low = "Memory Usage Low: " + intToString(LmemUsage) + "%";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }

        ui->mainList->addItem(" ");
        //cpu use
        if(set.cpuUse) {
            cpuUse = "CPU Usage: " + intToString((int)cpuUthread.cpuUsage+.5) + "%";
            qcpuUse = QString::fromStdString(cpuUse);
            ui->mainList->addItem(qcpuUse);
        }
        //cpu use HighLow
        if(set.HLcpuUse) {
            high = "CPU Usage High: " + intToString(HcpuUse) + "%";
            low = "CPU Usage Low: " + intToString(LcpuUse) + "%";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }

        ui->mainList->addItem(" ");
        //cpuspeed
        if(set.cpuSpeed) {
            if(cpuSthread.cpuSpeed == 0)
                cpuSpeed = "Calculating...(cpu usage will spike)";
            else
                cpuSpeed = "CPU Speed: " + doubleToString(cpuSthread.cpuSpeed) + "Ghz";
            qcpuSpeed = QString::fromStdString(cpuSpeed);
            ui->mainList->addItem(qcpuSpeed);
        }
        //cpuspeed HighLow
        if(set.HLcpuSpeed) {
            high = "CPU Speed High: " + doubleToString(HcpuSpeed) + "Ghz";
            low = "CPU Speed Low: " + doubleToString(LcpuSpeed) + "Ghz";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }

        ui->mainList->addItem(" ");
        //cputemp
        if(set.cpuTemp) {
            QProcess *process = new QProcess(this);
            process->start("cpuTemp.exe");
            process->waitForFinished();
            cpuTempValue = process->exitCode();
            cpuTemp = "CPU Temp: " + intToString(cpuTempValue) + "°C";
            qcpuTemp = QString::fromStdString(cpuTemp);
            ui->mainList->addItem(qcpuTemp);
        }
        //cputemp HighLow
        if(set.HLcpuTemp) {
            high = "CPU Temp High: " + intToString(HcpuTemp) + "°C";
            low = "CPU Temp Low: " + intToString(LcpuTemp) + "°C";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }

        ui->mainList->addItem(" ");
        //gputemp
        if(set.gpuTemp) {
            gpu.getTemp();
            gpuTemp = "GPU Temp: " + intToString(gpu.gputemp) + "°C";
            qgpuTemp = QString::fromStdString(gpuTemp);
            ui->mainList->addItem(qgpuTemp);
        }
        //gputemp HighLow
        if(set.HLgpuTemp) {
            high = "GPU Temp High: " + intToString(HgpuTemp) + "°C";
            low = "GPU Temp Low: " + intToString(LgpuTemp) + "°C";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }

        ui->mainList->setStyleSheet(display->style);
    }
    updateHighLow();
}

void MainWindow::updateHighLow() {
    if (mem.memoryUsage > HmemUsage) {
        HmemUsage = mem.memoryUsage;
    }
    if (mem.memoryUsage < LmemUsage) {
        LmemUsage = mem.memoryUsage;
    }

    if ((int)cpuUthread.cpuUsage+.5 > HcpuUse) {
        HcpuUse = (int)cpuUthread.cpuUsage+.5;
    }
    if ((int)cpuUthread.cpuUsage+.5 < LcpuUse) {
        LcpuUse = (int)cpuUthread.cpuUsage+.5;
    }

    if (cpuSthread.cpuSpeed > HcpuSpeed) {
        HcpuSpeed = cpuSthread.cpuSpeed;
    }
    if (cpuSthread.cpuSpeed < LcpuSpeed || LcpuSpeed == 0) {
        LcpuSpeed = cpuSthread.cpuSpeed;
    }

    if (cpuTempValue > HcpuTemp) {
        HcpuTemp = cpuTempValue;
    }
    if (cpuTempValue < LcpuTemp) {
        LcpuTemp = cpuTempValue;
    }

    if (gpu.gputemp > HgpuTemp) {
        HgpuTemp = gpu.gputemp;
    }
    if (gpu.gputemp < LgpuTemp) {
        LgpuTemp = gpu.gputemp;
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

void MainWindow::on_actionNumeric_Display_5_triggered()
{
    if (set.gpuTemp) {
        set.gpuTemp = false;
    }
    else {
        set.gpuTemp = true;
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

void MainWindow::on_actionDisplay_Settings_triggered()
{
    display->tempstyle = ui->mainList->styleSheet();
    display->setModal(false);
    display->show();
}



void MainWindow::on_actionSession_High_Low_triggered()
{
    if (set.HLmemUsage) {
        set.HLmemUsage = false;
    }
    else {
        set.HLmemUsage = true;
    }
    updateProg();
}

void MainWindow::on_actionSession_High_Low_2_triggered()
{
    if (set.HLcpuUse) {
        set.HLcpuUse = false;
    }
    else {
        set.HLcpuUse = true;
    }
    updateProg();
}

void MainWindow::on_actionSession_High_Low_3_triggered()
{
    if (set.HLcpuSpeed) {
        set.HLcpuSpeed = false;
    }
    else {
        set.HLcpuSpeed = true;
    }
    updateProg();
}

void MainWindow::on_actionSession_High_Low_4_triggered()
{
    if (set.HLcpuTemp) {
        set.HLcpuTemp = false;
    }
    else {
        set.HLcpuTemp = true;
    }
    updateProg();
}

void MainWindow::on_actionSession_High_Low_5_triggered()
{
    if (set.HLgpuTemp) {
        set.HLgpuTemp = false;
    }
    else {
        set.HLgpuTemp = true;
    }
    updateProg();
}


