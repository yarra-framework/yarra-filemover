#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <chrono>
#include <ctime>
#include <string>

#include <QCoreApplication>
#include <QtCore>

#include "yfm_process.h"


yfmProcess::yfmProcess(QObject *parent) :
    QObject(parent)
  , notification(&log)
  , configuration(&log)
{
    returnValue=0;
    casesProcessed=0;
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

    if (!moveCases())
    {
        log.error("Problems during transfer of files");
    }

    log.info(QString::number(casesProcessed)+" cases moved");

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
    yearPrefix=QDateTime::currentDateTime().toString("yyyy").toStdString();
    time(&currentTime);
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
        // If target path does not exist, check if parent path exists. If not,
        // there seems to be a mounting problem. Otherwise, try creating the
        // target path.
        if (!fs::exists(target.parent_path()))
        {
            log.error("Target folder not acccessible");
            return false;
        }
        else
        {
            try
            {
                fs::create_directories(target);
            }
            catch (const fs::filesystem_error & e)
            {
                std::string errorMsg="Unable to create target folder " + target.string() + "  Cause: " + boost::diagnostic_information(e);
                log.error(QString::fromStdString(errorMsg));
                return false;
            }
        }
    }

    // TODO: Check upfront if we can write files into target folder

    return true;
}


bool yfmProcess::moveCases()
{
    casesProcessed=0;
    fs::path sourcePath(configuration.locationSource.toStdString());

    if (fs::is_directory(sourcePath))
    {
        try
        {
            log.info("");

            // Recursively iterate through folder
            fs::directory_iterator dir_entry(sourcePath);
            fs::directory_iterator iter_end;

            while (dir_entry != iter_end)
            {
                if (fs::is_directory(dir_entry->path()))
                {
                    time_t folderWriteTime=fs::last_write_time(dir_entry->path());
                    int hoursSinceWrite=int(difftime(currentTime,folderWriteTime)/3600.);

                    if (hoursSinceWrite>=configuration.waitHours)
                    {
                        fs::path currentFolder=dir_entry->path().stem();
                        fs::path folderPrefix=fs::path(getTargetPrefix(currentFolder));
                        fs::path targetPath=fs::path(configuration.locationTarget.toStdString()) / folderPrefix / currentFolder;

                        log.info("Processing case \"" + QString::fromStdString(currentFolder.string()) + "\"...");
                        log.info("Target folder: " + QString::fromStdString(targetPath.string()) );

                        try
                        {
                            if (fs::exists(targetPath))
                            {
                                log.error("Target folder already exists "+QString::fromStdString(targetPath.string()));
                                log.error("Skipping case.");
                                ++dir_entry;
                                continue;
                            }
                        }
                        catch (const fs::filesystem_error & e)
                        {
                            std::string errorMsg="Unable to check for folder " + targetPath.string() + "  Cause: " + boost::diagnostic_information(e);
                            log.error(QString::fromStdString(errorMsg));
                            return false;
                        }

                        try
                        {
                            fs::create_directories(targetPath);
                        }
                        catch (const fs::filesystem_error & e)
                        {
                            std::string errorMsg="Unable to create folder " + targetPath.string() + "  Cause: " + boost::diagnostic_information(e);
                            log.error(QString::fromStdString(errorMsg));
                            return false;
                        }

                        if (!fs::exists(targetPath))
                        {
                            std::string errorMsg="Folder was not created " + targetPath.string();
                            log.error(QString::fromStdString(errorMsg));
                            return false;
                        }

                        if (!copyFiles(dir_entry->path(), targetPath))
                        {
                            log.error("Copying files failed.");
                            return false;
                        }

                        try
                        {
                            log.info("Removing case on source location");
                            fs::remove_all(dir_entry->path());
                        }
                        catch (const fs::filesystem_error & e)
                        {
                            std::string errorMsg="Unable to remove folder " + dir_entry->path().string() + "  Cause: " + boost::diagnostic_information(e);
                            log.error(QString::fromStdString(errorMsg));
                            return false;
                        }

                        log.info("");
                        casesProcessed++;
                    }
                }

                ++dir_entry;
            }
        }
        catch (const fs::filesystem_error & e)
        {
            std::string errorMsg="Unable to access directory " + sourcePath.string() + "  Cause: " + boost::diagnostic_information(e);
            log.error(QString::fromStdString(errorMsg));
            return false;
        }
    }
    else
    {
        log.error("Can't access path " + configuration.locationSource);
        return false;
    }

    return true;
}


std::string yfmProcess::getTargetPrefix(fs::path filename)
{
    if (configuration.prefixMode==yfmConfiguration::prefix_year)
    {
        return yearPrefix;
    }
    else
    {
        return "";
    }
}


bool yfmProcess::copyFiles(fs::path sourceFolder, fs::path targetFolder)
{
    try
    {
        fs::directory_iterator entry(sourceFolder);
        fs::directory_iterator iter_end;

        while (entry != iter_end)
        {
            uintmax_t sourceSize=fs::file_size(entry->path());

            fs::path destName=targetFolder / entry->path().filename();

            try
            {
                fs::copy_file(entry->path(), destName, fs::copy_option::fail_if_exists);
            }
            catch (const fs::filesystem_error & e)
            {
                std::string errorMsg="Unable to copy file " + entry->path().string() + " to " + destName.string() + "  Cause: " + boost::diagnostic_information(e);
                log.error(QString::fromStdString(errorMsg));
                return false;
            }

            uintmax_t destSize=fs::file_size(destName);

            if (sourceSize!=destSize)
            {
                log.error("File sizes to not match: " + QString::number(sourceSize) + " vs " + QString::number(destSize));
                return false;
            }

            // Output filename and sizes for later debugging
            log.info("File copied " + QString::fromStdString(entry->path().filename().string()) + " (" + QString::number(sourceSize)+" -> "+QString::number(destSize)+")" );

            ++entry;
        }
    }
    catch (const fs::filesystem_error & e)
    {
        std::string errorMsg="Unable to iterate folder " + sourceFolder.string() + "  Cause: " + boost::diagnostic_information(e);
        log.error(QString::fromStdString(errorMsg));
        return false;
    }

    return true;
}

