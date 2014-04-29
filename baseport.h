/****************************************************************
 * baseport.h
 * GrblHoming - chrisy fork of zapmaker fork on Github
 *
 * 24 Apr 2014
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#ifndef BASEPORT_H
#define BASEPORT_H

#include <QtGlobal>
#include <QString>


class BasePort
{
public:
    BasePort();
    virtual ~BasePort();

public:
    virtual bool OpenComport(QString commPortStr, QString baudRate) = 0;
    virtual int PollComport(char *buf, int size) = 0;
    virtual int PollComportLine(char *buf, int size) = 0;
    virtual int SendBuf(const char *buf, int size) = 0;
    virtual void CloseComport() = 0;
    virtual void Reset() = 0;
    virtual void flush() = 0;
    virtual bool isPortOpen() = 0;
    virtual QString getDetectedLineFeed() = 0;
    virtual int bytesAvailable() = 0;
    virtual void setCharSendDelayMs(int charSendDelayMs) = 0;
};

class BasePortFactory
{
public:
    static BasePort *CreateFromURL(QString url);
};

#endif // BASEPORT_H
