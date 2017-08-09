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
    void testTwinValvesRouting();

private:
    std::shared_ptr<MachineGraph> makeTwinMachine(std::unordered_map<std::string, int> & nodes, std::shared_ptr<PluginAbstractFactory> factory);
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

void TwinValvesTest::testTwinValvesRouting()
{
    try {
        std::shared_ptr<StringPluginFactory> strFactory = std::make_shared<StringPluginFactory>();

        std::unordered_map<std::string, int> containers;
        std::shared_ptr<MachineGraph> graph = makeTwinMachine(containers, strFactory);

        std::shared_ptr<TranslationStack> prologTStack = std::make_shared<PrologTranslationStack>();
        FluidicMachineModel model(graph, prologTStack, 3, 2, 300);

        model.setContinuousFlow(containers["c1"], containers["c4"], 300 * units::ml/units::hr);
        model.processFlows();

        std::string expected = "SET PUMP P1: dir 1, rate 300ml/hSET PUMP P2: dir 0, rate 0ml/hMOVE VALVE V1 3MOVE VALVE V2 3";
        std::string generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow(containers["c1"], containers["c4"]);
        model.setContinuousFlow(containers["c5"], containers["c3"], 300 * units::ml/units::hr);
        model.processFlows();

        expected = "SET PUMP P1: dir 0, rate 0ml/hSET PUMP P2: dir 1, rate 300ml/hMOVE VALVE V1 1MOVE VALVE V2 1";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        model.stopContinuousFlow(containers["c5"], containers["c3"]);
        model.setContinuousFlow(containers["c5"], containers["c4"], 300 * units::ml/units::hr);
        model.processFlows();

        expected = "SET PUMP P1: dir 0, rate 0ml/hSET PUMP P2: dir 1, rate 300ml/hMOVE VALVE V1 2MOVE VALVE V2 2";
        generated = strFactory->getCommandsSent();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

        qDebug() << "impossible flow";

        model.stopContinuousFlow(containers["c5"], containers["c4"]);
        model.setContinuousFlow(containers["c1"], containers["c3"], 300 * units::ml/units::hr);

        try {
            model.processFlows();
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

void TwinValvesTest::initTestCase() {
    PrologExecutor::createEngine(std::string(QTest::currentAppName()));
}

void TwinValvesTest::cleanupTestCase() {
    PrologExecutor::destoryEngine();
}

QTEST_APPLESS_MAIN(TwinValvesTest)

#include "tst_twinvalvestest.moc"
