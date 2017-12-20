#include "recorder.h"

Recorder::Recorder()
{

}

void Recorder::handleStartRequest(imagingCamera *cam)
{
    try
    {
        fg = cam->fg;
        pBuffer = cam->pMem0;
        void **aviRef = new void*[1];
        IoCreateAVIGray(aviRef,"test.avi",1024,768,20);
        isRecording = true;
        lastid = Fg_getLastPicNumberEx(cam->fg,0,cam->pMem0);
        qint64 currentIndex=0;
        while(isRecording)
        {
            currentIndex = Fg_getLastPicNumberEx(cam->fg,0,cam->pMem0);
            if(lastid<currentIndex)
            {
                IoWriteAVIPicture(aviRef[0],1,(uchar*)Fg_getImagePtrEx(fg,lastid,0,cam->pMem0));

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
            IoWriteAVIPicture(aviRef[0],1,(uchar*)Fg_getImagePtrEx(fg,lastid,0,cam->pMem0));
            lastid++;
        }

        IoCloseAVI(aviRef[0]);

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
