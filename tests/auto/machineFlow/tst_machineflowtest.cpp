#include <QString>
#include <QtTest>

#include <sstream>
#include <utils/machineflow.h>

class MachineFlowTest : public QObject
{
    Q_OBJECT

public:
    MachineFlowTest();

private:
    void printStack(const MachineFlow::FlowsVector & flows, std::stringstream & stream);

private Q_SLOTS:
    void testMachineFlow_T1();
    void testMachineFlow_T2();
    void testMachineFlow_T3();
    void testMachineFlow_T4();
    void testMachineFlow_T5();
};

MachineFlowTest::MachineFlowTest()
{
}

void MachineFlowTest::testMachineFlow_T1()
{
    std::stringstream stream;
    MachineFlow flow;

    flow.addFlow(1,2,300.0);
    flow.addFlow(2,4,300.0);
    flow.addFlow(4,5,300.0);

    printStack(flow.updateFlows(), stream);
    QVERIFY2(stream.str().compare("1245:300;") == 0,
             std::string("T1.1- stack must be \"1245:300;\" and is" + stream.str()).c_str());
    stream.str(std::string());
    stream.clear();

    flow.addFlow(1,2,0.0);
    flow.addFlow(1,3,300.0);
    flow.addFlow(2,4,0.0);
    flow.addFlow(3,4,300.0);
    flow.addFlow(4,5,0.0);
    flow.addFlow(4,5,300.0);

    printStack(flow.updateFlows(), stream);
    QVERIFY2(stream.str().compare("1345:300;") == 0,
             std::string("T1.2- stack must be \"1345:300;\" and is" + stream.str()).c_str());
    stream.str(std::string());
    stream.clear();

    flow.addFlow(1,2,300.0);
    flow.addFlow(2,4,300.0);
    flow.addFlow(4,5,300.0);

    printStack(flow.updateFlows(), stream);
    QVERIFY2(stream.str().compare("1345:300;1245:300;") == 0,
             std::string("T1.3- stack must be \"1345:300;1245:300;\" and is" + stream.str()).c_str());
    stream.str(std::string());
    stream.clear();

    flow.clear();

    printStack(flow.updateFlows(), stream);
    QVERIFY2(stream.str().compare("") == 0,
             std::string("T1.4- stack must be \"\" and is" + stream.str()).c_str());
    stream.str(std::string());
    stream.clear();
}

void MachineFlowTest::testMachineFlow_T2() {
    std::stringstream stream;
    MachineFlow flow;

    flow.addFlow(1,2,300.0);
    flow.addFlow(3,4,300.0);
    flow.addFlow(2,4,300.0);
    flow.addFlow(1,3,300.0);
    flow.addFlow(4,5,300.0);
    flow.addFlow(4,5,300.0);

    printStack(flow.updateFlows(), stream);
    QVERIFY2(stream.str().compare("1245:300;1345:300;") == 0,
             std::string("T2- stack must be \"1245:300;1345:300;\" and is" + stream.str()).c_str());
    stream.str(std::string());
    stream.clear();
}


void MachineFlowTest::testMachineFlow_T3() {
    std::stringstream stream;
    MachineFlow flow;

    flow.addFlow(1,2,300.0);
    flow.addFlow(3,4,300.0);
    flow.addFlow(4,5,300.0);
    flow.addFlow(1,3,300.0);
    flow.addFlow(4,5,300.0);
    flow.addFlow(2,4,300.0);

    printStack(flow.updateFlows(), stream);
    QVERIFY2(stream.str().compare("1345:300;1245:300;") == 0,
             std::string("T3- stack must be \"1345:300;1245:300;\" and is" + stream.str()).c_str());
    stream.str(std::string());
    stream.clear();

}

void MachineFlowTest::testMachineFlow_T4() {
    std::stringstream stream;
    MachineFlow flow;

    flow.addFlow(1,3,300.0);
    flow.addFlow(4,5,300.0);
    flow.addFlow(4,5,300.0);
    flow.addFlow(3,4,300.0);
    flow.addFlow(2,4,300.0);
    flow.addFlow(1,2,300.0);

    printStack(flow.updateFlows(), stream);
    QVERIFY2(stream.str().compare("1245:300;1345:300;") == 0,
             std::string("T4- stack must be \"1245:300;1345:300;\" and is" + stream.str()).c_str());
    stream.str(std::string());
}

void MachineFlowTest::testMachineFlow_T5() {
    std::stringstream stream;
    MachineFlow flow;

    flow.addFlow(1,2,300.0 * units::ml/units::hr);
    flow.addFlow(2,3,300.0 * units::ml/units::hr);

    printStack(flow.updateFlows(), stream);
    qDebug() << stream.str().c_str();

    stream.str(std::string());
    stream.clear();

    flow.removeFlow(1,2);
    //flow.addFlow(4,2,300.0 * units::ml/units::hr);

    printStack(flow.updateFlows(), stream);
    qDebug() << stream.str().c_str();

    stream.str(std::string());
    stream.clear();
}

void MachineFlowTest::printStack(const MachineFlow::FlowsVector & flows, std::stringstream & stream) {
    for (auto tuple: flows) {
        for(auto id: std::get<0>(tuple)) {
            stream << id;
        }
        stream << ":" << std::get<1>(tuple) << ";";
    }
}

QTEST_APPLESS_MAIN(MachineFlowTest)

#include "tst_machineflowtest.moc"
