#include "yfm_notification.h"
#include "yfm_configuration.h"
#include "yfm_log.h"


yfmNotification::yfmNotification(yfmLog* logInstance)
{
    log=logInstance;
}


void yfmNotification::sendErrorNotification(yfmConfiguration* configuration)
{
    if (configuration->notificationEnabled)
    {
        log->info("<Sending error notification>");

        mailSubject=configuration->serverName+": Problem while moving cases";
        mailFrom="Yarra File Mover";
        mailTo=configuration->notificationMail;
        mailBody="<p>A problem occured while moving files from server <strong>"  + configuration->serverName
                 + "</strong>. The lines below describe the error cause. More details can be found in the log on the server.</p>";

        mailBody+="<pre>";
        for (int i=0; i<log->errorLog.size(); i++)
        {
            mailBody+=log->errorLog.at(i)+"<br>";
        }
        mailBody+="</pre>";
    }
}


void yfmNotification::sendMail()
{
    QString header=mailFrom;
    header.append("To: " + mailTo + "\n");
    header.append("Subject: "+ mailSubject + "\n");
    header.append("MIME-Version: 1.0\n");

    header.append("X-Priority: 1 (Highest)\n");
    header.append("X-MSMail-Priority: High\n");
    header.append("Importance: High\n");

    header.append("Content-Type: text/html; charset=ISO-8859-1\n");
    header.append("\n");

    QProcess *process_mail=new QProcess();
    process_mail->start("sendmail -t");
    process_mail->waitForStarted();

    process_mail->write(header.toLatin1());
    process_mail->write(QString("<html><body>\n").toLatin1());
    process_mail->write(mailBody.toLatin1());
    process_mail->write(QString("</body></html>\n").toLatin1());

    process_mail->write(QString(".\n").toLatin1());
    //process_mail->closeWriteChannel();
    process_mail->waitForFinished();

    delete process_mail;
    process_mail=0;
}

