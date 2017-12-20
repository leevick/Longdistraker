#ifndef WEBCAM_H
#define WEBCAM_H

#include "camera.h"

class WebCam : public Camera
{
    Q_OBJECT
public:
    WebCam();
    virtual ~WebCam();
    virtual void open();
    virtual void close();
    virtual bool isOpen();
public slots:
    virtual void handleStartRequest();
    virtual void handleStopRequest();
protected slots:
    virtual void handleTimeout();

signals:
    void sendNewImages(QQueue<cv::Mat>);

private:
    cv::VideoCapture m_vidcap;
};

#endif // WEBCAM_H
