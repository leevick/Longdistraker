#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
                                          ui(new Ui::MainWindow)
{
    try
    {
        //Initilize UI
        ui->setupUi(this);
        ui->comboBoxCameraSelection->addItem("Web Camera");
        ui->comboBoxCameraSelection->addItem("Guidance Camera");
        ui->comboBoxCameraSelection->addItem("Imaging Camera");
        ui->comboBoxCameraSelection->addItem("Simulation Source");

        //Allocate Memory
        m_camera = new Camera *[4];
        m_camera[0] = new WebCam();
        //m_camera[1] = new guideCamera();
        m_camera[2] = new imagingCamera();
        m_recorder = new Recorder();

        m_recorder->moveToThread(&threadVideoIO);
        connect(&threadVideoIO,&QThread::finished,m_recorder,&QObject::deleteLater);
        threadVideoIO.start(QThread::HighestPriority);

        //Connect signals and slots
        connect(ui->pushButtonOpenOrClose, SIGNAL(released(void)), this, SLOT(handleCaptureRequest(void)));
        // connect(m_camera[2], SIGNAL(selectSerialPort(const QList<QString> &, int *)), this, SLOT(selectSerialPort(const QList<QString> &, int *)), Qt::BlockingQueuedConnection);
        connect(m_recorder,SIGNAL(selectVideoPath(string &)),this,SLOT(selectVideoPath(string &)),Qt::BlockingQueuedConnection);
        connect(ui->pushButtonRecord,SIGNAL(released(void)),this,SLOT(startOrStopRecording(void)));
        // connect(this,SIGNAL(raiseStartRequest(threadCapture*)),m_recorder,SLOT(handleStartRequest(threadCapture*)),Qt::BlockingQueuedConnection);
        connect(this,SIGNAL(raiseRecordStartRequest(imagingCamera*)),m_recorder,SLOT(handleStartRequest(imagingCamera*)));
        connect(this,SIGNAL(raiseRecordStopRequest(void)),m_recorder,SLOT(handleStopRequest(void)),Qt::DirectConnection);

        log("Main window created");
    }
    catch (Exception e)
    {
        log("Main window creation failed!");
    }
}

MainWindow::~MainWindow()
{
}

void MainWindow::log(QString s)
{
    QDateTime time;
    time = time.currentDateTime();
    ui->log->insertPlainText(time.toString("HH:MM:ss") + "\t" + s + "\n");
    ui->log->moveCursor(QTextCursor::End);
    return;
}

void MainWindow::print2log(QString s)
{
    QDateTime time;
    time = time.currentDateTime();
    ui->log->insertPlainText(time.toString("HH:MM:ss") + "\t" + s + "\n");
    ui->log->moveCursor(QTextCursor::End);
    return;
}

void MainWindow::handleCaptureRequest()
{
    QString cameraName = ui->comboBoxCameraSelection->itemText(ui->comboBoxCameraSelection->currentIndex());
    if (!isConnected)
    {
        try
        {
            log("Connecting to " + cameraName + "...");
            ui->pushButtonOpenOrClose->setDisabled(true);
            ui->comboBoxCameraSelection->setDisabled(true);

            m_screen = new screen(ui->labelImage,m_camera[ui->comboBoxCameraSelection->currentIndex()]);
            m_camera[ui->comboBoxCameraSelection->currentIndex()]->moveToThread(&threadCapture);
            threadCapture.start(QThread::NormalPriority);

            //m_camera[2]->moveToThread(&threadRecord);
            //threadRecord.start(QThread::TimeCriticalPriority);
            


            connect(this,SIGNAL(raiseCaptureStartRequest(void)),m_camera[ui->comboBoxCameraSelection->currentIndex()],SLOT(handleStartRequest(void)),Qt::QueuedConnection);
            connect(this,SIGNAL(raiseCaptureStopRequest(void)),m_camera[ui->comboBoxCameraSelection->currentIndex()],SLOT(handleStopRequest(void)),Qt::QueuedConnection);
            if(ui->comboBoxCameraSelection->currentIndex()!=2)
            {
                connect(this,SIGNAL(raiseCaptureStartRequest(void)),
                        m_camera[2],SLOT(handleStartRequest(void)),
                        Qt::QueuedConnection);

                connect(this,SIGNAL(raiseCaptureStopRequest(void)),
                        m_camera[2],SLOT(handleStopRequest(void)),
                        Qt::QueuedConnection);
            } 
            connect(m_camera[ui->comboBoxCameraSelection->currentIndex()],SIGNAL(raiseStartDisplayRequest()),m_screen
                    ,SLOT(handleStartRequest()));
            emit raiseCaptureStartRequest();

            isConnected = true;
            ui->pushButtonOpenOrClose->setText("Disconnect");
            ui->pushButtonOpenOrClose->setDisabled(false);
            ui->pushButtonRecord->setDisabled(false);
            log("Connection to " + cameraName + " established!");
        }
        catch (Exception e)
        {
            log(e.errorMessage);
            log("Attempt to connect " + cameraName + " failed!");
            ui->pushButtonOpenOrClose->setDisabled(false);
            ui->comboBoxCameraSelection->setDisabled(false);
        }
    }
    else
    {
        try
        {
            log("Disconnecting from " + cameraName + "...");
            ui->pushButtonOpenOrClose->setDisabled(true);

            //m_recorder->stopRecording();
            //m_threadcapture->stopCapture();
            //m_threadrecord->stopCapture();
            emit raiseCaptureStopRequest();
            threadCapture.quit();

            isConnected = false;
            ui->pushButtonOpenOrClose->setText("Connect");
            ui->pushButtonOpenOrClose->setDisabled(false);
            ui->comboBoxCameraSelection->setDisabled(false);
            ui->pushButtonRecord->setDisabled(true);

            log("Disconnected from " + cameraName + "!");
        }
        catch (Exception e)
        {
            log(e.errorMessage);
            log("Attempt to disconnect from " + cameraName + " failed!");
        }
    }
    return;
}

void MainWindow::selectSerialPort(const QList<QString> &boardNames, int *selectedPort)
{
    selectSerialPortDialog d(this, boardNames, selectedPort);
    d.exec();
}

void MainWindow::selectVideoPath(string & path)
{
    QString qpath;
    QFileDialog d(this,tr("Select path"),".",tr("Any files (*.avi)"));
    d.setFileMode(QFileDialog::AnyFile);
    qpath= d.getSaveFileName();
    path = qpath.toStdString();
    return ;
}

void MainWindow::startOrStopRecording()
{
    try
    {
        if(isRecording)
        {
           emit raiseRecordStopRequest();
           ui->pushButtonRecord->setText("Start recording");
        }
        else
        {
            //emit raiseStartRequest(m_threadrecord);
            //m_threadrecord->startCapture(m_camera[2],500);
            emit raiseRecordStartRequest((imagingCamera *)m_camera[2]);
            ui->pushButtonRecord->setText("Stop recording");
            isRecording = true;
        }
    }
    catch(Exception e)
    {
        throw e;
    }
}

