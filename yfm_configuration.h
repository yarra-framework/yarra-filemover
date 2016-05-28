#ifndef YFM_CONFIGURATION_H
#define YFM_CONFIGURATION_H

#include <QtCore>


class yfmLog;

class yfmConfiguration
{
public:
    yfmConfiguration(yfmLog* logInstance);

    QString     serverName;
    int         waitHours;
    bool        useYearSubfolder;

    QString     notificationMail;
    bool        notificationEnabled;

    QString     locationSource;
    QString     locationTarget;


    bool load();

protected:
    yfmLog* log;

};

#endif // YFM_CONFIGURATION_H
