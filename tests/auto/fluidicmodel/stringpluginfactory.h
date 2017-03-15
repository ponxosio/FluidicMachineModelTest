#ifndef STRINGPLUGINFACTORY_H
#define STRINGPLUGINFACTORY_H

#include <sstream>

#include <interfaces/model/plugininterface/pluginabstractfactory.h>

#include "stringpumpproduct.h"
#include "stringvalveproduct.h"

class StringPluginFactory : public PluginAbstractFactory
{
public:
    StringPluginFactory() {
        stream = new std::stringstream();
    }

    virtual ~StringPluginFactory() {
        delete stream;
    }

    inline virtual std::shared_ptr<HeaterPluginProduct> makeHeater(const PluginConfiguration & configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<LightPluginProduct> makeLight(const PluginConfiguration & configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<OdSensorPluginProduct> makeOdSensor(const PluginConfiguration & configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<PumpPluginProduct> makePump(const PluginConfiguration & configuration) {
        return std::make_shared<StringPumpProduct>(configuration.getName(), stream);
    }

    inline virtual std::shared_ptr<StirerPluginProduct> makeStirer(const PluginConfiguration & configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<ValvePluginProduct> makeValve(const PluginConfiguration & configuration) {
        return std::make_shared<StringValveProduct>(configuration.getName(), stream);
    }

    inline std::string getCommandsSent() {
        std::string command = stream->str();

        stream->str("");
        stream->clear();
        return command;
    }

 protected:
    std::stringstream* stream;
};

#endif // STRINGPLUGINFACTORY_H
