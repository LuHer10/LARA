#include "UDPHandler.h"

UDPHandler::UDPHandler(QObject *parent) : QObject(parent)
{
    receiverSocket.bind(QHostAddress::AnyIPv4, receivePort);
    connect(&receiverSocket, &QUdpSocket::readyRead, this, &UDPHandler::processPendingDatagrams);
}

void UDPHandler::sendJoystickData(float lx, float ly, float rx, float ry)
{
    QString message = QString("%1 %2 %3 %4").arg(lx).arg(ly).arg(rx).arg(ry);
    QByteArray datagram = message.toUtf8();
    senderSocket.writeDatagram(datagram, serverAddress, sendPort);
}

void UDPHandler::processPendingDatagrams()
{
    while (receiverSocket.hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(receiverSocket.pendingDatagramSize());
        receiverSocket.readDatagram(datagram.data(), datagram.size());

        QString receivedMessage = QString::fromUtf8(datagram);
        emit dataReceived(receivedMessage);
    }
}
