#include "exception.h"

Exception::Exception()
{

}
Exception::Exception(QString errorMessage)
{
    this->errorMessage = errorMessage;
}

