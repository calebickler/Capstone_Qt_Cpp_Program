#include "displaysettings.h"
#include "ui_displaysettings.h"
#include "qcolordialog.h"
#include "qdebug.h"

QString font;
QString back;
QString graph;
QString style;
QString tempstyle;

displaysettings::displaysettings(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::displaysettings)
{
    ui->setupUi(this);
}

displaysettings::~displaysettings()
{
    delete ui;
}

void displaysettings::on_pushButton_clicked()
{
    QColor bgColor;
    std::string colorstyle;
    char* temp = new char[100];
    bgColor = QColorDialog::getColor(Qt::white, this);
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
    style = font + back + graph;
}

void displaysettings::on_buttonBox_rejected()
{
    style = tempstyle;
}

void displaysettings::on_pushButton_2_clicked()
{
    QColor bgColor;
    std::string colorstyle;
    char* temp = new char[100];
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
    style = font + back + graph;
}

void displaysettings::on_buttonBox_accepted()
{

}
