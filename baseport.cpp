/****************************************************************
 * baseport.cpp
 * GrblHoming - chrisy fork of zapmaker fork on Github
 *
 * 24 Apr 2014
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#include <Qstring>
#include <QUrl>

#include "baseport.h"
#include "rs232.h"
#include "tcpport.h"

// Default constructor
BasePort::BasePort()
{
    return;
}

// Default destructor
BasePort::~BasePort()
{
    return;
}

BasePort *BasePortFactory::CreateFromURL(QString urlstr)
{
    QUrl url(urlstr);

    if (url.scheme() == "tcp") {
        return new TCPPort();
    }
    return new RS232();
}
