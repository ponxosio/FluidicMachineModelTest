#include "stringvalveproduct.h"

StringValveProduct::StringValveProduct(std::stringstream* stream) :
     ValvePluginProduct()
{
   this->stream = stream;
}

StringValveProduct::~StringValveProduct() {

}

void StringValveProduct::moveToPosition(int position) {
    *stream << "MOVE VALVE " << position << ";";
}

void StringValveProduct::closeValve() {
    *stream << "MOVE VALVE HOME";
}
