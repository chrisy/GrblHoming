/****************************************************************
 * tcpport.cpp
 * GrblHoming - chrisy fork of zapmaker fork on Github
 *
 * 24 Apr 2014
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include <QString>
#include <QUrl>

#include "definitions.h"
#include "tcpport.h"
#include "sleep.h"

// TODO:
// - Handle socket closing on us - attempt to reopen
// - Handle socket can't be reopened (higher level functionality probably)
// - Telnet options processing - can we set baud rate in-band?

TCPPort::TCPPort() :
    socket(NULL), detectedEOL(0), charSendDelayMs(DEFAULT_CHAR_SEND_DELAY_MS)
{
    return;
}

TCPPort::~TCPPort()
{
    this->CloseComport();
}

bool TCPPort::OpenComport(QString commPortStr, QString baudRate)
{
    Q_UNUSED(baudRate);

    QUrl url(commPortStr);

    if (url.scheme() != "tcp")
        return false;

    if (socket != NULL)
        this->CloseComport();

    socket = new QTcpSocket();
    socket->connectToHost(url.host(), url.port(DEFAULT_TCP_PORT));

    if (!socket->waitForConnected(5000))
    {
        this->CloseComport();
        return false;
    }

    // Disable Nagle
    socket->setSocketOption(QAbstractSocket::LowDelayOption, 1);

    return socket->isOpen();
}

int TCPPort::PollComport(char *buf, int size)
{
    if (socket == NULL || !socket->isOpen())
        return 0;

    qint64 n = socket->bytesAvailable();
    if (n <= 0)
        return n;

    n = socket->read(buf, size);
    return n;
}

// This is different than QIoDevice.readline() - this method only returns data if it has a full line in the
// input buffer. It never removes items unless it can remove a full line.
int TCPPort::PollComportLine(char *buf, int size)
{
    if (socket == NULL || !socket->isOpen())
        return 0;

    qint64 n = socket->bytesAvailable();
#ifdef DIAG
    diag("pollcomportline %d bytes available", n);
#endif
    if (n <= 0)
        return n;

    if (!socket->canReadLine())
        return 0;

    n = socket->readLine(buf, size);
#ifdef DIAG
    diag ("pollcomportline %d bytes read", n);
#endif
    if (n <= 0)
        return n;

    if (detectedEOL == 0)
    {
        // algorithm assumes we received both eol chars if there are two in this peek
        qint64 pos = 0;
        char firstEOL = 0;
        char secondEOL = 0;
        for (qint64 i = 0; i < n; i++)
        {
            char b = buf[i];
            if (b == '\n' || b == '\r')
            {
                if (firstEOL == 0)
                {
                    firstEOL = b;
                    pos = i;
                }
                else if ((pos + 1) == i)
                {
                    secondEOL = b;
                    break;
                }
                else
                    break;
            }
        }

        if (firstEOL != 0)
        {
            if (secondEOL != 0)
            {
                detectedEOL = secondEOL;
                detectedLineFeed = firstEOL;
                detectedLineFeed += secondEOL;
            }
            else
            {
                detectedEOL = firstEOL;
                detectedLineFeed = firstEOL;
            }
        }
    }

    return n;
}

int TCPPort::SendBuf(const char *buf, int size)
{
    if (socket == NULL || !socket->isOpen())
        return 0;

    if (size <= 0)
    {
        err( qPrintable(QObject::tr("Unexpected: Told to send %d bytes\n")), size) ;
        return 1;
    }

    char b[300] = {0};
    memcpy(b, buf, size);

#ifdef DIAG
    printf("Sending to port %s [%s]:", socket->portName().toLocal8Bit().constData(), b);
    for (int x= 0; x < size; x++)
    {
        printf("%02X ", buf[x]);
    }
    printf("\n");
    fflush(stdout);
#endif

    socket->waitForBytesWritten(-1);

    // On very fast PCs running Windows we have to slow down the sending of bytes to grbl
    // because grbl loses bytes due to its interrupt service routine (ISR) taking too many clock
    // cycles away from serial handling.
    int result = 0;
    for (int i = 0; i < size; i++)
    {
        result = socket->write(&buf[i], 1);
        if (result == 0)
        {
            err("Unable to write bytes to port probably due to outgoing queue full. Write data lost!");
            break;
        }
        else if (result == -1)
        {
            err("Error writing to port. Write data lost!");
            result = 0;
            break;
        }
        socket->waitForBytesWritten(-1);

        if (charSendDelayMs > 0)
        {
            SLEEP(charSendDelayMs);
        }
    }

    return result;
}


void TCPPort::CloseComport()
{
    if (socket != NULL)
    {
        if (socket->isOpen())
            socket->close();
        delete socket;
        socket = NULL;
    }
}

void TCPPort::Reset() //still to test
{
    if (socket != NULL)
        socket->reset();
}

void TCPPort::flush()
{
    qint64 n = 1;
    char buf[255];

    while (n > 0)
        n = PollComport(buf, 255);
}

bool TCPPort::isPortOpen()
{
    if (socket == NULL)
        return false;

    return socket->isOpen();
}

QString TCPPort::getDetectedLineFeed()
{
    return detectedLineFeed;
}

int TCPPort::bytesAvailable()
{
    qint64 n = socket->bytesAvailable();
    return n;
}

void TCPPort::setCharSendDelayMs(int csd)
{
    charSendDelayMs = csd;
}
