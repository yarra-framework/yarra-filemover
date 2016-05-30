#ifndef YFM_PROCESS_H
#define YFM_PROCESS_H

#include <QtCore>

#define BOOST_NO_CXX11_SCOPED_ENUMS
#include <boost/filesystem.hpp>
#undef BOOST_NO_CXX11_SCOPED_ENUMS

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
    bool checkFolders();
    bool moveCases();
    void prepare();
    bool copyFiles(fs::path sourceFolder, fs::path targetFolder);

    std::string getTargetPrefix(fs::path filename);

    std::string yearPrefix;
    time_t currentTime;

    int casesProcessed;

signals:
    void finished();


public slots:
    void run();

};


#endif // YFM_PROCESS_H

