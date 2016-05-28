#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <chrono>

#include <QCoreApplication>
#include <QtCore>

#include "yfm_process.h"


yfmProcess::yfmProcess(QObject *parent) :
    QObject(parent)
  , notification(&log)
{
    returnValue=0;
}


void yfmProcess::run()
{
    log.initialize();

    if (!configuration.load())
    {
        log.error("Loading configuration failed");
        terminate();
        return;
    }

    terminate();
}


void yfmProcess::terminate()
{
    log.finalize();

    if (!log.errorLog.empty())
    {
        notification.sendErrorNotification(&configuration);
    }

    emit finished();
}

