#include "recorder.h"

Recorder::Recorder()
{

}

void Recorder::handleStartRequest(imagingCamera *cam)
{
    try
    {
        int depth = 1;
        int width = cam->getWidth();
        int height = cam->getHeight();
        int size=height*width*depth;
        void **aviRef = new void*[1];
        unsigned char *buffer = new unsigned char[size];
        cv::Mat temp;
        temp.create(height,width,CV_8U);
        fg = cam->fg;
        pBuffer = cam->pMem0;
        isRecording = true;
        lastid = Fg_getLastPicNumberEx(cam->fg,0,cam->pMem0);
        qint64 startid = lastid;
        qint64 currentIndex=0;
        QRunnable *task;
        while(isRecording)
        {
            currentIndex = Fg_getLastPicNumberEx(cam->fg,0,cam->pMem0);
            if(lastid<currentIndex)
            {

                task = new taskVideoIO((char *)Fg_getImagePtrEx(fg,lastid,0,pBuffer),cam->getWidth()*cam->getHeight(),lastid);
                QThreadPool::globalInstance()->start(task);
                lastid++;

            }else if(lastid==currentIndex)
            {
                continue;
            }
            else
            {
                lastid= currentIndex;
            }
        }
        currentIndex = Fg_getLastPicNumberEx(cam->fg,0,cam->pMem0);
        while(lastid<currentIndex)
        {
            task = new taskVideoIO((char *)Fg_getImagePtrEx(fg,lastid,0,pBuffer),cam->getWidth()*cam->getHeight(),lastid);
            QThreadPool::globalInstance()->start(task);
            lastid++;
        }

        string path;
        emit selectVideoPath(path);
        video = new cv::VideoWriter();
        double fps = cam->getFps();
        bool flag =video->open(path ,cv::VideoWriter::fourcc('M','P','E','G'),cam->getFps(),cv::Size(width,height),false);
        
        for(int i=startid;i<lastid;i++)
        {
            FILE *p;
            char name[10];
            sprintf(name,"%d.raw",i);
            p = fopen(name,"rb");
            fread(buffer,1,size,p);
            fclose(p);
            temp.data = buffer;
            video->write(temp);
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
    //captureTimer->stop();
    //video->release();
    return;
}

void Recorder::handleTimeout()
{
        frame.data = (uchar *)Fg_getImagePtrEx(fg,lastid,0,pBuffer);
        lastid+=1;
        video->write(frame);
}