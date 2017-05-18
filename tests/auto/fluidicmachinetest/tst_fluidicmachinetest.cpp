#include <QtTest>

#include <sstream>

#include <fluidicmachinemodel/machinegraph.h>

#include <fluidicmachinemodel/fluidicnode/valvenode.h>
#include <commonmodel/functions/function.h>
#include <commonmodel/functions/pumppluginfunction.h>
#include <commonmodel/functions/valvepluginroutefunction.h>

#include <commonmodel/plugininterface/pluginconfiguration.h>
#include <commonmodel/plugininterface/pluginabstractfactory.h>

class FluidicMachineTest : public QObject
{
    Q_OBJECT

public:
    FluidicMachineTest();
    ~FluidicMachineTest();

private:
    MachineGraph makeMachineGraph();
    MachineGraph makeComplexValveGraph(std::unordered_map<std::string, int> & containerMap);

private slots:
    void testMachineGraph_structure();
    void testMachineGraph_tubeActions();
    void testMachineGraph_connectedComponents_whole();
    void testMachineGraph_connectedComponents_cut_05();
    void testMachineGraph_connectedComponents_cut_valve_5();
    void testMachineGraph_connectedComponents_cut_valve_5_copy();
    void testComplexValve_connectedComponents_101();
    void testMachineGraph_copy();
};

FluidicMachineTest::FluidicMachineTest()
{

}

FluidicMachineTest::~FluidicMachineTest()
{

}

void FluidicMachineTest::testMachineGraph_structure() {
    try {
        MachineGraph mGraph = makeMachineGraph();

        QVERIFY2(mGraph.getNumOpenContainers() == 3,
                 std::string("Number of opens containers is not 3, received " + std::to_string(mGraph.getNumOpenContainers())).c_str());

        QVERIFY2(mGraph.isCloseContainer(2), "C2 is not close container");
        QVERIFY2(mGraph.isOpenContainer(0), "C0 is not open container");
        QVERIFY2(mGraph.isOpenContainer(1), "C1 is not open container");
        QVERIFY2(mGraph.isOpenContainer(3), "C3 is not open container");
        QVERIFY2(mGraph.isPump(4), "P is not a pump");
        QVERIFY2(mGraph.isValve(5), "V is not a valve");

        stringstream stream;

        MachineGraph::GraphType::EdgeVectorPtr edges = mGraph.getArrivingTubes(5);
        for (std::shared_ptr<TubeEdge> e: *edges.get()) {
            stream << std::to_string(e->getIdSource()) << "->" << std::to_string(e->getIdTarget()) << ";";
        }

        QVERIFY2(stream.str().compare("0->5;1->5;") == 0,
                 std::string("Arriving edges to 5 incorrect. spected=\"1->5;0->5;\", received=" + stream.str()).c_str());
        stream.str(std::string());
        stream.clear();

        edges = mGraph.getLeavingTubes(5);
        for (std::shared_ptr<TubeEdge> e: *edges.get()) {
            stream << std::to_string(e->getIdSource()) << "->" << std::to_string(e->getIdTarget()) << ";";
        }

        QVERIFY2(stream.str().compare("5->2;") == 0,
                 std::string("Leaving edges to 5 incorrect. spected=\"5->2;\", received=" + stream.str()).c_str());
    } catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }
 }

