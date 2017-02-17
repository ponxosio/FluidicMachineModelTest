#include <QString>
#include <QtTest>

#include <sstream>
#include <vector>

#include <machinegraph.h>

#include <fluidicnode/valvenode.h>
#include <fluidicnode/functions/function.h>
#include <fluidicnode/functions/pumppluginfunction.h>
#include <fluidicnode/functions/valvepluginroutefunction.h>

#include <machine_graph_utils/rules_generation/machinegraphgenerator.h>

#include <plugininterface/pluginconfiguration.h>
#include <plugininterface/pluginabstractfactory.h>

class MachineGraphGeneratorTest : public QObject
{
    Q_OBJECT

public:
    MachineGraphGeneratorTest();

private:
    std::shared_ptr<MachineGraph> makeMachineGraph();
    MachineGraph makeComplexValveGraph_unidirectional(std::unordered_map<std::string, int> & containerMap);
    MachineGraph makeComplexValveGraph_bidirectional(std::unordered_map<std::string, int> & containerMap);
    std::shared_ptr<MachineGraph> makeMultipathWashMachineGraph();

private Q_SLOTS:
    void simpleGraph();
    void complexValveGraph_unidirectional();
    void complexValveGraph_bidirectional();
    void multipathMachine();
};

MachineGraphGeneratorTest::MachineGraphGeneratorTest()
{
}

