#ifndef STRINGPLUGINFACTORY_H
#define STRINGPLUGINFACTORY_H

#include <sstream>

#include <commonmodel/plugininterface/pluginabstractfactory.h>

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

    inline virtual std::shared_ptr<HeaterPluginProduct> makeHeater(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<LightPluginProduct> makeLight(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }


    inline virtual std::shared_ptr<StirerPluginProduct> makeStirer(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<ShakePluginProduct> makeShaker(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<CentrifugatePluginProduct> makeCentrifugate(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }


    inline virtual std::shared_ptr<ElectrophoresisPluginProduct> makeElectrophorer(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }


    inline virtual std::shared_ptr<OdSensorPluginProduct> makeOdSensor(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<TemperatureSensorProduct> makeTemperatureSensor(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<LuminiscenceSensorProduct> makeLuminiscenseSensor(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<FluorescenceSensorProduct> makeFluorescenceSensor(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }

    inline virtual std::shared_ptr<VolumeSensorProduct> makeVolumeSensor(std::shared_ptr<PluginConfiguration> configuration) {
        return nullptr;
    }


    inline virtual std::shared_ptr<PumpPluginProduct> makePump(std::shared_ptr<PluginConfiguration> configuration) {
        return std::make_shared<StringPumpProduct>(configuration->getName(), stream);
    }

    inline virtual std::shared_ptr<ValvePluginProduct> makeValve(std::shared_ptr<PluginConfiguration> configuration) {
        return std::make_shared<StringValveProduct>(configuration->getName(), stream);
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
