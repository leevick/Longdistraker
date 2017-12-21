#ifndef SIMCAMERA_H
#define SIMCAMERA_H
#include<camera.h>
#include<opencv2/opencv.hpp>
#include<opencv2/core.hpp>


class simCamera : public Camera
{
public:
    simCamera();
    ~simCamera();

    void open();
    void close();
public slots:
    void handleStartRequest();
    void handleStopRequest();

protected slots:
    void handleTimeout();
    void handleNewImageRequest(unsigned char*&buffer);

private:
    double locationX =0;
    double locationY =0;
    cv::Mat spaceStation;
    cv::Mat spaceBackground;
    QTimer *captureTimer;
};

#endif // SIMCAMERA_H