void MachineGraphGeneratorTest::simpleGraph()
{
    try {
        std::shared_ptr<MachineGraph> mGraph = makeMachineGraph();
        MachineGraphGenerator generator(mGraph);

        QVERIFY2(generator.calculateNext(), "fail to get V1P-1 graph state");
        QVERIFY2(generator.getStateStr().compare("-1;1;") == 0,
                 std::string("expected state -1;1; != " + generator.getStateStr()).c_str());

        std::vector<std::shared_ptr<MachineGraph>> stateV1_Pm1 = generator.getActualGraphGenerated()->getConnectedComponents();
        QVERIFY2(stateV1_Pm1.size() == 2, "STATE V1P-1: don't has 2 connected components");

        stringstream stream;

        stream << "digraph{" <<
                  "1;"
                  "1->5[label=\" direction=Inverted, cutted=1\"];" <<
                  "}";

        QVERIFY2(stateV1_Pm1[1]->toString().compare(stream.str()) == 0,
                std::string("STATE V1P-1: 2º connected component: expected= " + stream.str() + ", received= " + stateV1_Pm1[1]->toString()).c_str());
        stream.str("");
        stream.clear();

        stream << "digraph{" <<
                  "0;" <<
                  "2;" <<
                  "3;" <<
                  "4;" <<
                  "5;" <<
                  "0->5[label=\" direction=Inverted, cutted=0\"];" <<
                  "5->2[label=\" direction=Inverted, cutted=0\"];" <<
                  "2->4[label=\" direction=Inverted, cutted=0\"];" <<
                  "4->3[label=\" direction=Inverted, cutted=0\"];" <<
                  "}";

        QVERIFY2(stateV1_Pm1[0]->toString().compare(stream.str()) == 0,
                std::string("STATE V1P-1: 1º connected component: expected= " + stream.str() + ", received= " + stateV1_Pm1[0]->toString()).c_str());
        stream.str("");
        stream.clear();

        QVERIFY2(generator.calculateNext(), "fail to get V1P1 graph state");
        QVERIFY2(generator.getStateStr().compare("1;1;") == 0,
                 std::string("expected state 1;1; != " + generator.getStateStr()).c_str());


        std::vector<std::shared_ptr<MachineGraph>> stateV1_P1 = generator.getActualGraphGenerated()->getConnectedComponents();
        QVERIFY2(stateV1_P1.size() == 2, "STATE V1P1: don't has 2 connected components");

        stream << "digraph{" <<
                  "1;"
                  "1->5[label=\" direction=Regular, cutted=1\"];" <<
                  "}";

        QVERIFY2(stateV1_P1[1]->toString().compare(stream.str()) == 0,
                std::string("STATE V1P1: 2º connected component: expected= " + stream.str() + ", received= " + stateV1_P1[1]->toString()).c_str());
        stream.str("");
        stream.clear();

        stream << "digraph{" <<
                  "0;" <<
                  "2;" <<
                  "3;" <<
                  "4;" <<
                  "5;" <<
                  "0->5[label=\" direction=Regular, cutted=0\"];" <<
                  "5->2[label=\" direction=Regular, cutted=0\"];" <<
                  "2->4[label=\" direction=Regular, cutted=0\"];" <<
                  "4->3[label=\" direction=Regular, cutted=0\"];" <<
                  "}";

        QVERIFY2(stateV1_P1[0]->toString().compare(stream.str()) == 0,
                std::string("STATE V1P1: 1º connected component: expected= " + stream.str() + ", received= " + stateV1_P1[0]->toString()).c_str());
        stream.str("");
        stream.clear();

        QVERIFY2(generator.calculateNext(), "fail to get V2P-1 graph state");
        QVERIFY2(generator.getStateStr().compare("-1;2;") == 0,
                 std::string("expected state -1;2; != " + generator.getStateStr()).c_str());

        std::vector<std::shared_ptr<MachineGraph>> stateV2_Pm1 = generator.getActualGraphGenerated()->getConnectedComponents();

        QVERIFY2(stateV2_Pm1.size() == 2, "STATE V2P-1: don't has 2 connected components");

        stream << "digraph{"
                  "0;"
                  "0->5[label=\" direction=Inverted, cutted=1\"];"
                  "}";

        QVERIFY2(stateV2_Pm1[0]->toString().compare(stream.str()) == 0,
                std::string("STATE V2P-1: 1º connected component: expected= " + stream.str() + ", received= " + stateV2_Pm1[0]->toString()).c_str());
        stream.str("");
        stream.clear();

        stream << "digraph{"
                  "1;"
                  "2;"
                  "3;"
                  "4;"
                  "5;"
                  "1->5[label=\" direction=Inverted, cutted=0\"];"
                  "5->2[label=\" direction=Inverted, cutted=0\"];"
                  "2->4[label=\" direction=Inverted, cutted=0\"];"
                  "4->3[label=\" direction=Inverted, cutted=0\"];"
                  "}";

        QVERIFY2(stateV2_Pm1[1]->toString().compare(stream.str()) == 0,
                std::string("STATE V2P-1: 2º connected component: expected= " + stream.str() + ", received= " + stateV2_Pm1[1]->toString()).c_str());
        stream.str("");
        stream.clear();

        QVERIFY2(generator.calculateNext(), "fail to get V2P1 graph state");
        QVERIFY2(generator.getStateStr().compare("1;2;") == 0,
                 std::string("expected state 1;2; != " + generator.getStateStr()).c_str());

        std::vector<std::shared_ptr<MachineGraph>> stateV2_P1 = generator.getActualGraphGenerated()->getConnectedComponents();

        QVERIFY2(stateV2_P1.size() == 2, "STATE V2P1: don't has 2 connected components");

        stream << "digraph{"
                  "0;"
                  "0->5[label=\" direction=Regular, cutted=1\"];"
                  "}";

        QVERIFY2(stateV2_P1[0]->toString().compare(stream.str()) == 0,
                std::string("STATE V2P-1: 1º connected component: expected= " + stream.str() + ", received= " + stateV2_P1[0]->toString()).c_str());
        stream.str("");
        stream.clear();

        stream << "digraph{"
                  "1;"
                  "2;"
                  "3;"
                  "4;"
                  "5;"
                  "1->5[label=\" direction=Regular, cutted=0\"];"
                  "5->2[label=\" direction=Regular, cutted=0\"];"
                  "2->4[label=\" direction=Regular, cutted=0\"];"
                  "4->3[label=\" direction=Regular, cutted=0\"];"
                  "}";

        QVERIFY2(stateV2_P1[1]->toString().compare(stream.str()) == 0,
                std::string("STATE V2P-1: 2º connected component: expected= " + stream.str() + ", received= " + stateV2_P1[1]->toString()).c_str());
        stream.str("");
        stream.clear();

    } catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }
}

