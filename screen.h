#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include <QLabel>
#include <QPixmap>
#include <QQueue>
#include <QMutex>
#include <QTimer>
#include <opencv2/opencv.hpp>
using namespace std;

class screen : public QWidget
{
    Q_OBJECT
public:
    explicit screen(QWidget *parent = nullptr);
    explicit screen(QLabel * l);
    static QImage screen::Mat2QImage(cv::Mat const& src);

signals:

public slots:
    void newImage(cv::Mat matImg);
    void refreshImage();

private:
    int m_frameRate =0;
    QSize m_frameSize;
    QRect m_targetLoc;
    QLabel *m_label;
    QPixmap m_pixmap;
    QQueue<cv::Mat> m_frameQue;
    QMutex m_frameMutex;
    QTimer *m_timer;
};

#endif // SCREEN_H
