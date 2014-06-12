#include <QApplication>
#include <QDir>
#include <QString>
#include <QDebug>
#include "lpcconfig.h"

#define LPCXpresso_DIR  "C:\\nxp\\LPCXpresso_7.1.1_125\\lpcxpresso"

LpcConfig::LpcConfig(QObject *parent) :
    QObject(parent)
{
    config = new QSettings(QString(qApp->applicationDirPath() + "/config.ini"), QSettings::IniFormat);
    lpcxpresso = config->value("LPCXpresso", LPCXpresso_DIR).toString();
    driver = config->value("driver", "hid").toString();
    image = config->value("image").toString();
    multiple = config->value("multiple", false).toBool();

    setLPCXpresso(lpcxpresso);
    setDriver(driver);
    setImage(image);
    setMultiple(multiple);
}

LpcConfig::~LpcConfig()
{
    config->sync();
    delete config;
}

void LpcConfig::getDriver(QString &drv)
{
    drv = driver;
}

void LpcConfig::setDriver(QString &drv)
{
    if ((drv.compare("winusb") == 0) || (drv.compare("hid") == 0)) {
        driver = drv;
    } else {
        driver = QString("hid");
    }
    config->setValue("driver", driver);
    config->sync();
}

void LpcConfig::getImage(QString &img)
{
    img = image;
}

void LpcConfig::setImage(QString &img)
{
    if (QFile(img).exists()) {
        image = img;
    } else {
        image = QString();
    }
    config->setValue("image", image);
    config->sync();
}

void LpcConfig::isMultiple(bool &multi)
{
    mutex.lock();
    multi = multiple;
    mutex.unlock();
}

void LpcConfig::setMultiple(bool &multi)
{
    mutex.lock();
    multiple = multi;
    config->setValue("multiple", multiple);
    config->sync();
    mutex.unlock();
}

void LpcConfig::getLPCXpresso(QString &lpc)
{
    lpc = lpcxpresso;
}

void LpcConfig::setLPCXpresso(QString &lpc)
{
    if (QDir(lpc).exists()) {
        lpcxpresso = lpc;
    } else {
        lpcxpresso = QString(LPCXpresso_DIR);
    }
    config->setValue("LPCXpresso", lpcxpresso);
    config->sync();
}
