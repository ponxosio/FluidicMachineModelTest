#include "stringpumpproduct.h"

StringPumpProduct::StringPumpProduct(const std::string & name, std::stringstream * stream) :
    PumpPluginProduct(), name(name)
{
    this->stream = stream;
}

StringPumpProduct::~StringPumpProduct() {

}

void StringPumpProduct::setPumpState(int dir, double rate) {
    *stream << "SET PUMP " << name << ": dir " << dir << ", rate " << rate;
}
