#ifndef STRINGVALVEPRODUCT_H
#define STRINGVALVEPRODUCT_H

#include <sstream>

#include <commonmodel/plugininterface/valvepluginproduct.h>

class StringValveProduct : public ValvePluginProduct
{
public:
    StringValveProduct(const std::string & name, std::stringstream* stream);
    virtual ~StringValveProduct();

    virtual void moveToPosition(int position);
    virtual void closeValve();

protected:
    std::string name;
    std::stringstream * stream;
};

#endif // STRINGVALVEPRODUCT_H
