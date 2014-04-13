#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "Memory\Memory.h"
#include "CPU\CPUusage.h"
#include "CPU\CPUusagethread.h"
#include "CPU\CPUspeed.h"
#include "CPU\CPUtemp.h"
#include "Settings\settings.h"
#include "displaysettings.h"
#include "GPU\GPUtemp.h"
#include "OHM.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>

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

//classes
Memory mem;
CPUusagethread cpuUthread;
CPUtemp ctemp;
CPUspeed cspeed;
GPUtemp gpu;
Settings set;
OHM ohm;

//strings
QString qcpuUse;
QString qmemUse;
QString qcpuSpeed;
QString qcpuTemp;
QString qgpuTemp;

QTimer *timer;
displaysettings *display;
QProcess *OHMpro;
boolean loaded = true;
boolean OHMoff = false;
boolean OHMmessage = true;
boolean fromfile = false;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    //if metric is on start thread
    if(set.cpuUse)
        cpuUthread.start();
    //end threads
    display = new displaysettings(this);
    ui->setupUi(this);
    //setStyleSheet("background-color: black;");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProg()));
    timer->start(500);

    //load display settings//
    QString fileName = "displaysettings.ini";
    QFile mFile(fileName);
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Could not read file.\n";
        return;
    }

    display->fromsettings = 1;
    QTextStream in(&mFile);
    QString mText = in.readLine();
    ui->mainList->setStyleSheet(mText);
    display->style = mText;
    display->button1 = in.readLine();
    display->button2 = in.readLine();
    display->button3 = in.readLine();

    mFile.close();

    //load on/off settings
    fileName = "settings.ini";
    QFile mFile2(fileName);
    if(!mFile2.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Could not read file.\n";
        return;
    }

    QTextStream in2(&mFile2);
    set.cpuSpeed = in2.readLine().toULong();
    set.cpuUse = in2.readLine().toULong();
    set.cpuTemp = in2.readLine().toULong();
    set.memUse = in2.readLine().toULong();
    set.gpuTemp = in2.readLine().toULong();
    set.HLcpuUse = in2.readLine().toULong();
    set.HLmemUsage = in2.readLine().toULong();
    set.HLcpuSpeed = in2.readLine().toULong();
    set.HLcpuTemp  = in2.readLine().toULong();
    set.HLgpuTemp = in2.readLine().toULong();
    set.refresh = in2.readLine().toULong();
    fromfile = true;

    mFile.close();

    //open hardware monitor
    //XML editing
    //rewrite entire file emitting any of our settings
    //open config
    QFile OHMconfig("debug/OpenHardwareMonitor/OpenHardwareMonitor.config");
    if(!OHMconfig.open(QFile::ReadWrite | QFile::Text))
    {
       qDebug() << "Could not read file.\n";
       return;
    }
    QTextStream OHMts(&OHMconfig);
    //create temp file
    QFile OHMnewConfig("debug/OpenHardwareMonitor/temp.txt");
    if(!OHMnewConfig.open(QFile::ReadWrite | QFile::Text))
    {
       qDebug() << "Could not read file.\n";
       return;
    }
    QTextStream OHMconNew(&OHMnewConfig);
    //put approprite lines into temp
    OHMconNew << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    OHMconNew << "<configuration>\n";
    OHMconNew << "  <appSettings>\n";
    OHMconNew << "    <add key=\"minTrayMenuItem\" value=\"true\" />\n";
    OHMconNew << "    <add key=\"startMinMenuItem\" value=\"true\" />\n";
    OHMconNew.flush();
    QString line;
    int count = 0;
    while(!OHMts.atEnd()) {
        line = OHMts.readLine() + "\n";
        if ((count > 2) && (line != "    <add key=\"minTrayMenuItem\" value=\"false\" />\n") && (line != "    <add key=\"startMinMenuItem\" value=\"false\" />\n") &&
                (line != "    <add key=\"minTrayMenuItem\" value=\"true\" />\n") && (line != "    <add key=\"startMinMenuItem\" value=\"true\" />\n")) {
            OHMconNew << line;
            OHMconNew.flush();
        }
        count++;
    }
    OHMconfig.remove();
    OHMnewConfig.rename("debug/OpenHardwareMonitor/OpenHardwareMonitor.config");
    OHMnewConfig.close();
    OHMconfig.close();

    qDebug() << QDir().absolutePath();

    //check to see if running
    QProcess *process = new QProcess();
    process->start("cpuSpeed.exe");
    process->waitForFinished();
    if (process->exitCode() < 1) {
        OHMpro = new QProcess();
        OHMpro->start("OpenHardwareMonitor/OpenHardwareMonitor.exe");
        loaded = false;
    }
    updateProg();
}

