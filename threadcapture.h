#ifndef THREADCAPTURE_H
#define THREADCAPTURE_H

#include <QThread>
#include <QTimer>
#include <QObject>
#include "camera.h"


class threadCapture : public QThread
{
    Q_OBJECT
    public:
        bool startCapture(Camera *camera,int interval=0);
        void stopCapture();

    signals:
        void newImage(cv::Mat grab,int colorConversion);
        void log(QString s);

    protected:
        void run();

    private:
        Camera *m_camera;
        int desiredInterval;
        int desiredFrameRate = 0;
        int actualFrameRate = 0;
        cv::VideoWriter *videoWriter;
        cv::Mat         grab;
        QTimer *captureTimer;
        QTimer *countTimer;
    volatile bool m_stop;
    volatile int frameCount=0;

    private slots:
    void captureTimeoutHandler();
    void countTimeoutHandler();
};

#endif // THREADCAPTURE_H
