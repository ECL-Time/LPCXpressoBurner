#include <QFileDialog>
#include <QDateTime>
#include <QMessageBox>
#include <QDebug>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->radioButton_hid, SIGNAL(clicked()), this, SLOT(configDriver()));
    connect(ui->radioButton_winusb, SIGNAL(clicked()), this, SLOT(configDriver()));
    connect(ui->pushButton_file, SIGNAL(clicked()), this, SLOT(configImage()));
    connect(ui->checkBox_multiple, SIGNAL(clicked()), this, SLOT(configMultiple()));

    connect(ui->pushButton_boot, SIGNAL(clicked()), this, SIGNAL(runBoot()));
    connect(ui->pushButton_link, SIGNAL(clicked()), this, SIGNAL(runLink()));
    connect(ui->pushButton_target, SIGNAL(clicked()), this, SIGNAL(runTarget()));
    connect(ui->pushButton_burn, SIGNAL(clicked()), this, SIGNAL(runBurn()));
    connect(ui->pushButton_run, SIGNAL(clicked()), this, SIGNAL(runMultiple()));
    connect(ui->pushButton_stop, SIGNAL(clicked()), this, SIGNAL(stopMultiple()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    showDriver();
    showImage();
    showButtons();
}

void MainWindow::showDriver()
{
    QString drv;

    getDriver(drv);
    if (drv == "winusb") {
        ui->radioButton_winusb->setChecked(true);
    } else {
        ui->radioButton_hid->setChecked(true);
    }
}

void MainWindow::showImage()
{
    QString img;
    getImage(img);

    if (QFile(img).exists()) {
        ui->lineEdit_file->setText(img);
    }
}

void MainWindow::showButtons()
{
    bool multi;
    isMultiple(multi);
    if (multi) {
        ui->checkBox_multiple->setChecked(true);
        ui->pushButton_boot->setEnabled(false);
        ui->pushButton_link->setEnabled(false);
        ui->pushButton_target->setEnabled(false);
        ui->pushButton_burn->setEnabled(false);
        ui->pushButton_run->setEnabled(true);
        ui->pushButton_stop->setEnabled(false);
    } else {
        ui->checkBox_multiple->setChecked(false);
        ui->pushButton_boot->setEnabled(true);
        ui->pushButton_link->setEnabled(true);
        ui->pushButton_target->setEnabled(true);
        ui->pushButton_burn->setEnabled(true);
        ui->pushButton_run->setEnabled(false);
        ui->pushButton_stop->setEnabled(false);
    }
}

void MainWindow::configDriver()
{
    QString drv;
    if (ui->radioButton_winusb->isChecked()) {
        drv = "winusb";
    } else {
        drv = "hid";
    }
    setDriver(drv);
}

void MainWindow::configImage()
{
    QFileDialog fd(this, tr("Select image file"), "", "");

    fd.setFileMode(QFileDialog::ExistingFile);
    fd.setViewMode(QFileDialog::Detail);

    if(fd.exec())
    {
        QStringList fileNames = fd.selectedFiles();
        QString fileName = fileNames.at(0);
        setImage(fileName);
        ui->lineEdit_file->setText(fileName);
    }
}

void MainWindow::configMultiple()
{
    bool multi;
    if (ui->checkBox_multiple->isChecked()) {
        multi = true;
    } else {
        multi = false;
    }
    setMultiple(multi);
    showButtons();
}

void MainWindow::cmdStarted()
{
        bool multi;
    isMultiple(multi);
    if (ui->checkBox_multiple->isEnabled())
        ui->checkBox_multiple->setEnabled(false);
    if (ui->pushButton_boot->isEnabled())
        ui->pushButton_boot->setEnabled(false);
    if (ui->pushButton_link->isEnabled())
        ui->pushButton_link->setEnabled(false);
    if (ui->pushButton_target->isEnabled())
        ui->pushButton_target->setEnabled(false);
    if (ui->pushButton_burn->isEnabled())
        ui->pushButton_burn->setEnabled(false);
    if (ui->pushButton_run->isEnabled())
        ui->pushButton_run->setEnabled(false);
    if(multi) {
        ui->pushButton_stop->setEnabled(true);
    }
}

void MainWindow::cmdFinished(int,QProcess::ExitStatus)
{
    bool multi;
    isMultiple(multi);
    if (!multi) {
        ui->checkBox_multiple->setEnabled(true);
        showButtons();
    } else {
        ui->pushButton_stop->setEnabled(true);
    }
}

void MainWindow::cmdOutput(QString &output)
{
    ui->textEdit_command->append(output);
}
