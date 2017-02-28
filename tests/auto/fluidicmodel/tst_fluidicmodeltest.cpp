#include <QString>
#include <QtTest>

#include <fluidicmachinemodel.h>
#include <fluidicnode/valvenode.h>
#include <fluidicnode/functions/function.h>
#include <fluidicnode/functions/pumppluginfunction.h>
#include <fluidicnode/functions/valvepluginroutefunction.h>

#include <machine_graph_utils/graphrulesgenerator.h>

#include <rules/arithmetic/variable.h>
#include <rules/predicate.h>
#include <rules/equality.h>

#include <plugininterface/pluginconfiguration.h>
#include <plugininterface/pluginabstractfactory.h>

#include "stringpluginfactory.h"
#include "prologtranslationstack.h"

class FluidicmodelTest : public QObject
{
    Q_OBJECT

public:
    FluidicmodelTest();

private:
    std::shared_ptr<MachineGraph> makeMultipathWashMachineGraph(std::unordered_map<std::string, int> & nodesMap, std::shared_ptr<PluginAbstractFactory> factory);

private Q_SLOTS:
    void testCase1();
};

FluidicmodelTest::FluidicmodelTest()
{
}

void FluidicmodelTest::testCase1()
{
    try {
        std::shared_ptr<StringPluginFactory> strFactory = std::make_shared<StringPluginFactory>();

        std::unordered_map<std::string, int> nodesMap;
        std::shared_ptr<MachineGraph> multipathMachine = makeMultipathWashMachineGraph(nodesMap, strFactory);

        std::shared_ptr<PrologTranslationStack> plStack = std::make_shared<PrologTranslationStack>();
        FluidicMachineModel fluidicModel(multipathMachine, plStack, 3, 0);

        fluidicModel.setContinuousFlow(1,2,300);

        fluidicModel.calculateNewRoute();

        qDebug() << strFactory->getCommandsSent();

    } catch(std::exception & e) {
        QFAIL(std::string("Execpetion occured, message: " + std::string(e.what())).c_str());
    }

}

/*
 * +--+    +--+     +--+    +---+
 * |C1+---->P8+----->C6+---->V12|                                      +-V10-V11-V12+
 * +--+    +--+     +-++    +-+-+                             +--+     |0:close     |
 *                    |       |                        +------+C5|     +------------+
 *                    2       |                        |      +--+     |1:open      |
 *  +-----V17----+  +---+     |      +---+             |               +------------+
 *  |0:close     |  |V14|1----------0>V16<2-----+      3
 *  +------------+  +---+     |      +---+    +--+   +-v-+    +--+
 *  |1:1>0       |    0       |        1      |P9<--0|V17<2---+C4|     +V13-V14-V15-V16-+
 *  +------------+    |     +-v+       |      +-++   +-^-+    +--+     |0:close         |
 *  |2:2>0       |    |     |C2<-------+        |      1               +----------------+
 *  +------------+    2     +-^+       1        |      |               |1:2>0           |
 *  |3:3>0       |  +-v-+     |      +---+      |      |      +--+     +----------------+
 *  +------------+  |V13|1----------0>V15<2-----+      +------+C3|     |2:2>1           |
 *                  +---+     |      +---+                    +--+     +----------------+
 *                    0       |                                        |3:1>0           |
 *                    |       |                                        +----------------+
 * +--+   +---+     +-v+    +-+-+
 * |C0+--->V10+----->C7+---->V11|
 * +--+   +---+     +--+    +---+
 *
 * C0,C1,C2,C3,C4,C5: open container,
 * C6,C7: close container,
 * P8,P9: unidirectional pump,
 * V10,V11,V12,V13,V14,V15,V16,V17: valve with the corresponding thruth table.
 *
 */
