#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include <QtCore>

#include "yfm_log.h"


#define YFM_VERSION "YFM 0.16"

yfmLog::yfmLog()
{
}


void yfmLog::initialize()
{
    info("\n################################################################## "+QString(YFM_VERSION)+" ##", false);
    info("Process started");
}


void yfmLog::finalize()
{
    info("Process terminated");
    info("##############################################################################", false);
}


void yfmLog::error(QString description, bool timeStamp)
{
    QString entry=description;

    if (timeStamp)
    {
        entry=QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss") + " -- ERROR: " + entry;
    }

    // Error entries should be placed in both logs.
    errorLog.append(entry);
    infoLog.append(entry);

    std::cout << entry.toStdString() << std::endl;
}


void yfmLog::info(QString description, bool timeStamp)
{
    QString entry=description;

    if (timeStamp)
    {
        entry=QDateTime::currentDateTime().toString("dd.MM.yy hh:mm:ss") + " -- " + entry;
    }

    infoLog.append(entry);

    std::cout << entry.toStdString() << std::endl;
}