void FluidicMachineTest::testMachineGraph_tubeActions() {
    try {
        MachineGraph mGraph = makeMachineGraph();

        mGraph.cutTube(1,5);
        QVERIFY2(mGraph.isTubeCutted(1,5), "edge 1->5 is not cutted");
        mGraph.uncutTube(1,5);
        QVERIFY2(!mGraph.isTubeCutted(1,5), "edge 1->5 is not uncutted");

        mGraph.cutAllTubesConnectedTo(5);
        QVERIFY2(mGraph.isTubeCutted(1,5), "edge 1->5 is not cutted");
        QVERIFY2(mGraph.isTubeCutted(0,5), "edge 0->5 is not cutted");
        QVERIFY2(mGraph.isTubeCutted(5,2), "edge 5->2 is not cutted");

        mGraph.uncutAllTubesConnectedTo(5);
        QVERIFY2(!mGraph.isTubeCutted(1,5), "edge 1->5 is not uncutted");
        QVERIFY2(!mGraph.isTubeCutted(0,5), "edge 0->5 is not uncutted");
        QVERIFY2(!mGraph.isTubeCutted(5,2), "edge 5->2 is not uncutted");

        QVERIFY2(mGraph.getTubeDirection(1,5) == TubeEdge::unknow, "edge 1->5 is not unknow by default");
        mGraph.setTubeDirection(1,5,TubeEdge::inverted);
        QVERIFY2(mGraph.getTubeDirection(1,5) == TubeEdge::inverted, "edge 1->5 is has not been changed to inverted");
        mGraph.setTubeDirection(1,5,TubeEdge::regular);
        QVERIFY2(mGraph.getTubeDirection(1,5) == TubeEdge::regular, "edge 1->5 is has not been changed to regular");
    } catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }
}

void FluidicMachineTest::testMachineGraph_connectedComponents_whole() {
    try {
        MachineGraph mGraph_copy = makeMachineGraph();

        std::vector<std::shared_ptr<MachineGraph>> connectedComponentsVector = mGraph_copy.getConnectedComponents();
        QVERIFY2(connectedComponentsVector.size() == 1,
                 std::string("number of connected components is not 1, is " + std::to_string(connectedComponentsVector.size())).c_str());

        std::shared_ptr<MachineGraph> cc1 = connectedComponentsVector[0];
        QVERIFY2(mGraph_copy.toString().compare(cc1->toString()) == 0,
                 std::string("Connected component is not equals to the whole graph, " + mGraph_copy.toString() + " != " + cc1->toString()).c_str());

    } catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }
}

void FluidicMachineTest::testMachineGraph_connectedComponents_cut_05() {
    try {
        MachineGraph mGraph_copy = makeMachineGraph();

        mGraph_copy.cutTube(1,5);
        std::vector<std::shared_ptr<MachineGraph>> connectedComponentsVector = mGraph_copy.getConnectedComponents();

        QVERIFY2(connectedComponentsVector.size() == 2,
                 std::string("number of connected components is not 2, is " + std::to_string(connectedComponentsVector.size())).c_str());

        std::shared_ptr<MachineGraph> cc1 = connectedComponentsVector[0];
        std::shared_ptr<MachineGraph> cc2 = connectedComponentsVector[1];

        QVERIFY2(cc2->toString().compare("digraph{1;1->5[label=\" direction=Unknow, cutted=1\"];}") == 0,
                 std::string("connected component 2 is not this graph: digraph{1;1->5[label=\" direction=Unknow, cutted=1\"];} != " + cc2->toString()).c_str());

        std::stringstream expectedCc1Str;
        expectedCc1Str << "digraph{" <<
                          "0;2;3;4;5;" <<
                          "0->5[label=\" direction=Unknow, cutted=0\"];" <<
                          "5->2[label=\" direction=Unknow, cutted=0\"];" <<
                          "2->4[label=\" direction=Unknow, cutted=0\"];" <<
                          "4->3[label=\" direction=Unknow, cutted=0\"];}";

        QVERIFY2(cc1->toString().compare(expectedCc1Str.str()) == 0,
                 std::string("connected component 2 is not this graph:" + expectedCc1Str.str() + " != " + cc1->toString()).c_str());

    } catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }
}