std::shared_ptr<MachineGraph> FluidicmodelTest::makeMultipathWashMachineGraph(std::unordered_map<std::string, int> & nodesMap, std::shared_ptr<PluginAbstractFactory> factory) {
    std::shared_ptr<MachineGraph> mGraph = std::make_shared<MachineGraph>();

    PluginConfiguration config_p1;
    config_p1.setName("P8");
    std::shared_ptr<Function> pumpf1 = std::make_shared<PumpPluginFunction>(factory, config_p1);

    PluginConfiguration config_p2;
    config_p2.setName("P9");
    std::shared_ptr<Function> pumpf2 = std::make_shared<PumpPluginFunction>(factory, config_p2);

    PluginConfiguration config_v10;
    config_p2.setName("V10");
    std::shared_ptr<Function> route_v10 = std::make_shared<ValvePluginRouteFunction>(factory, config_v10);

    PluginConfiguration config_v11;
    config_p2.setName("V11");
    std::shared_ptr<Function> route_v11 = std::make_shared<ValvePluginRouteFunction>(factory, config_v11);

    PluginConfiguration config_v12;
    config_p2.setName("V12");
    std::shared_ptr<Function> route_v12 = std::make_shared<ValvePluginRouteFunction>(factory, config_v12);

    PluginConfiguration config_v13;
    config_p2.setName("V13");
    std::shared_ptr<Function> route_v13 = std::make_shared<ValvePluginRouteFunction>(factory, config_v13);

    PluginConfiguration config_v14;
    config_p2.setName("V14");
    std::shared_ptr<Function> route_v14 = std::make_shared<ValvePluginRouteFunction>(factory, config_v14);

    PluginConfiguration config_v15;
    config_p2.setName("V15");
    std::shared_ptr<Function> route_v15 = std::make_shared<ValvePluginRouteFunction>(factory, config_v15);

    PluginConfiguration config_v16;
    config_p2.setName("V16");
    std::shared_ptr<Function> route_v16 = std::make_shared<ValvePluginRouteFunction>(factory, config_v16);

    PluginConfiguration config_v17;
    config_p2.setName("V17");
    std::shared_ptr<Function> route_v17 = std::make_shared<ValvePluginRouteFunction>(factory, config_v17);

    int c0 = mGraph->emplaceContainer(1, open, 100.0);
    int c1 = mGraph->emplaceContainer(1, open, 100.0);
    int c2 = mGraph->emplaceContainer(4, open, 100.0);
    int c3 = mGraph->emplaceContainer(1, open, 100.0);
    int c4 = mGraph->emplaceContainer(1, open, 100.0);
    int c5 = mGraph->emplaceContainer(1, open, 100.0);

    int c6 = mGraph->emplaceContainer(3, close, 100.0);
    int c7 = mGraph->emplaceContainer(3, close, 100.0);

    int p8 = mGraph->emplacePump(2, PumpNode::unidirectional, pumpf1);
    int p9 = mGraph->emplacePump(3, PumpNode::unidirectional, pumpf2);

    ValveNode::TruthTable tableType1;
    std::vector<std::unordered_set<int>> empty;
    tableType1.insert(std::make_pair(0, empty));
    std::vector<std::unordered_set<int>> pos11 = {{0,1}};
    tableType1.insert(std::make_pair(1, pos11));

    ValveNode::TruthTable tableType2;
    tableType2.insert(std::make_pair(0, empty));
    std::vector<std::unordered_set<int>> pos12 = {{0,2}};
    tableType2.insert(std::make_pair(1, pos12));
    std::vector<std::unordered_set<int>> pos22 = {{1,2}};
    tableType2.insert(std::make_pair(2, pos22));
    std::vector<std::unordered_set<int>> pos32 = {{0,1}};
    tableType2.insert(std::make_pair(3, pos32));

    ValveNode::TruthTable tableType3;
    tableType3.insert(std::make_pair(0, empty));
    std::vector<std::unordered_set<int>> pos13 = {{1,0}};
    tableType3.insert(std::make_pair(1, pos13));
    std::vector<std::unordered_set<int>> pos23 = {{2,0}};
    tableType3.insert(std::make_pair(2, pos23));
    std::vector<std::unordered_set<int>> pos33 = {{3,0}};
    tableType3.insert(std::make_pair(3, pos33));

    int v10 = mGraph->emplaceValve(2, tableType1, route_v10);
    int v11 = mGraph->emplaceValve(2, tableType1, route_v11);
    int v12 = mGraph->emplaceValve(2, tableType1, route_v12);

    int v13 = mGraph->emplaceValve(3, tableType2, route_v13);
    int v14 = mGraph->emplaceValve(3, tableType2, route_v14);
    int v15 = mGraph->emplaceValve(3, tableType2, route_v15);
    int v16 = mGraph->emplaceValve(3, tableType2, route_v16);

    int v17 = mGraph->emplaceValve(4, tableType3, route_v17);

    nodesMap["c0"] = c0;
    nodesMap["c1"] = c1;
    nodesMap["c2"] = c2;
    nodesMap["c3"] = c3;
    nodesMap["c4"] = c4;
    nodesMap["c5"] = c5;
    nodesMap["c6"] = c6;
    nodesMap["c7"] = c7;
    nodesMap["p8"] = p8;
    nodesMap["p9"] = p9;
    nodesMap["v10"] = v10;
    nodesMap["v11"] = v11;
    nodesMap["v12"] = v12;
    nodesMap["v13"] = v13;
    nodesMap["v14"] = v14;
    nodesMap["v15"] = v15;
    nodesMap["v16"] = v16;
    nodesMap["v17"] = v17;

    mGraph->connectNodes(c1,p8,0,0);
    mGraph->connectNodes(p8,c6,1,0);
    mGraph->connectNodes(c6,v14,1,2);
    mGraph->connectNodes(c6,v12,2,0);
    mGraph->connectNodes(v12,c2,1,1);
    mGraph->connectNodes(v14,v13,0,2);
    mGraph->connectNodes(v14,v16,1,0);
    mGraph->connectNodes(v16,c2,1,2);
    mGraph->connectNodes(p9,v16,1,2);
    mGraph->connectNodes(v17,p9,0,2);
    mGraph->connectNodes(c3,v17,0,1);
    mGraph->connectNodes(c4,v17,0,2);
    mGraph->connectNodes(c5,v17,0,3);
    mGraph->connectNodes(v13,c7,0,1);
    mGraph->connectNodes(v13,v15,1,0);
    mGraph->connectNodes(v15,c2,1,3);
    mGraph->connectNodes(p9,v15,0,2);
    mGraph->connectNodes(c7,v10,0,1);
    mGraph->connectNodes(c7,v11,2,0);
    mGraph->connectNodes(v10,c0,0,0);
    mGraph->connectNodes(v11,c2,1,0);

    return mGraph;
}

QTEST_APPLESS_MAIN(FluidicmodelTest)

#include "tst_fluidicmodeltest.moc"
