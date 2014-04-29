/****************************************************************
 * tcpport.h
 * GrblHoming - chrisy fork of zapmaker fork on Github
 *
 * 24 Apr 2014
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#ifndef TCPPORT_H
#define TCPPORT_H

#include <QObject>
#include <QString>
#include <QAbstractSocket>
#include <QTcpSocket>

#include "baseport.h"

class TCPPort : public BasePort
{
public:
    TCPPort();
    virtual ~TCPPort();

public:
    bool OpenComport(QString commPortStr, QString baudRate);
    int PollComport(char *buf, int size);
    int PollComportLine(char *buf, int size);
    int SendBuf(const char *buf, int size);
    void CloseComport();
    void Reset();
    void flush();
    bool isPortOpen();
    QString getDetectedLineFeed();
    int bytesAvailable();
    void setCharSendDelayMs(int charSendDelayMs);

private:
    QTcpSocket *socket;
    char detectedEOL;
    QString detectedLineFeed;
    int charSendDelayMs;
};

#endif // TCPPORT_H
