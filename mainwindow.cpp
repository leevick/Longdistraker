#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_screen = new screen(ui->labelImage);
    m_camera = new WebCam();
    m_camera->open(0);
    m_threadcapture = new threadCapture();
    connect(m_threadcapture,SIGNAL(newImage(cv::Mat)),m_screen,SLOT(newImage(cv::Mat)),Qt::DirectConnection);
    m_threadcapture->startCapture(m_camera);

}

MainWindow::~MainWindow()
{
    m_threadcapture->stopCapture();
    m_threadcapture->terminate();
    disconnect(this,0,0,0);
    delete m_threadcapture;
    m_camera->close();
    delete m_camera;
    delete m_screen;
    delete ui;
}
