#include "stringvalveproduct.h"

StringValveProduct::StringValveProduct(const std::string & name, std::stringstream* stream) :
     ValvePluginProduct(), name(name)
{
   this->stream = stream;
}

StringValveProduct::~StringValveProduct() {

}

void StringValveProduct::moveToPosition(int position) {
    *stream << "MOVE VALVE " << name << " " << position;
}

void StringValveProduct::closeValve() {
    *stream << "MOVE VALVE " << name << " " << 0;
}
