#ifndef STRINGVALVEPRODUCT_H
#define STRINGVALVEPRODUCT_H

#include <sstream>

#include <commonmodel/plugininterface/valvepluginproduct.h>

class StringValveProduct : public ValvePluginProduct
{
public:
    StringValveProduct(std::stringstream* stream);
    virtual ~StringValveProduct();

    virtual void moveToPosition(int position);
    virtual void closeValve();

protected:
    std::stringstream * stream;
};

#endif // STRINGVALVEPRODUCT_H
