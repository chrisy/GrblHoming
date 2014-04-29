/****************************************************************
 * sleep.h
 * GrblHoming - chrisy fork of zapmaker fork on Github
 *
 * 24 Apr 2014
 * GPL License (see LICENSE file)
 * Software is provided AS-IS
 ****************************************************************/

#ifndef SLEEP_H
#define SLEEP_H

#include <QtGlobal>

#if defined(Q_OS_LINUX) || defined(Q_OS_MACX) || defined(Q_OS_ANDROID)
#include <unistd.h>
#else
#if QT_VERSION < QT_VERSION_CHECK(5, 0, 0)
#include <Windows.h>
#else
// TODO - R - if I leave out Windows.h then Sleep is not found???
#include <Windows.h>
//#include <QtGui/QWindow>
#endif
#endif

#if defined(Q_OS_LINUX) || defined(Q_OS_MACX) || defined(Q_OS_ANDROID)
#define SLEEP(x) usleep(1000 * x);
#else
#define SLEEP(x) Sleep(x);
#endif

#endif // SLEEP_H
