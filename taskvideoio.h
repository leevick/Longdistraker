#ifndef TASKVIDEOIO_H
#define TASKVIDEOIO_H

#include<QThread>
#include<QThreadPool>
#include<QRunnable>
#include<QProcess>
#include<iostream>
#include<stdio.h>

using namespace std;
class taskVideoIO: public QRunnable
{
public:
    taskVideoIO(char *data,int size,int id);
    void run();
private:
    char *data;
    int size;
    int id;
};
#endif // TASKVIDEOIO_H
