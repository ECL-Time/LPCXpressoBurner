#ifndef LPCCONFIG_H
#define LPCCONFIG_H

#include <QObject>
#include <QMutex>
#include <QSettings>

class LpcConfig : public QObject
{
    Q_OBJECT

public:
    LpcConfig(QObject *parent = 0);
    ~LpcConfig();

public slots:
    void getDriver(QString &);
    void setDriver(QString &);
    void getImage(QString &);
    void setImage(QString &);
    void isMultiple(bool &);
    void setMultiple(bool &);
    void getLPCXpresso(QString &);
    void setLPCXpresso(QString &);

private:
    QMutex mutex;
    QSettings *config;
    QString lpcxpresso;
    QString driver;
    QString image;
    bool multiple;
};

#endif // LPCCONFIG_H
