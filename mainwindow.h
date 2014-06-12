#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QFile>
#include <QProcess>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void init();

public slots:
    void cmdStarted();
    void cmdFinished(int,QProcess::ExitStatus);
    void cmdOutput(QString &);

signals:
    void getDriver(QString &);
    void setDriver(QString &);
    void getImage(QString &);
    void setImage(QString &);
    void isMultiple(bool &);
    void setMultiple(bool &);

    void runBoot();
    void runLink();
    void runTarget();
    void runBurn();
    void runMultiple();
    void stopMultiple();

private:
    Ui::MainWindow *ui;
    void showDriver();
    void showImage();
    void showButtons();

private slots:
    void configDriver();
    void configImage();
    void configMultiple();
};

#endif // MAINWINDOW_H
