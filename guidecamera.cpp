#include "guidecamera.h"


guideCamera::guideCamera()
{
    colorConversion = CV_GRAY2RGB;
}

guideCamera::~guideCamera()
{
    close();
    PylonTerminate();
}

bool guideCamera::open(int id)
{
    try
    {
        PylonInitialize();
        m_camera = new CInstantCamera( CTlFactory::GetInstance().CreateFirstDevice());
        m_camera->MaxNumBuffer = 5;
        m_camera->StartGrabbing();
        return true;
    }catch(const GenericException &e)
    {
        Exception f("Failed to open guidance camera!");
        throw f;
        return false;
    }
}

void guideCamera::close()
{

}

bool guideCamera::isOpen()
{
    return m_camera->IsGrabbing();
}

bool guideCamera::getNextFrame(cv::Mat *grab)
{
    if (!grab)
        return false;

    m_camera->RetrieveResult(5000,m_ptrGrabResult);
    if(m_ptrGrabResult->GrabSucceeded())
    {
        imageSize = getImageSize();
        grab->create(imageSize.height(),imageSize.width(),CV_8U);
        grab->data = (uchar *)m_ptrGrabResult->GetBuffer();
    }
    return !grab->empty();
}

// Always fixed using OpenCV interface to USB guideCameras
QSize guideCamera::getImageSize()
{
    if(m_ptrGrabResult->GrabSucceeded())
        return QSize(m_ptrGrabResult->GetWidth(),m_ptrGrabResult->GetHeight());
    else return QSize(0,0);
}
