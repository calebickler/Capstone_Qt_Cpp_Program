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
#include "Macro/Macro.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsItem>
#include <QColorDialog>
#include <QMouseEvent>
#include <QCursor>
#include <QInputDialog>

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
Graph grapher;
Macro macro;

QString buildtext;
int textcount;

//strings
QString qcpuUse;
QString qmemUse;
QString qcpuSpeed;
QString qcpuTemp;
QString qgpuTemp;
int corrupt;

QList<int> textDisplay;

//graphs
QGraphicsScene* sceneMem;
QGraphicsScene* sceneCPUS;
QGraphicsScene* sceneCPUT;
QGraphicsScene* sceneCPUU;
QGraphicsScene* sceneGPUU;
QGraphicsScene* sceneMacro;

QTimer *timer;
displaysettings *display;
QProcess *OHMpro;
boolean OHMoff = false;
boolean OHMmessage = true;
boolean OHMloaded = false;
boolean fromfile = false;

//moving Graphics
int isSelected;
int selected;



MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    isSelected = 0;
    selected = 0;
    installEventFilter(this);
    sceneMem = new QGraphicsScene(0, 0, 371, 271);
    sceneCPUS = new QGraphicsScene(0, 0, 371, 271);
    sceneCPUT = new QGraphicsScene(0, 0, 371, 271);
    sceneCPUU = new QGraphicsScene(0, 0, 371, 271);
    sceneGPUU = new QGraphicsScene(0, 0, 371, 271);
    sceneMacro = new QGraphicsScene(0, 0, 236, 261);
    //if metric is on start thread
    macro.start();
    cpuUthread.start();
    keyboardThread.start();
    //end threads
    display = new displaysettings(this);
    ui->setupUi(this);
    //setStyleSheet("color: red;");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(updateProg()));
    timer->start(500);


    //load display settings//
    QString fileName = "displaysettings";
    QFile mFile(fileName);
    int c = 1;
    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Could not read display settings file.\n";
        display->style = "color: rgb(124,255,37);background-color: rgb(0,0,0);";
        display->button1 = "background-color: rgb(124,255,37); border: 1px solid black;";
        display->button2 = "background-color: rgb(0,0,0); border: 1px solid black;";
        display->button3 = display->button1;
        display->fontcolor.setNamedColor("#7cff25");
        display->kHighlight.setNamedColor("#FFFFFF");
        keyboardThread.setHighlight(display->kHighlight);
        display->graphcolor = display->fontcolor;
        display->fromsettings = 1;
        display->update();
        c = 0;
        display->writeFile();
    }
    else {
    if(c == 1)
        {
            display->fromsettings = 1;
        }
        QTextStream in(&mFile);
        QString mText = in.readLine();
        if(mText.length() >= 47 && mText.length() <= 59)
        {
            ui->mainList->setStyleSheet(mText);
            display->style = mText;
            display->button1 = in.readLine();
            display->button2 = in.readLine();
            display->button3 = in.readLine();
            display->fontcolor.setNamedColor(in.readLine());
            display->kHighlight.setNamedColor(in.readLine());
            keyboardThread.setHighlight(display->kHighlight);
            keyboardThread.hfromset = 1;
            display->graphcolor.setNamedColor(in.readLine());
            display->update();
        }
        else{qDebug() << "corrupt file";}
        mFile.close();
    }

    //load on/off settings
    fileName = "settings";
    QFile mFile2(fileName);
    if(!mFile2.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Could not read settings file.\n";
        if(set.Keyboard)
            ui->actionKeyboard_Log->setChecked(1);
            ui->actionNumeric_Display->setChecked(1);
            ui->actionSession_High_Low->setChecked(1);
            ui->actionNumeric_Display_2->setChecked(1);
            ui->actionSession_High_Low_2->setChecked(1);
            ui->actionNumeric_Display_3->setChecked(1);
            ui->actionSession_High_Low_3->setChecked(1);
            ui->actionNumeric_Display_4->setChecked(1);
            ui->actionSession_High_Low_4->setChecked(1);
            ui->actionNumeric_Display_6->setChecked(1);
            ui->actionNumeric_Display_5->setChecked(1);
            ui->actionSession_High_Low_5->setChecked(1);
    }
    else {
        c++;
        QTextStream in2(&mFile2);
        set.cpuSpeed = myInt(in2.readLine(), 0, 1);
        set.cpuUse = myInt(in2.readLine(), 0, 1);
        set.cpuTemp = myInt(in2.readLine(), 0, 1);
        set.memUse = myInt(in2.readLine(), 0, 1);
        set.gpuTemp = myInt(in2.readLine(), 0, 1);
        set.HLcpuUse = myInt(in2.readLine(), 0, 1);
        set.HLmemUsage = myInt(in2.readLine(), 0, 1);
        set.HLcpuSpeed = myInt(in2.readLine(), 0, 1);
        set.HLcpuTemp  = myInt(in2.readLine(), 0, 1);
        set.HLgpuTemp = myInt(in2.readLine(), 0, 1);
        set.refresh = myInt(in2.readLine(), 500, 1500);
        set.Keyboard = myInt(in2.readLine(), 0, 1);
        set.memGraph = myInt(in2.readLine(), 0, 1);
        set.CPUSpeedGraph = myInt(in2.readLine(), 0, 1);
        set.CPUTempGraph = myInt(in2.readLine(), 0, 1);
        set.CPUUseGraph = myInt(in2.readLine(), 0, 1);
        set.GPUTempGraph = myInt(in2.readLine(), 0, 1);
        set.macro = myInt(in2.readLine(), 0, 1);
        int p,r;
        r = myInt(in2.readLine(),0,5000);
        for(p = 0; p < r; p++)
        {
            textDisplay.append(myInt(in2.readLine(),0,11));
        }
        ui->MemoryGraphView->setGeometry(myInt(in2.readLine(),0,5000), myInt(in2.readLine(),0,5000), 381, 281);

        ui->CPUTempView->setGeometry(myInt(in2.readLine(),0,5000), myInt(in2.readLine(),0,5000), 381, 281);

        ui->CPUUseView->setGeometry(myInt(in2.readLine(),0,5000), myInt(in2.readLine(),0,5000), 381, 281);

        ui->CPUSpeedView->setGeometry(myInt(in2.readLine(),0,5000), myInt(in2.readLine(),0,5000), 381, 281);

        ui->GPUTempView->setGeometry(myInt(in2.readLine(),0,5000), myInt(in2.readLine(),0,5000), 381, 281);

        ui->keyboardView->setGeometry(myInt(in2.readLine(),0,5000), myInt(in2.readLine(),0,5000), 496, 156);

        ui->MacroView->setGeometry(myInt(in2.readLine(),0,5000), myInt(in2.readLine(),0,5000), 251, 291);

        fromfile = true;

        mFile.close();

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
        if(set.Keyboard)
            ui->actionKeyboard_Log->setChecked(1);
        else
            ui->actionKeyboard_Log->setChecked(0);
        if(set.memUse)
            ui->actionNumeric_Display->setChecked(1);
        else
            ui->actionNumeric_Display->setChecked(0);
        if(set.HLmemUsage)
            ui->actionSession_High_Low->setChecked(1);
        else
            ui->actionSession_High_Low->setChecked(0);
        if(set.cpuUse)
            ui->actionNumeric_Display_2->setChecked(1);
        else
            ui->actionNumeric_Display_2->setChecked(0);
        if(set.HLcpuUse)
            ui->actionSession_High_Low_2->setChecked(1);
        else
            ui->actionSession_High_Low_2->setChecked(0);
        if(set.cpuSpeed)
            ui->actionNumeric_Display_3->setChecked(1);
        else
            ui->actionNumeric_Display_3->setChecked(0);
        if(set.HLcpuSpeed)
            ui->actionSession_High_Low_3->setChecked(1);
        else
            ui->actionSession_High_Low_3->setChecked(0);
        if(set.cpuTemp)
            ui->actionNumeric_Display_4->setChecked(1);
        else
            ui->actionNumeric_Display_4->setChecked(0);
        if(set.HLcpuTemp)
            ui->actionSession_High_Low_4->setChecked(1);
        else
            ui->actionSession_High_Low_4->setChecked(0);
        if(set.cpuCoreTemp)
            ui->actionNumeric_Display_6->setChecked(1);
        else
            ui->actionNumeric_Display_6->setChecked(0);
        if(set.gpuTemp)
            ui->actionNumeric_Display_5->setChecked(1);
        else
            ui->actionNumeric_Display_5->setChecked(0);
        if(set.HLgpuTemp)
            ui->actionSession_High_Low_5->setChecked(1);
        else
            ui->actionSession_High_Low_5->setChecked(0);
        if(set.memGraph) {
            ui->actionGraph_Display->setChecked(1);
            ui->MemoryGraphView->setVisible(true);
        }
        else {
            ui->actionGraph_Display->setChecked(0);
            ui->MemoryGraphView->setVisible(false);
        }
        if(set.CPUUseGraph) {
            ui->actionGraph_Display_2->setChecked(1);
            ui->CPUUseView->setVisible(true);
        }
        else {
            ui->actionGraph_Display_2->setChecked(0);
            ui->CPUUseView->setVisible(false);
        }
        if(set.CPUSpeedGraph) {
            ui->actionGraph_Display_3->setChecked(1);
            ui->CPUSpeedView->setVisible(true);
        }
        else {
            ui->actionGraph_Display_3->setChecked(0);
            ui->CPUSpeedView->setVisible(false);
        }
        if(set.CPUTempGraph) {
            ui->actionGraph_Display_4->setChecked(1);
            ui->CPUTempView->setVisible(true);
        }
        else {
            ui->actionGraph_Display_4->setChecked(0);
            ui->CPUTempView->setVisible(false);
        }
        if(set.GPUTempGraph) {
            ui->actionGraph_Display_5->setChecked(1);
            ui->GPUTempView->setVisible(true);
        }
        else {
            ui->actionGraph_Display_5->setChecked(0);
            ui->GPUTempView->setVisible(false);
        }


    }


    //IF NO SETTINGS FOUND FOR QLIST
    if(!fromfile)
       {
        textDisplay.append(0);
        textDisplay.append(1);
        textDisplay.append(11);
        textDisplay.append(2);
        textDisplay.append(3);
        textDisplay.append(11);
        textDisplay.append(4);
        textDisplay.append(5);
        textDisplay.append(11);
        textDisplay.append(6);
        textDisplay.append(7);
        textDisplay.append(8);
        textDisplay.append(11);
        textDisplay.append(9);
        textDisplay.append(10);
        writeFile();
    }


    ui->actionMacro_Recorder->setChecked(1);
    ui->KeyboardTime10->setChecked(1);


    //open hardware monitor
    //XML editing
    //rewrite entire file emitting any of our settings
    //open config
    QFile OHMconfig("OpenHardwareMonitor/OpenHardwareMonitor.config");
    if(!OHMconfig.open(QFile::ReadWrite | QFile::Text))
    {
       qDebug() << "Could not read OHMconfig file.\n";
    }
    else {
        QTextStream OHMts(&OHMconfig);
        //create temp file
        QFile OHMnewConfig("OpenHardwareMonitor/temp.txt");
        if(!OHMnewConfig.open(QFile::ReadWrite | QFile::Text))
        {
           qDebug() << "Could not read temp file.\n";
        }
        else {
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
            OHMnewConfig.rename("OpenHardwareMonitor/OpenHardwareMonitor.config");
            OHMnewConfig.close();
            OHMconfig.close();
        }
    }

    OHMpro = new QProcess();
    OHMpro->start("OpenHardwareMonitor/OpenHardwareMonitor.exe");


    QProcess *WMIserver = new QProcess();
    WMIserver->start("WMIserver.exe");

    updateProg();
    if (c < 2) {//one settings file is missing
        help();
    }
}