void FluidicMachineTest::testMachineGraph_connectedComponents_cut_valve_5() {
    try {
        MachineGraph mGraph = makeMachineGraph();

        mGraph.cutAllTubesConnectedTo(5);
        std::vector<std::shared_ptr<MachineGraph>> connectedComponentsVector = mGraph.getConnectedComponents();

        QVERIFY2(connectedComponentsVector.size() == 4,
                 std::string("number of connected components is not 4, is " + std::to_string(connectedComponentsVector.size())).c_str());

        std::shared_ptr<MachineGraph> cc1 = connectedComponentsVector[0];
        std::shared_ptr<MachineGraph> cc2 = connectedComponentsVector[1];
        std::shared_ptr<MachineGraph> cc3 = connectedComponentsVector[2];
        std::shared_ptr<MachineGraph> cc4 = connectedComponentsVector[3];

        stringstream stream;
        stream << "digraph{"
                  "0;"
                  "0->5[label=\" direction=Unknow, cutted=1\"];"
                  "}";

        QVERIFY2(cc1->toString().compare(stream.str()) == 0,
                 std::string("connected component 1 is not this graph:" + stream.str() + " != " + cc1->toString()).c_str());
        stream.str(std::string());
        stream.clear();

        stream << "digraph{"
                  "1;"
                  "1->5[label=\" direction=Unknow, cutted=1\"];"
                  "}";

        QVERIFY2(cc2->toString().compare(stream.str()) == 0,
                 std::string("connected component 2 is not this graph:" + stream.str() + " != " + cc2->toString()).c_str());
        stream.str(std::string());
        stream.clear();

        stream << "digraph{"
                  "2;"
                  "3;"
                  "4;"
                  "2->4[label=\" direction=Unknow, cutted=0\"];"
                  "4->3[label=\" direction=Unknow, cutted=0\"];"
                  "}";

        QVERIFY2(cc3->toString().compare(stream.str()) == 0,
                 std::string("connected component 3 is not this graph:" + stream.str() + " != " + cc3->toString()).c_str());
        stream.str(std::string());
        stream.clear();

        stream << "digraph{"
                  "5;"
                  "5->2[label=\" direction=Unknow, cutted=1\"];"
                  "}";

        QVERIFY2(cc4->toString().compare(stream.str()) == 0,
                 std::string("connected component 4 is not this graph:" + stream.str() + " != " + cc4->toString()).c_str());

    } catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }
}

