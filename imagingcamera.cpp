#include "imagingcamera.h"

imagingCamera::imagingCamera()
{

}
imagingCamera::~imagingCamera(){}
bool imagingCamera::open(int id)
{
    try
    {
        QList<QString> boardNames;

        // start up
        iPortCountCheck = getBoardInfo(boardNames);
        if((iRet = clGetNumSerialPorts(&iPortCount)) == CL_ERR_NO_ERR && iPortCount == iPortCountCheck ){

             emit selectSerialPort(boardNames,&iPortNr);
           // initialize serial port
          if((iRet = clSerialInit(iPortNr, &hSer)) != CL_ERR_NO_ERR){
            // get size of Buffer
            clGetErrorText(iRet, NULL, &iErrTextSize);
            if( iErrTextSize > 0 ){
              char *ErrText = NULL;
              // retrieve the Buffer
              ErrText = new char[iErrTextSize];
              clGetErrorText(iRet, ErrText, &iErrTextSize);
              throw Exception("Cannot open connection to CameraLink port:"+QString(ErrText));
              delete ErrText;
            }
            return false;
          }

          // set baudrate
          if ( (iRet = clSetBaudRate(hSer, clSerBaudRate)) != CL_ERR_NO_ERR) {
            // get size of Buffer
            clGetErrorText(iRet, NULL, &iErrTextSize);
            if( iErrTextSize > 0 ){
              char *ErrText = NULL;
              ErrText = new char[iErrTextSize];
              clGetErrorText(iRet, ErrText, &iErrTextSize);
              delete ErrText;
            }
            throw Exception("Cannot set baud rate to requested value");
            return false;
          }
          
          

        }
        else{

          return false;
        }

        return true;
    }
    catch(Exception e)
    {
        throw e;
        return false;
    }
    return true;
}
void imagingCamera::close(){return;}
bool imagingCamera::isOpen(){return true;}
bool imagingCamera::getNextFrame(cv::Mat *grab){return true;}
QSize imagingCamera::getImageSize(){return QSize(0,0);}

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
