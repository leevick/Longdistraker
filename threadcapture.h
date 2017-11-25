#ifndef THREADCAPTURE_H
#define THREADCAPTURE_H

#include <QThread>
#include <QObject>
#include "camera.h"


class threadCapture : public QThread
{
    Q_OBJECT
    public:
        bool startCapture(Camera *camera);
        void stopCapture();

    signals:
        void newImage(cv::Mat grab);

    protected:
        void run();

    private:
        Camera *m_camera;
    volatile bool m_stop;
};

#endif // THREADCAPTURE_H
