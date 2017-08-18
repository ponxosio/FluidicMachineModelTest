#ifndef HEATERSTRINGPRODUCT_H
#define HEATERSTRINGPRODUCT_H

#include <sstream>

#include <commonmodel/plugininterface/heaterpluginproduct.h>

class HeaterStringProduct : public HeaterPluginProduct
{
public:
    HeaterStringProduct(std::stringstream* stream);
    virtual ~HeaterStringProduct();

    virtual void changeTemperature(units::Temperature value);
    virtual void turnOff();

    inline std::string getString() const {
        return stream->str();
    }

protected:
    std::stringstream* stream;
};

#endif // HEATERSTRINGPRODUCT_H
