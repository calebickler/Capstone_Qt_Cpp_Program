#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

std::string memUsage;
char* temp = new char[500];
CPU cpu;
std::string cpuUse;
QString qcpuUse;
QString qmemUse;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setStyleSheet("background-color: black;");
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProg()));
    timer->start(750);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProg() {
    //Get memory Usage

    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    memUsage = "";
    sprintf(temp,"%ld",status.dwMemoryLoad);
    memUsage.append("Memory Usage: ");
    memUsage.append(temp);
    memUsage.append("%");
    qmemUse = QString::fromStdString(memUsage);
    //memUsage
    //Get CPU Usage
    cpuUse = "";
    cpu.getUsage();
    sprintf(temp,"%d",(int)(cpu.cpuUsage+.5));
    cpuUse.append("CPU Usage: ");
    cpuUse.append(temp);
    cpuUse.append("%");
    qcpuUse = QString::fromStdString(cpuUse);
    //cpuUsage
    ui->MemoryUse->setText(qmemUse);
    ui->CPUuse->setText(qcpuUse);
    ui->CPUtemp->setText("CPU temp: updated");
    ui->CPUspeed->setText("CPU speed: updated");
}
