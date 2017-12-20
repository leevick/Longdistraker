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
        bool startCapture(Camera *camera, int interval = 0, QThread::Priority pri = QThread::TimeCriticalPriority);
        void stopCapture();
    signals:
        void newImage(cv::Mat *grab, int colorConversion, int count);

    protected:
        void run();

    private:
        Camera *m_camera;
        int desiredInterval = 0;
        int desiredFrameRate = 0;
        cv::Mat *grab;
        QTimer *captureTimer;
        volatile bool canCapture = true;

    private slots:
        void captureTimeoutHandler();
};
#endif // THREADCAPTURE_H
