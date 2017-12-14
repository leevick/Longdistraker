#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
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

        //Connect signals and slots
        connect(ui->pushButtonOpenOrClose,SIGNAL(clicked(bool)),this,SLOT(connectCamera()));
        
        // m_camera->open(0);
        // m_threadcapture = new threadCapture();
        // connect(m_threadcapture,SIGNAL(newImage(cv::Mat)),m_screen,SLOT(newImage(cv::Mat)),Qt::DirectConnection);
        // m_threadcapture->startCapture(m_camera);
        log("Main window created");
    }
    catch(QException e)
    {
        log("Main window creation failed!");
    }

}

MainWindow::~MainWindow()
{
    m_threadcapture->stopCapture();
    m_threadcapture->terminate();
    disconnect(this,0,0,0);
    delete m_threadcapture;
    for(int i=0;i<4;i++)
    {
        m_camera[i]->close();
        delete m_camera[i];
    }
    delete m_screen;
    delete ui;
}

void MainWindow::log(QString s)
{
    QDateTime time;
    time = time.currentDateTime();
    ui->log->insertPlainText(time.toString("HH:MM:ss")+"\t"+s+"\n");
    ui->log->moveCursor(QTextCursor::End);
    return;
}

void MainWindow::connectCamera()
{
    if(!isConnected)
    {
        try
        {
            log("Connecting to "+ui->comboBoxCameraSelection->itemText(ui->comboBoxCameraSelection->currentIndex())+"...");
            ui->pushButtonOpenOrClose->setDisabled(true);
            ui->comboBoxCameraSelection->setDisabled(true);
            m_camera[ui->comboBoxCameraSelection->currentIndex()]->open(0);
            m_threadcapture = new threadCapture();
            connect(m_threadcapture,SIGNAL(newImage(cv::Mat,int)),m_screen,SLOT(newImage(cv::Mat,int)),Qt::DirectConnection);
            m_threadcapture->startCapture(m_camera[ui->comboBoxCameraSelection->currentIndex()]);

            isConnected = true;
            ui->pushButtonOpenOrClose->setText("Disconnect");
            ui->pushButtonOpenOrClose->setDisabled(false);
            log("Connection to "+ui->comboBoxCameraSelection->itemText(ui->comboBoxCameraSelection->currentIndex())+" established!");
        }
        catch(QException e)
        {
            log("Attempt to connect "+ui->comboBoxCameraSelection->itemText(ui->comboBoxCameraSelection->currentIndex())+" failed!");
            m_threadcapture->terminate();
            disconnect(m_threadcapture);
            delete m_threadcapture;
            ui->pushButtonOpenOrClose->setDisabled(false);
            ui->comboBoxCameraSelection->setDisabled(false);
        }
    }else
    {
        try
        {
            log("Disconnecting from "+ui->comboBoxCameraSelection->itemText(ui->comboBoxCameraSelection->currentIndex())+"...");
            ui->pushButtonOpenOrClose->setDisabled(true);

            m_threadcapture->stopCapture();
            m_threadcapture->terminate();
            disconnect(m_threadcapture);
            delete m_threadcapture;
            m_camera[ui->comboBoxCameraSelection->currentIndex()]->close();

            isConnected = false;
            ui->pushButtonOpenOrClose->setText("Connect");
            ui->pushButtonOpenOrClose->setDisabled(false);
            ui->comboBoxCameraSelection->setDisabled(false);

            log("Disconnected from "+ui->comboBoxCameraSelection->itemText(ui->comboBoxCameraSelection->currentIndex())+"!");

        }
        catch(QException e)
        {
            log("Attempt to disconnect from "+ui->comboBoxCameraSelection->itemText(ui->comboBoxCameraSelection->currentIndex())+" failed!");
            m_threadcapture->terminate();
            disconnect(m_threadcapture);
            delete m_threadcapture;
        }
    }

    return;
}
