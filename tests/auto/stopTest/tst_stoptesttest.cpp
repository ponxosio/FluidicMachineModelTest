#include <QString>
#include <QtTest>

#include <commonmodel/functions/ranges/heaterworkingrange.h>
#include <commonmodel/functions/ranges/pumpworkingrange.h>
#include <commonmodel/functions/pumppluginfunction.h>
#include <commonmodel/functions/valvepluginroutefunction.h>
#include <commonmodel/functions/heatfunction.h>

#include <constraintengine/prologexecutor.h>
#include <constraintengine/prologtranslationstack.h>

#include <fluidicmachinemodel/machinegraph.h>
#include <fluidicmachinemodel/fluidicmachinemodel.h>

#include "stringpluginfactory.h"

class StopTestTest : public QObject
{
    Q_OBJECT

private:
    std::shared_ptr<MachineGraph> makeMachineGraph(std::unordered_map<std::string,int> & containers);

public:
    StopTestTest();

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testCase1();
};

StopTestTest::StopTestTest()
{
}

void StopTestTest::testCase1()
{
    try {
        std::unordered_map<std::string, int> containers;
        std::shared_ptr<PrologTranslationStack> stack = std::make_shared<PrologTranslationStack>();
        FluidicMachineModel model(makeMachineGraph(containers), stack, 3, 2, 200);

        std::shared_ptr<StringPluginFactory> factory = std::make_shared<StringPluginFactory>();
        model.updatePluginFactory(factory);

        model.setContinuousFlow(containers["c0"], containers["c3"], 200 * units::ml / units::hr);
        model.processFlows();
        model.getComponent(containers["c2"])->doOperation(Function::heat,1,20 * units::C);

        model.stopAllOperations();

        std::string generated = factory->getCommandsSent();
        qDebug() << "generated:";
        qDebug() << generated.c_str();

        std::string expected = "PUMP DIR 1 RATE 200ml/h;MOVE VALVE 1;HEAT 20ºC;STOP HEAT;STOP PUMP;MOVE VALVE HOME";
        qDebug() << "expected:";
        qDebug() << expected.c_str();

        QVERIFY2(generated.compare(expected) == 0, "Generated and expected output are not the same, check debug for more information.");

    } catch (std::exception & e) {
        QFAIL(e.what());
    }
}

/*
 *
 *                    +--------+----------+---------+
 *                    |0:close | 1:c0 >c2 | 2:c1 >c2|
 *   +---+            +--------+--------------------+
 *   |C_1+--------+   |       3: (c0 & c1)> c2      |
 *   +---+        |   +-----------------------------+
 *              +-v-+      +---+    +---+    +---+
 *              |V_5+----> |C_2+--> |P_4+--> |C_3|
 *   +---+      +-^-+      +---+    +---+    +---+
 *   |C_0|        |
 *   +------------+
 *
 *
 *  C_0, C_1, C_3 : open container,
 *  V_5: valve,
 *  P_4: bidirectional pump,
 *  C_2: close container,
 */
std::shared_ptr<MachineGraph> StopTestTest::makeMachineGraph(std::unordered_map<std::string,int> & containers) {
    std::shared_ptr<MachineGraph> mGraph = std::make_shared<MachineGraph>();
    PluginConfiguration config;
    std::shared_ptr<PluginAbstractFactory> factory = nullptr;

    std::shared_ptr<Function> pumpf = std::make_shared<PumpPluginFunction>(factory, config, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));
    std::shared_ptr<Function> routef = std::make_shared<ValvePluginRouteFunction>(factory, config);
    std::shared_ptr<Function> heaterf = std::make_shared<HeatFunction>(factory, config, 0 * units::ml, HeaterWorkingRange(0 * units::C, 100 * units::C));

    int c0 = mGraph->emplaceContainer(2, ContainerNode::open, 100.0);
    int c1 = mGraph->emplaceContainer(2, ContainerNode::open, 100.0);

    int c2 = mGraph->emplaceContainer(2, ContainerNode::close, 100.0);
    mGraph->getContainer(c2)->addOperation(heaterf);

    int c3 = mGraph->emplaceContainer(2, ContainerNode::open, 100.0);

    int p = mGraph->emplacePump(2, PumpNode::bidirectional, pumpf);

    ValveNode::TruthTable table;
    std::vector<std::unordered_set<int>> empty;
    table.insert(std::make_pair(0, empty));
    std::vector<std::unordered_set<int>> pos1 = {{0,2}};
    table.insert(std::make_pair(1, pos1));
    std::vector<std::unordered_set<int>> pos2 = {{1,2}};
    table.insert(std::make_pair(2, pos2));
    std::vector<std::unordered_set<int>> pos3 = {{0,1,2}};
    table.insert(std::make_pair(3, pos3));

    int v= mGraph->emplaceValve(3, table, routef);

    mGraph->connectNodes(c0,v,1,0);
    mGraph->connectNodes(c1,v,1,1);
    mGraph->connectNodes(v,c2,2,0);
    mGraph->connectNodes(c2,p,1,0);
    mGraph->connectNodes(p,c3,1,0);

    containers["c0"] = c0;
    containers["c1"] = c1;
    containers["c2"] = c2;
    containers["c3"] = c3;

    return mGraph;
}

void StopTestTest::initTestCase() {
    PrologExecutor::createEngine(std::string(QTest::currentAppName()));
}

void StopTestTest::cleanupTestCase() {
    PrologExecutor::destoryEngine();
}

QTEST_APPLESS_MAIN(StopTestTest)

#include "tst_stoptesttest.moc"