MainWindow::~MainWindow()
{
    OHMpro->close();
    QProcess *cleanup = new QProcess();
    cleanup->start("RefreshNotificationArea.exe");
    delete ui;
}

void MainWindow::updateProg() {

    if (set.collecting) {
        updateList();
        if (set.memGraph) {
            sceneMem->clear();
            grapher.draw(sceneMem, mem.array,10, mem.mu, display->graphcolor, display->fontcolor, "Memory Useage", 0, 100);
            ui->MemoryGraphView->setScene(sceneMem);
        }
        if (set.CPUSpeedGraph) {
            sceneCPUS->clear();
            grapher.draw(sceneCPUS, cspeed.array,10, cspeed.cs, display->graphcolor, display->fontcolor, "CPU Speed", 0, 10);
            ui->CPUSpeedView->setScene(sceneCPUS);
        }
        if (set.CPUTempGraph) {
            sceneCPUT->clear();
            grapher.draw(sceneCPUT, ctemp.array,10, ctemp.ct, display->graphcolor, display->fontcolor, "CPU Temperature", 0, 150);
            ui->CPUTempView->setScene(sceneCPUT);
        }
        if (set.CPUUseGraph) {
            sceneCPUU->clear();
            grapher.draw(sceneCPUU, cpuUthread.array,10, cpuUthread.cu, display->graphcolor, display->fontcolor, "CPU Useage", 0, 100);
            ui->CPUUseView->setScene(sceneCPUU);
        }
        if (set.GPUTempGraph) {
            sceneGPUU->clear();
            grapher.draw(sceneGPUU, gpu.array,10, gpu.gt, display->graphcolor, display->fontcolor, "GPU Temperature", 0, 150);
            ui->GPUTempView->setScene(sceneGPUU);
        }
    }


    if(set.macro)//macro widget is shown
    {
        sceneMacro->clear();
        macro.draw(sceneMacro, Qt::red, display->fontcolor);
        ui->MacroView->setScene(sceneMacro);
        ui->MacroList->clear();
        QString name = macro.loc;
        ui->MacroList->addItem(name.right((name.length() - name.lastIndexOf("/")) - 1));
        for (int i = 0; i < macro.counter; i++) {
            QChar key = static_cast<char>(macro.macro[i]);
            QString sKey = key;
            ui->MacroList->addItem(sKey);
        }
        ui->MacroList->setGeometry(ui->MacroView->x() + 20, ui->MacroView->y() + 130, 121, 151);
    }



    if(set.Keyboard)
    {
        keyboardThread.setLineColor(display->fontcolor);
        keyboardThread.draw();
        ui->keyboardView->setScene(keyboardThread.scene);
    }
    if(fromfile)
    {
        timer->start(set.refresh);


        fromfile = false;
    }


        if(set.updated) {
            set.updated = false;
            writeFile();
        }

        if (cspeed.cpuSpeed > 1) {
            OHMloaded = true;
        }

        if (cspeed.cpuSpeed == 0 && OHMloaded) {
            if (OHMoff == false) {
                OHMoff = true;
                OHMmessage = true;
            }
        }

        if (OHMoff) {
            if (OHMmessage) {
                qDebug() << "ohm is off";
                //graphs
                set.CPUSpeedGraph = false;
                set.CPUTempGraph = false;
                ui->actionGraph_Display_3->setChecked(0);
                ui->actionGraph_Display_4->setChecked(0);
                ui->CPUSpeedView->setVisible(false);
                ui->CPUTempView->setVisible(false);


                textDisplay.removeAt(textDisplay.indexOf(4));
                ui->actionNumeric_Display_3->setChecked(0);
                textDisplay.removeAt(textDisplay.indexOf(5));
                ui->actionSession_High_Low_3->setChecked(0);
                textDisplay.removeAt(textDisplay.indexOf(7));
                ui->actionSession_High_Low_4->setChecked(0);
                textDisplay.removeAt(textDisplay.indexOf(8));
                ui->actionNumeric_Display_6->setChecked(0);
                textDisplay.removeAt(textDisplay.indexOf(6));
                ui->actionNumeric_Display_4->setChecked(0);


                set.cpuSpeed = false;
                set.HLcpuSpeed = false;
                set.HLcpuTemp = false;
                set.cpuCoreTemp = false;
                set.cpuTemp = false;

                QMessageBox* msgBox = new QMessageBox( this );
                msgBox->setAttribute( Qt::WA_DeleteOnClose );
                msgBox->setStandardButtons( QMessageBox::Ok );
                msgBox->setWindowTitle( tr("GiS") );
                msgBox->setText( tr("Open Hardware Monitor has closed. This process is needed to provide you with all posible metrics. To get them back you can either:\n1. Restart GiS\n2. Re-open Open Hardware Monitor (Note: Open Hardware Monitor process will not stop when GiS is closed if this is chosen)\nThen, add back the metrics you want.") );
                msgBox->setModal( false );
                //msgBox->serIcon
                msgBox->open( this, SLOT(msgBoxClosed(QAbstractButton*)) );
                OHMmessage = false;
            }
        }
        ui->centralWidget->setStyleSheet(display->style);
        ui->mainList->setStyleSheet(display->style);
        ui->MacroView->setStyleSheet(display->style);
}