void FluidicMachineTest::testMachineGraph_connectedComponents_cut_valve_5_copy() {
    try {
        MachineGraph mGraph = makeMachineGraph();

        mGraph.cutAllTubesConnectedTo(5);
        std::vector<std::shared_ptr<MachineGraph>> connectedComponentsVector = mGraph.getConnectedComponents();

        QVERIFY2(connectedComponentsVector.size() == 4,
                 std::string("number of connected components is not 4, is " + std::to_string(connectedComponentsVector.size())).c_str());

        std::shared_ptr<MachineGraph> cc1 = connectedComponentsVector[0];
        std::shared_ptr<MachineGraph> cc2 = connectedComponentsVector[1];
        std::shared_ptr<MachineGraph> cc3 = connectedComponentsVector[2];
        std::shared_ptr<MachineGraph> cc4 = connectedComponentsVector[3];

        stringstream stream;
        stream << "digraph{"
                  "0;"
                  "0->5[label=\" direction=Unknow, cutted=1\"];"
                  "}";

        QVERIFY2(cc1->toString().compare(stream.str()) == 0,
                 std::string("original connected component 1 is not this graph:" + stream.str() + " != " + cc1->toString()).c_str());
        stream.str(std::string());
        stream.clear();

        stream << "digraph{"
                  "1;"
                  "1->5[label=\" direction=Unknow, cutted=1\"];"
                  "}";

        QVERIFY2(cc2->toString().compare(stream.str()) == 0,
                 std::string("original connected component 2 is not this graph:" + stream.str() + " != " + cc2->toString()).c_str());
        stream.str(std::string());
        stream.clear();

        stream << "digraph{"
                  "2;"
                  "3;"
                  "4;"
                  "2->4[label=\" direction=Unknow, cutted=0\"];"
                  "4->3[label=\" direction=Unknow, cutted=0\"];"
                  "}";

        QVERIFY2(cc3->toString().compare(stream.str()) == 0,
                 std::string("original connected component 3 is not this graph:" + stream.str() + " != " + cc3->toString()).c_str());
        stream.str(std::string());
        stream.clear();

        stream << "digraph{"
                  "5;"
                  "5->2[label=\" direction=Unknow, cutted=1\"];"
                  "}";

        QVERIFY2(cc4->toString().compare(stream.str()) == 0,
                 std::string("original connected component 4 is not this graph:" + stream.str() + " != " + cc4->toString()).c_str());

        MachineGraph mGraph_copy(mGraph);

        mGraph_copy.cutAllTubesConnectedTo(5);
        connectedComponentsVector = mGraph_copy.getConnectedComponents();

        QVERIFY2(connectedComponentsVector.size() == 4,
                 std::string("copy number of connected components is not 4, is " + std::to_string(connectedComponentsVector.size())).c_str());
        stream.str(std::string());
        stream.clear();

        cc1 = connectedComponentsVector[0];
        cc2 = connectedComponentsVector[1];
        cc3 = connectedComponentsVector[2];
        cc4 = connectedComponentsVector[3];

        /*qDebug() << cc1->toString().c_str();
        qDebug() << cc2->toString().c_str();
        qDebug() << cc3->toString().c_str();
        qDebug() << cc4->toString().c_str();*/

        stream << "digraph{"
                  "0;"
                  "0->5[label=\" direction=Unknow, cutted=1\"];"
                  "}";

        QVERIFY2(cc1->toString().compare(stream.str()) == 0,
                 std::string("copy connected component 1 is not this graph:" + stream.str() + " != " + cc1->toString()).c_str());
        stream.str(std::string());
        stream.clear();

        stream << "digraph{"
                  "1;"
                  "1->5[label=\" direction=Unknow, cutted=1\"];"
                  "}";

        QVERIFY2(cc2->toString().compare(stream.str()) == 0,
                 std::string("copy connected component 2 is not this graph:" + stream.str() + " != " + cc2->toString()).c_str());
        stream.str(std::string());
        stream.clear();

        stream << "digraph{"
                  "2;"
                  "3;"
                  "4;"
                  "2->4[label=\" direction=Unknow, cutted=0\"];"
                  "4->3[label=\" direction=Unknow, cutted=0\"];"
                  "}";

        QVERIFY2(cc3->toString().compare(stream.str()) == 0,
                 std::string("copy connected component 3 is not this graph:" + stream.str() + " != " + cc3->toString()).c_str());
        stream.str(std::string());
        stream.clear();

        stream << "digraph{"
                  "5;"
                  "5->2[label=\" direction=Unknow, cutted=1\"];"
                  "}";

        QVERIFY2(cc4->toString().compare(stream.str()) == 0,
                 std::string("copy connected component 4 is not this graph:" + stream.str() + " != " + cc4->toString()).c_str());


        mGraph_copy.uncutAllTubesConnectedTo(5);
        mGraph.cutTube(1,5);

         std::vector<std::shared_ptr<MachineGraph>> copyComponents = mGraph_copy.getConnectedComponents();
         std::vector<std::shared_ptr<MachineGraph>> originalComponents = mGraph.getConnectedComponents();

         QVERIFY2(copyComponents.size() == 1,
                  std::string("copy components after uncutting is not of size 1 is size " + std::to_string(copyComponents.size())).c_str());
         QVERIFY2(originalComponents.size() == 4,
                  std::string("original components after uncutting is not of size 4 is size " + std::to_string(copyComponents.size())).c_str());

    } catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }
}

