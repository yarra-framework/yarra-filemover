#include <QCoreApplication>
#include <QtCore>

#include "yfm_process.h"


int main(int argc, char* argv[])
{
    QCoreApplication app(argc, argv);
    yfmProcess instance;

    // Connect the application signals
    QObject::connect(&instance, SIGNAL(finished()),
             &app, SLOT(quit()));

    QTimer::singleShot(0, &instance, SLOT(run()));
    app.exec();

    return instance.returnValue;
}
