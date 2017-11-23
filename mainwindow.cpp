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
    connect(m_threadcapture,SIGNAL(newImage(Mat)),m_screen,SLOT(newImage(Mat)),Qt::DirectConnection);
    m_threadcapture->startCapture(m_camera);

}

MainWindow::~MainWindow()
{
    delete ui;
}
