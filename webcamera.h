#ifndef WEBCAM_H
#define WEBCAM_H

#include "camera.h"

class WebCam : public Camera
{
public:
    WebCam();
    virtual ~WebCam();

    virtual bool open(int device);
    virtual void close();
    virtual bool isOpen();
    virtual bool getNextFrame(cv::Mat *grab);
    virtual QSize getImageSize();

private:
    cv::VideoCapture m_vidcap;
};

#endif // WEBCAM_H
