#include <QApplication>
#include <QObject>
#include <QDebug>
#include "mainwindow.h"
#include "lpcconfig.h"
#include "lpccommand.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    LpcConfig conf;
    MainWindow w;
    LpcCommand cmd;

    QObject::connect(&cmd, SIGNAL(getLPCXpresso(QString&)), &conf, SLOT(getLPCXpresso(QString&)));
    QObject::connect(&cmd, SIGNAL(getDriver(QString&)), &conf, SLOT(getDriver(QString&)));
    QObject::connect(&cmd, SIGNAL(getImage(QString&)), &conf, SLOT(getImage(QString&)));
    QObject::connect(&cmd, SIGNAL(isMultiple(bool&)), &conf, SLOT(isMultiple(bool&)));
    QObject::connect(&cmd, SIGNAL(setMultiple(bool&)), &conf, SLOT(setMultiple(bool&)));

    QObject::connect(&w, SIGNAL(getDriver(QString&)), &conf, SLOT(getDriver(QString&)));
    QObject::connect(&w, SIGNAL(setDriver(QString&)), &conf, SLOT(setDriver(QString&)));
    QObject::connect(&w, SIGNAL(getImage(QString&)), &conf, SLOT(getImage(QString&)));
    QObject::connect(&w, SIGNAL(setImage(QString&)), &conf, SLOT(setImage(QString&)));
    QObject::connect(&w, SIGNAL(isMultiple(bool&)), &conf, SLOT(isMultiple(bool&)));
    QObject::connect(&w, SIGNAL(setMultiple(bool&)), &conf, SLOT(setMultiple(bool&)));

    QObject::connect(&cmd, SIGNAL(cmdOutput(QString&)), &w, SLOT(cmdOutput(QString&)));
    QObject::connect(&cmd, SIGNAL(started()), &w, SLOT(cmdStarted()));
    QObject::connect(&cmd, SIGNAL(finished(int,QProcess::ExitStatus)), &w, SLOT(cmdFinished(int,QProcess::ExitStatus)));

    QObject::connect(&w, SIGNAL(runBoot()), &cmd, SLOT(runBoot()));
    QObject::connect(&w, SIGNAL(runLink()), &cmd, SLOT(runLink()));
    QObject::connect(&w, SIGNAL(runTarget()), &cmd, SLOT(runTarget()));
    QObject::connect(&w, SIGNAL(runBurn()), &cmd, SLOT(runBurn()));
    QObject::connect(&w, SIGNAL(runMultiple()), &cmd, SLOT(runMultiple()));
    QObject::connect(&w, SIGNAL(stopMultiple()), &cmd, SLOT(stopMultiple()));

    cmd.init();
    w.init();
    w.show();

    return a.exec();
}
