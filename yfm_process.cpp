#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <chrono>

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
    /*
    fs::path folderPath(folder);

    if (fs::is_directory(folderPath))
    {
        try
        {
            // Recursively iterate through folder
            fs::recursive_directory_iterator dir_entry(folderPath);
            fs::recursive_directory_iterator iter_end;

            while (dir_entry != iter_end)
            {
                if (dir_entry->path().extension()==".dat")
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
                        LOG("ERROR: Unable to read file " << dir_entry->path() << " (" << e.what() << ")");
                    }
                }

                ++dir_entry;
            }

        }
        catch (const fs::filesystem_error & e)
        {
            LOG("ERROR: Unable to access directory " << folderPath << " (" << boost::diagnostic_information(e) << ")");
        }
    }
    else
    {

    }

    */

    return true;
}


bool yfmProcess::removeEmptyFolders()
{
    return true;
}


