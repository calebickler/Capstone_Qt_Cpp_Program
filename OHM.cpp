#include "OHM.h"
#include <QTcpSocket>
#include <QString>

QTcpSocket* socket[5];
int port;
int tries;

//server is accecpting connections from 10540 - 10544

OHM::OHM() {
    port = 0;
    loaded = false;
    tries = 0;
    socket[0] = new QTcpSocket();
    socket[0]->connectToHost("127.0.0.1",10540);

    socket[1] = new QTcpSocket();
    socket[1]->connectToHost("127.0.0.1",10541);

    socket[2] = new QTcpSocket();
    socket[2]->connectToHost("127.0.0.1",10542);

    socket[3] = new QTcpSocket();
    socket[3]->connectToHost("127.0.0.1",10543);

    socket[4] = new QTcpSocket();
    socket[4]->connectToHost("127.0.0.1",10544);
}

void OHM::update() {
    CPUspeed = 0;
    for (int i = 0; i < 10; i++) {
        CPUtemp[i] = 0;
    }


    char out[1];
    out[0] = 'u';
    socket[port]->waitForConnected();
    socket[port]->write(out,1);
    socket[port]->flush();

    char c[200];
    socket[port]->read(c, 200);
    QString in(c);
    QStringList s;
    s = in.split(" ");

    CPUspeed = s[0].toInt();

    if (CPUspeed > 0) {
        int count = 0;
        int tempor;
        for (int i = 1; i < (s.length() - 1); i++) {
            tempor = s[i].toInt();
            if (tempor > 0 && tempor < 250) {
                CPUtemp[count] = tempor;
                count++;
            }
        }
    }

    if (CPUspeed == 0 || CPUtemp[0] == 0) {
        tries++;
        if (tries > 2) {
            tries = 0;
            port++;
            if (port == 5) {
                port = 0;
            }
        }
    }
    else {
        loaded = true;
    }

}
