#include "displaysettings.h"
#include "ui_displaysettings.h"
#include "qcolordialog.h"
#include "qdebug.h"
#include <QFile>
#include <QTextStream>
#include <string>

QString font;
QString back;
QString graph;
QString style;
QString tempstyle;
QColor fontcolor;
QColor tempfcolor;
QColor graphcolor;
QColor tempgcolor;
QColor kHighlight;
QColor tempkcolor;
QString button1, button2, button3;
int fromsettings = 0;
int fromkey = 0;
int ok = 1;

displaysettings::displaysettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::displaysettings)
{
    ui->setupUi(this);
    fontcolor.setRgb(70,255,69);
}

displaysettings::~displaysettings()
{
    delete ui;
}

void displaysettings::on_pushButton_clicked()
{
    QColor bgColor;
    std::string colorstyle;
    char* temp = new char[200];
    bgColor = QColorDialog::getColor(Qt::white, this);
    fontcolor = bgColor;
    colorstyle = "background-color: rgb(";
    font = "color: rgb(";
    sprintf(temp, "%d", bgColor.red());
    colorstyle.append(temp);
    font.append(temp);
    colorstyle.append(",");
    font.append(",");
    sprintf(temp, "%d", bgColor.green());
    colorstyle.append(temp);
    font.append(temp);
    colorstyle.append(",");
    font.append(",");
    sprintf(temp, "%d", bgColor.blue());
    colorstyle.append(temp);
    font.append(temp);
    colorstyle.append("); border: 1px solid black;");
    font.append(");");
    ui->pushButton->setStyleSheet(QString::fromStdString(colorstyle));
    button1 = QString::fromStdString(colorstyle);
    style = font + back;
}

void displaysettings::on_buttonBox_rejected()
{
    style = tempstyle;
    kHighlight = temphcolor;
    graphcolor = tempgcolor;
    fontcolor = tempfcolor;
}

void displaysettings::on_pushButton_2_clicked()
{
    QColor bgColor;
    std::string colorstyle;
    char* temp = new char[200];
    bgColor = QColorDialog::getColor(Qt::white, this);
    colorstyle = "background-color: rgb(";
    back = "background-color: rgb(";
    sprintf(temp, "%d", bgColor.red());
    colorstyle.append(temp);
    back.append(temp);
    colorstyle.append(",");
    back.append(",");
    sprintf(temp, "%d", bgColor.green());
    colorstyle.append(temp);
    back.append(temp);
    colorstyle.append(",");
    back.append(",");
    sprintf(temp, "%d", bgColor.blue());
    colorstyle.append(temp);
    back.append(temp);
    colorstyle.append("); border: 1px solid black;");
    back.append(");");
    ui->pushButton_2->setStyleSheet(QString::fromStdString(colorstyle));
    button2 = QString::fromStdString(colorstyle);
    style = font + back;
}

void displaysettings::update()
{
    if(fromsettings == 1)
    {
        if(button1.length() <= 60 && button1.length() >= 54)
        {
            ui->pushButton->setStyleSheet(button1);
        }
        else{ok = 0;}
        if(button2.length() <= 60 && button2.length() >= 54)
        {
            ui->pushButton_2->setStyleSheet(button2);
        }
        else{ok = 0;}
        if(button3.length() <= 60 && button3.length() >= 54)
        {
            ui->pushButton_3->setStyleSheet(button3);
        }
        else{ok = 0;}
        if(ok)
        {
            font = QString::fromStdString((button1.toStdString().substr(11, button1.toStdString().find(";"))));
            font.append(";");
            back = QString::fromStdString((button2.toStdString().substr(0, button2.toStdString().find(";"))));
            back.append(";");
            graph = button3;
        }
        else
        {
            qDebug() << "display settings file corrupt";
        }
    }
    if(fromkey == 1)
    {
        on_buttonBox_accepted();
        fromkey = 0;
    }
}

void displaysettings::on_buttonBox_accepted()
{
    writeFile();
}

void displaysettings::on_pushButton_3_clicked()
{
    QColor bgColor;
    std::string colorstyle;
    char* temp = new char[200];
    bgColor = QColorDialog::getColor(Qt::white, this);
    graphcolor = bgColor;
    colorstyle = "background-color: rgb(";
    graph = "color: rgb(";
    sprintf(temp, "%d", bgColor.red());
    colorstyle.append(temp);
    graph.append(temp);
    colorstyle.append(",");
    graph.append(",");
    sprintf(temp, "%d", bgColor.green());
    colorstyle.append(temp);
    graph.append(temp);
    colorstyle.append(",");
    graph.append(",");
    sprintf(temp, "%d", bgColor.blue());
    colorstyle.append(temp);
    graph.append(temp);
    colorstyle.append("); border: 1px solid black;");
    graph.append(");");
    ui->pushButton_3->setStyleSheet(QString::fromStdString(colorstyle));
    button3 = QString::fromStdString(colorstyle);
}

void displaysettings::writeFile(){
    QString fileName = "displaysettings";

    QFile mFile(fileName);

    if(!mFile.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << "Could not write to file.\n";
        return;
    }
    QTextStream out(&mFile);
    out << style;
    out << "\n";
    out << button1;
    out << "\n";
    out << button2;
    out << "\n";
    out << button3;
    out << "\n";
    out << fontcolor.name();
    out << "\n";
    out << kHighlight.name();
    out << "\n";
    out << graphcolor.name();

    mFile.flush();
    mFile.close();
}
