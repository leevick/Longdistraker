#include "imagingcamera.h"

imagingCamera::imagingCamera()
{

}
imagingCamera::~imagingCamera(){}
bool imagingCamera::open(int id){return true;}
void imagingCamera::close(){return;}
bool imagingCamera::isOpen(){return true;}
bool imagingCamera::getNextFrame(cv::Mat *grab){return true;}
QSize imagingCamera::getImageSize(){return QSize(0,0);}
