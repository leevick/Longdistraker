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
        m_screen = new screen(ui->labelImage);
        m_camera = new Camera *[4];
        m_camera[0] = new WebCam();
        m_camera[1] = new guideCamera();
        m_camera[2] = new imagingCamera();
        m_threadcapture = new threadCapture();

        //Connect signals and slots
        connect(ui->pushButtonOpenOrClose, SIGNAL(clicked(bool)), this, SLOT(connectCamera()));
        connect((QObject *)m_camera[2], SIGNAL(selectSerialPort(const QList<QString> &, int *)), this, SLOT(selectSerialPort(const QList<QString> &, int *)), Qt::DirectConnection);
        connect(m_threadcapture,SIGNAL(log(QString)),this,SLOT(print2log(QString)),Qt::QueuedConnection);

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

void MainWindow::connectCamera()
{
    QString cameraName = ui->comboBoxCameraSelection->itemText(ui->comboBoxCameraSelection->currentIndex());
    if (!isConnected)
    {
        try
        {
            log("Connecting to " + cameraName + "...");
            ui->pushButtonOpenOrClose->setDisabled(true);
            ui->comboBoxCameraSelection->setDisabled(true);
            m_camera[ui->comboBoxCameraSelection->currentIndex()]->open(0);
            connect(m_threadcapture, SIGNAL(newImage(cv::Mat, int)), m_screen, SLOT(newImage(cv::Mat, int)), Qt::DirectConnection);
            m_threadcapture->startCapture(m_camera[ui->comboBoxCameraSelection->currentIndex()]);

            isConnected = true;
            ui->pushButtonOpenOrClose->setText("Disconnect");
            ui->pushButtonOpenOrClose->setDisabled(false);
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

            m_threadcapture->stopCapture();
            m_threadcapture->terminate();
            disconnect(m_threadcapture);
            m_camera[ui->comboBoxCameraSelection->currentIndex()]->close();

            isConnected = false;
            ui->pushButtonOpenOrClose->setText("Connect");
            ui->pushButtonOpenOrClose->setDisabled(false);
            ui->comboBoxCameraSelection->setDisabled(false);

            log("Disconnected from " + cameraName + "!");
        }
        catch (Exception e)
        {
            log(e.errorMessage);
            log("Attempt to disconnect from " + cameraName + " failed!");
            m_threadcapture->terminate();
            disconnect(m_threadcapture);
        }
    }

    return;
}

void MainWindow::selectSerialPort(const QList<QString> &boardNames, int *selectedPort)
{
    selectSerialPortDialog d(this, boardNames, selectedPort);
    d.exec();
}
