
#include "Macro.h"
#include <QGraphicsTextItem>
#include <QDebug>
#include <QKeyEvent>
#include <QInputDialog>
#include <QFileDialog>
#include <windows.h>

Macro::keys keyStuff[104] = {            //KEYS                                                          ///INDEX

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

Macro::Macro() {
    recording = 0;
    counter = 0;
    delay = 500;
    playing = 0;
    on = 1;
    off = 0;
    activationKey = 96;//default
    activationKeyRec = false;
    loc = "debug/macros/";
}

void Macro::draw(QGraphicsScene* scene, QColor background, QColor font) {
    //size 241, 281
    //QBrush Ybrush(Qt::blue);
    //scene->setBackgroundBrush(Ybrush);

    QFont titleFont("Times", 10, QFont::Bold);
    QGraphicsTextItem * titleItem = new QGraphicsTextItem;
    titleItem->setPos(70,10);
    titleItem->setDefaultTextColor(font);
    titleItem->setFont(titleFont);
    titleItem->setPlainText("Macro Recorder");
    scene->addItem(titleItem);

    QFont actFont("Times", 9, QFont::Bold);
    QGraphicsTextItem * actItem = new QGraphicsTextItem;
    actItem->setPos(30,40);
    actItem->setDefaultTextColor(font);
    actItem->setFont(actFont);
    if (activationKeyRec == false) {
        QChar key = static_cast<char>(activationKey);
        QString sKey = key;
        actItem->setPlainText("Activation Key: (" + sKey + ")");
    }
    else {
        actItem->setPlainText("Activation Key: RECORDING");
    }
    scene->addItem(actItem);

    QBrush onB(font);
    QPen onP(font);
    scene->addRect(160,150,40,40, onP, onB);
    scene->addRect(160,200,40,40, onP, onB);

    QFont onFont("Times", 11, QFont::Bold);
    QGraphicsTextItem * onItem = new QGraphicsTextItem;
    onItem->setPos(166,158);
    if (on) {
        onItem->setDefaultTextColor(Qt::yellow);
    }
    else {
       onItem->setDefaultTextColor(Qt::black);
    }
    onItem->setFont(onFont);
    onItem->setPlainText("On");
    scene->addItem(onItem);

    QFont offFont("Times", 11, QFont::Bold);
    QGraphicsTextItem * offItem = new QGraphicsTextItem;
    offItem->setPos(166,208);
    if (on) {
        offItem->setDefaultTextColor(Qt::black);
    }
    else {
       offItem->setDefaultTextColor(Qt::yellow);
    }
    offItem->setFont(offFont);
    offItem->setPlainText("Off");
    scene->addItem(offItem);

    if (recording) {
        QBrush recB(Qt::red);
        QPen recP(Qt::red);
        scene->addRect(60,80,30,30, recP, recB);
    }
    else {
        QBrush recB(font);
        QPen recP(font);
        scene->addRect(60,80,30,30, recP, recB);
    }

    QBrush loadB(font);
    QPen loadP(font);
    scene->addRect(140,80,30,30, loadP, loadB);

    QFont loadFont("Times", 7, QFont::Bold);
    QGraphicsTextItem * loadItem = new QGraphicsTextItem;
    loadItem->setPos(140,85);
    loadItem->setDefaultTextColor(Qt::black);
    loadItem->setFont(loadFont);
    loadItem->setPlainText("Load");
    scene->addItem(loadItem);

    QFont recFont("Times", 8, QFont::Bold);
    QGraphicsTextItem * recItem = new QGraphicsTextItem;
    recItem->setPos(61,83);
    recItem->setDefaultTextColor(Qt::black);
    recItem->setFont(recFont);
    recItem->setPlainText("Rec");
    scene->addItem(recItem);

}

void Macro:: run() {
    while(true)
    {
        while(playing)
        {
            playMacro();
            Sleep(delay);
        }
    }
}

void Macro::keyPressed(QKeyEvent *e)
{
    if(recording)
    {
        macro[counter] = e->key();
        counter++;
    }/*
    else
    {
        if(e->key() == activationKey && on == 1)
        {
            if(playing)
            {
                playing = 0;
                qDebug() << "not playing";
            }
            else
            {
                playing = 1;
                qDebug() << "playing";
            }
        }
    }*/
}

void Macro::recHit() {
    if(recording == 1)
    {
        recording = 0;
        bool ok;
        QInputDialog *popup = new QInputDialog();
        QString text = popup->getText(0, "Name Macro", "Please name your macro to be saved to file.", QLineEdit::Normal,
                                               "", &ok);
        if(ok)
        {
            writeFile(text);
        }
        qDebug() << "Record Off";
    }
    else
    {
        recording = 1;
        qDebug() << "Record On";
    }
}

void Macro::writeFile(QString s)
{
        loc = "debug/macros/";
        int i;
        s.append(".macro");
        loc.append(s);
        QFile mFile(loc);

        qDebug() << loc;

        if(!mFile.open(QFile::WriteOnly | QFile::Text))
        {
            qDebug() << "Could not write to file.\n";
            return;
        }
        QTextStream out(&mFile);
        i = 0;
        while(macro[i] != 0)
        {
            out << macro[i];
            out << "\n";
            i++;
        }
        mFile.flush();
        mFile.close();

}


void Macro::readFile()
{
    loc = QFileDialog::getOpenFileName(0, "/debug/macros", tr("*.macro"));

    QFile mFile(loc);

    if(!mFile.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << "Could not read from file.\n";
        return;
    }
    QTextStream in(&mFile);
    int i = 0;
    //std::fill_n(macro, 1000, 0);
    while(!in.atEnd())
    {
        macro[i] = in.readLine().toInt();
        i++;
    }

    counter = i;
}

void Macro::onHit() {
    qDebug() << "On";
    on = 1;
    off = 0;
}

void Macro::offHit() {
    qDebug() << "Off";
    off = 1;
    on = 0;
}

void Macro::pressKey(int vkey)
{
    INPUT ip;
    ip.type = INPUT_KEYBOARD;
    ip.ki.wScan = 0;
    ip.ki.time = 0;
    ip.ki.dwExtraInfo = 0;

    int j;
    for(j = 0; j < 104; j++)
    {
        if(vkey == keyStuff[j].ascii)
        {
            qDebug() << keyStuff[j].ascii;
            ip.ki.wVk = keyStuff[j].vk;
            ip.ki.dwFlags = 0;
            SendInput(1, &ip, sizeof(INPUT));
        }
    }

}

void Macro::playMacro(){
    int i;
    counter = 0;
    i = 0;
    while(macro[i] != 0) //reads off first 100 keys
    {
        //qDebug() << macro[i];
        pressKey(macro[i]);
        i++;//keys are stored as ascii integers; see www.asciitable.com
    }
}

void Macro::loadHit() {
    readFile();
    qDebug() << "Load";
}

void Macro::setActivation() {
    qDebug() << "set Activation";
}
