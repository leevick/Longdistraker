#include "taskvideoio.h"

taskVideoIO::taskVideoIO(char *data,int size,int id)
{
    this->id = id;
    this->data = data;
    this->size = size;
}

void taskVideoIO::run()
{
    char s[10];
    sprintf(s,"%d.raw",id);
    FILE *p =fopen(s,"wb");
    fwrite(data,1,size,p);
    fclose(p);
    return;
}
