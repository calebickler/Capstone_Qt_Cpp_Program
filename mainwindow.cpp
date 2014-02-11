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
    setStyleSheet("background-color: black;");
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProg()));
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProg() {
//get metric usage
    mem.getUsage();
//push metrics into string & Qstring
    memUsage = "Memory Usage: " + intToString(mem.memoryUsage) + "%";
    qmemUse = QString::fromStdString(memUsage);
    cpuUse = "CPU Usage: " + intToString((int)cpuUthread.cpuUsage+.5) + "%";
    qcpuUse = QString::fromStdString(cpuUse);
    if(cpuSthread.cpuSpeed == 0)
        cpuSpeed = "Calculating...(cpu usage will spike)";
    else
        cpuSpeed = "CPU Speed: " + doubleToString(cpuSthread.cpuSpeed) + "Ghz";
    qcpuSpeed = QString::fromStdString(cpuSpeed);
//set label text
    ui->MemoryUse->setText(qmemUse);
    ui->CPUuse->setText(qcpuUse);
    ui->CPUtemp->setText("CPU temp: updated");
    ui->CPUspeed->setText(qcpuSpeed);
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
