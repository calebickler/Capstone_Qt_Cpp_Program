#ifndef MACRO_H
#define MACRO_H

#include <QGraphicsScene>
#include <QtCore>

class Macro : public QThread
{
public:
    Macro();
    struct keys{
        int vk;
        int ascii;
    };

    void draw(QGraphicsScene* scene, QColor background, QColor font);
    void recHit();
    void onHit();
    void offHit();
    void loadHit();
    void run();
    void record();
    void playMacro();
    long delay;
    int playing;
    int on,off;
    int recording;
    int counter;
    int activationKey;
    int winActivationKey;
    bool activationKeyRec;
    void pressKey(int);
    QString loc;
    void readFile();
    void writeFile(QString);
    void keyPressed(QKeyEvent *e);
    int macro[1000];
};

#endif // MACRO_H