void MachineGraphGeneratorTest::complexValveGraph_unidirectional() {
    try {
        std::unordered_map<std::string, int> nodeMap;
        std::shared_ptr<MachineGraph> complexValve = std::make_shared<MachineGraph>(makeComplexValveGraph_unidirectional(nodeMap));

        std::vector<std::string> spected = {"1;0;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Unknow, cutted=1\"];6->7[label=\" direction=Unknow, cutted=1\"];7->3[label=\" direction=Regular, cutted=0\"];7->2[label=\" direction=Unknow, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "0;1;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Unknow, cutted=1\"];5->7[label=\" direction=Unknow, cutted=1\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Unknow, cutted=0\"];7->2[label=\" direction=Regular, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "1;1;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=0\"];7->2[label=\" direction=Regular, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "1;0;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Unknow, cutted=1\"];6->7[label=\" direction=Unknow, cutted=1\"];7->3[label=\" direction=Unknow, cutted=0\"];7->2[label=\" direction=Regular, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "0;1;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Unknow, cutted=1\"];5->7[label=\" direction=Unknow, cutted=1\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=0\"];7->2[label=\" direction=Unknow, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "1;1;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=0\"];7->2[label=\" direction=Regular, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "1;0;3; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Regular, cutted=1\"];6->7[label=\" direction=Regular, cutted=1\"];7->3[label=\" direction=Regular, cutted=1\"];7->2[label=\" direction=Regular, cutted=1\"];7->4[label=\" direction=Regular, cutted=0\"];}",
                                           "0;1;3; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=1\"];5->7[label=\" direction=Regular, cutted=1\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=1\"];7->2[label=\" direction=Regular, cutted=1\"];7->4[label=\" direction=Regular, cutted=0\"];}",
                                           "1;1;3; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=1\"];7->2[label=\" direction=Regular, cutted=1\"];7->4[label=\" direction=Regular, cutted=0\"];}"};

        std::vector<std::string> received;
        MachineGraphGenerator generator(complexValve);
        while(generator.calculateNext()) {
            received.push_back(generator.getStateStr() + " : " + generator.getActualGraphGenerated()->toString());
        }

        QVERIFY2(spected.size() == received.size(), std::string("Expected number of generated graph is not the same as the calculated ones, " +
                 std::to_string(spected.size()) + "!=" + std::to_string(received.size())).c_str());

        for(int i = 0; i < spected.size(); i++) {
            QVERIFY2(spected[i].compare(received[i]) == 0, std::string(std::to_string(i) + " element is not as expected: " +
                                                                       spected[i] + "!=" + received[i]).c_str());
        }
    }  catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }

}