void FluidicMachineTest::testMachineGraph_copy() {
    try{
        MachineGraph mGraph = makeMachineGraph();
        MachineGraph mGraph_copy(mGraph);

        QVERIFY2(mGraph.toString().compare(mGraph_copy.toString()) == 0,
                 std::string("Copied graph are not the same: " + mGraph.toString() + " != " + mGraph_copy.toString()).c_str());

        mGraph.cutTube(0,5);
        QVERIFY2(mGraph.isTubeCutted(0,5) && !mGraph_copy.isTubeCutted(0,5),
                 "not independent error: after cutting tube in original tube is also cutted in copy,");

        mGraph.uncutTube(0,5);
        QVERIFY2(!mGraph.isTubeCutted(0,5) && !mGraph_copy.isTubeCutted(0,5),
                 "not independent error: after uncutting tube in original tube is also cutted in copy,");

        mGraph.cutAllTubesConnectedTo(5);

        QVERIFY2(mGraph.isTubeCutted(1,5) && !mGraph_copy.isTubeCutted(1,5), "origial edge 1->5 is cutted and copy graph is also cutted");
        QVERIFY2(mGraph.isTubeCutted(0,5) && !mGraph_copy.isTubeCutted(0,5), "origial edge 0->5 is cutted and copy graph is also cutted");
        QVERIFY2(mGraph.isTubeCutted(5,2) && !mGraph_copy.isTubeCutted(5,2), "origial edge 5->2 is cutted and copy graph is also cutted");

        mGraph.uncutAllTubesConnectedTo(5);
        QVERIFY2(!mGraph.isTubeCutted(1,5) && !mGraph_copy.isTubeCutted(1,5), "origial edge 1->5 is uncutted and copy graph is also cutted");
        QVERIFY2(!mGraph.isTubeCutted(0,5) && !mGraph_copy.isTubeCutted(0,5), "origial edge 0->5 is uncutted and copy graph is also cutted");
        QVERIFY2(!mGraph.isTubeCutted(5,2) && !mGraph_copy.isTubeCutted(5,2), "origial edge 5->2 is uncutted and copy graph is also cutted");

        mGraph_copy.cutTube(0,5);
        QVERIFY2(mGraph_copy.isTubeCutted(0,5) && !mGraph.isTubeCutted(0,5),
                 "not independent error: after cutting tube in copy tube is also cutted in origial,");

        mGraph_copy.uncutTube(0,5);
        QVERIFY2(!mGraph_copy.isTubeCutted(0,5) && !mGraph.isTubeCutted(0,5),
                 "not independent error: after uncutting tube in copy tube is also cutted in origial,");

        mGraph_copy.cutAllTubesConnectedTo(5);

        QVERIFY2(mGraph_copy.isTubeCutted(1,5) && !mGraph.isTubeCutted(1,5), "copy edge 1->5 is cutted and origial graph is also cutted");
        QVERIFY2(mGraph_copy.isTubeCutted(0,5) && !mGraph.isTubeCutted(0,5), "copy edge 0->5 is cutted and origial graph is also cutted");
        QVERIFY2(mGraph_copy.isTubeCutted(5,2) && !mGraph.isTubeCutted(5,2), "copy edge 5->2 is cutted and origial graph is also cutted");

        mGraph_copy.uncutAllTubesConnectedTo(5);
        QVERIFY2(!mGraph_copy.isTubeCutted(1,5) && !mGraph.isTubeCutted(1,5), "copy edge 1->5 is uncutted and origial graph is also cutted");
        QVERIFY2(!mGraph_copy.isTubeCutted(0,5) && !mGraph.isTubeCutted(0,5), "copy edge 0->5 is uncutted and origial graph is also cutted");
        QVERIFY2(!mGraph_copy.isTubeCutted(5,2) && !mGraph.isTubeCutted(5,2), "copy edge 5->2 is uncutted and origial graph is also cutted");
    } catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }
}

