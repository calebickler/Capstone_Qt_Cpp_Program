#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>

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
    std::string memUsage;
    char* temp = new char[500];
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    GlobalMemoryStatusEx(&status);
    memUsage = "";
    sprintf(temp,"%ld",status.dwMemoryLoad);
    memUsage.append("Memory Usage: ");
    memUsage.append(temp);
    memUsage.append("%");
    QString memory = QString::fromStdString(memUsage);
    //memUsage
    //Get CPU Usage
    CPU cpu;
    std::string cpuUse;
    cpuUse = "";
    cpu.getUsage();
    sprintf(temp,"%1.lf",cpu.cpuUsage);
    cpuUse.append("CPU Usage: ");
    cpuUse.append(temp);
    cpuUse.append("%");
    QString qcpuUse = QString::fromStdString(cpuUse);
    //cpuUsage
    ui->MemoryUse->setText(memory);
    ui->CPUuse->setText(qcpuUse);
    ui->CPUtemp->setText("CPU temp: updated");
    ui->CPUspeed->setText("CPU speed: updated");
}
