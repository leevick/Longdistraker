#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Exception>
#include <QDateTime>
#include <QFileDialog>
#include <screen.h>
#include <selectserialportdialog.h>

#include <webcamera.h>
#include <guidecamera.h>
#include <imagingcamera.h>
#include <simcamera.h>
#include <recorder.h>

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
signals:
    void raiseCaptureStartRequest();
    void raiseRecordStartRequest(imagingCamera *Camera);
    void raiseCaptureStopRequest();
    void raiseRecordStopRequest();

public slots:
    void selectSerialPort(const QList<QString> &boardNames, int *selectedPort);
    void print2log(QString s);
    void selectVideoPath(string & path);
    void startOrStopRecording();

private slots:
    void handleCaptureRequest();

private:
    Ui::MainWindow *ui;
    screen *m_screen;

    QThread threadCapture;
    QThread threadRecord;
    QThread threadVideoIO;

    Camera **m_camera;
    Recorder *m_recorder;

    bool isConnected = false;
    bool isRecording = false;
};

#endif // MAINWINDOW_H
