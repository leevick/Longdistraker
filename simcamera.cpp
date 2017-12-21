#include "simcamera.h"

simCamera::simCamera()
{
    width =2330;
    height = 1750;
    fps = 20;
}

simCamera::~simCamera()
{
}

void simCamera::open(){}
void simCamera::close(){}
void simCamera::handleStartRequest()
{
    try
    {
        spaceBackground.create(height,width,CV_8U);
        spaceStation = cv::imread("station.jpg");
        cv::Mat ROI = spaceStation.clone();
        cv::cvtColor(ROI,spaceStation,CV_RGB2GRAY);
        captureTimer = new QTimer();
        emit raiseStartDisplayRequest();

    }catch(Exception e)
    {
        throw e;
    }

}
void simCamera::handleStopRequest()
{

}
void simCamera::handleTimeout()
{

}
void simCamera::handleNewImageRequest(unsigned char*&buffer)
{
    static uint64 timestamp=0;
    static int row_prev =0;
    static int col_prev =0;
    int row = height/2+height*cos((double)timestamp/100)/4;
    int col = width/2+height*sin((double)timestamp/100)/4;
    // cv::Mat temp = spaceBackground.clone();
    // //spaceBackground(cv::Rect(0,0,100,150))=spaceStation(cv::Rect(0,0,100,150));
    
    for(int i=0;i<spaceStation.rows;i++)
    {
        memset(buffer+(i+row_prev)*width+col_prev,0,spaceStation.cols);
    }


    for(int i=0;i<spaceStation.rows;i++)
    {
        memcpy(buffer+(i+row)*width+col,spaceStation.data+i*spaceStation.cols,spaceStation.cols);
    }

    timestamp++;
    return;
}
