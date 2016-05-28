#ifndef YFM_PROCESS_H
#define YFM_PROCESS_H

#include <QtCore>

#include <yfm_log.h>
#include <yfm_configuration.h>
#include <yfm_notification.h>


class yfmProcess : public QObject
{
    Q_OBJECT

public:
    explicit yfmProcess(QObject *parent = 0);

    int returnValue;

    yfmConfiguration configuration;
    yfmLog           log;
    yfmNotification  notification;

    void terminate();


signals:
    void finished();


public slots:
    void run();


};


#endif // YFM_PROCESS_H

