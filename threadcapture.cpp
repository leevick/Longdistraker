#include "threadcapture.h"

bool threadCapture::startCapture(Camera *camera,int interval,QThread::Priority pri)
{
    try
    {
        m_camera = camera;
        m_camera->open(0);
        desiredInterval = interval;
        QSize size = m_camera->getImageSize();
        grab = new cv::Mat[m_camera->framePerGrab];
        for(int i=0;i<m_camera->framePerGrab;i++)
        {
            grab[i].create(size.height(),size.width(),CV_8U);
        }

        if(interval!=0)
        {
            desiredFrameRate = 1000/interval;
        }
        else
        {
            desiredFrameRate = 0;
        }
        canCapture = true;
        start(pri);
        return true;
    }
    catch(Exception e)
    {
        throw e;
        return false;
    }
}

void threadCapture::stopCapture()
{
    canCapture = false;
}

void threadCapture::run()
{
    try
    {
        if(desiredInterval!=0)
        {
            captureTimer = new QTimer();
            captureTimer->setInterval(desiredInterval);
            connect(captureTimer,SIGNAL(timeout()),this,SLOT(captureTimeoutHandler()),Qt::DirectConnection);
            captureTimer->start();
        }

        while(canCapture)
        {
            if(desiredInterval==0)
            {
                m_camera->getNextFrame(grab);
                emit newImage(grab,m_camera->colorConversion,m_camera->framePerGrab);
            }
        }
        if(desiredInterval!=0)
        captureTimer->stop();
        m_camera->close();
    }
    catch(Exception e)
    {
        throw e;
    }
}

void threadCapture::captureTimeoutHandler()
{
    emit newImage(grab,m_camera->colorConversion,m_camera->framePerGrab);
}