void MachineGraphGeneratorTest::complexValveGraph_bidirectional() {
    try {
        std::unordered_map<std::string, int> nodeMap;
        std::shared_ptr<MachineGraph> complexValve = std::make_shared<MachineGraph>(makeComplexValveGraph_bidirectional(nodeMap));

        std::vector<std::string> spected = {"-1;-1;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Inverted, cutted=0\"];5->7[label=\" direction=Inverted, cutted=0\"];1->6[label=\" direction=Inverted, cutted=0\"];6->7[label=\" direction=Inverted, cutted=0\"];7->3[label=\" direction=Inverted, cutted=0\"];7->2[label=\" direction=Inverted, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "0;-1;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Unknow, cutted=1\"];5->7[label=\" direction=Unknow, cutted=1\"];1->6[label=\" direction=Inverted, cutted=0\"];6->7[label=\" direction=Inverted, cutted=0\"];7->3[label=\" direction=Unknow, cutted=0\"];7->2[label=\" direction=Inverted, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "1;-1;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Inverted, cutted=0\"];6->7[label=\" direction=Inverted, cutted=0\"];7->3[label=\" direction=Regular, cutted=0\"];7->2[label=\" direction=Inverted, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "-1;0;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Inverted, cutted=0\"];5->7[label=\" direction=Inverted, cutted=0\"];1->6[label=\" direction=Unknow, cutted=1\"];6->7[label=\" direction=Unknow, cutted=1\"];7->3[label=\" direction=Inverted, cutted=0\"];7->2[label=\" direction=Unknow, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "1;0;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Unknow, cutted=1\"];6->7[label=\" direction=Unknow, cutted=1\"];7->3[label=\" direction=Regular, cutted=0\"];7->2[label=\" direction=Unknow, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "-1;1;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Inverted, cutted=0\"];5->7[label=\" direction=Inverted, cutted=0\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Inverted, cutted=0\"];7->2[label=\" direction=Regular, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "0;1;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Unknow, cutted=1\"];5->7[label=\" direction=Unknow, cutted=1\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Unknow, cutted=0\"];7->2[label=\" direction=Regular, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "1;1;1; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=0\"];7->2[label=\" direction=Regular, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "-1;-1;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Inverted, cutted=0\"];5->7[label=\" direction=Inverted, cutted=0\"];1->6[label=\" direction=Inverted, cutted=0\"];6->7[label=\" direction=Inverted, cutted=0\"];7->3[label=\" direction=Inverted, cutted=0\"];7->2[label=\" direction=Inverted, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "0;-1;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Unknow, cutted=1\"];5->7[label=\" direction=Unknow, cutted=1\"];1->6[label=\" direction=Inverted, cutted=0\"];6->7[label=\" direction=Inverted, cutted=0\"];7->3[label=\" direction=Inverted, cutted=0\"];7->2[label=\" direction=Unknow, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "1;-1;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Inverted, cutted=0\"];6->7[label=\" direction=Inverted, cutted=0\"];7->3[label=\" direction=Inverted, cutted=0\"];7->2[label=\" direction=Regular, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "-1;0;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Inverted, cutted=0\"];5->7[label=\" direction=Inverted, cutted=0\"];1->6[label=\" direction=Unknow, cutted=1\"];6->7[label=\" direction=Unknow, cutted=1\"];7->3[label=\" direction=Unknow, cutted=0\"];7->2[label=\" direction=Inverted, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "1;0;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Unknow, cutted=1\"];6->7[label=\" direction=Unknow, cutted=1\"];7->3[label=\" direction=Unknow, cutted=0\"];7->2[label=\" direction=Regular, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "-1;1;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Inverted, cutted=0\"];5->7[label=\" direction=Inverted, cutted=0\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=0\"];7->2[label=\" direction=Inverted, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "0;1;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Unknow, cutted=1\"];5->7[label=\" direction=Unknow, cutted=1\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=0\"];7->2[label=\" direction=Unknow, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "1;1;2; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=0\"];7->2[label=\" direction=Regular, cutted=0\"];7->4[label=\" direction=Unknow, cutted=1\"];}",
                                           "-1;-1;3; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Inverted, cutted=0\"];5->7[label=\" direction=Inverted, cutted=0\"];1->6[label=\" direction=Inverted, cutted=0\"];6->7[label=\" direction=Inverted, cutted=0\"];7->3[label=\" direction=Inverted, cutted=1\"];7->2[label=\" direction=Inverted, cutted=1\"];7->4[label=\" direction=Inverted, cutted=0\"];}",
                                           "0;-1;3; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Inverted, cutted=1\"];5->7[label=\" direction=Inverted, cutted=1\"];1->6[label=\" direction=Inverted, cutted=0\"];6->7[label=\" direction=Inverted, cutted=0\"];7->3[label=\" direction=Inverted, cutted=1\"];7->2[label=\" direction=Inverted, cutted=1\"];7->4[label=\" direction=Inverted, cutted=0\"];}",
                                           "-1;0;3; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Inverted, cutted=0\"];5->7[label=\" direction=Inverted, cutted=0\"];1->6[label=\" direction=Inverted, cutted=1\"];6->7[label=\" direction=Inverted, cutted=1\"];7->3[label=\" direction=Inverted, cutted=1\"];7->2[label=\" direction=Inverted, cutted=1\"];7->4[label=\" direction=Inverted, cutted=0\"];}",
                                           "1;0;3; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Regular, cutted=1\"];6->7[label=\" direction=Regular, cutted=1\"];7->3[label=\" direction=Regular, cutted=1\"];7->2[label=\" direction=Regular, cutted=1\"];7->4[label=\" direction=Regular, cutted=0\"];}",
                                           "0;1;3; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=1\"];5->7[label=\" direction=Regular, cutted=1\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=1\"];7->2[label=\" direction=Regular, cutted=1\"];7->4[label=\" direction=Regular, cutted=0\"];}",
                                           "1;1;3; : digraph{0;1;2;3;4;5;6;7;0->5[label=\" direction=Regular, cutted=0\"];5->7[label=\" direction=Regular, cutted=0\"];1->6[label=\" direction=Regular, cutted=0\"];6->7[label=\" direction=Regular, cutted=0\"];7->3[label=\" direction=Regular, cutted=1\"];7->2[label=\" direction=Regular, cutted=1\"];7->4[label=\" direction=Regular, cutted=0\"];}"};

        std::vector<std::string> received;
        MachineGraphGenerator generator(complexValve);
        while(generator.calculateNext()) {
            received.push_back(generator.getStateStr() + " : " + generator.getActualGraphGenerated()->toString());

            qDebug() << generator.getStateStr().c_str() << ":" << generator.getActualGraphGenerated()->toString().c_str();
        }

        QVERIFY2(spected.size() == received.size(), std::string("Expected number of generated graph is not the same as the calculated ones, " +
                 std::to_string(spected.size()) + "!=" + std::to_string(received.size())).c_str());

        for(int i = 0; i < spected.size(); i++) {
            QVERIFY2(spected[i].compare(received[i]) == 0, std::string(std::to_string(i) + " element is not as expected: " +
                                                                       spected[i] + "!=" + received[i]).c_str());
        }
    }  catch (std::exception & e) {
        QFAIL(std::string("exception ocurred, message " + std::string(e.what())).c_str());
    }

}

