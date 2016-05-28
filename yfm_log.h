#ifndef YFM_LOG_H
#define YFM_LOG_H

#include <QStringList>
#include <QString>


class yfmLog
{
public:
    yfmLog();

    QStringList infoLog;
    QStringList errorLog;

    void initialize();
    void finalize();

    void error(QString description, bool timeStamp=true);
    void info (QString description, bool timeStamp=true);

};

#endif // YFM_LOG_H
