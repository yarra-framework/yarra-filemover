#ifndef YFM_CONFIGURATION_H
#define YFM_CONFIGURATION_H

#include <QtCore>


class yfmLog;

class yfmConfiguration
{
public:

    enum prefixmodes
    {
        prefix_none=0,
        prefix_year=1
    };

    yfmConfiguration(yfmLog* logInstance);

    QString     serverName;
    int         waitHours;
    prefixmodes prefixMode;

    QString     notificationMail;
    bool        notificationEnabled;

    QString     locationSource;
    QString     locationTarget;


    bool load();

protected:
    yfmLog* log;

};

#endif // YFM_CONFIGURATION_H
