#ifndef IMAGINGCAMERA_H
#define IMAGINGCAMERA_H

#include <Exception>
#include <QMessageBox>
#include <QList>
#include <QQueue>

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
    virtual void open();
    virtual void close();
    Fg_Struct *fg;
    dma_mem * pMem0;
    
public slots:
    virtual void handleStartRequest();
    virtual void handleStopRequest();
protected slots:
    virtual void handleTimeout();
    void handleNewImageRequest(unsigned char *&buffer);
signals:
    void sendNewImages(QQueue<cv::Mat>);
    void selectSerialPort(const QList<QString> &boardNames,int *selectedPort);
    
private:

    int nCamPort		=	PORT_A;		// Port (PORT_A / PORT_B)
    frameindex_t lastPicNr = 0;
    QQueue<cv::Mat> images;

};

#endif // IMAGINGCAMERA_H
