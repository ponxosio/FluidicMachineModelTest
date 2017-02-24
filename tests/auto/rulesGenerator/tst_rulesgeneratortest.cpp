#include <QString>
#include <QtTest>
#include <QTemporaryDir>

#include <fstream>

#include <machinegraph.h>

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

#include "stringtranslationstack.h"
#include "prologtranslationstack.h"
#include "prologexecutor.h"

class RulesGeneratorTest : public QObject
{
    Q_OBJECT

public:
    RulesGeneratorTest();
private:
    std::shared_ptr<MachineGraph> makeMachineGraph();
    std::shared_ptr<MachineGraph> makeMultipathWashMachineGraph();
    MachineGraph makeComplexValveGraph(std::unordered_map<std::string, int> & containerMap);
    std::shared_ptr<MachineGraph> makeComplexPumpGraph(std::unordered_map<std::string, int> & containerMap);

    void savePrologFile(const QString & path, PrologTranslationStack stack) throw (std::runtime_error);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();

    void checkSimpleGraph_varDomain();
    void checkSimpleGraph_prologRules();
    void checkComplexValveGraph_prologRules();
    void checkComplexPumpGraph_prologRules();
    void checkMultipathMachine_prologRules();

    void temporalTest_samePropertyTube();
};

RulesGeneratorTest::RulesGeneratorTest()
{
}

