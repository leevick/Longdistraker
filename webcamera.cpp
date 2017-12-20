#include "webcamera.h"
WebCam::WebCam()
{
    colorConversion = CV_BGR2RGB;
    width = 640;
    height = 480;
    fps = 25;
    imageBuffer = new cv::Mat[framesPerGrab];
}

WebCam::~WebCam()
{
    close();
}

void WebCam::open()
{
    try
    {
        if (!m_vidcap.isOpened())
            m_vidcap.open(0);
    }
    catch(Exception e)
    {
        throw e;
    }
}

void WebCam::close()
{
    try
    {
        if (m_vidcap.isOpened())
            m_vidcap.release();
    }
    catch(Exception e)
    {
        throw e;
    }
}

bool WebCam::isOpen()
{
    try
    {
        return m_vidcap.isOpened();
    }
    catch(Exception e)
    {
        throw e;
    }
}
void WebCam::handleStartRequest()
{
    try
    {
        open();
        captureTimer = new QTimer();
        connect(captureTimer,SIGNAL(timeout(void)),this,SLOT(handleTimeout(void)),Qt::DirectConnection);
        captureTimer->setInterval(1000/fps);
        captureTimer->start();
        m_isCapturing = true;
    }
    catch(Exception e)
    {
        throw e;
    }
}

void WebCam::handleStopRequest()
{
    try
    {
        captureTimer->stop();
        this->close();
        m_isCapturing = false;
    }
    catch(Exception e)
    {
        throw e;
    }
    
}

void WebCam::handleTimeout()
{
    cv::Mat grab;
    QQueue<cv::Mat> images;
    m_vidcap>>grab;
    images.enqueue(grab);
    emit sendNewImages(images);
}
