#include <unordered_map>

#include <QString>
#include <QtTest>

#include <fluidicmachinemodel/fluidicmachinemodel.h>
#include <fluidicmachinemodel/machine_graph_utils/graphrulesgenerator.h>
#include <fluidicmachinemodel/fluidicnode/valvenode.h>

#include <commonmodel/functions/function.h>
#include <commonmodel/functions/pumppluginfunction.h>
#include <commonmodel/functions/valvepluginroutefunction.h>
#include <commonmodel/plugininterface/pluginconfiguration.h>

#include <constraintengine/prologtranslationstack.h>

#include <pythonPlugins/actuators/pythonpluginabstractfactory.h>
#include <pythonPlugins/environment/PluginFileLoader.h>
#include <pythonPlugins/environment/PythonEnvironment.h>

#include "stringpluginfactory.h"
#include "stringtranslationstack.h"

class TwinValvesTest : public QObject
{
    Q_OBJECT

    typedef std::vector<std::unordered_set<int>> TL;

public:
    TwinValvesTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testRulesString();
    void testEvoprogRulesString();

    void testTwinValvesRouting();
    void test3TwinsValvesRouting();
    void testEvoprogTwinRouting();
    void testEvoprogTwinNoCleaningRouting();

private:
    std::shared_ptr<MachineGraph> makeTwinMachine(std::unordered_map<std::string, int> & nodes, std::shared_ptr<PluginAbstractFactory> factory);
    std::shared_ptr<MachineGraph> make3TwinsMachine(std::unordered_map<std::string, int> & nodes, std::shared_ptr<PluginAbstractFactory> factory);
    std::shared_ptr<MachineGraph> makeEvoprogTwinMachine(std::unordered_map<std::string, int> & nodes, std::shared_ptr<PluginAbstractFactory> factory);
    std::shared_ptr<MachineGraph> makeEvoprogTwinMachineNoCleaning(std::unordered_map<std::string, int> & nodes, std::shared_ptr<PluginAbstractFactory> factory);
};

TwinValvesTest::TwinValvesTest()
{
}

void TwinValvesTest::testRulesString()
{
    try {
        std::shared_ptr<PluginAbstractFactory> factory = NULL;

        std::unordered_map<std::string, int> nodesMap;
        std::shared_ptr<MachineGraph> machine = makeTwinMachine(nodesMap, factory);

        GraphRulesGenerator generator(machine, 3, 2);
        std::shared_ptr<Rule> rule = generator.getRules().back();

        StringTranslationStack translate;
        rule->fillTranslationStack(&translate);

        translate.addHeadToRestrictions();
        const std::string & strRule = *translate.getTranslatedRestriction().begin();

        qDebug() << "restrictions";
        qDebug() << strRule.c_str();

        std::string ruleToCheck = "((V_7==V_8)";
        std::string chunkGenerated = strRule.substr(0, ruleToCheck.length());

        QVERIFY2(ruleToCheck.compare(chunkGenerated) == 0, std::string("generated rule does not start with: " + ruleToCheck).c_str());
    } catch (std::exception & e) {
        QFAIL(e.what());
    }
}

void TwinValvesTest::testEvoprogRulesString() {
    try {
        std::shared_ptr<PluginAbstractFactory> factory = NULL;

        std::unordered_map<std::string, int> nodesMap;
        std::shared_ptr<MachineGraph> machine = makeEvoprogTwinMachine(nodesMap, factory);

        GraphRulesGenerator generator(machine, 3, 2);
        std::shared_ptr<Rule> rule = generator.getRules().back();

        StringTranslationStack translate;
        rule->fillTranslationStack(&translate);

        translate.addHeadToRestrictions();
        const std::string & strRule = *translate.getTranslatedRestriction().begin();

        qDebug() << "restrictions";
        qDebug() << strRule.c_str();

        std::string ruleToCheck = "((V_7==V_8)";
        std::string chunkGenerated = strRule.substr(0, ruleToCheck.length());

        QVERIFY2(ruleToCheck.compare(chunkGenerated) == 0, std::string("generated rule does not start with: " + ruleToCheck).c_str());
    } catch (std::exception & e) {
        QFAIL(e.what());
    }
}