void MachineGraphGeneratorTest::multipathMachine() {
    std::shared_ptr<MachineGraph> machine = makeMultipathWashMachineGraph();

    MachineGraphGenerator generator(machine);

    int i = 0;
    while(generator.calculateNext()) {
        i++;
    }
    qDebug() << i;
}

std::shared_ptr<MachineGraph> MachineGraphGeneratorTest::makeMachineGraph() {
    std::shared_ptr<MachineGraph> mGraph = std::make_shared<MachineGraph>();
    PluginConfiguration config;
    std::shared_ptr<PluginAbstractFactory> factory = nullptr;

    std::shared_ptr<Function> pumpf = std::make_shared<PumpPluginFunction>(factory, config);
    std::shared_ptr<Function> routef = std::make_shared<ValvePluginRouteFunction>(factory, config);

    int c0 = mGraph->emplaceContainer(2, open, 100.0);
    int c1 = mGraph->emplaceContainer(2, open, 100.0);
    int c2 = mGraph->emplaceContainer(2, close, 100.0);
    int c3 = mGraph->emplaceContainer(2, open, 100.0);

    int p = mGraph->emplacePump(2, PumpNode::bidirectional, pumpf);

    ValveNode::TruthTable table;
    std::vector<std::unordered_set<int>> empty;
    table.insert(std::make_pair(0, empty));
    std::vector<std::unordered_set<int>> pos1 = {{0,2}};
    table.insert(std::make_pair(1, pos1));
    std::vector<std::unordered_set<int>> pos2 = {{1,2}};
    table.insert(std::make_pair(2, pos2));

    int v= mGraph->emplaceValve(3, table, routef);

    mGraph->connectNodes(c0,v,1,0);
    mGraph->connectNodes(c1,v,1,1);
    mGraph->connectNodes(v,c2,2,0);
    mGraph->connectNodes(c2,p,1,0);
    mGraph->connectNodes(p,c3,1,0);

    return mGraph;
}

