#include <QApplication>
#include <QDir>
#include <QMessageBox>
#include <QDateTime>
#include <QIODevice>
#include <QDebug>
#include "lpccommand.h"

LpcCommand::LpcCommand(QObject *parent) :
    QObject(parent)
{
    isBurnNext = true;
    isBootReady = false;
    isLinkReady = false;
    isTargetReady = false;
    isBurnReady = false;

    connect(&cmd, SIGNAL(readyReadStandardOutput()), this, SLOT(readStdOutput()));
    connect(&cmd, SIGNAL(readyReadStandardError()), this, SLOT(readStdError()));
    connect(&cmd, SIGNAL(error(QProcess::ProcessError)), this, SLOT(readError()));
    connect(&cmd, SIGNAL(started()), this, SIGNAL(started()));
//    connect(&cmd, SIGNAL(finished(int,QProcess::ExitStatus)), this, SIGNAL(finished(int,QProcess::ExitStatus)));
    connect(&cmd, SIGNAL(finished(int,QProcess::ExitStatus)), this, SLOT(cmdFinished(int,QProcess::ExitStatus)));
}

LpcCommand::~LpcCommand()
{

}

void LpcCommand::init()
{
    QString lpcxpresso;
    getLPCXpresso(lpcxpresso);
    QDir lpcDir(lpcxpresso);
    if (lpcDir.isAbsolute()) {
        exe = QString(lpcDir.absolutePath() + "/bin/crt_emu_lpc11_13_nxp.exe");
        script = QString(lpcDir.absolutePath() + "/bin/Scripts/bootLPCXpresso.cmd");
    } else {
        exe = QString(qApp->applicationDirPath() + "/" + lpcxpresso + "/bin/crt_emu_lpc11_13_nxp.exe");
        script = QString(qApp->applicationDirPath() + "/" + lpcxpresso + "/bin/Scripts/bootLPCXpresso.cmd");
    }
}

/* Scripts/bootLPCXpresso.cmd */
void LpcCommand::runBoot()
{
    QString driver;
    getDriver(driver);
    if (!QFile(script).exists()) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "File not found: " + script);
        return;
    }

    QString out("<HTML><font color=\"blue\">[" + QDateTime::currentDateTime().toString(Qt::ISODate) + "]_BootScript</font></HTML>");
    emit cmdOutput(out);
    out = QString(script + " " + driver + "\n");
    cmdOutput(out);
    cmdId = CMD_ID_BOOT;
    cmd.start(script, QStringList(driver), QIODevice::ReadWrite);
}

/* crt_emu_lpc11_13_nxp -info-emu -wire=winusb */
void LpcCommand::runLink()
{
    QString driver;
    getDriver(driver);
    if (!QFile(exe).exists()) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "File not found: " + script);
        return;
    }

    QString out("<HTML><font color=\"blue\">[" + QDateTime::currentDateTime().toString(Qt::ISODate) + "]_Check_LPC-Link</font></HTML>");
    cmdOutput(out);
    out = QString(exe + " -info-emu" + " -wire=" + driver);
    cmdOutput(out);
    cmdId = CMD_ID_LINK;
    cmd.start(exe, QStringList("-info-emu") + QStringList("-wire=" + driver), QIODevice::ReadWrite);
}

/* crt_emu_lpc11_13_nxp -info-target -pLPC11U14/201 -wire=winusb -4 */
void LpcCommand::runTarget()
{
    QString driver;
    getDriver(driver);
    if (!QFile(exe).exists()) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "File not found: " + script);
        return;
    }

    QString out("<HTML><font color=\"blue\">[" + QDateTime::currentDateTime().toString(Qt::ISODate) + "]_Check_Target</font></HTML>");
    cmdOutput(out);
    out = QString(exe + " -info-target" + " -pLPC11U14/201" + " -wire=" + driver + " -4" + "\n");
    cmdOutput(out);
    cmdId = CMD_ID_TARGET;
    cmd.start(exe, QStringList("-info-target") + QStringList("-pLPC11U14/201") + QStringList("-wire=" + driver) + QStringList("-4"), QIODevice::ReadWrite);
}

/* crt_emu_lpc11_13_nxp -pLPC11U14/201 -wire=winusb -flash-load-exec nxp_lpcxpresso_11u14_periph_blinky.axf */
void LpcCommand::runBurn()
{
    QString driver;
    getDriver(driver);
    QString image;
    getImage(image);
    if (!QFile(exe).exists()) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "File not found: " + script);
        return;
    }
    if (image.isEmpty()) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "Image file could not be found!");
        return;
    }

    QString out("<HTML><font color=\"blue\">[" + QDateTime::currentDateTime().toString(Qt::ISODate) + "]_Burn_Image</font></HTML>");
    cmdOutput(out);
    out = QString(exe  + " -pLPC11U14/201" + " -wire=" + driver + " -flash-load-exec " + image + "\n");
    cmdOutput(out);
    cmdId = CMD_ID_BURN;
    cmd.start(exe, QStringList("-pLPC11U14/201") + QStringList("-wire=" + driver) + QStringList("-flash-load-exec") + QStringList(image), QIODevice::ReadWrite);
}