void TwinValvesTest::testTwinValvesRouting()
{
    try {
        std::shared_ptr<StringPluginFactory> strFactory = std::make_shared<StringPluginFactory>();

        std::unordered_map<std::string, int> containers;
        std::shared_ptr<MachineGraph> graph = makeTwinMachine(containers, strFactory);

        std::shared_ptr<TranslationStack> prologTStack = std::make_shared<PrologTranslationStack>();
        FluidicMachineModel model(graph, prologTStack, 3, 2, 300);

        model.setContinuousFlow(containers["c1"], containers["c4"], 300 * units::ml/units::hr);
        model.processFlows({});

        std::string expected = "SET PUMP P1: dir 1, rate 300ml/hSET PUMP P2: dir 0, rate 0ml/hMOVE VALVE V1 3MOVE VALVE V2 3";
        std::string generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow(containers["c1"], containers["c4"]);
        model.setContinuousFlow(containers["c5"], containers["c3"], 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "SET PUMP P1: dir 0, rate 0ml/hSET PUMP P2: dir 1, rate 300ml/hMOVE VALVE V1 1MOVE VALVE V2 1";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow(containers["c5"], containers["c3"]);
        model.setContinuousFlow(containers["c5"], containers["c4"], 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "SET PUMP P1: dir 0, rate 0ml/hSET PUMP P2: dir 1, rate 300ml/hMOVE VALVE V1 2MOVE VALVE V2 2";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        qDebug() << "impossible flow";

        model.stopContinuousFlow(containers["c5"], containers["c4"]);
        model.setContinuousFlow(containers["c1"], containers["c3"], 300 * units::ml/units::hr);

        try {
            model.processFlows({});
            QFAIL("flow c1->c3 need to fail!");

        } catch (std::runtime_error & ignored) {
            QVERIFY(true);
        } catch (std::exception & ignored) {
            QFAIL("Not expected execption thrown!");
        }

    } catch (std::exception & e) {
        QFAIL(e.what());
    }
}

void TwinValvesTest::test3TwinsValvesRouting()
{
    try {
        std::shared_ptr<StringPluginFactory> strFactory = std::make_shared<StringPluginFactory>();

        std::unordered_map<std::string, int> containers;
        std::shared_ptr<MachineGraph> graph = make3TwinsMachine(containers, strFactory);

        qDebug() << graph->toString().c_str();

        qDebug() << containers["mediaA"] << "mediaA";
        qDebug() << containers["mediaB"] << "mediaB";

        qDebug() << containers["wasteA"] << "wasteA";
        qDebug() << containers["wasteB"] << "wasteB";
        qDebug() << containers["wasteC"] << "wasteC";
        qDebug() << containers["store"] << "store";

        qDebug() << containers["cellstat"] << "cellstat";

        qDebug() << containers["pA"] << "pA";
        qDebug() << containers["pB"] << "pB";

        qDebug() << containers["vC_A"] << "vC_A";
        qDebug() << containers["vC_B"] << "vC_B";
        qDebug() << containers["vC_C"] << "vC_C";

        std::shared_ptr<TranslationStack> prologTStack = std::make_shared<PrologTranslationStack>();
        FluidicMachineModel model(graph, prologTStack, 3, 0, 300);

        model.processFlows({});

        std::string expected = "";
        std::string generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        int c1 = containers["mediaB"];
        int c2 = containers["cellstat"];
        int c3 = containers["store"];

        qDebug() << c1 << "," << c2 << "," << c3;

        model.setContinuousFlow({c1, c2, c3}, 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        //QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        /*model.stopContinuousFlow(c1, c4);

        c1 = containers["mediaA"];
        c2 = containers["chemoA"];
        c3 = containers["cellstat"];
        c4 = containers["wasteC"];

        qDebug() << c1 << "," << c2 << "," << c3 << "," << c4;

        model.setContinuousFlow({c1, c2, c3, c4}, 300 * units::ml/units::hr);
        model.processFlows();

        expected = "SET PUMP P1: dir 0, rate 0ml/hSET PUMP P2: dir 1, rate 300ml/hMOVE VALVE V1 1MOVE VALVE V2 1";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();*/

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");
    } catch (std::exception & e) {
        QFAIL(e.what());
    }
}

void TwinValvesTest::testEvoprogTwinRouting() {
    try {
        std::shared_ptr<StringPluginFactory> strFactory = std::make_shared<StringPluginFactory>();

        std::unordered_map<std::string, int> containers;
        std::shared_ptr<MachineGraph> graph = makeEvoprogTwinMachine(containers, strFactory);

        qDebug() << graph->toString().c_str();

        qDebug() << containers["mediaA"] << "mediaA";
        qDebug() << containers["mediaB"] << "mediaB";

        qDebug() << containers["water"] << "water";
        qDebug() << containers["EtOH"] << "EtOH";
        qDebug() << containers["NaOH"] << "NaOH";
        qDebug() << containers["air"] << "air";

        qDebug() << containers["wasteA"] << "wasteA";
        qDebug() << containers["wasteB"] << "wasteB";
        qDebug() << containers["wasteC"] << "wasteC";

        qDebug() << containers["wasteClean"] << "wasteClean";

        qDebug() << containers["chemoA"] << "chemoA";
        qDebug() << containers["chemoB"] << "chemoB";
        qDebug() << containers["cellstat"] << "cellstat";

        qDebug() << containers["pA"] << "pA";
        qDebug() << containers["pB"] << "pB";

        qDebug() << containers["pWater"] << "pWater";
        qDebug() << containers["pNaOH"] << "pNaOH";
        qDebug() << containers["pEtOH"] << "pEtOH";
        qDebug() << containers["pAir"] << "pAir";

        qDebug() << containers["vA_A"] << "vA_A";
        qDebug() << containers["vA_B"] << "vA_B";

        qDebug() << containers["vB_A"] << "vB_A";
        qDebug() << containers["vB_B"] << "vB_B";

        qDebug() << containers["vC_A"] << "vC_A";
        qDebug() << containers["vC_B"] << "vC_B";
        qDebug() << containers["vC_C"] << "vC_C";

        qDebug() << containers["vClean"] << "vClean";

        std::shared_ptr<TranslationStack> prologTStack = std::make_shared<PrologTranslationStack>();
        FluidicMachineModel model(graph, prologTStack, 4, 1, 300);

        qDebug() << "stop";
        model.processFlows({});

        std::string expected = "SET PUMP PA: dir 0, rate 0ml/hSET PUMP PB: dir 0, rate 0ml/hSET PUMP P_Water: dir 0, rate 0ml/hSET PUMP P_ETOH: dir 0, rate 0ml/hSET PUMP P_NaOH: dir 0, rate 0ml/hSET PUMP P_Air: dir 0, rate 0ml/hMOVE VALVE VA_B 0MOVE VALVE VA_A 0MOVE VALVE VB_A 0MOVE VALVE VB_B 0MOVE VALVE VC_A 0MOVE VALVE VC_B 0MOVE VALVE VC_C 0MOVE VALVE V_CLEAN 1";
        std::string generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        qDebug() << "mediaA->chemoA->wasteA";

        int c1 = containers["mediaA"];
        int c2 = containers["chemoA"];
        int c3 = containers["wasteA"];

        model.setContinuousFlow({c1, c2, c3}, 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "SET PUMP PA: dir 1, rate 300ml/hSET PUMP PB: dir 0, rate 0ml/hSET PUMP P_Water: dir 0, rate 0ml/hSET PUMP P_ETOH: dir 0, rate 0ml/hSET PUMP P_NaOH: dir 0, rate 0ml/hSET PUMP P_Air: dir 0, rate 0ml/hMOVE VALVE VA_B 3MOVE VALVE VA_A 3MOVE VALVE VB_A 0MOVE VALVE VB_B 0MOVE VALVE VC_A 2MOVE VALVE VC_B 2MOVE VALVE VC_C 2MOVE VALVE V_CLEAN 1";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow(containers["mediaA"], containers["wasteA"]);

        qDebug() << "mediaB->chemoB->wasteB";

        c1 = containers["mediaB"];
        c2 = containers["chemoB"];
        c3 = containers["wasteB"];

        model.setContinuousFlow({c1, c2, c3}, 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "SET PUMP PA: dir 0, rate 0ml/hSET PUMP PB: dir 1, rate 300ml/hSET PUMP P_Water: dir 0, rate 0ml/hSET PUMP P_ETOH: dir 0, rate 0ml/hSET PUMP P_NaOH: dir 0, rate 0ml/hSET PUMP P_Air: dir 0, rate 0ml/hMOVE VALVE VA_B 0MOVE VALVE VA_A 0MOVE VALVE VB_A 3MOVE VALVE VB_B 3MOVE VALVE VC_A 1MOVE VALVE VC_B 1MOVE VALVE VC_C 1MOVE VALVE V_CLEAN 1";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        qDebug() << "mediaB->chemoB->wasteB";
        qDebug() << "mediaA->chemoA->cellstat->wasteC";

        c1 = containers["mediaA"];
        c2 = containers["chemoA"];
        c3 = containers["cellstat"];
        int c4 = containers["wasteC"];

        model.setContinuousFlow({c1, c2, c3, c4}, 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "SET PUMP PA: dir 1, rate 300ml/hSET PUMP PB: dir 1, rate 300ml/hSET PUMP P_Water: dir 0, rate 0ml/hSET PUMP P_ETOH: dir 0, rate 0ml/hSET PUMP P_NaOH: dir 0, rate 0ml/hSET PUMP P_Air: dir 0, rate 0ml/hMOVE VALVE VA_B 3MOVE VALVE VA_A 3MOVE VALVE VB_A 3MOVE VALVE VB_B 3MOVE VALVE VC_A 1MOVE VALVE VC_B 1MOVE VALVE VC_C 1MOVE VALVE V_CLEAN 1";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow(containers["mediaA"], containers["wasteC"]);
        model.stopContinuousFlow(containers["mediaB"], containers["wasteB"]);

        qDebug() << "mediaB->chemoB->wasteB";
        qDebug() << "mediaA->chemoA->wasteA";

        c1 = containers["mediaA"];
        c2 = containers["chemoA"];
        c3 = containers["wasteA"];

        model.setContinuousFlow({c1, c2, c3}, 300 * units::ml/units::hr);

        c1 = containers["mediaB"];
        c2 = containers["chemoB"];
        c3 = containers["wasteB"];

        model.setContinuousFlow({c1, c2, c3}, 300 * units::ml/units::hr);

        model.processFlows({});

        expected = "SET PUMP PA: dir 1, rate 300ml/hSET PUMP PB: dir 1, rate 300ml/hSET PUMP P_Water: dir 0, rate 0ml/hSET PUMP P_ETOH: dir 0, rate 0ml/hSET PUMP P_NaOH: dir 0, rate 0ml/hSET PUMP P_Air: dir 0, rate 0ml/hMOVE VALVE VA_B 3MOVE VALVE VA_A 3MOVE VALVE VB_A 3MOVE VALVE VB_B 3MOVE VALVE VC_A 4MOVE VALVE VC_B 4MOVE VALVE VC_C 4MOVE VALVE V_CLEAN 1";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow(containers["mediaA"], containers["wasteA"]);
        model.stopContinuousFlow(containers["mediaB"], containers["wasteB"]);

        qDebug() << "water->chemoA->wasteClean";

        c1 = containers["water"];
        c2 = containers["chemoA"];
        c3 = containers["wasteClean"];

        model.setContinuousFlow({c1, c2, c3}, 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "SET PUMP PA: dir 0, rate 0ml/hSET PUMP PB: dir 0, rate 0ml/hSET PUMP P_Water: dir 1, rate 300ml/hSET PUMP P_ETOH: dir 0, rate 0ml/hSET PUMP P_NaOH: dir 0, rate 0ml/hSET PUMP P_Air: dir 0, rate 0ml/hMOVE VALVE VA_B 2MOVE VALVE VA_A 2MOVE VALVE VB_A 0MOVE VALVE VB_B 0MOVE VALVE VC_A 0MOVE VALVE VC_B 0MOVE VALVE VC_C 0MOVE VALVE V_CLEAN 1";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow(containers["water"], containers["wasteClean"]);

        qDebug() << "NaOH->chemoB->wasteClean";

        c1 = containers["NaOH"];
        c2 = containers["chemoB"];
        c3 = containers["wasteClean"];

        model.setContinuousFlow({c1, c2, c3}, 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "SET PUMP PA: dir 0, rate 0ml/hSET PUMP PB: dir 0, rate 0ml/hSET PUMP P_Water: dir 0, rate 0ml/hSET PUMP P_ETOH: dir 0, rate 0ml/hSET PUMP P_NaOH: dir 1, rate 300ml/hSET PUMP P_Air: dir 0, rate 0ml/hMOVE VALVE VA_B 0MOVE VALVE VA_A 0MOVE VALVE VB_A 2MOVE VALVE VB_B 2MOVE VALVE VC_A 0MOVE VALVE VC_B 0MOVE VALVE VC_C 0MOVE VALVE V_CLEAN 3";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow({containers["NaOH"], containers["chemoB"], containers["wasteClean"]});

        qDebug() << "EtOH->chemoB->cellstat->wasteClean";

        c1 = containers["NaOH"];
        c2 = containers["chemoB"];
        c3 = containers["cellstat"];
        c4 = containers["wasteClean"];

        model.setContinuousFlow({c1, c2, c3, c4}, 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "SET PUMP PA: dir 0, rate 0ml/hSET PUMP PB: dir 0, rate 0ml/hSET PUMP P_Water: dir 0, rate 0ml/hSET PUMP P_ETOH: dir 0, rate 0ml/hSET PUMP P_NaOH: dir 1, rate 300ml/hSET PUMP P_Air: dir 0, rate 0ml/hMOVE VALVE VA_B 0MOVE VALVE VA_A 0MOVE VALVE VB_A 1MOVE VALVE VB_B 1MOVE VALVE VC_A 3MOVE VALVE VC_B 3MOVE VALVE VC_C 3MOVE VALVE V_CLEAN 3";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow({containers["NaOH"], containers["chemoB"], containers["cellstat"], containers["wasteClean"]});

        qDebug() << "NaOH->chemoA->cellstat->wasteClean";

        c1 = containers["NaOH"];
        c2 = containers["chemoA"];
        c3 = containers["cellstat"];
        c4 = containers["wasteClean"];

        model.setContinuousFlow({c1, c2, c3, c4}, 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "SET PUMP PA: dir 0, rate 0ml/hSET PUMP PB: dir 0, rate 0ml/hSET PUMP P_Water: dir 0, rate 0ml/hSET PUMP P_ETOH: dir 0, rate 0ml/hSET PUMP P_NaOH: dir 1, rate 300ml/hSET PUMP P_Air: dir 0, rate 0ml/hMOVE VALVE VA_B 1MOVE VALVE VA_A 1MOVE VALVE VB_A 1MOVE VALVE VB_B 1MOVE VALVE VC_A 3MOVE VALVE VC_B 3MOVE VALVE VC_C 3MOVE VALVE V_CLEAN 3";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow({containers["NaOH"], containers["chemoA"], containers["cellstat"], containers["wasteClean"]});
    } catch (std::exception & e) {
        QFAIL(e.what());
    }
}

void TwinValvesTest::testEvoprogTwinNoCleaningRouting() {
    try {
        std::shared_ptr<StringPluginFactory> strFactory = std::make_shared<StringPluginFactory>();

        std::unordered_map<std::string, int> containers;
        std::shared_ptr<MachineGraph> graph = makeEvoprogTwinMachineNoCleaning(containers, strFactory);

        qDebug() << graph->toString().c_str();

        qDebug() << containers["mediaA"] << "mediaA";
        qDebug() << containers["mediaB"] << "mediaB";

        qDebug() << containers["d1"] << "d1";
        qDebug() << containers["d2"] << "d2";

        qDebug() << containers["wasteA"] << "wasteA";
        qDebug() << containers["wasteB"] << "wasteB";
        qDebug() << containers["wasteC"] << "wasteC";

        qDebug() << containers["wasteClean"] << "wasteClean";

        qDebug() << containers["chemoA"] << "chemoA";
        qDebug() << containers["chemoB"] << "chemoB";
        qDebug() << containers["cellstat"] << "cellstat";

        qDebug() << containers["pA"] << "pA";
        qDebug() << containers["pB"] << "pB";

        qDebug() << containers["p1"] << "p1";
        qDebug() << containers["p2"] << "p2";

        qDebug() << containers["vA_A"] << "vA_A";
        qDebug() << containers["vA_B"] << "vA_B";

        qDebug() << containers["vB_A"] << "vB_A";
        qDebug() << containers["vB_B"] << "vB_B";

        qDebug() << containers["vC_A"] << "vC_A";
        qDebug() << containers["vC_B"] << "vC_B";
        qDebug() << containers["vC_C"] << "vC_C";

        std::shared_ptr<TranslationStack> prologTStack = std::make_shared<PrologTranslationStack>();
        FluidicMachineModel model(graph, prologTStack, 3, 0, 300);

        model.processFlows({});

        std::string expected = "";
        std::string generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        int c1 = containers["mediaB"];
        int c2 = containers["chemoB"];
        int c3 = containers["cellstat"];
        int c4 = containers["wasteC"];

        qDebug() << c1 << "," << c2 << "," << c3 << "," << c4;

        model.setContinuousFlow({c1, c2, c3, c4}, 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        //QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow(c1, c4);

        c1 = containers["mediaA"];
        c2 = containers["chemoA"];
        c3 = containers["cellstat"];
        c4 = containers["wasteC"];

        qDebug() << c1 << "," << c2 << "," << c3 << "," << c4;

        model.setContinuousFlow({c1, c2, c3, c4}, 300 * units::ml/units::hr);
        model.processFlows({});

        expected = "SET PUMP P1: dir 0, rate 0ml/hSET PUMP P2: dir 1, rate 300ml/hMOVE VALVE V1 1MOVE VALVE V2 1";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");
    } catch (std::exception & e) {
        QFAIL(e.what());
    }
}

std::shared_ptr<MachineGraph> TwinValvesTest::makeTwinMachine(std::unordered_map<std::string, int> & nodesMap, std::shared_ptr<PluginAbstractFactory> factory) {
    std::shared_ptr<MachineGraph> mGraph = std::make_shared<MachineGraph>();

    PluginConfiguration config_p1;
    config_p1.setName("P1");
    std::shared_ptr<Function> pumpf1 = std::make_shared<PumpPluginFunction>(factory, config_p1, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_p2;
    config_p2.setName("P2");
    std::shared_ptr<Function> pumpf2 = std::make_shared<PumpPluginFunction>(factory, config_p2, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_v1;
    config_v1.setName("V1");
    std::shared_ptr<Function> route_v1 = std::make_shared<ValvePluginRouteFunction>(factory, config_v1);

    PluginConfiguration config_v2;
    config_v2.setName("V2");
    std::shared_ptr<Function> route_v2 = std::make_shared<ValvePluginRouteFunction>(factory, config_v2);

    int c1 = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int c3 = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int c4 = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int c5 = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);

    int c2 = mGraph->emplaceContainer(2, ContainerNode::close, 100.0);

    int p1 = mGraph->emplacePump(2, PumpNode::unidirectional, pumpf1);
    int p2 = mGraph->emplacePump(2, PumpNode::unidirectional, pumpf2);

    TL empty;

    ValveNode::TruthTable tableType1;
    tableType1.insert(std::make_pair(0, empty));
    tableType1.insert(std::make_pair(1, TL({{1,2}})));
    tableType1.insert(std::make_pair(2, TL({{1,2}})));
    tableType1.insert(std::make_pair(3, TL({{0,2}})));

    ValveNode::TruthTable tableType2;
    tableType2.insert(std::make_pair(0, empty));
    tableType2.insert(std::make_pair(1, TL({{0,1}})));
    tableType2.insert(std::make_pair(2, TL({{0,2}})));
    tableType2.insert(std::make_pair(3, TL({{0,2}})));

    int v1 = mGraph->emplaceValve(3, tableType1, route_v1);
    int v2 = mGraph->emplaceValve(3, tableType2, route_v2);

    nodesMap["c1"] = c1;
    nodesMap["c3"] = c3;
    nodesMap["c4"] = c4;
    nodesMap["c5"] = c5;

    nodesMap["c2"] = c2;

    nodesMap["p1"] = p1;
    nodesMap["p2"] = p2;

    nodesMap["v1"] = v1;
    nodesMap["v2"] = v2;

    mGraph->connectNodes(c1,p1,0,0);
    mGraph->connectNodes(c5,p2,0,0);

    mGraph->connectNodes(p1,v1,1,0);
    mGraph->connectNodes(p2,v1,1,1);

    mGraph->connectNodes(v1,c2,2,0);
    mGraph->connectNodes(c2,v2,1,0);

    mGraph->connectNodes(v2,c3,1,0);
    mGraph->connectNodes(v2,c4,2,0);

    mGraph->setValvesAsTwins({v1, v2});

    return mGraph;
}

std::shared_ptr<MachineGraph> TwinValvesTest::make3TwinsMachine(std::unordered_map<std::string, int> & nodesMap, std::shared_ptr<PluginAbstractFactory> factory) {
    std::shared_ptr<MachineGraph> mGraph = std::make_shared<MachineGraph>();

    PluginConfiguration config_pA;
    config_pA.setName("PA");
    std::shared_ptr<Function> pumpfA = std::make_shared<PumpPluginFunction>(factory, config_pA, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_pB;
    config_pB.setName("PB");
    std::shared_ptr<Function> pumpfB = std::make_shared<PumpPluginFunction>(factory, config_pB, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_vC_A;
    config_vC_A.setName("VC_A");
    std::shared_ptr<Function> route_vC_A = std::make_shared<ValvePluginRouteFunction>(factory, config_vC_A);

    PluginConfiguration config_vC_B;
    config_vC_B.setName("VC_B");
    std::shared_ptr<Function> route_vC_B = std::make_shared<ValvePluginRouteFunction>(factory, config_vC_B);

    PluginConfiguration config_vC_C;
    config_vC_C.setName("VC_C");
    std::shared_ptr<Function> route_vC_C = std::make_shared<ValvePluginRouteFunction>(factory, config_vC_C);

    int mediaA = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int mediaB = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);

    int wasteA = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int wasteB = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int wasteC = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int store = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);

    int cellstat = mGraph->emplaceContainer(3, ContainerNode::close, 100.0);

    int pA = mGraph->emplacePump(2, PumpNode::unidirectional, pumpfA);
    int pB = mGraph->emplacePump(2, PumpNode::unidirectional, pumpfB);

    TL empty;

    ValveNode::TruthTable tableType1_A;
    tableType1_A.insert(std::make_pair(0, empty));
    tableType1_A.insert(std::make_pair(1, TL({{0,1}})));
    tableType1_A.insert(std::make_pair(2, TL({{0,2}})));

    int vC_A = mGraph->emplaceValve(3, tableType1_A, route_vC_A);
    int vC_B = mGraph->emplaceValve(3, tableType1_A, route_vC_B);
    int vC_C = mGraph->emplaceValve(3, tableType1_A, route_vC_C);

    nodesMap["mediaA"] = mediaA;
    nodesMap["mediaB"] = mediaB;

    nodesMap["wasteA"] = wasteA;
    nodesMap["wasteB"] = wasteB;
    nodesMap["wasteC"] = wasteC;
    nodesMap["store"] = store;

    nodesMap["cellstat"] = cellstat;

    nodesMap["pA"] = pA;
    nodesMap["pB"] = pB;

    nodesMap["vC_A"] = vC_A;
    nodesMap["vC_B"] = vC_B;
    nodesMap["vC_C"] = vC_C;

    mGraph->connectNodes(mediaA,pA,0,0);
    mGraph->connectNodes(mediaB,pB,0,0);

    mGraph->connectNodes(pA,vC_A,1,0);
    mGraph->connectNodes(pB,vC_B,1,0);

    mGraph->connectNodes(vC_A,wasteA,1,0);
    mGraph->connectNodes(vC_A,cellstat,2,0);

    mGraph->connectNodes(vC_B,wasteB,1,0);
    mGraph->connectNodes(vC_B,cellstat,2,1);

    mGraph->connectNodes(cellstat,vC_C,2,0);

    mGraph->connectNodes(vC_C,wasteC,1,0);
    mGraph->connectNodes(vC_C,store,2,0);

    //mGraph->setValvesAsTwins({vC_B, vC_C, vC_A});

    return mGraph;

}

std::shared_ptr<MachineGraph> TwinValvesTest::makeEvoprogTwinMachine(std::unordered_map<std::string, int> & nodesMap, std::shared_ptr<PluginAbstractFactory> factory) {
    std::shared_ptr<MachineGraph> mGraph = std::make_shared<MachineGraph>();

    PluginConfiguration config_pA;
    config_pA.setName("PA");
    std::shared_ptr<Function> pumpfA = std::make_shared<PumpPluginFunction>(factory, config_pA, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_pB;
    config_pB.setName("PB");
    std::shared_ptr<Function> pumpfB = std::make_shared<PumpPluginFunction>(factory, config_pB, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_pW;
    config_pW.setName("P_Water");
    std::shared_ptr<Function> pumpf_w = std::make_shared<PumpPluginFunction>(factory, config_pW, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_pEtOH;
    config_pEtOH.setName("P_ETOH");
    std::shared_ptr<Function> pumpf_ETOH = std::make_shared<PumpPluginFunction>(factory, config_pEtOH, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_pNAOH;
    config_pNAOH.setName("P_NaOH");
    std::shared_ptr<Function> pumpf_NAOH = std::make_shared<PumpPluginFunction>(factory, config_pNAOH, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_pAir;
    config_pAir.setName("P_Air");
    std::shared_ptr<Function> pumpf_Air = std::make_shared<PumpPluginFunction>(factory, config_pAir, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_vA_A;
    config_vA_A.setName("VA_A");
    std::shared_ptr<Function> route_vA_A = std::make_shared<ValvePluginRouteFunction>(factory, config_vA_A);

    PluginConfiguration config_vA_B;
    config_vA_B.setName("VA_B");
    std::shared_ptr<Function> route_vA_B = std::make_shared<ValvePluginRouteFunction>(factory, config_vA_B);

    PluginConfiguration config_vB_A;
    config_vB_A.setName("VB_A");
    std::shared_ptr<Function> route_vB_A = std::make_shared<ValvePluginRouteFunction>(factory, config_vB_A);

    PluginConfiguration config_vB_B;
    config_vB_B.setName("VB_B");
    std::shared_ptr<Function> route_vB_B = std::make_shared<ValvePluginRouteFunction>(factory, config_vB_B);

    PluginConfiguration config_vC_A;
    config_vC_A.setName("VC_A");
    std::shared_ptr<Function> route_vC_A = std::make_shared<ValvePluginRouteFunction>(factory, config_vC_A);

    PluginConfiguration config_vC_B;
    config_vC_B.setName("VC_B");
    std::shared_ptr<Function> route_vC_B = std::make_shared<ValvePluginRouteFunction>(factory, config_vC_B);

    PluginConfiguration config_vC_C;
    config_vC_C.setName("VC_C");
    std::shared_ptr<Function> route_vC_C = std::make_shared<ValvePluginRouteFunction>(factory, config_vC_C);

    PluginConfiguration config_vClean;
    config_vClean.setName("V_CLEAN");
    std::shared_ptr<Function> route_vClean = std::make_shared<ValvePluginRouteFunction>(factory, config_vClean);

    int mediaA = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int mediaB = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);

    int wasteA = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int wasteB = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int wasteC = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int wasteClean = mGraph->emplaceContainer(3, ContainerNode::open, 100.0);

    int water = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int NaOH = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int EtOH = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int Air = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);

    int chemoA = mGraph->emplaceContainer(2, ContainerNode::close, 100.0);
    int chemoB = mGraph->emplaceContainer(2, ContainerNode::close, 100.0);
    int cellstat = mGraph->emplaceContainer(3, ContainerNode::close, 100.0);

    int pA = mGraph->emplacePump(2, PumpNode::unidirectional, pumpfA);
    int pB = mGraph->emplacePump(2, PumpNode::unidirectional, pumpfB);
    int pWater = mGraph->emplacePump(2, PumpNode::unidirectional, pumpf_w);
    int pEtOH = mGraph->emplacePump(2, PumpNode::unidirectional, pumpf_ETOH);
    int pNaOH = mGraph->emplacePump(2, PumpNode::unidirectional, pumpf_NAOH);
    int pAir = mGraph->emplacePump(2, PumpNode::unidirectional, pumpf_Air);

    TL empty;

    ValveNode::TruthTable tableType1_A;
    tableType1_A.insert(std::make_pair(0, empty));
    tableType1_A.insert(std::make_pair(1, TL({{1,2}})));
    tableType1_A.insert(std::make_pair(2, TL({{1,2}})));
    tableType1_A.insert(std::make_pair(3, TL({{0,2}})));

    ValveNode::TruthTable tableType1_B;
    tableType1_B.insert(std::make_pair(0, empty));
    tableType1_B.insert(std::make_pair(1, TL({{0,2}})));
    tableType1_B.insert(std::make_pair(2, TL({{0,1}})));
    tableType1_B.insert(std::make_pair(3, TL({{0,2}})));

    ValveNode::TruthTable tableType2_A;
    tableType2_A.insert(std::make_pair(0, empty));
    tableType2_A.insert(std::make_pair(1, TL({{0,2}})));
    tableType2_A.insert(std::make_pair(2, TL({{0,1}})));
    tableType2_A.insert(std::make_pair(3, TL({{0,1}})));
    tableType2_A.insert(std::make_pair(4, TL({{0,1}})));

    ValveNode::TruthTable tableType2_B;
    tableType2_B.insert(std::make_pair(0, empty));
    tableType2_B.insert(std::make_pair(1, TL({{0,1}})));
    tableType2_B.insert(std::make_pair(2, TL({{0,2}})));
    tableType2_B.insert(std::make_pair(3, TL({{0,2}})));
    tableType2_B.insert(std::make_pair(4, TL({{0,1}})));

    ValveNode::TruthTable tableType2_C;
    tableType2_C.insert(std::make_pair(0, empty));
    tableType2_C.insert(std::make_pair(1, TL({{0,1}})));
    tableType2_C.insert(std::make_pair(2, TL({{0,1}})));
    tableType2_C.insert(std::make_pair(3, TL({{0,2}})));
    tableType2_C.insert(std::make_pair(4, TL({{0,1}})));

    ValveNode::TruthTable tableType3;
    tableType3.insert(std::make_pair(1, TL({{0,4,5}})));
    tableType3.insert(std::make_pair(2, TL({{1,4,5}})));
    tableType3.insert(std::make_pair(3, TL({{2,4,5}})));
    tableType3.insert(std::make_pair(4, TL({{3,4,5}})));

    int vA_A = mGraph->emplaceValve(3, tableType1_A, route_vA_A);
    int vA_B = mGraph->emplaceValve(3, tableType1_B, route_vA_B);

    int vB_A = mGraph->emplaceValve(3, tableType1_A, route_vB_A);
    int vB_B = mGraph->emplaceValve(3, tableType1_B, route_vB_B);

    int vC_A = mGraph->emplaceValve(3, tableType2_A, route_vC_A);
    int vC_B = mGraph->emplaceValve(3, tableType2_B, route_vC_B);
    int vC_C = mGraph->emplaceValve(3, tableType2_C, route_vC_C);

    int vClean = mGraph->emplaceValve(6, tableType3, route_vClean);

    nodesMap["mediaA"] = mediaA;
    nodesMap["mediaB"] = mediaB;

    nodesMap["water"] = water;
    nodesMap["EtOH"] = EtOH;
    nodesMap["NaOH"] = NaOH;
    nodesMap["air"] = Air;

    nodesMap["wasteA"] = wasteA;
    nodesMap["wasteB"] = wasteB;
    nodesMap["wasteC"] = wasteC;

    nodesMap["wasteClean"] = wasteClean;

    nodesMap["chemoA"] = chemoA;
    nodesMap["chemoB"] = chemoB;
    nodesMap["cellstat"] = cellstat;

    nodesMap["pA"] = pA;
    nodesMap["pB"] = pB;

    nodesMap["pWater"] = pWater;
    nodesMap["pNaOH"] = pNaOH;
    nodesMap["pEtOH"] = pEtOH;
    nodesMap["pAir"] = pAir;

    nodesMap["vA_A"] = vA_A;
    nodesMap["vA_B"] = vA_B;

    nodesMap["vB_A"] = vB_A;
    nodesMap["vB_B"] = vB_B;

    nodesMap["vC_A"] = vC_A;
    nodesMap["vC_B"] = vC_B;
    nodesMap["vC_C"] = vC_C;

    nodesMap["vClean"] = vClean;

    mGraph->connectNodes(water,pWater,0,0);
    mGraph->connectNodes(EtOH,pEtOH,0,0);
    mGraph->connectNodes(NaOH,pNaOH,0,0);
    mGraph->connectNodes(Air,pAir,0,0);

    mGraph->connectNodes(pWater,vClean,1,0);
    mGraph->connectNodes(pEtOH,vClean,1,1);
    mGraph->connectNodes(pNaOH,vClean,1,2);
    mGraph->connectNodes(pAir,vClean,1,3);

    mGraph->connectNodes(vClean,vA_A,4,1);
    mGraph->connectNodes(vClean,vB_A,5,1);

    mGraph->connectNodes(mediaA,pA,0,0);
    mGraph->connectNodes(mediaB,pB,0,0);

    mGraph->connectNodes(pA,vA_A,1,0);
    mGraph->connectNodes(pB,vB_A,1,0);

    mGraph->connectNodes(vA_A,chemoA,2,0);
    mGraph->connectNodes(vB_A,chemoB,2,0);

    mGraph->connectNodes(chemoA,vA_B,1,0);
    mGraph->connectNodes(chemoB,vB_B,1,0);

    mGraph->connectNodes(vA_B,wasteClean,1,0);
    mGraph->connectNodes(vA_B,vC_A,2,0);
    mGraph->connectNodes(vB_B,wasteClean,1,1);
    mGraph->connectNodes(vB_B,vC_B,2,0);

    mGraph->connectNodes(vC_A,wasteA,1,0);
    mGraph->connectNodes(vC_A,cellstat,2,0);
    mGraph->connectNodes(vC_B,wasteB,1,0);
    mGraph->connectNodes(vC_B,cellstat,2,1);

    mGraph->connectNodes(cellstat,vC_C,2,0);

    mGraph->connectNodes(vC_C,wasteC,1,0);
    mGraph->connectNodes(vC_C,wasteClean,2,2);

    mGraph->setValvesAsTwins({vA_A, vA_B});
    mGraph->setValvesAsTwins({vB_A, vB_B});
    mGraph->setValvesAsTwins({vC_B, vC_C, vC_A});

    return mGraph;
}

std::shared_ptr<MachineGraph> TwinValvesTest::makeEvoprogTwinMachineNoCleaning(std::unordered_map<std::string, int> & nodesMap, std::shared_ptr<PluginAbstractFactory> factory) {
    std::shared_ptr<MachineGraph> mGraph = std::make_shared<MachineGraph>();

    PluginConfiguration config_pA;
    config_pA.setName("PA");
    std::shared_ptr<Function> pumpfA = std::make_shared<PumpPluginFunction>(factory, config_pA, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_pB;
    config_pB.setName("PB");
    std::shared_ptr<Function> pumpfB = std::make_shared<PumpPluginFunction>(factory, config_pB, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_p1;
    config_p1.setName("P1");
    std::shared_ptr<Function> pumpf_1 = std::make_shared<PumpPluginFunction>(factory, config_p1, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_p2;
    config_p2.setName("P2");
    std::shared_ptr<Function> pumpf_2 = std::make_shared<PumpPluginFunction>(factory, config_p2, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration config_vA_A;
    config_vA_A.setName("VA_A");
    std::shared_ptr<Function> route_vA_A = std::make_shared<ValvePluginRouteFunction>(factory, config_vA_A);

    PluginConfiguration config_vA_B;
    config_vA_B.setName("VA_B");
    std::shared_ptr<Function> route_vA_B = std::make_shared<ValvePluginRouteFunction>(factory, config_vA_B);

    PluginConfiguration config_vB_A;
    config_vB_A.setName("VB_A");
    std::shared_ptr<Function> route_vB_A = std::make_shared<ValvePluginRouteFunction>(factory, config_vB_A);

    PluginConfiguration config_vB_B;
    config_vB_B.setName("VB_B");
    std::shared_ptr<Function> route_vB_B = std::make_shared<ValvePluginRouteFunction>(factory, config_vB_B);

    PluginConfiguration config_vC_A;
    config_vC_A.setName("VC_A");
    std::shared_ptr<Function> route_vC_A = std::make_shared<ValvePluginRouteFunction>(factory, config_vC_A);

    PluginConfiguration config_vC_B;
    config_vC_B.setName("VC_B");
    std::shared_ptr<Function> route_vC_B = std::make_shared<ValvePluginRouteFunction>(factory, config_vC_B);

    PluginConfiguration config_vC_C;
    config_vC_C.setName("VC_C");
    std::shared_ptr<Function> route_vC_C = std::make_shared<ValvePluginRouteFunction>(factory, config_vC_C);

    PluginConfiguration config_vClean;
    config_vClean.setName("V_CLEAN");
    std::shared_ptr<Function> route_vClean = std::make_shared<ValvePluginRouteFunction>(factory, config_vClean);

    int mediaA = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int mediaB = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);

    int wasteA = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int wasteB = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int wasteC = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int wasteClean = mGraph->emplaceContainer(3, ContainerNode::open, 100.0);

    int d1 = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);
    int d2 = mGraph->emplaceContainer(1, ContainerNode::open, 100.0);

    int chemoA = mGraph->emplaceContainer(2, ContainerNode::close, 100.0);
    int chemoB = mGraph->emplaceContainer(2, ContainerNode::close, 100.0);
    int cellstat = mGraph->emplaceContainer(3, ContainerNode::close, 100.0);

    int pA = mGraph->emplacePump(2, PumpNode::unidirectional, pumpfA);
    int pB = mGraph->emplacePump(2, PumpNode::unidirectional, pumpfB);

    int p1 = mGraph->emplacePump(2, PumpNode::unidirectional, pumpf_1);
    int p2 = mGraph->emplacePump(2, PumpNode::unidirectional, pumpf_2);

    TL empty;

    ValveNode::TruthTable tableType1_A;
    tableType1_A.insert(std::make_pair(0, empty));
    tableType1_A.insert(std::make_pair(1, TL({{1,2}})));
    tableType1_A.insert(std::make_pair(2, TL({{1,2}})));
    tableType1_A.insert(std::make_pair(3, TL({{0,2}})));

    ValveNode::TruthTable tableType1_B;
    tableType1_B.insert(std::make_pair(0, empty));
    tableType1_B.insert(std::make_pair(1, TL({{0,2}})));
    tableType1_B.insert(std::make_pair(2, TL({{0,1}})));
    tableType1_B.insert(std::make_pair(3, TL({{0,2}})));

    ValveNode::TruthTable tableType2_A;
    tableType2_A.insert(std::make_pair(0, empty));
    tableType2_A.insert(std::make_pair(1, TL({{0,2}})));
    tableType2_A.insert(std::make_pair(2, TL({{0,1}})));
    tableType2_A.insert(std::make_pair(3, TL({{0,1}})));
    tableType2_A.insert(std::make_pair(4, TL({{0,1}})));

    ValveNode::TruthTable tableType2_B;
    tableType2_B.insert(std::make_pair(0, empty));
    tableType2_B.insert(std::make_pair(1, TL({{0,1}})));
    tableType2_B.insert(std::make_pair(2, TL({{0,2}})));
    tableType2_B.insert(std::make_pair(3, TL({{0,2}})));
    tableType2_B.insert(std::make_pair(4, TL({{0,1}})));

    ValveNode::TruthTable tableType2_C;
    tableType2_C.insert(std::make_pair(0, empty));
    tableType2_C.insert(std::make_pair(1, TL({{0,1}})));
    tableType2_C.insert(std::make_pair(2, TL({{0,1}})));
    tableType2_C.insert(std::make_pair(3, TL({{0,2}})));
    tableType2_C.insert(std::make_pair(4, TL({{0,1}})));

    ValveNode::TruthTable tableType3;
    tableType3.insert(std::make_pair(0, empty));
    tableType3.insert(std::make_pair(1, TL({{0,4,5}})));
    tableType3.insert(std::make_pair(2, TL({{1,4,5}})));
    tableType3.insert(std::make_pair(3, TL({{2,4,5}})));
    tableType3.insert(std::make_pair(4, TL({{3,4,5}})));

    int vA_A = mGraph->emplaceValve(3, tableType1_A, route_vA_A);
    int vA_B = mGraph->emplaceValve(3, tableType1_B, route_vA_B);

    int vB_A = mGraph->emplaceValve(3, tableType1_A, route_vB_A);
    int vB_B = mGraph->emplaceValve(3, tableType1_B, route_vB_B);

    int vC_A = mGraph->emplaceValve(3, tableType2_A, route_vC_A);
    int vC_B = mGraph->emplaceValve(3, tableType2_B, route_vC_B);
    int vC_C = mGraph->emplaceValve(3, tableType2_C, route_vC_C);

    nodesMap["mediaA"] = mediaA;
    nodesMap["mediaB"] = mediaB;

    nodesMap["d1"] = d1;
    nodesMap["d2"] = d2;

    nodesMap["wasteA"] = wasteA;
    nodesMap["wasteB"] = wasteB;
    nodesMap["wasteC"] = wasteC;

    nodesMap["wasteClean"] = wasteClean;

    nodesMap["chemoA"] = chemoA;
    nodesMap["chemoB"] = chemoB;
    nodesMap["cellstat"] = cellstat;

    nodesMap["pA"] = pA;
    nodesMap["pB"] = pB;

    nodesMap["p1"] = p1;
    nodesMap["p2"] = p2;

    nodesMap["vA_A"] = vA_A;
    nodesMap["vA_B"] = vA_B;

    nodesMap["vB_A"] = vB_A;
    nodesMap["vB_B"] = vB_B;

    nodesMap["vC_A"] = vC_A;
    nodesMap["vC_B"] = vC_B;
    nodesMap["vC_C"] = vC_C;

    mGraph->connectNodes(d1,p1,0,0);
    mGraph->connectNodes(d2,p2,0,0);

    mGraph->connectNodes(p1,vA_A,1,1);
    mGraph->connectNodes(p2,vB_A,1,1);

    mGraph->connectNodes(mediaA,pA,0,0);
    mGraph->connectNodes(mediaB,pB,0,0);

    mGraph->connectNodes(pA,vA_A,1,0);
    mGraph->connectNodes(pB,vB_A,1,0);

    mGraph->connectNodes(vA_A,chemoA,2,0);
    mGraph->connectNodes(vB_A,chemoB,2,0);

    mGraph->connectNodes(chemoA,vA_B,1,0);
    mGraph->connectNodes(chemoB,vB_B,1,0);

    mGraph->connectNodes(vA_B,wasteClean,1,0);
    mGraph->connectNodes(vA_B,vC_A,2,0);
    mGraph->connectNodes(vB_B,wasteClean,1,1);
    mGraph->connectNodes(vB_B,vC_B,2,0);

    mGraph->connectNodes(vC_A,wasteA,1,0);
    mGraph->connectNodes(vC_A,cellstat,2,0);
    mGraph->connectNodes(vC_B,wasteB,1,0);
    mGraph->connectNodes(vC_B,cellstat,2,1);

    mGraph->connectNodes(cellstat,vC_C,2,0);

    mGraph->connectNodes(vC_C,wasteC,1,0);
    mGraph->connectNodes(vC_C,wasteClean,2,2);

    mGraph->setValvesAsTwins({vA_A, vA_B});
    mGraph->setValvesAsTwins({vB_A, vB_B});
    //mGraph->setValvesAsTwins({vC_B, vC_C, vC_A});

    return mGraph;
}

void TwinValvesTest::initTestCase() {
    PrologExecutor::createEngine(std::string(QTest::currentAppName()));
}

void TwinValvesTest::cleanupTestCase() {
    PrologExecutor::destoryEngine();
}

QTEST_APPLESS_MAIN(TwinValvesTest)

#include "tst_twinvalvestest.moc"
