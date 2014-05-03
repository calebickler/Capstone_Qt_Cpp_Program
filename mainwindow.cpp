#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include "Memory\Memory.h"
#include "CPU\CPUusage.h"
#include "CPU\CPUusagethread.h"
#include "CPU\CPUspeed.h"
#include "CPU\CPUtemp.h"
#include "Graph\Graph.h"
#include "Settings\settings.h"
#include "displaysettings.h"
#include "GPU\GPUtemp.h"
#include "OHM.h"
#include "Keyboard/keyboard.h"
#include "Keyboard/keyboardthread.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#define MAX 10

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
KeyboardThread keyboardThread;
CPUtemp ctemp;
CPUspeed cspeed;
GPUtemp gpu;
Settings set;
OHM ohm;
Graph testGraph("Test Graph", 0, 100);

//strings
QString qcpuUse;
QString qmemUse;
QString qcpuSpeed;
QString qcpuTemp;
QString qgpuTemp;

//metric storage array for graphing
double memArr[MAX];
int cpuUseArr[MAX];
double cpuSpeedArr[MAX];
int cpuTempArr[MAX];
int gpuTempArr[MAX];
//metric loop counters
int mu;
int cu;
int cs;
int ct;
int gt;

QTimer *timer;
displaysettings *display;
QProcess *OHMpro;
boolean OHMoff = false;
boolean OHMmessage = true;
boolean fromfile = false;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{


    //if metric is on start thread
    cpuUthread.start();
    keyboardThread.start();
    //end threads
    display = new displaysettings(this);
    ui->setupUi(this);
    //setStyleSheet("background-color: black;");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProg()));
    timer->start(500);
    ui->KeyboardTime10->setChecked(1);//check keyboard refresh

    //load display settings//
    QString fileName = "displaysettings.ini";
    QFile mFile(fileName);
    int c = 1;
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Could not read display settings file.\n";
        c = 0;
        return;
    }
    if(c == 1)
    {
        display->fromsettings = 1;
    }
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
    set.Keyboard = in2.readLine().toULong();
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


    OHMpro = new QProcess();
    OHMpro->start("OpenHardwareMonitor/OpenHardwareMonitor.exe");


    QProcess *WMIserver = new QProcess();
    WMIserver->start("WMIserver.exe");

    updateProg();
}

MainWindow::~MainWindow()
{
    OHMpro->close();
    QProcess *cleanup = new QProcess();
    cleanup->start("RefreshNotificationArea.exe");
    delete ui;
}

