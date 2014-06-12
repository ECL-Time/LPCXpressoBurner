#ifndef LPCCOMMAND_H
#define LPCCOMMAND_H

#include <QObject>
#include <QString>
#include <QProcess>
#include <QTimer>
#include <QByteArray>

class LpcCommand : public QObject
{
    Q_OBJECT
public:
    explicit LpcCommand(QObject *parent = 0);
    ~LpcCommand();
    void init();

signals:
    void started();
    void finished(int, QProcess::ExitStatus);
    void getLPCXpresso(QString &);
    void cmdOutput(QString &);
    void getDriver(QString &);
    void getImage(QString &);
    void isMultiple(bool &);
    void setMultiple(bool &);

public slots:
    void runBoot();
    void runLink();
    void runTarget();
    void runBurn();
    void runMultiple();
    void stopMultiple();

private:
    QString exe;
    QString script;
    QProcess cmd;
    QTimer timer;
    bool isBurnNext;
    bool isBootReady;       // cannot be detected
    bool isLinkReady;
    bool isTargetReady;
    bool isBurnReady;
    enum CMD_ID {
        CMD_ID_BOOT,
        CMD_ID_LINK,
        CMD_ID_TARGET,
        CMD_ID_BURN,
    };
    CMD_ID cmdId;
    QByteArray cmdBuffer;

private slots:
    void readStdOutput();
    void readStdError();
    void readError();
    void cmdFinished(int, QProcess::ExitStatus);
    void bootFinished();
};

#endif // LPCCOMMAND_H
