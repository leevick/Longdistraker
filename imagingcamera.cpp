#include "imagingcamera.h"

imagingCamera::imagingCamera()
{
  colorConversion = CV_GRAY2RGB;
  fps = 20;
  width = 1024;
  height = 768;
  framesPerGrab = 20;
  imageBuffer = new cv::Mat[framesPerGrab];
  for(int i=0;i<framesPerGrab;i++)
  {
      imageBuffer->create(height,width,CV_8U);
  }
}

imagingCamera::~imagingCamera()
{

}

void imagingCamera::open()
{
    if(m_isOpen)return;

    try
    {       
        int nr_of_buffer	=	128;			// Number of memory buffer
        int nBoard			=	0;			// Board Number
        const char *dllName = "Acq_FullAreaGray8";

        // Initialization of the microEnable frame grabber
        if((fg = Fg_Init(dllName,nBoard)) == NULL) 
        {
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        }
        // Setting the image size
        int bitAlignment = FG_LEFT_ALIGNED;
        if (Fg_setParameter(fg,FG_WIDTH,&width,nCamPort) < 0) 
        {
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        }
        if (Fg_setParameter(fg,FG_HEIGHT,&height,nCamPort) < 0) {
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        }

        if (Fg_setParameter(fg,FG_BITALIGNMENT,&bitAlignment,nCamPort) < 0) 
        {
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        }

        // Memory allocation
        int format;
        Fg_getParameter(fg,FG_FORMAT,&format,nCamPort);
        size_t bytesPerPixel = 2;
        switch(format){
        case FG_GRAY:	bytesPerPixel = 1; break;
        case FG_GRAY16:	bytesPerPixel = 2; break;
        case FG_COL24:	bytesPerPixel = 3; break;
        case FG_COL32:	bytesPerPixel = 4; break;
        case FG_COL30:	bytesPerPixel = 5; break;
        case FG_COL48:	bytesPerPixel = 6; break;
        }

        size_t totalBufSize = width*height*nr_of_buffer*bytesPerPixel;
        if((pMem0 = Fg_AllocMemEx(fg,totalBufSize,nr_of_buffer)) == NULL){
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        } else {
          //fprintf(stdout,"%d framebuffer allocated for port %d ok\n",nr_of_buffer,nCamPort);
        }

        unsigned int triggerMode = ATM_GENERATOR;
        if(Fg_setParameterWithType(fg,FG_AREATRIGGERMODE, &triggerMode, 0,FG_PARAM_TYPE_UINT32_T) < 0)
        {
            throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        }

        double fps = this->fps;
        if(Fg_setParameterWithType(fg,FG_TRIGGER_FRAMESPERSECOND, &fps, 0,FG_PARAM_TYPE_DOUBLE) < 0)
        {
            throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        }

        double nExposureInMicroSec = 200;
        if(Fg_setParameterWithType(fg,FG_TRIGGER_PULSEFORMGEN0_WIDTH, &nExposureInMicroSec, 0,FG_PARAM_TYPE_DOUBLE) < 0)
        {
            throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        }

      //  unsigned int CC1 = CC_PULSEGEN0;
      //   if(Fg_setParameter(fg,FG_CCSEL0, &CC1, 0) < 0)
      //   {
      //       throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
      //       return false;
      //   }

        unsigned int triggerStatus = TS_ACTIVE;
        if(Fg_setParameterWithType(fg,FG_TRIGGERSTATE, &triggerStatus, 0,FG_PARAM_TYPE_UINT32_T) < 0)
        {
            throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        }

        if((Fg_AcquireEx(fg,nCamPort,GRAB_INFINITE,ACQ_STANDARD,pMem0)) < 0){
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        }
        m_isOpen = true;
    }
    catch(Exception e)
    {
        throw e;
    }
}


void imagingCamera::close()
{
  if(m_isOpen)
  {
    Fg_stopAcquireEx(fg,nCamPort,pMem0,0);
    Fg_FreeMemEx(fg, pMem0);
    Fg_FreeGrabber(fg);
  }
  return;
}

void imagingCamera::handleStartRequest()
{
    try
    {  
        if(!m_isOpen)
        {
          open();
        }
        captureTimer=new QTimer();
        connect(captureTimer,SIGNAL(timeout(void)),this,SLOT(handleTimeout(void)));
        captureTimer->setInterval(500);
        //captureTimer->start();
    }
    catch(Exception e)
    {
        close();
        throw e;
    }

}

void imagingCamera::handleStopRequest()
{
    try
    {
        captureTimer->stop();
        m_isCapturing = false;
    }
    catch(Exception e)
    {
        throw e;
    }
}

void imagingCamera::handleTimeout()
{
    lastPicNr = Fg_getLastPicNumberBlockingEx(fg,lastPicNr+framesPerGrab,nCamPort,10,pMem0);
        if(lastPicNr <0)
        {
            throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
        }
}