MachineGraph MachineGraphGeneratorTest::makeComplexValveGraph_unidirectional(std::unordered_map<std::string, int> & containerMap) {
    MachineGraph mGraph;
    PluginConfiguration config;
    std::shared_ptr<PluginAbstractFactory> factory = nullptr;

    std::shared_ptr<Function> pumpf = std::make_shared<PumpPluginFunction>(factory, config);
    std::shared_ptr<Function> routef = std::make_shared<ValvePluginRouteFunction>(factory, config);

    int c0 = mGraph.emplaceContainer(2, open, 100.0);
    int c1 = mGraph.emplaceContainer(2, open, 100.0);
    int c2 = mGraph.emplaceContainer(2, open, 100.0);
    int c3 = mGraph.emplaceContainer(2, open, 100.0);
    int c4 = mGraph.emplaceContainer(2, open, 100.0);

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

MachineGraph MachineGraphGeneratorTest::makeComplexValveGraph_bidirectional(std::unordered_map<std::string, int> & containerMap) {
    MachineGraph mGraph;
    PluginConfiguration config;
    std::shared_ptr<PluginAbstractFactory> factory = nullptr;

    std::shared_ptr<Function> pumpf = std::make_shared<PumpPluginFunction>(factory, config);
    std::shared_ptr<Function> routef = std::make_shared<ValvePluginRouteFunction>(factory, config);

    int c0 = mGraph.emplaceContainer(2, open, 100.0);
    int c1 = mGraph.emplaceContainer(2, open, 100.0);
    int c2 = mGraph.emplaceContainer(2, open, 100.0);
    int c3 = mGraph.emplaceContainer(2, open, 100.0);
    int c4 = mGraph.emplaceContainer(2, open, 100.0);

    int p1 = mGraph.emplacePump(2, PumpNode::bidirectional, pumpf);
    int p2 = mGraph.emplacePump(2, PumpNode::bidirectional, pumpf);

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

std::shared_ptr<MachineGraph> MachineGraphGeneratorTest::makeMultipathWashMachineGraph() {
    std::shared_ptr<MachineGraph> mGraph = std::make_shared<MachineGraph>();

    PluginConfiguration config;
    std::shared_ptr<PluginAbstractFactory> factory = nullptr;

    std::shared_ptr<Function> pumpf = std::make_shared<PumpPluginFunction>(factory, config);
    std::shared_ptr<Function> routef = std::make_shared<ValvePluginRouteFunction>(factory, config);

    int sample = mGraph->emplaceContainer(1, open, 100.0);
    int media = mGraph->emplaceContainer(1, open, 100.0);
    int waste = mGraph->emplaceContainer(4, open, 100.0);
    int water = mGraph->emplaceContainer(1, open, 100.0);
    int ethanol = mGraph->emplaceContainer(1, open, 100.0);
    int naoh = mGraph->emplaceContainer(1, open, 100.0);

    int chemo = mGraph->emplaceContainer(3, close, 100.0);
    int cell = mGraph->emplaceContainer(3, close, 100.0);

    int p1 = mGraph->emplacePump(2, PumpNode::unidirectional, pumpf);
    int p2 = mGraph->emplacePump(3, PumpNode::unidirectional, pumpf);

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

    int v1 = mGraph->emplaceValve(2, tableType1, routef);
    int v5 = mGraph->emplaceValve(2, tableType1, routef);
    int v4 = mGraph->emplaceValve(2, tableType1, routef);

    int v2 = mGraph->emplaceValve(3, tableType2, routef);
    int v3 = mGraph->emplaceValve(3, tableType2, routef);
    int v6 = mGraph->emplaceValve(3, tableType2, routef);
    int v7 = mGraph->emplaceValve(3, tableType2, routef);

    int v8 = mGraph->emplaceValve(4, tableType3, routef);

    mGraph->connectNodes(media,p1,0,0);
    mGraph->connectNodes(p1,chemo,1,0);
    mGraph->connectNodes(chemo,v3,1,2);
    mGraph->connectNodes(chemo,v4,2,0);
    mGraph->connectNodes(v4,waste,1,1);
    mGraph->connectNodes(v3,v2,0,2);
    mGraph->connectNodes(v3,v7,1,0);
    mGraph->connectNodes(v7,waste,1,2);
    mGraph->connectNodes(v7,p2,2,1);
    mGraph->connectNodes(p2,v8,2,0);
    mGraph->connectNodes(v8,water,1,0);
    mGraph->connectNodes(v8,ethanol,2,0);
    mGraph->connectNodes(v8,naoh,3,0);
    mGraph->connectNodes(v2,cell,0,1);
    mGraph->connectNodes(v2,v6,1,0);
    mGraph->connectNodes(v6,waste,1,3);
    mGraph->connectNodes(v6,p2,2,0);
    mGraph->connectNodes(cell,v1,0,1);
    mGraph->connectNodes(cell,v5,2,0);
    mGraph->connectNodes(v1,sample,0,0);
    mGraph->connectNodes(v5,waste,1,0);

    return mGraph;
}

QTEST_APPLESS_MAIN(MachineGraphGeneratorTest)

#include "tst_machinegraphgeneratortest.moc"