void FluidicMachineTest::testComplexValve_connectedComponents_101() {
    try {
        std::unordered_map<std::string, int> cMap;
        MachineGraph machine = makeComplexValveGraph(cMap);

        machine.getValve(cMap["v"])->setActualPosition(1);
        machine.cutTube(cMap["v"], cMap["c4"]);
        machine.cutTube(cMap["c1"], cMap["p2"]);
        machine.cutTube(cMap["p2"], cMap["v"]);

        machine.setTubeDirection(cMap["c0"], cMap["p1"], TubeEdge::regular);
        machine.setTubeDirection(cMap["p1"], cMap["v"], TubeEdge::regular);
        machine.setTubeDirection(cMap["v"], cMap["c3"], TubeEdge::regular);

        const std::vector<std::shared_ptr<MachineGraph>> & connectedComponents = machine.getConnectedComponents();
        std::vector<std::string> expected = { "digraph{7;2;7->4[label=\" direction=Unknow, cutted=1\"];7->2[label=\" direction=Unknow, cutted=0\"];}",
                                              "digraph{7;5;0;3;7->4[label=\" direction=Unknow, cutted=1\"];0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=0\"];}",
                                              "digraph{1;1->6[label=\" direction=Unknow, cutted=1\"];}",
                                              "digraph{4;}",
                                              "digraph{6;6->7[label=\" direction=Unknow, cutted=1\"];}"};

        std::vector<std::string> received;
        for(std::shared_ptr<MachineGraph> component: connectedComponents) {
            std::string str = component->toString();
            qDebug() << str.c_str();
            received.push_back(str);
        }

        QVERIFY2(connectedComponents.size() == expected.size(),
                 std::string("Expected connected components: " + std::to_string(connectedComponents.size()) + ",  received: " + std::to_string(connectedComponents.size())).c_str());

        for (int i = 0; i < expected.size(); i++) {
            std::string received_i = received[i];
            QVERIFY2(received_i.compare(expected[i]) == 0,
                     std::string("Fail connected component " + std::to_string(i) + ": expected " + expected[i] + ", recived " + received_i).c_str());
        }

        int sizeLeaving_1_0 = connectedComponents[1]->getLeavingTubes(cMap["c0"])->size();
        QVERIFY2(sizeLeaving_1_0 == 1, std::string(std::to_string(sizeLeaving_1_0) + " received is not 1").c_str());

        int sizeLeaving_0_v = connectedComponents[0]->getLeavingTubes(cMap["v"])->size();
        QVERIFY2(sizeLeaving_0_v == 2, std::string(std::to_string(sizeLeaving_0_v) + " received is not 2").c_str());
        int sizeArriving_0_v = connectedComponents[0]->getArrivingTubes(cMap["v"])->size();
        QVERIFY2(sizeArriving_0_v == 0, std::string(std::to_string(sizeArriving_0_v) + " received is not 0").c_str());

        int sizeLeaving_1_v = connectedComponents[1]->getLeavingTubes(cMap["v"])->size();
        QVERIFY2(sizeLeaving_1_v == 2, std::string(std::to_string(sizeLeaving_1_v) + " received is not 2").c_str());
        int sizeArriving_1_v = connectedComponents[1]->getArrivingTubes(cMap["v"])->size();
        QVERIFY2(sizeArriving_1_v == 1, std::string(std::to_string(sizeArriving_1_v) + " received is not 1").c_str());

    } catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }
}

MachineGraph FluidicMachineTest::makeMachineGraph() {
    MachineGraph mGraph;
    PluginConfiguration config;
    std::shared_ptr<PluginAbstractFactory> factory = nullptr;

    std::shared_ptr<Function> pumpf = std::make_shared<PumpPluginFunction>(factory, config, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));
    std::shared_ptr<Function> routef = std::make_shared<ValvePluginRouteFunction>(factory, config);

    int c0 = mGraph.emplaceContainer(2, ContainerNode::open, 100.0);
    int c1 = mGraph.emplaceContainer(2, ContainerNode::open, 100.0);
    int c2 = mGraph.emplaceContainer(2, ContainerNode::close, 100.0);
    int c3 = mGraph.emplaceContainer(2, ContainerNode::open, 100.0);

    int p = mGraph.emplacePump(2, PumpNode::bidirectional, pumpf);

    ValveNode::TruthTable table;
    std::vector<std::unordered_set<int>> empty;
    table.insert(std::make_pair(0, empty));
    int v= mGraph.emplaceValve(3, table, routef);

    mGraph.connectNodes(c0,v,1,0);
    mGraph.connectNodes(c1,v,1,1);
    mGraph.connectNodes(v,c2,2,0);
    mGraph.connectNodes(c2,p,1,0);
    mGraph.connectNodes(p,c3,1,0);

    return mGraph;
}

MachineGraph FluidicMachineTest::makeComplexValveGraph(std::unordered_map<std::string, int> & containerMap) {
    MachineGraph mGraph;
    PluginConfiguration config;
    std::shared_ptr<PluginAbstractFactory> factory = nullptr;

    std::shared_ptr<Function> pumpf = std::make_shared<PumpPluginFunction>(factory, config, PumpWorkingRange(0 * units::ml/units::hr, 999 * units::ml/units::hr));
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
    table.insert(std::make_pair(3, po3));
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

QTEST_APPLESS_MAIN(FluidicMachineTest)

#include "tst_fluidicmachinetest.moc"
