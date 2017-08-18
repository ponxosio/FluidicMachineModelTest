#include "stringpumpproduct.h"

StringPumpProduct::StringPumpProduct(std::stringstream * stream) :
    PumpPluginProduct()
{
    this->stream = stream;
}

StringPumpProduct::~StringPumpProduct() {

}

void StringPumpProduct::setPumpState(int dir, units::Volumetric_Flow rate) {
    *stream << "PUMP DIR " << dir << " RATE " << rate.to(units::ml / units::hr) << "ml/h;";
}

void StringPumpProduct::stopPump() {
    *stream << "STOP PUMP;";
}
