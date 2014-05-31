#ifndef DISPLAYSETTINGS_H
#define DISPLAYSETTINGS_H

#include <QDialog>

namespace Ui {
class displaysettings;
}

class displaysettings : public QDialog
{
    Q_OBJECT

public:
    explicit displaysettings(QWidget *parent = 0);
    void update();
    ~displaysettings();
    QString font, graph, back;
    QString style, tempstyle;
    QString button1, button2, button3;
    QColor fontcolor;
    QColor graphcolor;
    QColor kHighlight;
    int fromsettings, fromkey;


private slots:

    void on_pushButton_clicked();

    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::displaysettings *ui;
};

#endif // DISPLAYSETTINGS_H
