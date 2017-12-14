#ifndef IMAGINGCAMERA_H
#define IMAGINGCAMERA_H

#include <camera.h>

class imagingCamera : public Camera
{
public:
    imagingCamera();
    ~imagingCamera();
    bool open(int id);
    void close();
    bool isOpen();
    bool getNextFrame(cv::Mat *grab);
    QSize getImageSize();
};

#endif // IMAGINGCAMERA_H