void RulesGeneratorTest::checkSimpleGraph_prologRules()
{
    try {
        PrologTranslationStack stack;
        std::shared_ptr<MachineGraph> graphPtr = makeMachineGraph();

        GraphRulesGenerator rulesGenerator(graphPtr, 3, 0);
        for (const std::shared_ptr<Rule> & rule : rulesGenerator.getRules()) {
            rule->fillTranslationStack(&stack);
            stack.addHeadToRestrictions();
        }

        QTemporaryDir temp;
        if (temp.isValid()) {
            QString generatedFilePath = temp.path() + "/simpleMachine.pl";
            savePrologFile(generatedFilePath, stack);

            PrologExecutor executor(generatedFilePath.toStdString(), stack.getVarTable());

            //test c_0->c_3
            std::unordered_map<std::string, int> actualState;
            actualState["C_0"] = -1300;
            actualState["C_3"] = 1300;

            std::unordered_map<std::string, int> newState;
            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C_0 -> C_3");

            std::unordered_map<std::string, int> expected;
            expected["V_5"] = 1;
            expected["P_4"] = 1;
            expected["R_4"] = 300;

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C0 -> C3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            //test c_0<-c_3
            actualState["C_0"] = 4300;
            actualState["C_3"] = -4300;

            expected["P_4"] = -1;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C0 <- C3");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C0 <- C3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 -> c_3
            actualState["C_1"] = -2300;
            actualState["C_3"] = 2300;

            expected["V_5"] = 2;
            expected["P_4"] = 1;
            expected["R_4"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C1 -> C3");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C1 -> C3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 <- c_3
            actualState["C_1"] = 4300;
            actualState["C_3"] = -4300;

            expected["V_5"] = 2;
            expected["P_4"] = -1;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C1 <- C3");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C1 <- C3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 | c_0 -> c_3
            actualState["C_0"] = -1300;
            actualState["C_1"] = -2300;
            actualState["C_3"] = 3300;

            expected.clear();
            expected["V_5"] = 3;
            expected["P_4"] = 1;
            expected["R_4"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow c_1 | c_0 -> c_3");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow c_1 | c_0 -> c_3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 | c_0 <- c_3
            actualState["C_1"] = 4300;
            actualState["C_0"] = 4300;
            actualState["C_3"] = -4300;

            expected.clear();
            expected["V_5"] = 3;
            expected["P_4"] = -1;
            expected["R_4"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow c_1 | c_0 <- c_3");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow c_1 | c_0 <- c_3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            //impossible rates test c_1 | c_0 -> c_3 diff rate
            actualState["C_1"] = -2300;
            actualState["C_0"] = -1400;
            actualState["C_3"] = -3700;

            QVERIFY2(!executor.executePredicate(actualState, newState), "posible to do flow c_1 | c_0 -> c_3 diff rate");

            //impossible path test c_0 -> c_1
            actualState["C_1"] = -1300;
            actualState["C_0"] = 1300;

            QVERIFY2(!executor.executePredicate(actualState, newState), "posible to do flow c_1 <- c_0");


        } else {
            QFAIL("QT TEST ERROR: error creating temporal directory");
        }
    } catch (std::exception & e) {
        QFAIL(std::string("exception: " + std::string(e.what())).c_str());
    } catch (PlException &ex ) {
        QFAIL(std::string("prolog exception: " + std::string((char*) ex)).c_str());
    }
}

void RulesGeneratorTest::checkSimpleGraph_varDomain() {
    try {
        StringTranslationStack stack;
        std::shared_ptr<MachineGraph> machine = makeMachineGraph();

        GraphRulesGenerator rulesGenerator(machine,3,0);
        for (const std::shared_ptr<Rule> & rule : rulesGenerator.getRules()) {
            rule->fillTranslationStack(&stack);
            stack.addHeadToRestrictions();
        }

        std::vector<std::string> expectedValues = {"P_4=[-1,1]",
                                                   "R_4=[0,999]",
                                                   "V_5=[0,3]",
                                                   "C_0=[-6999,-6001][-4999,-4001][-2999,-2001][-1999,-1001][0,0][1001,1999][2001,2999][4001,4999][6001,6999]",
                                                   "C_1=[-5999,-5001][-4999,-4001][-2999,-2001][-1999,-1001][0,0][1001,1999][2001,2999][4001,4999][5001,5999]",
                                                   "C_3=[-4999,-4001][-3999,-3001][-2999,-2001][-1999,-1001][0,0][1001,1999][2001,2999][3001,3999][4001,4999]",
                                                   "C_2=[-6999,-6001][-5999,-5001][-4999,-4001][-3999,-3001][-2999,-2001][-1999,-1001][0,0][1001,1999][2001,2999][3001,3999][4001,4999][5001,5999][6001,6999]",
                                                   "T_0_5=[-6999,-6001][-5999,-5001][-4999,-4001][-3999,-3001][-2999,-2001][-1999,-1001][0,0][1001,1999][2001,2999][3001,3999][4001,4999][5001,5999][6001,6999]",
                                                   "T_1_5=[-6999,-6001][-5999,-5001][-4999,-4001][-3999,-3001][-2999,-2001][-1999,-1001][0,0][1001,1999][2001,2999][3001,3999][4001,4999][5001,5999][6001,6999]",
                                                   "T_5_2=[-6999,-6001][-5999,-5001][-4999,-4001][-3999,-3001][-2999,-2001][-1999,-1001][0,0][1001,1999][2001,2999][3001,3999][4001,4999][5001,5999][6001,6999]",
                                                   "T_2_4=[-6999,-6001][-5999,-5001][-4999,-4001][-3999,-3001][-2999,-2001][-1999,-1001][0,0][1001,1999][2001,2999][3001,3999][4001,4999][5001,5999][6001,6999]",
                                                   "T_4_3=[-6999,-6001][-5999,-5001][-4999,-4001][-3999,-3001][-2999,-2001][-1999,-1001][0,0][1001,1999][2001,2999][3001,3999][4001,4999][5001,5999][6001,6999]"
                                                  };

        const std::vector<std::string> & translatedRulesVector = stack.getTranslatedRestriction();
        for(const std::string & rule: translatedRulesVector) {
            qDebug() << rule.c_str();
        }

        QVERIFY2(translatedRulesVector.size() == expectedValues.size() + 1,
                 std::string("expected rules size:" + std::to_string(expectedValues.size()) + ", received: " +
                             std::to_string(translatedRulesVector.size())).c_str());

        for(int i = 0; i < expectedValues.size(); i++) {
            QVERIFY2(translatedRulesVector[i].compare(expectedValues[i]) == 0,
                     std::string("i: " + std::to_string(i) +".Expected rule: " + expectedValues[i] + ", received: " + translatedRulesVector[i]).c_str());
        }
    } catch (std::exception & e) {
        QFAIL(std::string("exception: " + std::string(e.what())).c_str());
    }
}

void RulesGeneratorTest::checkComplexValveGraph_prologRules() {
    try {
        PrologTranslationStack stack;
        std::unordered_map<std::string, int> nodeMap;
        std::shared_ptr<MachineGraph> complexValve = std::make_shared<MachineGraph>(makeComplexValveGraph(nodeMap));

        GraphRulesGenerator rulesGenerator(complexValve, 3, 0);
        for (const std::shared_ptr<Rule> & rule : rulesGenerator.getRules()) {
            rule->fillTranslationStack(&stack);
            stack.addHeadToRestrictions();
        }

        QTemporaryDir temp;
        if (temp.isValid()) {
            QString generatedFilePath = /*temp.path() +*/ "X:/complexValve.pl";
            savePrologFile(generatedFilePath, stack);

            PrologExecutor executor(generatedFilePath.toStdString(), stack.getVarTable());

            //test c_0->c_3
            std::unordered_map<std::string, int> actualState;
            actualState["C_0"] = -1300;
            actualState["C_3"] = 1300;

            std::unordered_map<std::string, int> newState;
            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C_0 -> C_3");

            std::unordered_map<std::string, int> expected;
            expected["V_7"] = 1;
            expected["P_5"] = 1;
            expected["R_5"] = 300;
            expected["P_6"] = 0;
            expected["R_6"] = 0;

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C0 -> C3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            //test c_0<-c_3
            actualState["C_0"] = 8300;
            actualState["C_3"] = -8300;

            expected["P_5"] = -1;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C0 <- C3");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C0 <- C3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_0 -> c_2
            actualState["C_0"] = -1300;
            actualState["C_2"] = 1300;

            expected["P_5"] = 1;
            expected["V_7"] = 2;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C0 -> C2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C0 -> C2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_0 <- c_2
            actualState["C_0"] = 4300;
            actualState["C_2"] = -4300;

            expected["P_5"] = -1;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C0 <- C2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C0 <- C2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 -> c_2
            actualState["C_1"] = -2300;
            actualState["C_2"] = 2300;

            expected.clear();
            expected["V_7"] = 1;
            expected["P_5"] = 0;
            expected["R_5"] = 0;
            expected["P_6"] = 1;
            expected["R_6"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C1 -> C2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C1 -> C2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 <- c_2
            actualState["C_1"] = 4300;
            actualState["C_2"] = -4300;

            expected["P_6"] = -1;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C1 <- C2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C1 <- C2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 -> c_3
            actualState["C_1"] = -2300;
            actualState["C_3"] = 2300;

            expected["V_7"] = 2;
            expected["P_6"] = 1;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C1 -> C3");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C1 -> C3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 <- c_3
            actualState["C_1"] = 8300;
            actualState["C_3"] = -8300;

            expected["V_7"] = 2;
            expected["P_6"] = -1;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow C1 <- C3");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow C1 <- C3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 | c_0 -> c_4
            actualState["C_0"] = -1300;
            actualState["C_1"] = -2400;
            actualState["C_4"] = 3700;

            expected.clear();
            expected["V_7"] = 4;
            expected["P_5"] = 1;
            expected["R_5"] = 300;
            expected["P_6"] = 1;
            expected["R_6"] = 400;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow c_1 | c_0 -> c_4");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow c_1 | c_0 -> c_4. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 | c_0 <- c_4
            actualState["C_1"] = 16300;
            actualState["C_0"] = 16300;
            actualState["C_4"] = -16300;

            expected.clear();
            expected["V_7"] = 4;
            expected["P_5"] = -1;
            expected["R_5"] = 300;
            expected["P_6"] = -1;
            expected["R_6"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow c_1 | c_0 <- c_4");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow c_1 | c_0 <- c_4. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            //impossible rates test c_1 | c_0 <- c_4 diff rate
            actualState["C_1"] = 16300;
            actualState["C_0"] = 16400;
            actualState["C_4"] = -16700;

            QVERIFY2(!executor.executePredicate(actualState, newState), "posible to do flow c_1 | c_0 <- c_4 diff rate");

            //impossible path test c_0 -> c_1
            actualState["C_1"] = -1300;
            actualState["C_0"] = 1300;

            QVERIFY2(!executor.executePredicate(actualState, newState), "posible to do flow c_1 <- c_0");


        } else {
            QFAIL("QT TEST ERROR: error creating temporal directory");
        }
    } catch (std::exception & e) {
        QFAIL(std::string("exception: " + std::string(e.what())).c_str());
    } catch (PlException &ex ) {
        QFAIL(std::string("prolog exception: " + std::string((char *) ex)).c_str());
    }
}

void RulesGeneratorTest::checkMultipathMachine_prologRules() {
    try {
        PrologTranslationStack stack;
        std::shared_ptr<MachineGraph> complexValve = makeMultipathWashMachineGraph();

        GraphRulesGenerator rulesGenerator(complexValve, 3, 0);
        int i = 0;
        for (const std::shared_ptr<Rule> & rule : rulesGenerator.getRules()) {
            rule->fillTranslationStack(&stack);
            stack.addHeadToRestrictions();
            i++;
        }

        QTemporaryDir temp;
        if (temp.isValid()) {
            QString generatedFilePath = "X:/multipathValve.pl";
            savePrologFile(generatedFilePath, stack);
        }else {
            QFAIL("QT TEST ERROR: error creating temporal directory");
        }
    } catch (std::exception & e) {
        QFAIL(std::string("exception: " + std::string(e.what())).c_str());
    }
}

void RulesGeneratorTest::checkComplexPumpGraph_prologRules() {
    try{
        PrologTranslationStack stack;
        std::unordered_map<std::string, int> nodeMap;
        std::shared_ptr<MachineGraph> graphPtr = makeComplexPumpGraph(nodeMap);

        GraphRulesGenerator rulesGenerator(graphPtr, 3, 0);
        for (const std::shared_ptr<Rule> & rule : rulesGenerator.getRules()) {
            rule->fillTranslationStack(&stack);
            stack.addHeadToRestrictions();
        }

        QTemporaryDir temp;
        if (temp.isValid()) {
            QString generatedFilePath = /*temp.path() +*/ "X:/simpleMachine.pl";
            savePrologFile(generatedFilePath, stack);

            PrologExecutor executor(generatedFilePath.toStdString(), stack.getVarTable());

            //test c_0->c_2
            std::unordered_map<std::string, int> actualState;
            actualState["C_0"] = -1300;
            actualState["C_2"] = 1300;

            std::unordered_map<std::string, int> newState;
            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow c_0->c_2");

            std::unordered_map<std::string, int> expected;
            expected["V_5"] = 1;
            expected["V_6"] = 0;
            expected["V_7"] = 0;
            expected["P_4"] = 1;
            expected["R_4"] = 300;

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow c_0->c_2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_0 <- c_2
            actualState["C_0"] = 4300;
            actualState["C_2"] = -4300;

            expected.clear();
            expected["V_5"] = 1;
            expected["V_6"] = 0;
            expected["V_7"] = 0;
            expected["P_4"] = -1;
            expected["R_4"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow c_0 <- c_2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow c_0 <- c_2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_0 -> c_3
            actualState["C_0"] = -1300;
            actualState["C_3"] = 1300;

            expected.clear();
            expected["V_5"] = 1;
            expected["V_6"] = 0;
            expected["V_7"] = 1;
            expected["P_4"] = 1;
            expected["R_4"] = 300;
            expected["C_2"] = 1300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow c_0 -> c_3");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow c_0 -> c_3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_0 <- c_3 //fail
            actualState["C_0"] = 8300;
            actualState["C_3"] = -8300;

            QVERIFY2(!executor.executePredicate(actualState, newState), "posible to do flow c_0 <- c_3");

            // test c_0 <- c_3 | c_2
            actualState["C_0"] = 12300;
            actualState["C_3"] = -8300;
            actualState["C_2"] = -4300;

            expected.clear();
            expected["V_5"] = 1;
            expected["V_6"] = 0;
            expected["V_7"] = 1;
            expected["P_4"] = -1;
            expected["R_4"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow c_0 <- c_3 | c_2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow c_0 <- c_3 | c_2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 -> c_2
            actualState["C_1"] = -2300;
            actualState["C_2"] = 2300;

            expected.clear();
            expected["V_5"] = 0;
            expected["V_6"] = 1;
            expected["V_7"] = 0;
            expected["P_4"] = 1;
            expected["R_4"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow test c_1 -> c_2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow test c_1 -> c_2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 <- c_2
            actualState["C_1"] = 4300;
            actualState["C_2"] = -4300;

            expected.clear();
            expected["V_5"] = 0;
            expected["V_6"] = 1;
            expected["V_7"] = 0;
            expected["P_4"] = -1;
            expected["R_4"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow test c_1 <- c_2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow test c_1 <- c_2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 -> c_3
            actualState["C_1"] = -2300;
            actualState["C_3"] = 2300;

            expected.clear();
            expected["V_5"] = 0;
            expected["V_6"] = 1;
            expected["V_7"] = 1;
            expected["P_4"] = 1;
            expected["R_4"] = 300;
            expected["C_2"] = 2300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow test c_1 -> c_3");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow test c_1 -> c_3. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_1 <- c_3 //fail
            actualState["C_1"] = 8300;
            actualState["C_3"] = -8300;

            QVERIFY2(!executor.executePredicate(actualState, newState), "posible to do flow c_1 <- c_3");

            // test c_1 <- c_3 | c_2
            actualState["C_1"] = 12300;
            actualState["C_3"] = -8300;
            actualState["C_2"] = -4300;

            expected.clear();
            expected["V_5"] = 0;
            expected["V_6"] = 1;
            expected["V_7"] = 1;
            expected["P_4"] = -1;
            expected["R_4"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow test c_1 <- c_3 | c_2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow test c_1 <- c_3 | c_2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_0 | c_1 -> c_3 | c_2
            actualState["C_0"] = -1300;
            actualState["C_1"] = -2300;
            actualState["C_3"] = 3300;
            actualState["C_2"] = 3300;

            expected.clear();
            expected["V_5"] = 1;
            expected["V_6"] = 1;
            expected["V_7"] = 1;
            expected["P_4"] = 1;
            expected["R_4"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow test c_0 | c_1 -> c_3 | c_2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow test c_0 | c_1 -> c_3 | c_2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_0 | c_1 <- c_3 | c_2
            actualState["C_0"] = 12300;
            actualState["C_1"] = 12300;
            actualState["C_3"] = -8300;
            actualState["C_2"] = -4300;

            expected.clear();
            expected["V_5"] = 1;
            expected["V_6"] = 1;
            expected["V_7"] = 1;
            expected["P_4"] = -1;
            expected["R_4"] = 300;

            QVERIFY2(executor.executePredicate(actualState, newState), "imposible to do flow test c_0 | c_1 <- c_3 | c_2");

            for(auto pairExpected: expected) {
                std::string expectedName = pairExpected.first;
                int expectedValue = pairExpected.second;
                int calculatedValue = newState[expectedName];

                QVERIFY2(expectedValue == calculatedValue, std::string("flow test c_0 | c_1 <- c_3 | c_2. " + expectedName + " has not the expected value, " +
                                                                       std::to_string(expectedValue) + "!=" + std::to_string(calculatedValue)).c_str());
            }
            newState.clear();
            actualState.clear();

            // test c_0 -> c_1 //fail
            actualState["C_0"] = -1300;
            actualState["C_1"] = 1300;

            QVERIFY2(!executor.executePredicate(actualState, newState), "flow c_0 -> c_1 should be imposible");

        } else {
            QFAIL("QT TEST ERROR: error creating temporal directory");
        }
    } catch (std::exception & e) {
        QFAIL(std::string("exception: " + std::string(e.what())).c_str());
    } catch (PlException &ex ) {
        QFAIL(std::string("prolog exception: " + std::string((char *) ex)).c_str());
    }
}

void RulesGeneratorTest::temporalTest_samePropertyTube() {
    qDebug() << QTest::currentAppName();
}

std::shared_ptr<MachineGraph> RulesGeneratorTest::makeMachineGraph() {
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
    std::vector<std::unordered_set<int>> pos3 = {{0,1,2}};
    table.insert(std::make_pair(3, pos3));

    int v= mGraph->emplaceValve(3, table, routef);

    mGraph->connectNodes(c0,v,1,0);
    mGraph->connectNodes(c1,v,1,1);
    mGraph->connectNodes(v,c2,2,0);
    mGraph->connectNodes(c2,p,1,0);
    mGraph->connectNodes(p,c3,1,0);

    return mGraph;
}

MachineGraph RulesGeneratorTest::makeComplexValveGraph(std::unordered_map<std::string, int> & containerMap) {
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

std::shared_ptr<MachineGraph> RulesGeneratorTest::makeMultipathWashMachineGraph() {
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

std::shared_ptr<MachineGraph> RulesGeneratorTest::makeComplexPumpGraph(std::unordered_map<std::string, int> & containerMap) {
    std::shared_ptr<MachineGraph> mGraph = std::make_shared<MachineGraph>();

    PluginConfiguration config;
    std::shared_ptr<PluginAbstractFactory> factory = nullptr;

    std::shared_ptr<Function> pumpf = std::make_shared<PumpPluginFunction>(factory, config);
    std::shared_ptr<Function> routef = std::make_shared<ValvePluginRouteFunction>(factory, config);

    int c0 = mGraph->emplaceContainer(1, open, 100.0);
    int c1 = mGraph->emplaceContainer(1, open, 100.0);
    int c2 = mGraph->emplaceContainer(1, open, 100.0);
    int c3 = mGraph->emplaceContainer(1, open, 100.0);

    int p1 = mGraph->emplacePump(4, PumpNode::bidirectional, pumpf);

    ValveNode::TruthTable tableType1;
    tableType1.insert(std::make_pair(0, std::vector<std::unordered_set<int>>()));
    tableType1.insert(std::make_pair(1, std::vector<std::unordered_set<int>>{{0,1}}));

    int v0 = mGraph->emplaceValve(2, tableType1, routef);
    int v1 = mGraph->emplaceValve(2, tableType1, routef);
    int v2 = mGraph->emplaceValve(2, tableType1, routef);

    containerMap.insert(std::make_pair("c0", c0));
    containerMap.insert(std::make_pair("c1", c1));
    containerMap.insert(std::make_pair("c2", c2));
    containerMap.insert(std::make_pair("c3", c3));
    containerMap.insert(std::make_pair("v0", v0));
    containerMap.insert(std::make_pair("v1", v1));
    containerMap.insert(std::make_pair("v2", v2));
    containerMap.insert(std::make_pair("p1", p1));

    mGraph->connectNodes(c0, v0, 0, 0);
    mGraph->connectNodes(c1, v1, 0, 0);
    mGraph->connectNodes(v0, p1, 1, 0);
    mGraph->connectNodes(v1, p1, 1, 1);
    mGraph->connectNodes(p1, c2, 2, 0);
    mGraph->connectNodes(p1, v2, 3, 0);
    mGraph->connectNodes(v2, c3, 1, 0);

    return mGraph;
}

void RulesGeneratorTest::savePrologFile(const QString & path, PrologTranslationStack stack) throw (std::runtime_error) {
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        throw(std::runtime_error("impossible to open file " + path.toStdString()));
    }

    QTextStream fout(&file);

    fout << ":- use_module(library(clpfd))." << "\n";
    fout << "\n";

    std::string heather = stack.generateMethodHeather();
    fout << QString::fromStdString(heather) << "\n";

    const std::vector<std::string> & prologSentences = stack.getTranslatedRestriction();

    for(auto it = prologSentences.begin(); it != prologSentences.end(); ++it) {
        fout << QString::fromStdString(*it) << "," << "\n";
    }
    fout << QString::fromStdString(stack.generateLabelingFoot());

    file.close();
}

void RulesGeneratorTest::initTestCase() {
    PrologExecutor::createEngine(std::string(QTest::currentAppName()));
}

void RulesGeneratorTest::cleanupTestCase() {
    PrologExecutor::destoryEngine();
}

QTEST_APPLESS_MAIN(RulesGeneratorTest)

#include "tst_rulesgeneratortest.moc"
