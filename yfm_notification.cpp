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

        // TODO: Compose email

    }
}


void yfmNotification::sendMail()
{
    QString header=mailFrom;
    header.append("To: " + mailTo + "\n");
    header.append("Subject: "+mailSubject + "\n");
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

