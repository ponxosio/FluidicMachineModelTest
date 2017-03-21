#include <QString>
#include <QtTest>

#include <fluidicmachinemodel/machinegraph.h>

#include <fluidicmachinemodel/fluidicnode/valvenode.h>
#include <commonmodel/functions/function.h>
#include <commonmodel/functions/pumppluginfunction.h>
#include <commonmodel/functions/valvepluginroutefunction.h>

#include <fluidicmachinemodel/machine_graph_utils/machinegraphiterator.h>

#include <commonmodel/plugininterface/pluginconfiguration.h>
#include <commonmodel/plugininterface/pluginabstractfactory.h>

class GraphiteratortestTest : public QObject
{
    Q_OBJECT

public:
    GraphiteratortestTest();
private:
    MachineGraph makeComplexValveGraph(std::unordered_map<std::string, int> & containerMap);

private Q_SLOTS:
    void emptyAvoidNodes();
    void fullAvoidNodes();
};

GraphiteratortestTest::GraphiteratortestTest()
{
}

void GraphiteratortestTest::emptyAvoidNodes()
{
    try {
        unordered_map<std::string, int> containerMap;
        std::shared_ptr<MachineGraph> comlexValve = std::make_shared<MachineGraph>(makeComplexValveGraph(containerMap));

        std::vector<std::string> calculated;
        MachineGraphIterator it(containerMap["v"],comlexValve);
        while(it.advance()) {
            qDebug() << it.getActualNode();
            calculated.push_back(std::to_string(it.getActualNode()));
        }

        std::vector<std::string> expedted = {"7",
                                            "6",
                                            "1",
                                            "5",
                                            "0",
                                            "4",
                                            "2",
                                            "3"};

        QVERIFY2(calculated.size() == expedted.size(),
                 std::string("expected iterator size:" + std::to_string(expedted.size()) + ", received: " +
                             std::to_string(calculated.size())).c_str());

        for(int i = 0; i < expedted.size(); i++) {
            QVERIFY2(calculated[i].compare(expedted[i]) == 0,
                     std::string("i: " + std::to_string(i) +".Expected : " + expedted[i] + ", received: " + calculated[i]).c_str());
        }
    } catch (std::exception & e) {
        QFAIL(std::string("exception: " + std::string(e.what())).c_str());
    }
}

void GraphiteratortestTest::fullAvoidNodes() {
    try {
        unordered_map<std::string, int> containerMap;
        std::shared_ptr<MachineGraph> comlexValve = std::make_shared<MachineGraph>(makeComplexValveGraph(containerMap));

        std::vector<std::string> calculated;

        std::unordered_set<int> nodesToAvoid;
        nodesToAvoid.insert(containerMap["v"]);

        MachineGraphIterator it(containerMap["p1"],comlexValve, nodesToAvoid);
        while(it.advance()) {
            qDebug() << it.getActualNode();
            calculated.push_back(std::to_string(it.getActualNode()));
        }

        std::vector<std::string> expedted = {"5",
                                            "0"};

        QVERIFY2(calculated.size() == expedted.size(),
                 std::string("expected iterator size:" + std::to_string(expedted.size()) + ", received: " +
                             std::to_string(calculated.size())).c_str());

        for(int i = 0; i < expedted.size(); i++) {
            QVERIFY2(calculated[i].compare(expedted[i]) == 0,
                     std::string("i: " + std::to_string(i) +".Expected : " + expedted[i] + ", received: " + calculated[i]).c_str());
        }
    } catch (std::exception & e) {
        QFAIL(std::string("exception: " + std::string(e.what())).c_str());
    }
}

MachineGraph GraphiteratortestTest::makeComplexValveGraph(std::unordered_map<std::string, int> & containerMap) {
    MachineGraph mGraph;
    PluginConfiguration config;
    std::shared_ptr<PluginAbstractFactory> factory = nullptr;

    std::shared_ptr<Function> pumpf = std::make_shared<PumpPluginFunction>(factory, config);
    std::shared_ptr<Function> routef = std::make_shared<ValvePluginRouteFunction>(factory, config);

    int c0 = mGraph.emplaceContainer(2, ContainerNode::open, 100.0);
    int c1 = mGraph.emplaceContainer(2, ContainerNode::open, 100.0);
    int c2 = mGraph.emplaceContainer(2, ContainerNode::open, 100.0);
    int c3 = mGraph.emplaceContainer(2, ContainerNode::open, 100.0);
    int c4 = mGraph.emplaceContainer(2, ContainerNode::open, 100.0);

    int p1 = mGraph.emplacePump(2, PumpNode::unidirectional, pumpf);
    int p2 = mGraph.emplacePump(2, PumpNode::unidirectional, pumpf);

    ValveNode::TruthTable table;
    std::vector<std::unordered_set<int>> empty;
    table.insert(std::make_pair(0, empty));
    std::vector<std::unordered_set<int>> po1 = {{0,4},{1,2}};
    table.insert(std::make_pair(1, po1));
    std::vector<std::unordered_set<int>> po2 = {{1,4},{0,2}};
    table.insert(std::make_pair(2, po2));
    std::vector<std::unordered_set<int>> po3 = {{0,1,3}};
    table.insert(std::make_pair(4, po3));
    int v= mGraph.emplaceValve(5, table, routef);

    containerMap.insert(std::make_pair("c0", c0));
    containerMap.insert(std::make_pair("c1", c1));
    containerMap.insert(std::make_pair("c2", c2));
    containerMap.insert(std::make_pair("c3", c3));
    containerMap.insert(std::make_pair("c4", c4));
    containerMap.insert(std::make_pair("v", v));
    containerMap.insert(std::make_pair("p1", p1));
    containerMap.insert(std::make_pair("p2", p2));

    mGraph.connectNodes(c0,p1,1,0);
    mGraph.connectNodes(p1,v,1,0);
    mGraph.connectNodes(c1,p2,1,0);
    mGraph.connectNodes(p2,v,1,1);
    mGraph.connectNodes(v,c2,2,0);
    mGraph.connectNodes(v,c3,4,0);
    mGraph.connectNodes(v,c4,3,0);

    return mGraph;
}

QTEST_APPLESS_MAIN(GraphiteratortestTest)

#include "tst_graphiteratortesttest.moc"