MainWindow::~MainWindow()
{
    OHMpro->close();
    delete ui;
}

void MainWindow::updateProg() {
    //highlows
    std::string high;
    std::string low;
    ohm.update();
    ui->mainList->clear();
    if(fromfile)
    {
        timer->start(set.refresh);
        fromfile = false;
    }
    if (set.collecting) {

        if (set.memUse || set.HLmemUsage) {
            mem.getUsage();
        }
        //memory
        if(set.memUse) {
            memUsage = "Memory Usage: " + intToString(mem.memoryUsage) + "%";
            qmemUse = QString::fromStdString(memUsage);
            ui->mainList->addItem(qmemUse);
        }
        //memory HighLow
        if(set.HLmemUsage) {
            high = "Memory Usage High: " + intToString(mem.HmemUsage) + "%";
            low = "Memory Usage Low: " + intToString(mem.LmemUsage) + "%";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }


        if (set.cpuUse || set.HLcpuUse) {
            ui->mainList->addItem(" ");
        }
        //cpu use
        if(set.cpuUse) {
            cpuUse = "CPU Usage: " + intToString(cpuUthread.cpuUsage) + "%";
            qcpuUse = QString::fromStdString(cpuUse);
            ui->mainList->addItem(qcpuUse);
        }
        //cpu use HighLow
        if(set.HLcpuUse) {
            high = "CPU Usage High: " + intToString(cpuUthread.HcpuUse) + "%";
            low = "CPU Usage Low: " + intToString(cpuUthread.LcpuUse) + "%";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }



        if (set.cpuSpeed || set.HLcpuSpeed) {
            ui->mainList->addItem(" ");
            cspeed.getSpeed();
            if (cspeed.cpuSpeed < 1) {
                if (loaded) {
                    OHMoff = true;
                    OHMmessage = true;
                }
            }
            else {
                loaded = true;
                OHMoff = false;
            }
        }

        //cpuspeed
        if(set.cpuSpeed) {
            cpuSpeed = "CPU Speed: " + doubleToString(cspeed.cpuSpeed) + "Ghz";
            qcpuSpeed = QString::fromStdString(cpuSpeed);
            ui->mainList->addItem(qcpuSpeed);
        }
        //cpuspeed HighLow
        if(set.HLcpuSpeed) {
            high = "CPU Speed High: " + doubleToString(cspeed.HcpuSpeed) + "Ghz";
            low = "CPU Speed Low: " + doubleToString(cspeed.LcpuSpeed) + "Ghz";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }



        if (set.cpuTemp || set.HLcpuTemp || set.cpuCoreTemp) {
            ctemp.getTemp();
            if (ctemp.cpuHighTemp < 1) {
                if (loaded) {
                    OHMoff = true;
                    OHMmessage = true;
                }
            }
            else {
                loaded = true;
                OHMoff = false;
            }
        }
        if (set.cpuTemp || set.HLcpuTemp) {
            ui->mainList->addItem(" ");
        }
        //cputemp
        if(set.cpuTemp) {
            ctemp.getTemp();
            cpuTemp = "CPU Temp: " + intToString(ctemp.cpuHighTemp) + "°C";
            qcpuTemp = QString::fromStdString(cpuTemp);
            ui->mainList->addItem(qcpuTemp);
        }
        //cputemp HighLow
        if(set.HLcpuTemp) {
            high = "CPU Temp High: " + intToString(ctemp.HcpuTemp) + "°C";
            low = "CPU Temp Low: " + intToString(ctemp.LcpuTemp) + "°C";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }

        if (set.cpuCoreTemp) {
            ui->mainList->addItem(" ");
        }
        //cpu core temps
        if(set.cpuCoreTemp) {
            if (ctemp.cpu0Temp != 0) {
                cpuTemp = "CPU Core 1 Temp: " + intToString(ctemp.cpu0Temp) + "°C";
                qcpuTemp = QString::fromStdString(cpuTemp);
                ui->mainList->addItem(qcpuTemp);
            }
            if (ctemp.cpu1Temp != 0) {
                cpuTemp = "CPU Core 2 Temp: " + intToString(ctemp.cpu1Temp) + "°C";
                qcpuTemp = QString::fromStdString(cpuTemp);
                ui->mainList->addItem(qcpuTemp);
            }
            if (ctemp.cpu2Temp != 0) {
                cpuTemp = "CPU Core 3 Temp: " + intToString(ctemp.cpu2Temp) + "°C";
                qcpuTemp = QString::fromStdString(cpuTemp);
                ui->mainList->addItem(qcpuTemp);
            }
            if (ctemp.cpu3Temp != 0) {
                cpuTemp = "CPU Core 4 Temp: " + intToString(ctemp.cpu3Temp) + "°C";
                qcpuTemp = QString::fromStdString(cpuTemp);
                ui->mainList->addItem(qcpuTemp);
            }
            if (ctemp.cpu4Temp != 0) {
                cpuTemp = "CPU Core 5 Temp: " + intToString(ctemp.cpu4Temp) + "°C";
                qcpuTemp = QString::fromStdString(cpuTemp);
                ui->mainList->addItem(qcpuTemp);
            }
            if (ctemp.cpu5Temp != 0) {
                cpuTemp = "CPU Core 6 Temp: " + intToString(ctemp.cpu5Temp) + "°C";
                qcpuTemp = QString::fromStdString(cpuTemp);
                ui->mainList->addItem(qcpuTemp);
            }
        }




        if (set.gpuTemp || set.HLgpuTemp) {
            ui->mainList->addItem(" ");
            gpu.getTemp();
        }

        //gputemp
        if(set.gpuTemp) {
            if(gpu.gputemp1 > 999 || gpu.gputemp1 < 0)
                set.gpuTemp = 0;
            else{
                gpuTemp = "GPU 1 Temp: " + intToString(gpu.gputemp1) + "°C";
                qgpuTemp = QString::fromStdString(gpuTemp);
                ui->mainList->addItem(qgpuTemp);
                if(gpu.numgpu > 1)
                {
                    gpuTemp = "GPU 2 Temp: " + intToString(gpu.gputemp2) + "°C";
                    qgpuTemp = QString::fromStdString(gpuTemp);
                    ui->mainList->addItem(qgpuTemp);
                    if(gpu.numgpu > 2)
                    {
                        gpuTemp = "GPU 3 Temp: " + intToString(gpu.gputemp3) + "°C";
                        qgpuTemp = QString::fromStdString(gpuTemp);
                        ui->mainList->addItem(qgpuTemp);
                    }
                }
            }
        }
        //gputemp HighLow
        if(set.HLgpuTemp) {
            high = "GPU Temp High: " + intToString(gpu.HgpuTemp) + "°C";
            low = "GPU Temp Low: " + intToString(gpu.LgpuTemp) + "°C";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }

        if(set.updated) {
            set.updated = false;
            QString fileName = "settings.ini";

            QFile mFile(fileName);

            if(!mFile.open(QFile::WriteOnly | QFile::Text))
            {
                qDebug() << "Could not write to file.\n";
                return;
            }
            QTextStream out(&mFile);
            out << set.cpuSpeed;
            out << "\n";
            out << set.cpuUse;
            out << "\n";
            out << set.cpuTemp;
            out << "\n";
            out << set.memUse;
            out << "\n";
            out << set.gpuTemp;
            out << "\n";
            out << set.HLcpuUse;
            out << "\n";
            out << set.HLmemUsage;
            out << "\n";
            out << set.HLcpuSpeed;
            out << "\n";
            out << set.HLcpuTemp;
            out << "\n";
            out << set.HLgpuTemp;
            out << "\n";
            out << set.refresh;

            mFile.flush();
            mFile.close();
        }

        if (OHMoff) {
            QProcess *process = new QProcess();
            process->start("cpuSpeed.exe");
            process->waitForFinished();
            if (process->exitCode() > 0) {
                OHMoff = false;
                set.cpuSpeed = true;
                set.HLcpuSpeed = true;
                set.HLcpuTemp = true;
                set.cpuCoreTemp = true;
                set.cpuTemp = true;
            }
            if (OHMmessage) {
                qDebug() << "ohm is off";
                set.cpuSpeed = false;
                set.HLcpuSpeed = false;
                set.HLcpuTemp = false;
                set.cpuCoreTemp = false;
                set.cpuTemp = false;
                QMessageBox::information(
                    this,
                    tr("GiS"),
                    tr("Open Hardware Monitor has closed. This process is needed to provide you with all posible metrics. To get them back you can either:\n1. Restart GiS\n2. Re-open Open Hardware Monitor (Note: Open Hardware Monitor process will not stop when GiS is closed if this is chosen)") );
                OHMmessage = false;
            }
        }

        ui->mainList->setStyleSheet(display->style);
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
    set.updated = true;
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
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_3_triggered()
{
    if (set.cpuSpeed) {
        set.cpuSpeed = false;
    }
    else {
        if (!OHMoff) {set.cpuSpeed = true;}
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_4_triggered()
{
    if (set.cpuTemp) {
        set.cpuTemp = false;
    }
    else {
        if (!OHMoff) {set.cpuTemp = true;}
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_6_triggered()  //show indivudal cores
{
    if (set.cpuCoreTemp) {
        set.cpuCoreTemp = false;
    }
    else {
        if (!OHMoff) {set.cpuCoreTemp = true;}
    }
    set.updated = true;
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
    set.updated = true;
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
    set.refresh = 1500;
    set.updated = true;
    timer->start(1500);
}

void MainWindow::on_action4_triggered()
{
    set.refresh = 1250;
    set.updated = true;
    timer->start(1250);
}

void MainWindow::on_action3_Medium_triggered()
{
    set.refresh = 1000;
    set.updated = true;
    timer->start(1000);
}

void MainWindow::on_action2_triggered()
{
    set.refresh = 750;
    set.updated = true;
    timer->start(750);
}

void MainWindow::on_action1_High_triggered()
{
    set.refresh = 500;
    set.updated = true;
    timer->start(500);
}

void MainWindow::on_actionDisplay_Settings_triggered()
{
    display->tempstyle = ui->mainList->styleSheet();
    display->setModal(false);
    display->update();
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
    set.updated = true;
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
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionSession_High_Low_3_triggered()
{
    if (set.HLcpuSpeed) {
        set.HLcpuSpeed = false;
    }
    else {
        if (!OHMoff) {set.HLcpuSpeed = true;}
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionSession_High_Low_4_triggered()
{
    if (set.HLcpuTemp) {
        set.HLcpuTemp = false;
    }
    else {
        if (!OHMoff) {set.HLcpuTemp = true;}
    }
    set.updated = true;
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
    set.updated = true;
    updateProg();
}




