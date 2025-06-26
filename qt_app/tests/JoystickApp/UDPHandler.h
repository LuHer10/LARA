#ifndef UDPHANDLER_H
#define UDPHANDLER_H

#include <QObject>
#include <QUdpSocket>

class UDPHandler : public QObject
{
    Q_OBJECT
public:
    explicit UDPHandler(QObject *parent = nullptr);

    Q_INVOKABLE void sendJoystickData(float lx, float ly, float rx, float ry);

signals:
    void dataReceived(QString data);

private slots:
    void processPendingDatagrams();

private:
    QUdpSocket senderSocket;
    QUdpSocket receiverSocket;

    QHostAddress serverAddress = QHostAddress("192.168.1.100"); // Replace with your server IP
    quint16 sendPort = 45454;
    quint16 receivePort = 45455;
};

#endif // UDPHANDLER_H