void MainWindow::updateList() {
    std::string high;
    std::string low;
    ohm.update();
    ui->mainList->clear();
    if (set.collecting) {

        if (set.memUse || set.HLmemUsage || set.memGraph) {
            mem.getUsage();
        }

        if (set.cpuSpeed || set.HLcpuSpeed || set.CPUSpeedGraph) {
            cspeed.getSpeed(ohm.CPUspeed);
        }
        if (set.cpuTemp || set.HLcpuTemp || set.cpuCoreTemp || set.CPUTempGraph) {
            ctemp.getTemp(ohm.CPUtemp);
        }

        if (set.gpuTemp || set.HLgpuTemp) {
            gpu.getTemp();
        }
    }
    /*
     * 0 memory
     * 1 mem highlow
     * 2 cpu use
     * 3 cpu use highlow
     * 4 cpu speed
     * 5 cpu speed highlow
     * 6 cpu temp
     * 7 cpu temp highlow
     * 8 cpu core temps
     * 9 gpu temp
     * 10 gpu temp highlow
     * 11 Blank line
     */

    for (int i = 0; i < textDisplay.length(); i++)
    {
        switch (textDisplay.at(i)) {
            case 0:
                memUsage = "Memory Usage: " + intToString(mem.memoryUsage) + "%";
                qmemUse = QString::fromStdString(memUsage);
                ui->mainList->addItem(qmemUse);
                break;
            case 1:
                high = "Memory Usage High: " + intToString(mem.HmemUsage) + "%";
                low = "Memory Usage Low: " + intToString(mem.LmemUsage) + "%";
                ui->mainList->addItem(QString::fromStdString(high));
                ui->mainList->addItem(QString::fromStdString(low));
                break;
            case 2:
                cpuUse = "CPU Usage: " + intToString(cpuUthread.cpuUsage) + "%";
                qcpuUse = QString::fromStdString(cpuUse);
                ui->mainList->addItem(qcpuUse);
                break;
            case 3:
                high = "CPU Usage High: " + intToString(cpuUthread.HcpuUse) + "%";
                low = "CPU Usage Low: " + intToString(cpuUthread.LcpuUse) + "%";
                ui->mainList->addItem(QString::fromStdString(high));
                ui->mainList->addItem(QString::fromStdString(low));
                break;
            case 4:
                cpuSpeed = "CPU Speed: " + doubleToString(cspeed.cpuSpeed) + "Ghz";
                qcpuSpeed = QString::fromStdString(cpuSpeed);
                ui->mainList->addItem(qcpuSpeed);
                break;
            case 5:
                high = "CPU Speed High: " + doubleToString(cspeed.HcpuSpeed) + "Ghz";
                low = "CPU Speed Low: " + doubleToString(cspeed.LcpuSpeed) + "Ghz";
                ui->mainList->addItem(QString::fromStdString(high));
                ui->mainList->addItem(QString::fromStdString(low));
                break;
            case 6:
                cpuTemp = "CPU Temp: " + intToString(ctemp.cpuHighTemp) + "°C";
                qcpuTemp = QString::fromStdString(cpuTemp);
                ui->mainList->addItem(qcpuTemp);
                break;
            case 7:
                high = "CPU Temp High: " + intToString(ctemp.HcpuTemp) + "°C";
                low = "CPU Temp Low: " + intToString(ctemp.LcpuTemp) + "°C";
                ui->mainList->addItem(QString::fromStdString(high));
                ui->mainList->addItem(QString::fromStdString(low));
                break;
            case 8:
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
                break;
            case 9:
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
                break;
            case 10:
                high = "GPU Temp High: " + intToString(gpu.HgpuTemp) + "°C";
                low = "GPU Temp Low: " + intToString(gpu.LgpuTemp) + "°C";
                ui->mainList->addItem(QString::fromStdString(high));
                ui->mainList->addItem(QString::fromStdString(low));
                break;
            case 11:
                ui->mainList->addItem("");
                break;
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

/*memorygraph 0
 * cpuTemp 1
 * cpuUse 2
 * cpuSpeed 3
 * gpuGraph 4
 * keyboard 5
 * macro 6
 * autoclicker 7 */

Macro::keys keyStaff[104] = {            //KEYS                                                          ///INDEX

                                                           0xA0,16777248,   0xC0,96,        0x1B,16777216, //left shift, tilde, escape                     :: 0,1,2
                                                           0x70,16777264,   0x71,16777265,  0x72,16777266, //F1-3                                          :: 3,4,5
                                                           0x73,16777267,   0x74,16777268,  0x75,16777269, //F4-6                                          :: 6,7,8
                                                           0x76,16777270,   0x77,16777271,  0x78,16777272, //F7-9                                          :: 9,10,11
                                                           0x79,16777273,   0x7A,16777274,  0x7B,16777275, //F10-12                                        :: 12,13,14
                                                           0x30,48,         0x31,49,        0x32,50, //0-2                                           :: 15,16,17
                                                           0x33,51,         0x34,52,        0x35,53, //3-5                                           :: 18,19,20
                                                           0x36,54,         0x37,55,        0x38,56, //6-8                                           :: 21,22,23
                                                           0x39,57,         0x41,65,        0x42,66, //9, A, B                                       :: 24,25,26
                                                           0x43,67,         0x44,68,        0x45,69, //C, D, E                                       :: 27,28,29
                                                           0x46,70,         0x47,71,        0x48,72, //F, G, H                                       :: 30,31,32
                                                           0x49,73,         0x4A,74,        0x4B,75, //I, J, K                                       :: 33,34,35
                                                           0x4C,76,         0x4D,77,        0x4E,78, //L, M, N                                       :: 36,37,38
                                                           0x4F,79,         0x50,80,        0x51,81, //O, P, Q                                       :: 39,40,41
                                                           0x52,82,         0x53,83,        0x54,84, //R, S, T                                       :: 42,43,44
                                                           0x55,85,         0x56,86,        0x57,87, //U, V, W                                       :: 45,46,47
                                                           0x58,88,         0x59,89,        0x5A,90, //X, Y, Z                                       :: 48,49,50
                                                           0x08,16777219,   0x09,16777217,  0x0D,16777220, //backspace, tab, enter                         :: 51,52,53
                                                           0x14,16777252,   0x20,32,        0x5B,16777250, //capslock, space, left windows                 :: 54,55,56
                                                           0x5C,16777250,   0x5D,16777301,  0x90,16777253, //right windows, option key, numlock            :: 57,58,59
                                                           0x60,48,         0x61,49,        0x62,50, //numpad 0-2                                    :: 60,61,62
                                                           0x63,51,         0x64,52,        0x65,53, //numpad 3-5                                    :: 63,64,65
                                                           0x66,54,         0x67,55,        0x68,56, //numpad 6-8                                    :: 66,67,68
                                                           0x69,57,         0x6A,42,        0x6B,43, //numpad 9, numpad multiply, numpad add         :: 69,70,71
                                                           0x6C,16777221,   0x6D,45,        0x6E,46, //numpad enter, numpad subract, numpad decimal  :: 72,73,74
                                                           0x6F,47,         0x21,16777238,  0x22,16777239, //numpad divide, page up, page down             :: 75,76,77
                                                           0x23,16777233,   0x24,16777232,  0x25,0, //end, home, left arrow                         :: 78,79,80
                                                           0x26,0,          0x27,0,         0x28,0, //up arrow, right arrow, down arrow             :: 81,82,83
                                                           0x2C,0,          0x2D,16777222,  0x2E,16777223, //printscreen, insert, delete                   :: 84,85,86
                                                           0xA1,16777248,   0xA2,16777249,  0xA3,16777249, //right shift, left control, right control      :: 87,88,89
                                                           0xBA,59,         0xBB,61,        0xBC,44, //semicolon, plus, comma                        :: 90,91,92
                                                           0xBD,45,         0xBE,46,        0xBF,47, //minus, period, backslash /                    :: 93,94,95
                                                           0xDB,91,         0xDC,92,        0xDD,93, //left bracket, forwardslash \, right bracket   :: 96,97,98
                                                           0x91,16777254,   0xA4,16777251,  0xA5,16777251, //scroll lock, left alt, right alt              :: 99,100,101
                                                           0xDE,0,           0x13,0};

void MainWindow::keyPressEvent(QKeyEvent *event) {
    //qDebug() << event->key();
    if (macro.activationKeyRec == true) {
        macro.activationKey = event->key();
        macro.activationKeyRec = false;
        int j;
        for(j = 0; j < 104; j++)
        {
            if(macro.activationKey == keyStaff[j].ascii)
            {
                macro.winActivationKey = keyStaff[j].vk;
                j = 110;
            }
        }
        macro.activationKeyRec = false;
        qDebug() << event->key();
    }
    if (isSelected == 1) {
        int x = 0;
        int y = 0;
        if (event->key() == 0x57) {
            y = -10;
        }
        if (event->key() == 0x53) {
            y = 10;
        }
        if (event->key() == 0x41) {
            x = -10;
        }
        if (event->key() == 0x44) {
            x = 10;
        }
        if (event->key() == 16777220 ) {
            isSelected = 0;
            this->setCursor(Qt::ArrowCursor);
        }
        else {
            switch(selected) {
                case 0:
                    ui->MemoryGraphView->setGeometry(ui->MemoryGraphView->x() + x, ui->MemoryGraphView->y() + y, 381, 281);
                    break;
                case 1:
                    ui->CPUTempView->setGeometry(ui->CPUTempView->x() + x, ui->CPUTempView->y() + y, 381, 281);
                    break;
                case 2:
                    ui->CPUUseView->setGeometry(ui->CPUUseView->x() + x, ui->CPUUseView->y() + y, 381, 281);
                    break;
                case 3:
                    ui->CPUSpeedView->setGeometry(ui->CPUSpeedView->x() + x, ui->CPUSpeedView->y() + y, 381, 281);
                    break;
                case 4:
                    ui->GPUTempView->setGeometry(ui->GPUTempView->x() + x, ui->GPUTempView->y() + y, 381, 281);
                    break;
                case 5:
                    ui->keyboardView->setGeometry(ui->keyboardView->x() + x, ui->keyboardView->y() + y, 496, 156);
                    break;
                case 6:
                    ui->MacroView->setGeometry(ui->MacroView->x() + x, ui->MacroView->y() + y, 251, 291);
                    break;
            }
        }
    }
    ui->MacroList->setGeometry(ui->MacroView->x() + 20, ui->MacroView->y() + 130, 121, 151);
    set.updated = 1;
}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    if (isSelected == 0) {
        if (event->button() == Qt::LeftButton && ui->MemoryGraphView->geometry().contains(event->pos()) && ui->MemoryGraphView->isVisible()) {
            this->setCursor(Qt::PointingHandCursor);
            isSelected = 1;
            selected = 0;
        }
        if (event->button() == Qt::LeftButton && ui->CPUTempView->geometry().contains(event->pos()) && ui->CPUTempView->isVisible()) {
            this->setCursor(Qt::PointingHandCursor);
            isSelected = 1;
            selected = 1;
        }
        if (event->button() == Qt::LeftButton && ui->CPUUseView->geometry().contains(event->pos()) & ui->CPUUseView->isVisible()) {
            this->setCursor(Qt::PointingHandCursor);
            isSelected = 1;
            selected = 2;
        }
        if (event->button() == Qt::LeftButton && ui->CPUSpeedView->geometry().contains(event->pos()) && ui->CPUSpeedView->isVisible()) {
            this->setCursor(Qt::PointingHandCursor);
            isSelected = 1;
            selected = 3;
        }
        if (event->button() == Qt::LeftButton && ui->GPUTempView->geometry().contains(event->pos()) && ui->GPUTempView->isVisible()) {
            this->setCursor(Qt::PointingHandCursor);
            isSelected = 1;
            selected = 4;
        }
        if (event->button() == Qt::LeftButton && ui->keyboardView->geometry().contains(event->pos()) && ui->keyboardView->isVisible()) {
            this->setCursor(Qt::PointingHandCursor);
            isSelected = 1;
            selected = 5;
        }
        if (event->button() == Qt::LeftButton && ui->MacroView->geometry().contains(event->pos()) && ui->MacroView->isVisible()) {
            if ((event->x() > (ui->MacroView->x() + 60 ) && event->x() < (ui->MacroView->x() + 110)) && (event->y() > (ui->MacroView->y() + 80 ) && event->y() < (ui->MacroView->y() + 150))) {
                ui->MacroList->clear();
                macro.loc = "";
                macro.recHit();
            }
            else {
                if ((event->x() > (ui->MacroView->x() + 160 ) && event->x() < (ui->MacroView->x() + 205)) && (event->y() > (ui->MacroView->y() + 180 ) && event->y() < (ui->MacroView->y() + 220))) {
                    macro.onHit();
                }
                else {
                    if ((event->x() > (ui->MacroView->x() + 160 ) && event->x() < (ui->MacroView->x() + 205)) && (event->y() > (ui->MacroView->y() + 230 ) && event->y() < (ui->MacroView->y() + 270))) {
                        macro.offHit();
                    }
                    else {
                        if ((event->x() > (ui->MacroView->x() + 140 ) && event->x() < (ui->MacroView->x() + 190)) && (event->y() > (ui->MacroView->y() + 80 ) && event->y() < (ui->MacroView->y() + 150))) {
                            macro.loadHit();
                        }
                        else {
                            this->setCursor(Qt::PointingHandCursor);
                            isSelected = 1;
                            selected = 6;
                        }
                    }
                }
            }
        }
    }
    else {
        switch(selected) {
            case 0:
                ui->MemoryGraphView->setGeometry(event->x() - 30,event->y() - 30,381, 281);
                isSelected = 0;
                break;
            case 1:
                ui->CPUTempView->setGeometry(event->x() - 30,event->y() - 30,381, 281);
                isSelected = 0;
                break;
            case 2:
                ui->CPUUseView->setGeometry(event->x() - 30,event->y() - 30,381, 281);
                isSelected = 0;
                break;
            case 3:
                ui->CPUSpeedView->setGeometry(event->x() - 30,event->y() - 30,381, 281);
                isSelected = 0;
                break;
            case 4:
                ui->GPUTempView->setGeometry(event->x() - 30,event->y() - 30,381, 281);
                isSelected = 0;
                break;
            case 5:
                ui->keyboardView->setGeometry(event->x() - 30,event->y() - 30,496, 156);
                isSelected = 0;
                break;
            case 6:
                ui->MacroView->setGeometry(event->x() - 30,event->y() - 30,251, 291);
                isSelected = 0;
                break;
        }
    this->setCursor(Qt::ArrowCursor);
    }
    ui->MacroList->setGeometry(ui->MacroView->x() + 20, ui->MacroView->y() + 130, 121, 151);
    set.updated = 1;
}


void MainWindow::on_actionNumeric_Display_triggered()
{
    if (set.memUse) {
        set.memUse = false;
        textDisplay.removeAt(textDisplay.indexOf(0));
        ui->actionNumeric_Display->setChecked(0);
    }
    else {
        textDisplay.append(0);
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
        textDisplay.removeAt(textDisplay.indexOf(2));
        ui->actionNumeric_Display_2->setChecked(0);
    }
    else {
        textDisplay.append(2);
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
        textDisplay.removeAt(textDisplay.indexOf(4));
        ui->actionNumeric_Display_3->setChecked(0);
    }
    else {
        set.cpuSpeed = true;
        textDisplay.append(4);
        ui->actionNumeric_Display_3->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_4_triggered()
{
    if (set.cpuTemp) {
        set.cpuTemp = false;
        textDisplay.removeAt(textDisplay.indexOf(6));
        ui->actionNumeric_Display_4->setChecked(0);
    }
    else {
        set.cpuTemp = true;
        textDisplay.append(6);
        ui->actionNumeric_Display_4->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_6_triggered()  //show indivudal cores
{
    if (set.cpuCoreTemp) {
        set.cpuCoreTemp = false;
        textDisplay.removeAt(textDisplay.indexOf(8));
        ui->actionNumeric_Display_6->setChecked(0);
    }
    else {
        set.cpuCoreTemp = true;
        textDisplay.append(8);
        ui->actionNumeric_Display_6->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionNumeric_Display_5_triggered()
{
    if (set.gpuTemp) {
        set.gpuTemp = false;
        textDisplay.removeAt(textDisplay.indexOf(9));
        ui->actionNumeric_Display_5->setChecked(0);
    }
    else {
        set.gpuTemp = true;
        textDisplay.append(9);
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
    qDebug() << display->back;
    qDebug() << display->font;
    display->tempfcolor = display->fontcolor;
    display->tempgcolor = display->graphcolor;
    display->temphcolor = display->kHighlight;
    display->tempstyle = ui->mainList->styleSheet();
    display->setModal(false);
    display->update();
    display->show();
}

void MainWindow::on_actionSession_High_Low_triggered()
{
    if (set.HLmemUsage) {
        set.HLmemUsage = false;
        textDisplay.removeAt(textDisplay.indexOf(1));
        ui->actionSession_High_Low->setChecked(0);
    }
    else {
        set.HLmemUsage = true;
        textDisplay.append(1);
        ui->actionSession_High_Low->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionSession_High_Low_2_triggered()
{
    if (set.HLcpuUse) {
        set.HLcpuUse = false;
        textDisplay.removeAt(textDisplay.indexOf(3));
        ui->actionSession_High_Low_2->setChecked(0);
    }
    else {
        set.HLcpuUse = true;
        textDisplay.append(3);
        ui->actionSession_High_Low_2->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionSession_High_Low_3_triggered()
{
    if (set.HLcpuSpeed) {
        set.HLcpuSpeed = false;
        textDisplay.removeAt(textDisplay.indexOf(5));
        ui->actionSession_High_Low_3->setChecked(0);
    }
    else {
        set.HLcpuSpeed = true;
        textDisplay.append(5);
        ui->actionSession_High_Low_3->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionSession_High_Low_4_triggered()
{
    if (set.HLcpuTemp) {
        set.HLcpuTemp = false;
        textDisplay.removeAt(textDisplay.indexOf(7));
        ui->actionSession_High_Low_4->setChecked(0);
    }
    else {
        set.HLcpuTemp = true;
        textDisplay.append(7);
        ui->actionSession_High_Low_4->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionSession_High_Low_5_triggered()
{
    if (set.HLgpuTemp) {
        set.HLgpuTemp = false;
        textDisplay.removeAt(textDisplay.indexOf(10));
        ui->actionSession_High_Low_5->setChecked(0);
    }
    else {
        set.HLgpuTemp = true;
        textDisplay.append(10);
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
        ui->keyboardView->setGeometry(200,200,496, 156);
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

void MainWindow::on_actionSet_Highlight_Color_triggered()
{
    display->kHighlight = QColorDialog::getColor(Qt::white, this);
    keyboardThread.setHighlight(display->kHighlight);
    keyboardThread.hfromset = 1;
    display->fromkey = 1;
    display->update();
}

void MainWindow::on_actionGraph_Display_triggered()
{
    if (set.memGraph) {
        set.memGraph = false;
        ui->MemoryGraphView->setVisible(false);
        ui->actionGraph_Display->setChecked(0);
    }
    else {
        set.memGraph = true;
        ui->MemoryGraphView->setGeometry(200,200,381, 281);
        ui->MemoryGraphView->setVisible(true);
        ui->actionGraph_Display->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionGraph_Display_2_triggered()
{
    // CPU use
    if (set.CPUUseGraph) {
        set.CPUUseGraph = false;
        ui->CPUUseView->setVisible(false);
        ui->actionGraph_Display_2->setChecked(0);
    }
    else {
        set.CPUUseGraph = true;
        ui->CPUUseView->setGeometry(200,200,381, 281);
        ui->CPUUseView->setVisible(true);
        ui->actionGraph_Display_2->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionGraph_Display_3_triggered()
{
    if (set.CPUSpeedGraph) {
        set.CPUSpeedGraph = false;
        ui->CPUSpeedView->setVisible(false);
        ui->actionGraph_Display_3->setChecked(0);
    }
    else {
        set.CPUSpeedGraph = true;
        ui->CPUSpeedView->setGeometry(200,200,381, 281);
        ui->CPUSpeedView->setVisible(true);
        ui->actionGraph_Display_3->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionGraph_Display_4_triggered()
{
    if (set.CPUTempGraph) {
        set.CPUTempGraph = false;
        ui->CPUTempView->setVisible(false);
        ui->actionGraph_Display_4->setChecked(0);
    }
    else {
        set.CPUTempGraph = true;
        ui->CPUTempView->setGeometry(200,200,381, 281);
        ui->CPUTempView->setVisible(true);
        ui->actionGraph_Display_4->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionGraph_Display_5_triggered()
{
    if (set.GPUTempGraph) {
        set.GPUTempGraph = false;
        ui->GPUTempView->setVisible(false);
        ui->actionGraph_Display_5->setChecked(0);
    }
    else {
        set.GPUTempGraph = true;
        ui->GPUTempView->setVisible(true);
        ui->GPUTempView->setGeometry(200,200,381, 281);
        ui->actionGraph_Display_5->setChecked(1);
    }
    set.updated = true;
    updateProg();
}

void MainWindow::on_actionAdd_Blank_Line_triggered()
{
    textDisplay.append(11);
}

void MainWindow::on_actionMacro_Recorder_triggered()
{
    if(set.macro){
        ui->actionMacro_Recorder->setChecked(0);
        set.macro = false;
        ui->MacroView->setVisible(false);
        ui->MacroList->setVisible(false);
    }
    else {
        ui->actionMacro_Recorder->setChecked(1);
        ui->MacroView->setGeometry(200,200,251, 291);
        set.macro = true;
        ui->MacroView->setVisible(true);
        ui->MacroList->setVisible(true);
    }
    set.updated = true;
    updateProg();
}

bool MainWindow::eventFilter(QObject *t, QEvent *e)
{

    if(set.macro)
    {
        if(e->type() == QEvent::KeyPress)
        {
            QKeyEvent *k = static_cast<QKeyEvent*>(e);
            macro.keyPressed(k);
        }
    }
    return QObject::eventFilter(t, e);
}

void MainWindow::on_actionRemove_Blank_Line_triggered()
{
    textDisplay.removeAt(textDisplay.lastIndexOf(11));
}

void MainWindow::on_actionSet_Activation_Key_triggered()
{
    macro.activationKeyRec = true;
}

void MainWindow::on_actionSet_Delay_triggered()
{
    bool ok;
    macro.delay = QInputDialog::getInt(this, tr("Set Macro Delay"), tr("Delay (ms):"), macro.delay, 0, 1000, 1, &ok);
}

void MainWindow::help() {
    QMessageBox* msgBox = new QMessageBox( this );
    msgBox->setAttribute( Qt::WA_DeleteOnClose );
    msgBox->setStandardButtons( QMessageBox::Ok );
    msgBox->setWindowTitle( tr("GiS") );
    QFont hFont("Times", 12);
    msgBox->setFont(hFont);
    msgBox->setText( tr("Welcome to Gaming Information Suite.\nTo add information click on the add menu and choose what you would like to be displayed.\nTo move graphics around you can:\n\tLeft click anywhere on the graphic and left click again where you would like it placed.\n\tLeft click on a graphic then use WASD keys to move it around. Then hit enter to set location.\nTo change display options click the Display Settings option in the main menu.\nTo open this dialouge again click on help in the main menu.") );
    msgBox->setModal( false );
    //msgBox->serIcon
    msgBox->open( this, SLOT(msgBoxClosed(QAbstractButton*)) );
}

void MainWindow::on_actionHelp_triggered() {
    help();
}

int MainWindow::myInt(QString s, int lower, int upper)
{
    int temp;
    bool ok;
    temp = s.toInt(&ok);
    if(ok)
    {
        if(temp >= lower && temp <= upper)
        {
            return temp;
        }
        else
        {
            qDebug() << "file is corrupt";
            if(upper < 2000)return upper;
            else return lower;
        }
    }
    else
    {
        qDebug() << "file is corrupt";
        if(upper < 2000)return upper;
        else return lower;
    }
}

void MainWindow::writeFile()
{
    QString fileName = "settings";

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
    out << "\n";
    out << set.memGraph;
    out << "\n";
    out << set.CPUSpeedGraph;
    out << "\n";
    out << set.CPUTempGraph;
    out << "\n";
    out << set.CPUUseGraph;
    out << "\n";
    out << set.GPUTempGraph;
    out << "\n";
    out << set.macro;
    out << "\n";
    out << textDisplay.length();
    out << "\n";
    foreach(const int x, textDisplay)
    {
        out << x;
        out << "\n";
    }
    out << ui->MemoryGraphView->y();
    out << "\n";
    out << ui->MemoryGraphView->x();
    out << "\n";
    out << ui->CPUTempView->y();
    out << "\n";
    out << ui->CPUTempView->x();
    out << "\n";
    out << ui->CPUUseView->y();
    out << "\n";
    out << ui->CPUUseView->x();
    out << "\n";
    out << ui->CPUSpeedView->y();
    out << "\n";
    out << ui->CPUSpeedView->x();
    out << "\n";
    out << ui->GPUTempView->y();
    out << "\n";
    out << ui->GPUTempView->x();
    out << "\n";
    out << ui->keyboardView->y();
    out << "\n";
    out << ui->keyboardView->x();
    out << "\n";
    out << ui->MacroView->y();
    out << "\n";
    out << ui->MacroView->x();
    out << "\n";
    mFile.flush();
    mFile.close();
}