void LpcCommand::runMultiple()
{
    QString driver;
    getDriver(driver);
    QString image;
    getImage(image);
    if (!QFile(exe).exists()) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "File not found: " + script);
        return;
    }
    if (image.isEmpty()) {
        QMessageBox messageBox;
        messageBox.critical(0, "Error", "Image file could not be found!");
        return;
    }

    bool multi = true;
    setMultiple(multi);

    if (!isBootReady) {
        runBoot();
        return;
    } else if (!isLinkReady) {
        runLink();
        return;
    } else if (!isTargetReady) {
        runTarget();
        return;
    } else {
        runBurn();
        return;
    }
}

void LpcCommand::cmdFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    bool multi;
    isMultiple(multi);

    bool stat = ((exitCode == 0) && (exitStatus == QProcess::NormalExit)) ? true : false;

    if ((cmdId == CMD_ID_LINK) && (QString(cmdBuffer).contains("0 Emulators available"))) {
        stat = false;
    }
    isBootReady = (cmdId == CMD_ID_BOOT) ? stat : isBootReady;
    isLinkReady = (cmdId == CMD_ID_LINK) ? stat : isLinkReady;
    isTargetReady = (cmdId == CMD_ID_TARGET) ? stat : isTargetReady;
    isBurnReady = (cmdId == CMD_ID_BURN) ? stat : isBurnReady;

    QString out;

    if (stat == true) {
        out = QString("<HTML><font color=\"green\"><strong>Command OK</strong></font><br/></HTML>");
        cmdOutput(out);
    } else {
        out = QString("<HTML><font color=\"red\"><strong>Command Failed</strong></font><br/></HTML>");
        cmdOutput(out);
        if ((multi) && (cmdId != CMD_ID_TARGET) ) {
            multi = false;
            setMultiple(multi);
        }
    }

    if (multi) {
        switch (cmdId) {
        case CMD_ID_BOOT:
//            runLink();
            QTimer::singleShot(3000, this, SLOT(runLink()));
            break;
        case CMD_ID_LINK:
            runTarget();
            break;
        case CMD_ID_TARGET:
            if (isTargetReady) {
                if (isBurnNext) {
                    out = QString("<HTML><font color=\"red\"><strong>Target Connected</strong></font><br/></HTML>");
                    cmdOutput(out);
                    runBurn();
                } else {
                    runTarget();
                }
            } else {
                if (!isBurnNext) {
                    isBurnNext = true;
                    out = QString("<HTML><font color=\"red\"><strong>Target Disconnected</strong></font><br/></HTML>");
                    cmdOutput(out);
                }
                runTarget();
            }
            break;
        case CMD_ID_BURN:
            if (isBurnReady) {
                isBurnNext = false;
                out = QString("<HTML><font color=\"red\"><strong>======== Burn Image OK ========</strong></font><br/></HTML>");
                cmdOutput(out);
                runTarget();
            }
            break;
        }
    }

    if ((cmdId == CMD_ID_BOOT) && (!multi) ) {
        QTimer::singleShot(3000, this, SLOT(bootFinished()));
    } else {
        emit finished(exitCode, exitStatus);
    }
}

void LpcCommand::stopMultiple()
{
    bool multi = false;
    setMultiple(multi);
    isBurnNext = true;
//    isBootReady = false;
//    isLinkReady = false;
    isTargetReady = false;
    isBurnReady = false;
#if 0
    if (cmd.state() != QProcess::NotRunning) {
        cmd.kill();
        cmdOutput(QString("<HTML><font color=\"red\"><strong>Command Killed</strong></font><br/></HTML>"));
    }
#endif
}

void LpcCommand::readStdOutput()
{
    bool multi;
    isMultiple(multi);

    if (multi && (cmdId == CMD_ID_TARGET))
        return;

    cmdBuffer = cmd.readAllStandardOutput();
    QString out(cmdBuffer);
    cmdOutput(out);
//    cmdOutput(QString(cmd.readAllStandardOutput()));
}

void LpcCommand::readStdError()
{
    bool multi;
    isMultiple(multi);

    if (multi && (cmdId == CMD_ID_TARGET))
        return;

    QString out(cmd.readAllStandardError());
    cmdOutput(out);
}

void LpcCommand::readError()
{
    QString out("<HTML><font color=\"red\"><strong>An Error Occured! Error Code is: " + QString::number(cmd.error()) + "</strong></font><br/></HTML>");
    cmdOutput(out);
}

void LpcCommand::bootFinished()
{
    emit finished(0, QProcess::NormalExit);
}
