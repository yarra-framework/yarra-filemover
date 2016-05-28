#ifndef YFM_CONFIGURATION_H
#define YFM_CONFIGURATION_H

#include <QtCore>


class yfmConfiguration
{
public:
    yfmConfiguration();

    QString     serverName;
    QStringList notificationMail;
    bool        notificationEnabled;

    QString     locationSource;
    QString     locationTarget;

    bool        useYearSubfolder;

    bool load();

};

#endif // YFM_CONFIGURATION_H
