#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QQueue>
#include <QMutex>
#include <QTimer>
#include <Exception.h>

#include <camera.h>
#include <opencv2/opencv.hpp>
using namespace std;

class screen : public QWidget
{
    Q_OBJECT
public:
    explicit screen(QWidget *parent = nullptr);
    explicit screen(QLabel * l,Camera *c);
    ~screen();

signals:
    void raiseNewImageRequest(unsigned char *&buffer);

public slots:
    void refreshImage();
    void handleStartRequest();
    void handleStopRequest();

private:
    int m_frameRate;
    Camera *m_camera;
    QSize m_frameSize;
    QLabel *m_label;
    QPixmap m_pixmap;
    QMutex m_frameMutex;
    QTimer *m_timer;
    std::vector<uchar> m_buffer;
    cv::Mat tempImg;

    uchar *buffer;
};

#endif // SCREEN_H
