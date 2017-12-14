#include "webcamera.h"
WebCam::WebCam()
{
    colorConversion = CV_BGR2RGB;
}

WebCam::~WebCam()
{
    close();
}

bool WebCam::open(int device)
{
    if (m_vidcap.isOpened())
        return true;

    if (!m_vidcap.open(device))
        return false;

    return true;
}

void WebCam::close()
{
    if (m_vidcap.isOpened())
        m_vidcap.release();
}

bool WebCam::isOpen()
{
    return m_vidcap.isOpened();
}

bool WebCam::getNextFrame(cv::Mat *grab)
{
    if (!grab)
        return false;

    m_vidcap >> *grab;

    return !grab->empty();
}

// Always fixed using OpenCV interface to USB webcams
QSize WebCam::getImageSize()
{
    return QSize(640, 480);
}
