#include "imagingcamera.h"

imagingCamera::imagingCamera()
{
  colorConversion = CV_GRAY2RGB;
}

imagingCamera::~imagingCamera()
{

}
bool imagingCamera::open(int id)
{

    try
    {       
        int nr_of_buffer	=	8;			// Number of memory buffer
        int nBoard			=	0;			// Board Number
        int MaxPics			=	100;		// Number of images to grab
        int status = 0;
        const char *dllName = "Acq_DualBaseAreaGray12";
        int boardType = 0;

        QList<QString> boardNames;

        //emit selectSerialPort(boardNames,&nBoard);
        nBoard = 0;

        // Initialization of the microEnable frame grabber
        if((fg = Fg_Init(dllName,nBoard)) == NULL) 
        {
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
          return false;
        }

        // Setting the image size
        int bitAlignment = FG_LEFT_ALIGNED;
        if (Fg_setParameter(fg,FG_WIDTH,&width,nCamPort) < 0) 
        {
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
          return false;
        }
        if (Fg_setParameter(fg,FG_HEIGHT,&height,nCamPort) < 0) {
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
          return false;
        }

        if (Fg_setParameter(fg,FG_BITALIGNMENT,&bitAlignment,nCamPort) < 0) 
        {
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
          return false;
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
          return false;
        } else {
          //fprintf(stdout,"%d framebuffer allocated for port %d ok\n",nr_of_buffer,nCamPort);
        }

        

        // check for advanced applets, which have a newly designed trigger, anyway set it to camptibility mode
        bool isAdvancedApplets = false; // 
        if (strstr(dllName, "Acq_")){
          isAdvancedApplets = true;
        }
        if (isAdvancedApplets){
          // set advanced applet into compatibility mode to use current AcquisitionApplets in order to use the same
          // trigger parameters. Otherwise check applet documentation in order to use the advanced trigger features
          uint32_t on = FG_ON;
          status = Fg_setParameterWithType(fg, FG_TRIGGER_LEGACY_MODE, &on, nCamPort, FG_PARAM_TYPE_UINT32_T);
          if (status != FG_OK){
            throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
            return false;
          }
        }

        // Setting the trigger and grabber mode
        int		nTriggerMode		= FREE_RUN;

        if(Fg_setParameter(fg,FG_TRIGGERMODE,&nTriggerMode,nCamPort)<0)		
        {
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
          return false;
        }

        if((Fg_AcquireEx(fg,nCamPort,GRAB_INFINITE,ACQ_STANDARD,pMem0)) < 0){
          throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
          return false;
        }
    }
    catch(Exception e)
    {
        throw e;
        return false;
    }
    return true;
}
void imagingCamera::close()
{
  Fg_stopAcquireEx(fg,nCamPort,pMem0,0);
	Fg_FreeMemEx(fg, pMem0);
	Fg_FreeGrabber(fg);
  return;
}
bool imagingCamera::isOpen(){return true;}
bool imagingCamera::getNextFrame(cv::Mat *grab)
{
    lastPicNr = Fg_getLastPicNumberBlockingEx(fg,lastPicNr+1,nCamPort,10,pMem0);
		if(lastPicNr <0){
      close();
			throw Exception(QString::number(Fg_getLastErrorNumber(fg),10)+":"+QString(Fg_getLastErrorDescription(fg)));
		}
    grab->create(height,width,CV_8U);
    memcpy(grab->data,(uchar *)Fg_getImagePtrEx(fg,lastPicNr,0,pMem0),height*width);
  return !grab->empty();
}
QSize imagingCamera::getImageSize(){return QSize(4096,3072);}

int imagingCamera::getBoardInfo(QList<QString> &list)
{
  int boardType;
  int i = 0;

  int maxNrOfboards = 10;// use a constant no. of boards to query, when evaluations versions minor to RT 5.2
  int nrOfBoardsFound = 0;
  int maxBoardIndex = -1;
  int iPortCount = 0;
  int nrOfBoardsPresent = 0;

  list.clear();

  // detect all boards
  nrOfBoardsPresent = getNrOfBoards();
  if(!nrOfBoardsPresent)
    return 0;

  for(i = 0; i < maxNrOfboards; i++) {
    int iPortNrOnBoard = 0;
    const char * boardName;
    bool skipIndex = false;
    boardType = Fg_getBoardType(i);
    switch(boardType) {
      case PN_MICROENABLE4AS1CL:
        boardName = "MicroEnable IV AS1-CL";
        iPortNrOnBoard = 1;
        break;
      case PN_MICROENABLE4AD1CL:
        boardName = "MicroEnable IV AD1-CL";
        iPortNrOnBoard = 1;
        break;
      case PN_MICROENABLE4VD1CL:
        boardName = "MicroEnable IV VD1-CL";
        iPortNrOnBoard = 2;
        break;
      case PN_MICROENABLE4AD4CL:
        boardName = "MicroEnable IV AD4-CL";
        iPortNrOnBoard = 2;
        break;
      case PN_MICROENABLE4VD4CL:
        boardName = "MicroEnable IV VD4-CL";
        iPortNrOnBoard = 2;
        break;
      // ignore the non-cameralink boards
      case PN_MICROENABLE3I:
      case PN_MICROENABLE3IXXL:
      case PN_MICROENABLE4AQ4GE:
      case PN_MICROENABLE4VQ4GE:
      default:
        boardName = "Unknown / Unsupported Board";
        skipIndex = true;
    }
    if (!skipIndex){
      nrOfBoardsFound++;
      maxBoardIndex = i;

      if(iPortNrOnBoard > 0){
        //if(i==0)
        //printf("Following serial ports are available:\n");
        for(int j = 0; j < iPortNrOnBoard; j++){
          iPortCount++;

          //printf("%d. Board_%u %s (%x) Port_%d\n", iPortCount-1, i, boardName, boardType, j);
          list.append(QString::number(iPortCount-1,10)+"."+"Board_"+QString::number(i,10)+" "+QString(boardName)+" ("+QString::number(boardType,16)+") Port_"+QString::number(j,10));
        }
      }
    }
    else{
    }
    if (nrOfBoardsFound >= nrOfBoardsPresent){
      break;// all boards are scanned
    }
  }
  return iPortCount;
}
