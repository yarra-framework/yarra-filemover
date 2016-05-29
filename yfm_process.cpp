#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>

#include <QCoreApplication>
#include <QtCore>

#include "yfm_process.h"


yfmProcess::yfmProcess(QObject *parent) :
    QObject(parent)
  , notification(&log)
  , configuration(&log)
{
    returnValue=0;
}


void yfmProcess::run()
{
    log.initialize();

    if (!configuration.load())
    {
        log.error("Loading configuration failed");
        terminate();
        return;
    }

    prepare();

    if (!checkFolders())
    {
        log.error("Unable to run file transfer");
        terminate();
        return;
    }

    if (!moveFiles())
    {
        log.error("Problems during transfer of files");
    }

    if (!removeEmptyFolders())
    {
        log.error("Problems during removal of empty folders");
    }

    terminate();
}


void yfmProcess::terminate()
{
    log.finalize();

    if (!log.errorLog.empty())
    {
        notification.sendErrorNotification(&configuration);
    }
    log.info("",false);

    emit finished();
}


void yfmProcess::prepare()
{
    yearPrefix=QDateTime::currentDateTime().toString("yyyy");
}


bool yfmProcess::checkFolders()
{
    fs::path source(configuration.locationSource.toStdString());
    fs::path target(configuration.locationTarget.toStdString());

    if (!fs::exists(source))
    {
        log.error("Source folder not acccessible");
        return false;
    }

    if (!fs::exists(target))
    {
        log.error("Target folder not acccessible");
        return false;
    }

    // TODO: Check if we can write files into target folder

    return true;
}


bool yfmProcess::moveFiles()
{
    fs::path sourcePath(configuration.locationSource.toStdString());

    if (fs::is_directory(sourcePath))
    {
        try
        {
            // Recursively iterate through folder
            fs::directory_iterator dir_entry(sourcePath);
            fs::directory_iterator iter_end;

            while (dir_entry != iter_end)
            {
                if (fs::is_directory(dir_entry->path()))
                {
                    log.info("Processing " + QString::fromStdString(dir_entry->path().stem().string()) );
                }

                /*
                //last_write_time
                if (dir_entry->path().extension()==".dat")
                //if (dir_entry->path().extension()==".dat")
                {
                    try
                    {
                        totalFilesFound++;

                        std::string aliasedPath=getAliasedPath(dir_entry->path().parent_path().string(), folder, alias);
                        std::string filename=dir_entry->path().filename().string();

                        DEBUG("Processing " << dir_entry->path().string());
                    }
                    catch (const std::runtime_error &e)
                    {
                        std::string errorMsg="Unable to access file " + dir_entry->path().string() + " (" + e.what() + ")";
                        log.error(QString::fromStdString(errorMsg));
                    }
                }
                */

                ++dir_entry;
            }

        }
        catch (const fs::filesystem_error & e)
        {
            std::string errorMsg="Unable to access directory " + sourcePath.string() + " (" + boost::diagnostic_information(e) + ")";
            log.error(QString::fromStdString(errorMsg));
        }
    }
    else
    {
        log.error("Can't access path " + configuration.locationSource);
        return false;
    }

    return true;
}


bool yfmProcess::removeEmptyFolders()
{
    return true;
}


