#include "OHM.h"
#include <QTcpSocket>
#include <QString>

QTcpSocket* socket;

OHM::OHM() {
    CPUspeed = 0;
    for (int i = 0; i < 20; i++) {
        CPUtemp[i] = 0;
    }
    socket = new QTcpSocket();
    socket->connectToHost("127.0.0.1",10543);
}

void OHM::update() {
    char out[1];
    out[0] = 'u';

    socket->waitForConnected();

    socket->write(out,1);
    socket->flush();


    char a[30];
    socket->read(a, 100);

    for (int i = 0; i < 30; i++) {
        qDebug() << a[i];
    }

}
