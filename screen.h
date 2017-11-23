#ifndef SCREEN_H
#define SCREEN_H

#include <QWidget>
#include <opencv2/opencv.hpp>
using  namespace cv;

class screen : public QWidget
{
    Q_OBJECT
public:
    explicit screen(QWidget *parent = nullptr);
    void screen(QWidget *);

signals:

public slots:
    void UpdateImage(Mat matImg);

private:
    int m_frameRate;
    QRect m_targetLoc;

};

#endif // SCREEN_H
