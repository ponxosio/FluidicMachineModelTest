#include <QString>
#include <QtTest>

#include <fluidicmachinemodel/machine_graph_utils/machinestate.h>

class MachinesateTest : public QObject
{
    Q_OBJECT

public:
    MachinesateTest();

private Q_SLOTS:
    void addStates();
};

MachinesateTest::MachinesateTest()
{
}

void MachinesateTest::addStates()
{
    int id = 2;
    std::vector<long long> statesVector = {230050, 820000};

    MachineState mState(3,2);
    for(long long state: statesVector) {
        mState.emplaceContainerVar(id);
        mState.addContainerState(id, state);
    }
    QVERIFY2(mState.getContainerState(id) == 1050050,
             std::string("states not equal: " + std::to_string(mState.getContainerState(id)) + "!= 1050050").c_str());
}

QTEST_APPLESS_MAIN(MachinesateTest)

#include "tst_machinesatetest.moc"
