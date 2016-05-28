#include "yfm_configuration.h"

#include <QtCore>


yfmConfiguration::yfmConfiguration()
{
    serverName="";
    notificationMail.clear();
    notificationEnabled=true;

    locationSource="";
    locationTarget="";

    useYearSubfolder=true;
}


bool yfmConfiguration::load()
{
    {
        QSettings configFile("filemover.ini", QSettings::IniFormat);
    }

    if (serverName.isEmpty())
    {
        return false;
    }

    return true;
}

