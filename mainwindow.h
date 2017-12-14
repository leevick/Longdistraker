#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QException>
#include <screen.h>
#include <threadcapture.h>
#include <webcamera.h>
#include <guidecamera.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    screen *m_screen;
    threadCapture *m_threadcapture;
    Camera * m_camera;
};

#endif // MAINWINDOW_H
