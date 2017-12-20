#include "recorder.h"

Recorder::Recorder()
{

}

void Recorder::handleStartRequest(imagingCamera *cam)
{
    try
    {
        //emit selectVideoPath(videoPath);
        video = new cv::VideoWriter();
        isRecording = true;
        cv::Mat temp;
        temp.create(768,1024,CV_8U);
        video->open("test.avi",cv::VideoWriter::fourcc('M','J','P','G'),20.0,cv::Size(1024,768),false);
        lastid = Fg_getLastPicNumberEx(cam->fg,0,cam->pMem0);
        long long currentid=0;
        while(isRecording)
        {
            currentid = Fg_getLastPicNumberEx(cam->fg,0,cam->pMem0);
            if(lastid<currentid)
            {
                temp.data = (uchar *)Fg_getImagePtrEx(cam->fg,lastid,0,cam->pMem0);
                video->write(temp);
                lastid++;
            }else if(lastid==currentid)
            {
                continue;
            }
            else
            {
                lastid = currentid;
            }
        }
        currentid = Fg_getLastPicNumberEx(cam->fg,0,cam->pMem0);
        while(lastid<=currentid)
        {
            temp.data = (uchar *)Fg_getImagePtrEx(cam->fg,lastid,0,cam->pMem0);
            video->write(temp);
            lastid++;
        }


        video->release();
    }
    catch(Exception e)
    {
        throw e;
    }
}

void Recorder::handleStopRequest()
{
    isRecording = false;
    return;
}
