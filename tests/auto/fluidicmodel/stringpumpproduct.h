#ifndef STRINGPUMPPRODUCT_H
#define STRINGPUMPPRODUCT_H

#include <sstream>

#include <commonmodel/plugininterface/pumppluginproduct.h>

class StringPumpProduct : public PumpPluginProduct
{
public:
    StringPumpProduct(const std::string & name, std::stringstream * stream);
    virtual ~StringPumpProduct();

    virtual void setPumpState(int dir, units::Volumetric_Flow rate);

protected:
    std::string name;
    std::stringstream * stream;
};

#endif // STRINGPUMPPRODUCT_H
