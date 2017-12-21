#ifndef CAMERA_H
#define CAMERA_H

#include <QThread>
#include <QQueue>
#include <QSize>
#include <QTimer>
#include <QMetaType>
#include <exception.h>
#include <opencv2/opencv.hpp>

using namespace std;
class Camera : public QObject
{
    Q_OBJECT
public:
    Camera() {}
    virtual ~Camera(){}
    virtual void open() = 0;

    virtual int getFramesPerGrab(){return 1;};
    virtual void setFramePerGrab(){};

    virtual void setFps(){}
    virtual double getFps(){return fps;}

    virtual void setExposureTime(){}
    virtual double getExposureTime(){return 0;}

    virtual int getColorConversion(){return colorConversion;}

    virtual int  getHeight(){return height;}
    virtual int  getWidth(){return width;}
    virtual void close() = 0;
    virtual bool isOpen(){return m_isOpen;}
    virtual bool isCapturing(){return m_isCapturing;}

    public slots:
    virtual void handleStartRequest()=0;
    virtual void handleStopRequest()=0;
    
    
    signals:
    void raiseStartDisplayRequest();
    void raiseStopDisplayRequest();

protected:

    bool m_isCapturing=false;
    bool m_isOpen = false;
    int width=0;
    int height=0;
    int fps=0;
    int framesPerGrab = 1;
    double exposureTime=0;
    int colorConversion=0;
    QTimer *captureTimer;
    cv::Mat *imageBuffer;
protected slots:
    virtual void handleNewImageRequest(unsigned char*&buffer){}
    virtual void handleTimeout()=0;
};

#endif // CAMERA_H