void MainWindow::updateProg() {

    //highlows
    std::string high;
    std::string low;
    ohm.update();
    ui->mainList->clear();

    testGraph.draw(memArr,10, mu, Qt::green, Qt::red);
    ui->testGraphView->setScene(testGraph.scene);

    if(set.Keyboard)
    {
        keyboardThread.draw();
        ui->actionKeyboard_Log->setChecked(1);
        ui->keyboardView->setScene(keyboardThread.scene);
    }
    if(fromfile)
    {
        timer->start(set.refresh);
        switch(set.refresh)
        {
            case 500:
                ui->action1_High->setChecked(1);
                break;
            case 750:
                ui->action2->setChecked(1);
                break;
            case 1000:
                ui->action3_Medium->setChecked(1);
                break;
            case 1250:
                ui->action4->setChecked(1);
                break;
            case 1500:
                ui->action5_Low->setChecked(1);
                break;
        }

        fromfile = false;
    }
    if (set.collecting) {

        if (set.memUse || set.HLmemUsage) {
            mem.getUsage();
        }
        //memory
        if(set.memUse) {
            memArr[mu] = mem.memoryUsage;
            if(mu == 9)
            {
                mu = 0;
            }
            else
            {
                mu++;
            }
            ui->actionNumeric_Display->setChecked(1);
            memUsage = "Memory Usage: " + intToString(mem.memoryUsage) + "%";
            qmemUse = QString::fromStdString(memUsage);
            ui->mainList->addItem(qmemUse);
        }
        //memory HighLow
        if(set.HLmemUsage) {
            ui->actionSession_High_Low->setChecked(1);
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
            cpuUseArr[cu] = cpuUthread.cpuUsage;
            if(cu == 9)
            {
                cu = 0;
            }
            else
            {
                cu++;
            }
            ui->actionNumeric_Display_2->setChecked(1);
            cpuUse = "CPU Usage: " + intToString(cpuUthread.cpuUsage) + "%";
            qcpuUse = QString::fromStdString(cpuUse);
            ui->mainList->addItem(qcpuUse);
        }
        //cpu use HighLow
        if(set.HLcpuUse) {
            ui->actionSession_High_Low_2->setChecked(1);
            high = "CPU Usage High: " + intToString(cpuUthread.HcpuUse) + "%";
            low = "CPU Usage Low: " + intToString(cpuUthread.LcpuUse) + "%";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }



        if (set.cpuSpeed || set.HLcpuSpeed) {
            ui->mainList->addItem(" ");
            cspeed.getSpeed(ohm.CPUspeed);
            if (cspeed.cpuSpeed < 1) {
                if (ohm.loaded) {
                    OHMoff = true;
                    ohm.loaded = false;
                    OHMmessage = true;
                }
            }
            else {
                OHMoff = false;
            }
        }

        //cpuspeed
        if(set.cpuSpeed) {
            cpuSpeedArr[cs] = cspeed.cpuSpeed;
            if(cs == 9)
            {
                cs = 0;
            }
            else
            {
                cs++;
            }
            ui->actionNumeric_Display_3->setChecked(1);
            cpuSpeed = "CPU Speed: " + doubleToString(cspeed.cpuSpeed) + "Ghz";
            qcpuSpeed = QString::fromStdString(cpuSpeed);
            ui->mainList->addItem(qcpuSpeed);
        }
        //cpuspeed HighLow
        if(set.HLcpuSpeed) {
            ui->actionSession_High_Low_3->setChecked(1);
            high = "CPU Speed High: " + doubleToString(cspeed.HcpuSpeed) + "Ghz";
            low = "CPU Speed Low: " + doubleToString(cspeed.LcpuSpeed) + "Ghz";
            ui->mainList->addItem(QString::fromStdString(high));
            ui->mainList->addItem(QString::fromStdString(low));
        }



        if (set.cpuTemp || set.HLcpuTemp || set.cpuCoreTemp) {
            ctemp.getTemp(ohm.CPUtemp);
            /*if (ctemp.cpuHighTemp < 1) {
                if (ohm.loaded) {
                    OHMoff = true;
                    OHMmessage = true;
                    ohm.loaded = false;
                }
            }
            else {
                OHMoff = false;
            }*/
        }
        if (set.cpuTemp || set.HLcpuTemp) {
            ui->mainList->addItem(" ");
        }
        //cputemp
        if(set.cpuTemp) {
            cpuTempArr[ct] = ctemp.cpuHighTemp;
            if(ct == 9)
            {
                ct = 0;
            }
            else
            {
                ct++;
            }
            ui->actionNumeric_Display_4->setChecked(1);
            cpuTemp = "CPU Temp: " + intToString(ctemp.cpuHighTemp) + "°C";
            qcpuTemp = QString::fromStdString(cpuTemp);
            ui->mainList->addItem(qcpuTemp);
        }
        //cputemp HighLow
        if(set.HLcpuTemp) {
            ui->actionSession_High_Low_4->setChecked(1);
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
            ui->actionNumeric_Display_6->setChecked(1);
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
            gpuTempArr[gt] = gpu.gputemps[0];
            if(gt == 9)
            {
                gt = 0;
            }
            else
            {
                gt++;
            }
            ui->actionNumeric_Display_5->setChecked(1);
            if(gpu.gputemps[0] == 9999)
            {
                gpuTemp = "GPU not found. Please turn off.";
                qgpuTemp = QString::fromStdString(gpuTemp);
                ui->mainList->addItem(qgpuTemp);
            }
            else{
                int j;
                for(int i = 0; i < gpu.numgpu; i++)
                {
                    j = i + 1;
                    gpuTemp = "GPU " + intToString(j) + " Temp: " + intToString(gpu.gputemps[i]) + "°C";
                    qgpuTemp = QString::fromStdString(gpuTemp);
                    ui->mainList->addItem(qgpuTemp);
                }
            }
        }
        //gputemp HighLow
        if(set.HLgpuTemp) {
            ui->actionSession_High_Low_5->setChecked(1);
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
            out << "\n";
            out << set.Keyboard;

            mFile.flush();
            mFile.close();
        }

        if (OHMoff) {
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
                    tr("Open Hardware Monitor has closed. This process is needed to provide you with all posible metrics. To get them back you can either:\n1. Restart GiS\n2. Re-open Open Hardware Monitor (Note: Open Hardware Monitor process will not stop when GiS is closed if this is chosen)\nThen, add back the metrics you want.") );
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
        ui->actionNumeric_Display->setChecked(0);
    }
    else {
        set.memUse = true;
        ui->actionNumeric_Display->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_2_triggered()
{
    if (set.cpuUse) {
        set.cpuUse = false;
        ui->actionNumeric_Display_2->setChecked(0);
    }
    else {
        set.cpuUse = true;
        ui->actionNumeric_Display_2->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_3_triggered()
{
    if (set.cpuSpeed) {
        set.cpuSpeed = false;
        ui->actionNumeric_Display_3->setChecked(0);
    }
    else {
        set.cpuSpeed = true;
        ui->actionNumeric_Display_3->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_4_triggered()
{
    if (set.cpuTemp) {
        set.cpuTemp = false;
        ui->actionNumeric_Display_4->setChecked(0);
    }
    else {
        set.cpuTemp = true;
        ui->actionNumeric_Display_4->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_6_triggered()  //show indivudal cores
{
    if (set.cpuCoreTemp) {
        set.cpuCoreTemp = false;
        ui->actionNumeric_Display_6->setChecked(0);
    }
    else {
        set.cpuCoreTemp = true;
        ui->actionNumeric_Display_6->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_5_triggered()
{
    if (set.gpuTemp) {
        set.gpuTemp = false;
        ui->actionNumeric_Display_5->setChecked(0);
    }
    else {
        set.gpuTemp = true;
        ui->actionNumeric_Display_5->setChecked(1);
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
    ui->action5_Low->setChecked(1);
    ui->action4->setChecked(0);
    ui->action3_Medium->setChecked(0);
    ui->action2->setChecked(0);
    ui->action1_High->setChecked(0);
    timer->start(1500);
}

void MainWindow::on_action4_triggered()
{
    set.refresh = 1250;
    set.updated = true;
    ui->action5_Low->setChecked(0);
    ui->action4->setChecked(1);
    ui->action3_Medium->setChecked(0);
    ui->action2->setChecked(0);
    ui->action1_High->setChecked(0);
    timer->start(1250);
}

void MainWindow::on_action3_Medium_triggered()
{
    set.refresh = 1000;
    set.updated = true;
    ui->action5_Low->setChecked(0);
    ui->action4->setChecked(0);
    ui->action3_Medium->setChecked(1);
    ui->action2->setChecked(0);
    ui->action1_High->setChecked(0);
    timer->start(1000);
}

void MainWindow::on_action2_triggered()
{
    set.refresh = 750;
    set.updated = true;
    ui->action5_Low->setChecked(0);
    ui->action4->setChecked(0);
    ui->action3_Medium->setChecked(0);
    ui->action2->setChecked(1);
    ui->action1_High->setChecked(0);
    timer->start(750);
}

void MainWindow::on_action1_High_triggered()
{
    set.refresh = 500;
    set.updated = true;
    ui->action5_Low->setChecked(0);
    ui->action4->setChecked(0);
    ui->action3_Medium->setChecked(0);
    ui->action2->setChecked(0);
    ui->action1_High->setChecked(1);
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
        ui->actionSession_High_Low->setChecked(0);
    }
    else {
        set.HLmemUsage = true;
        ui->actionSession_High_Low->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionSession_High_Low_2_triggered()
{
    if (set.HLcpuUse) {
        set.HLcpuUse = false;
        ui->actionSession_High_Low_2->setChecked(0);
    }
    else {
        set.HLcpuUse = true;
        ui->actionSession_High_Low_2->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionSession_High_Low_3_triggered()
{
    if (set.HLcpuSpeed) {
        set.HLcpuSpeed = false;
        ui->actionSession_High_Low_3->setChecked(0);
    }
    else {
        set.HLcpuSpeed = true;
        ui->actionSession_High_Low_3->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionSession_High_Low_4_triggered()
{
    if (set.HLcpuTemp) {
        set.HLcpuTemp = false;
        ui->actionSession_High_Low_4->setChecked(0);
    }
    else {
        set.HLcpuTemp = true;
        ui->actionSession_High_Low_4->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionSession_High_Low_5_triggered()
{
    if (set.HLgpuTemp) {
        set.HLgpuTemp = false;
        ui->actionSession_High_Low_5->setChecked(0);
    }
    else {
        set.HLgpuTemp = true;
        ui->actionSession_High_Low_5->setChecked(1);
    }
    set.updated = true;
    updateProg();
}





void MainWindow::on_actionKeyboard_Log_triggered()
{
    if(set.Keyboard){
        ui->actionKeyboard_Log->setChecked(0);
        set.Keyboard = false;
        ui->menuKeyboard->setEnabled(false);
        ui->keyboardView->setVisible(false);
    }
    else {
        ui->actionKeyboard_Log->setChecked(1);
        set.Keyboard = true;
        ui->menuKeyboard->setEnabled(true);
        ui->keyboardView->setVisible(true);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_KeyboardTime2_triggered()
{
    keyboardThread.setTime(2000);
    ui->KeyboardTime2->setChecked(1);
    ui->KeyboardTime4->setChecked(0);
    ui->KeyboardTime6->setChecked(0);
    ui->KeyboardTime8->setChecked(0);
    ui->KeyboardTime10->setChecked(0);
}

void MainWindow::on_KeyboardTime4_triggered()
{
    keyboardThread.setTime(4000);
    ui->KeyboardTime2->setChecked(0);
    ui->KeyboardTime4->setChecked(1);
    ui->KeyboardTime6->setChecked(0);
    ui->KeyboardTime8->setChecked(0);
    ui->KeyboardTime10->setChecked(0);
}

void MainWindow::on_KeyboardTime6_triggered()
{
    keyboardThread.setTime(6000);
    ui->KeyboardTime2->setChecked(0);
    ui->KeyboardTime4->setChecked(0);
    ui->KeyboardTime6->setChecked(1);
    ui->KeyboardTime8->setChecked(0);
    ui->KeyboardTime10->setChecked(0);
}

void MainWindow::on_KeyboardTime8_triggered()
{
    keyboardThread.setTime(8000);
    ui->KeyboardTime2->setChecked(0);
    ui->KeyboardTime4->setChecked(0);
    ui->KeyboardTime6->setChecked(0);
    ui->KeyboardTime8->setChecked(1);
    ui->KeyboardTime10->setChecked(0);
}

void MainWindow::on_KeyboardTime10_triggered()
{
    keyboardThread.setTime(10000);
    ui->KeyboardTime2->setChecked(0);
    ui->KeyboardTime4->setChecked(0);
    ui->KeyboardTime6->setChecked(0);
    ui->KeyboardTime8->setChecked(0);
    ui->KeyboardTime10->setChecked(1);
}
