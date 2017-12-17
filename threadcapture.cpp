#include "threadcapture.h"

bool threadCapture::startCapture(Camera *camera,int interval)
{
    desiredInterval = interval;
    countTimer = new QTimer();
    captureTimer = new QTimer();

    if(interval!=0)
    {
        desiredFrameRate = 1000/interval;
    }
    else
    {
        desiredFrameRate = 0;
    }

    if (isRunning())
        return false;

    if (!camera || !camera->isOpen())
        return false;

    m_camera = camera;
    m_stop = false;

    if (!m_camera->startCapture())
        return false;


    countTimer->setInterval(1000);
    connect(captureTimer,SIGNAL(timeout()),this,SLOT(captureTimeoutHandler()));
    connect(countTimer,SIGNAL(timeout()),this,SLOT(countTimeoutHandler()));
    countTimer->start();

    if(desiredInterval!=0)
    {
        captureTimer->setInterval(desiredInterval);
        captureTimer->start();
    }
    start(QThread::HighestPriority);
    return true;
}

void threadCapture::stopCapture()
{
    m_stop = true;
}

void threadCapture::run()
{
    if(desiredInterval!=0)
    {
        while(!m_stop)
        {
            if (!m_camera->getNextFrame(&grab)) 
            {
                continue;
            }
        };
    }
    else
    {
        while(!m_stop)
        {
            if (!m_camera->getNextFrame(&grab)) 
            {
                continue;
            }
            emit newImage(grab,m_camera->colorConversion);
            frameCount++;
        };
        
    }
    m_camera->stopCapture();
}

void threadCapture::captureTimeoutHandler()
{
    frameCount++;
    //videoWriter->write(*grab);
    emit newImage(grab,m_camera->colorConversion);

}

void threadCapture::countTimeoutHandler()
{
    std::cout<<"current actual frame rate="<<frameCount<<std::endl;
    actualFrameRate = frameCount;
    emit log("current actual frame rate="+QString::number(frameCount,10));
    frameCount = 0;

}
