#include "threadcapture.h"

bool threadCapture::startCapture(Camera *camera)
{
    if (isRunning())
        return false;

    if (!camera || !camera->isOpen())
        return false;

    m_camera = camera;
    m_stop = false;

    if (!m_camera->startCapture())
        return false;

    start(QThread::TimeCriticalPriority);

    return true;
}

void threadCapture::stopCapture()
{
    m_stop = true;
}

void threadCapture::run()
{
    cv::Mat grab;
    cv::Mat detect;

    while (!m_stop) {
        if (!m_camera->getNextFrame(&grab)) {
            msleep(10);
            continue;
        }
        cv::threshold(grab,detect,20,255,cv::THRESH_BINARY);
        emit newImage(detect);
    }

    m_camera->stopCapture();
}
