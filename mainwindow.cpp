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
    timer->start(1000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateProg() {
    ui->MemoryUse->setText("Memory: updated");
    ui->CPUuse->setText("CPU use: updated");
    ui->CPUtemp->setText("CPU temp: updated");
    ui->CPUspeed->setText("CPU speed: updated");
}
