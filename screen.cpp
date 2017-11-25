#include "screen.h"

screen::screen(QWidget *parent) : QWidget(parent)
{

}
screen::screen(QLabel *l)
{
    m_label = l;
    m_frameRate = 30;
    m_timer = new QTimer();
    m_timer->setInterval(1000/m_frameRate);
    m_timer->start();
    connect(m_timer,SIGNAL(timeout()),this,SLOT(refreshImage()));
}

void screen::newImage(cv::Mat matImg)
{
    if(m_frameMutex.tryLock())
    {
        m_frameQue.enqueue(matImg);
        m_frameMutex.unlock();
    }
    return;
}

void screen::refreshImage()
{
    if(m_frameMutex.tryLock())
    {
        if(!m_frameQue.empty())
        {
            cv::Mat frame = m_frameQue.dequeue();
            m_frameSize.setWidth(frame.cols);
            m_frameSize.setHeight(frame.rows);
            m_pixmap = m_pixmap.fromImage(Mat2QImage(frame));
            m_label->setPixmap(m_pixmap);
        }
        m_frameMutex.unlock();
    }
}

QImage screen::Mat2QImage(cv::Mat const& src)
{
     cv::Mat temp; // make the same cv::Mat
     cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
     QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
     dest.bits(); // enforce deep copy, see documentation
     return dest;
}
