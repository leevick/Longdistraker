#include "screen.h"

screen::screen(QWidget *parent) : QWidget(parent)
{

}
screen::screen(QLabel *l,Camera *c):
m_label(l),m_frameRate(c->getFps()),
m_frameSize(QSize(c->getWidth(),c->getHeight())),
m_pixmap(QPixmap(m_frameSize)),m_timer(new QTimer()),m_camera(c),buffer(new uchar[m_frameSize.width()*m_frameSize.height()])
{
    //initiallize params
    tempImg.create(m_frameSize.height(),m_frameSize.width(),CV_8U);
    l->setPixmap(m_pixmap);
    //setup timer
    m_timer->setInterval(1000/m_frameRate*2);
    memset(buffer,0,m_frameSize.width()*m_frameSize.height());
}

screen::~screen()
{
    delete m_timer;
}
void screen::refreshImage()
{
    emit raiseNewImageRequest(buffer);
    tempImg.data = buffer;
    cv::imencode(".bmp",tempImg,m_buffer);
    //if(m_frameMutex.tryLock())
    //{
        bool flag;
        flag =m_pixmap.loadFromData(&m_buffer[0],m_buffer.size(),"BMP",Qt::ImageConversionFlag::MonoOnly);
        m_label->setPixmap(m_pixmap);
    //}
    m_buffer.empty();
}

void screen::handleStartRequest()
{
    try
    {
        connect(this,SIGNAL(raiseNewImageRequest(unsigned char *&)),m_camera,SLOT(handleNewImageRequest(unsigned char *&)),Qt::BlockingQueuedConnection);
        connect(m_timer,SIGNAL(timeout()),this,SLOT(refreshImage()));
        m_timer->start();
    }catch(Exception e)
    {
        throw e;
    }

}
void screen::handleStopRequest()
{
    try
    {
        m_timer->start();
        disconnect(m_timer,SIGNAL(timeout()),this,SLOT(refreshImage()));
    }catch(Exception e)
    {
        throw e;
    }

}
