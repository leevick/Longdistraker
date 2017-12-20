#ifndef TASKVIDEOIO_H
#define TASKVIDEOIO_H

#include<QThread>
#include<QThreadPool>
#include<QRunnable>

#include<opencv2/opencv.hpp>

using namespace std;
class taskVideoIO: public QRunnable
{
public:
    taskVideoIO(cv::Mat mat,int id);
    void run();
private:
    cv::Mat img;
    int id;
};

#endif // TASKVIDEOIO_H
