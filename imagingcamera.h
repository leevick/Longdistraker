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

enum Tap{Base,Medium,Tap8,Tap10};

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
    Fg_Struct *fg;
    int nCamPort		=	PORT_A;		// Port (PORT_A / PORT_B)
    dma_mem * pMem0;
    frameindex_t lastPicNr = 0;
    int width = 4096;
    int height = 3072;
    int imagingCamera::getBoardInfo(QList<QString> &list);
};

#endif // IMAGINGCAMERA_H
