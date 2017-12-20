#ifndef RECORDER_H
#define RECORDER_H
#include<QQueue>
#include<QThread>
//#include<threadcapture.h>
#include<Exception.h>
#include<opencv2/opencv.hpp>
#include<imagingcamera.h>

using namespace std;
class Recorder : public QObject
{
    Q_OBJECT
public:
    Recorder();

signals:
    void selectVideoPath(string &path);

public slots:
    void handleStartRequest(imagingCamera *cam);
    void handleStopRequest();
private:
    volatile bool isRecording = false;
    string videoPath;
    cv::VideoWriter *video;
    unsigned char *pBuffer;
    Fg_Struct *fg;
    long long lastid=0;

    

};

#endif // RECORDER_H
