#ifndef YFM_PROCESS_H
#define YFM_PROCESS_H

#include <QtCore>

#include <boost/filesystem.hpp>
#include <boost/exception/all.hpp>

#include <yfm_log.h>
#include <yfm_configuration.h>
#include <yfm_notification.h>

namespace fs = boost::filesystem;


class yfmProcess : public QObject
{
    Q_OBJECT

public:
    explicit yfmProcess(QObject *parent = 0);

    int returnValue;

    yfmLog           log;
    yfmNotification  notification;
    yfmConfiguration configuration;

    void terminate();


signals:
    void finished();


public slots:
    void run();
    bool checkFolders();
    bool moveFiles();
    bool removeEmptyFolders();

};


#endif // YFM_PROCESS_H

