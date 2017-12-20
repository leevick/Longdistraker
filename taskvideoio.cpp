#include "taskvideoio.h"

taskVideoIO::taskVideoIO(cv::Mat mat,int id)
{
    img = mat.clone();
    this->id = id;
}

void taskVideoIO::run()
{
    QString path = QString::number(id)+".raw";
    ofstream fout(path.toStdString(),ios::trunc);
    fout<<img;
    fout.close();
    return;
}
