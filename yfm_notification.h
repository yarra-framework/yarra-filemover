#ifndef YFM_NOTIFICATION_H
#define YFM_NOTIFICATION_H

#include <QtCore>


class yfmConfiguration;
class yfmLog;

class yfmNotification
{
public:
    yfmNotification(yfmLog* logInstance);

    void sendErrorNotification(yfmConfiguration* configuration);

    QString mailTo;
    QString mailSubject;
    QString mailBody;

    void sendMail();

protected:
    yfmLog* log;

};


#endif // YFM_NOTIFICATION_H
