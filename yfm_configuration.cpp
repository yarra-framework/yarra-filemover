#include <QtCore>

#include "yfm_configuration.h"
#include "yfm_log.h"



yfmConfiguration::yfmConfiguration(yfmLog* logInstance)
{
    log=logInstance;

    serverName="";
    notificationMail.clear();
    notificationEnabled=false;

    locationSource="";
    locationTarget="";

    prefixMode=prefix_none;
    waitHours=12;
}


bool yfmConfiguration::load()
{
    {
        QSettings configFile("filemover.ini", QSettings::IniFormat);
        serverName          =configFile.value("Settings/ServerName"         ,"").toString();
        locationSource      =configFile.value("Settings/LocationSource"     ,"").toString();
        locationTarget      =configFile.value("Settings/LocationTarget"     ,"").toString();
        QStringList tempList=configFile.value("Settings/NotificationMail"   ,"").toStringList();
        notificationMail    =tempList.join(",");
        notificationEnabled =configFile.value("Settings/NotificationEnabled",false).toBool();
        waitHours           =configFile.value("Settings/WaitHours"          ,12   ).toInt();

        QString prefixModeString=configFile.value("Settings/PrefixMode","").toString();
        prefixModeString=prefixModeString.toUpper();

        if (prefixModeString=="YEAR")
        {
            prefixMode=prefix_year;
        }
        else
        {
            prefixMode=prefix_none;
        }

    }

    if (serverName.isEmpty())
    {
        log->error("No server name given");
        return false;
    }

    if (locationSource.isEmpty())
    {
        log->error("Source location cannot be empty");
        return false;
    }

    if (locationTarget.isEmpty())
    {
        log->error("Source location cannot be empty");
        return false;
    }

    return true;
}

