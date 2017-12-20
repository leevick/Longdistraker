#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QQueue>
#include <QMutex>
#include <QTimer>

#include <camera.h>
#include <opencv2/opencv.hpp>
using namespace std;

class screen : public QWidget
{
    Q_OBJECT
public:
    explicit screen(QWidget *parent = nullptr);
    explicit screen(QLabel * l,Camera *c);
    static QImage screen::Mat2QImage(cv::Mat const& src,int colorConversion);

signals:

public slots:
    void newImage(QQueue<cv::Mat> matImg);
    void refreshImage();

private:
    int m_frameRate;
    int m_colorConversion;
    int m_framePerGrab;
    QSize m_frameSize;
    QRect m_targetLoc;
    QLabel *m_label;
    QPixmap m_pixmap;
    QQueue<cv::Mat> m_frameQue;
    QMutex m_frameMutex;
    QTimer *m_timer;
};

#endif // SCREEN_H
