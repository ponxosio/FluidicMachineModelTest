#include "stringpumpproduct.h"

StringPumpProduct::StringPumpProduct(const std::string & name, std::stringstream * stream) :
    PumpPluginProduct(), name(name)
{
    this->stream = stream;
}

StringPumpProduct::~StringPumpProduct() {

}

void StringPumpProduct::setPumpState(int dir, units::Volumetric_Flow rate) {
    *stream << "SET PUMP " << name << ": dir " << dir << ", rate " << rate.to(units::ml / units::hr) << "ml/h";
}

void StringPumpProduct::stopPump() {
    *stream << "SET PUMP " << name << ", rate " << 0.0 << "ml/h";
}
