#ifndef IMAGINGCAMERA_H
#define IMAGINGCAMERA_H

#include <Exception>
#include <QMessageBox>
#include <QList>

#include <camera.h>

#include <clser.h>
#include <board_and_dll_chooser.h>
#include <os_type.h>

#include <fgrab_struct.h>
#include <fgrab_prototyp.h>
#include <fgrab_define.h>
#include <SisoDisplay.h>

class imagingCamera : public Camera
{
    Q_OBJECT
public:
    imagingCamera();
    ~imagingCamera();
    bool open(int id);
    void close();
    bool isOpen();
    bool getNextFrame(cv::Mat *grab);
    QSize getImageSize();
signals:
    void selectSerialPort(const QList<QString> &boardNames,int *selectedPort);

private:
    void *hSer = NULL;  // reference to serial port
    int iPortNr = -1;
    unsigned int iPortCount = 0;  // count of serial port in whole system
    int iPortCountCheck = -1;
    int iRet = CL_ERR_NO_ERR;

    unsigned int iErrTextSize = 0;
    unsigned int clSerBaudRate = CL_BAUDRATE_9600;	// baud rate of serial port, normally 9600

    int imagingCamera::getBoardInfo(QList<QString> &list);
};

#endif // IMAGINGCAMERA_H
