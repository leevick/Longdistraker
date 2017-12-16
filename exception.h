#ifndef EXCEPTION_H
#define EXCEPTION_H
#include <QException>


class Exception : public QException
{
public:
    Exception();
    Exception(QString errorMessage);
    QString errorMessage;
};

#endif // EXCEPTION_H
