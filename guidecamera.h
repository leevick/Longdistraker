#ifndef GUIDECAMERA_H
#define GUIDECAMERA_H
#include <camera.h>
#include <pylon/PylonIncludes.h>

using namespace Pylon;
using namespace std;

class guideCamera : public Camera
{
public:
    guideCamera();
    ~guideCamera();
    bool open(int id);
    void close();
    bool isOpen();
    bool getNextFrame(cv::Mat *grab);
    QSize getImageSize();
    QSize imageSize;
private:
    CInstantCamera *m_camera;
    CGrabResultPtr m_ptrGrabResult;

};

#endif // GUIDECAMERA_H
