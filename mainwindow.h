#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QException>
#include <QDateTime>
#include <screen.h>
#include <threadcapture.h>
#include <webcamera.h>
#include <guidecamera.h>
#include <imagingcamera.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void log(QString s);

public slots:
    void connectCamera();

private:
    Ui::MainWindow *ui;
    screen *m_screen;
    threadCapture *m_threadcapture;
    Camera **m_camera;

    bool isConnected = false;
};

#endif // MAINWINDOW_H
