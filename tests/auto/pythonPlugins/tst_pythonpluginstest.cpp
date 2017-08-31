#include <QString>
#include <QtTest>

#include <memory>

#include <fluidicmachinemodel/fluidicmachinemodel.h>
#include <fluidicmachinemodel/fluidicnode/valvenode.h>

#include <commonmodel/functions/function.h>
#include <commonmodel/functions/pumppluginfunction.h>
#include <commonmodel/functions/valvepluginroutefunction.h>
#include <commonmodel/plugininterface/pluginconfiguration.h>

#include <constraintengine/prologtranslationstack.h>

#include <pythonPlugins/actuators/pythonpluginabstractfactory.h>
#include <pythonPlugins/environment/PluginFileLoader.h>
#include <pythonPlugins/environment/PythonEnvironment.h>

#include "debugcommandsender.h"
#include "stringtranslationstack.h"

class PythonPluginsTest : public QObject
{
    Q_OBJECT

public:
    PythonPluginsTest();

private:
    std::shared_ptr<MachineGraph> makeMachineGraph(std::shared_ptr<CommandSender> com,
                                                   std::unordered_map<std::string, int> & containers);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void testPlugins();
};

PythonPluginsTest::PythonPluginsTest()
{
}

void PythonPluginsTest::testPlugins()
{
    std::shared_ptr<DebugCommandSender> com = std::make_shared<DebugCommandSender>();
    try {
        com->connect();

        PluginFileLoader::setPluginDir("X:/fluidicMachineModel/FluidicMachineModelTest/tests/auto/pythonPlugins/plugins");
        PythonEnvironment::GetInstance()->initEnvironment("X:/pluginScripts/interfacePlugins");

        std::unordered_map<std::string, int> containers;
        std::shared_ptr<MachineGraph> graph = makeMachineGraph(com, containers);

        std::shared_ptr<TranslationStack> prologTStack = std::make_shared<PrologTranslationStack>();
        FluidicMachineModel model(graph, prologTStack, 3, 2, 300);

        model.setContinuousFlow(containers["c1"], containers["c3"], 300 * units::ml/units::hr);
        model.processFlows({});

        std::string expected = "PUMP 8.33333333333e-051;MOVE 2;";
        std::string generated = com->getStr();

        qDebug() << "generated: " << generated.c_str();
        qDebug() << "expected: " << expected.c_str();

        QVERIFY2(expected.compare(generated) == 0, "expected and generated outputs are not the same check debug for more information");

    } catch (std::exception & e) {
        PythonEnvironment::GetInstance()->finishEnvironment();
        com->disconnect();

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
std::shared_ptr<MachineGraph> PythonPluginsTest::makeMachineGraph(
        std::shared_ptr<CommandSender> com,
        std::unordered_map<std::string, int> & containers)
{
    std::shared_ptr<MachineGraph> mGraph = std::make_shared<MachineGraph>();
    std::shared_ptr<PluginAbstractFactory> factory = std::make_shared<PythonPluginAbstractFactory>(com);

    PluginConfiguration pumpConfig("TestPump", "PeristalticPump", std::unordered_map<std::string,std::string>());
    std::shared_ptr<Function> pumpf =
            std::make_shared<PumpPluginFunction>(factory, pumpConfig, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));

    PluginConfiguration valveConfig("TestValve", "RoutingValve", std::unordered_map<std::string,std::string>());
    std::shared_ptr<Function> routef = std::make_shared<ValvePluginRouteFunction>(factory, valveConfig);

    int c0 = mGraph->emplaceContainer(2, ContainerNode::open, 100.0);
    int c1 = mGraph->emplaceContainer(2, ContainerNode::open, 100.0);
    int c2 = mGraph->emplaceContainer(2, ContainerNode::close, 100.0);
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

    containers["c1"] = c1;
    containers["c2"] = c2;
    containers["c3"] = c3;
    containers["p"] = p;
    containers["v"] = v;

    return mGraph;
}

void PythonPluginsTest::initTestCase() {
    PrologExecutor::createEngine(std::string(QTest::currentAppName()));
}

void PythonPluginsTest::cleanupTestCase() {
    PrologExecutor::destoryEngine();
}

QTEST_APPLESS_MAIN(PythonPluginsTest)

#include "tst_pythonpluginstest.moc"
