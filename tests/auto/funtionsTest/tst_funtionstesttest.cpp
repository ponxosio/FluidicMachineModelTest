#include <QString>
#include <QtTest>

#include <commonmodel/functions/function.h>
#include <commonmodel/functions/pumppluginfunction.h>
#include <commonmodel/functions/valvepluginroutefunction.h>
#include <commonmodel/functions/electrophoresisfunction.h>
#include <commonmodel/functions/lightfunction.h>
#include <commonmodel/functions/heatfunction.h>
#include <commonmodel/functions/measurefluorescencefunction.h>
#include <commonmodel/functions/measureodfunction.h>
#include <commonmodel/functions/measureluminiscencefunction.h>
#include <commonmodel/functions/measurevolumefunction.h>
#include <commonmodel/functions/measuretemperaturefunction.h>
#include <commonmodel/functions/stirfunction.h>
#include <commonmodel/functions/shakefunction.h>
#include <commonmodel/functions/centrifugatefunction.h>

#include<commonmodel/plugininterface/pluginabstractfactory.h>

#include <utils/utilsjson.h>

class FuntionsTestTest : public QObject
{
    Q_OBJECT

public:
    FuntionsTestTest();

private Q_SLOTS:
    void makeFunctions();
};

FuntionsTestTest::FuntionsTestTest()
{
}

void FuntionsTestTest::makeFunctions()
{
    /*PluginConfiguration configuration("p1", "electrophoresis", std::unordered_map<std::string,std::string>());
    ElectrophoresisWorkingRange range();

    UtilsJSON::checkPropertiesExists(std::vector<std::string>{"minVolume", "minVolumeUnits"}, functionObj);

    std::string minVolumeStr = functionObj["minVolume"];
    double minVolumeValue = std::atof(minVolumeStr.c_str());
    units::Volume minVolume = minVolumeValue * UtilsJSON::getVolumeUnits(functionObj["minVolumeUnits"]);

    return std::make_shared<ElectrophoresisFunction>(std::shared_ptr<PluginAbstractFactory>(),configuration, minVolume, range);*/
}

QTEST_APPLESS_MAIN(FuntionsTestTest)

#include "tst_funtionstesttest.moc"
