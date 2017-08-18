#include "heaterstringproduct.h"

HeaterStringProduct::HeaterStringProduct(std::stringstream* stream) {
    this->stream = stream;
}

HeaterStringProduct::~HeaterStringProduct() {

}

void HeaterStringProduct::changeTemperature(units::Temperature value) {
    *stream << "HEAT " << value.to(units::C) << "ºC;";
}

void HeaterStringProduct::turnOff() {
    *stream << "STOP HEAT;";
}
