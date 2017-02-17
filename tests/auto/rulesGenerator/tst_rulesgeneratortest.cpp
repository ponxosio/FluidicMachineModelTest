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

#include <plugininterface/pluginconfiguration.h>
#include <plugininterface/pluginabstractfactory.h>

#include "stringtranslationstack.h"
#include "prologtranslationstack.h"

class RulesGeneratorTest : public QObject
{
    Q_OBJECT

public:
    RulesGeneratorTest();
private:
    std::shared_ptr<MachineGraph> makeMachineGraph();
    std::shared_ptr<MachineGraph> makeMultipathWashMachineGraph();
    MachineGraph makeComplexValveGraph(std::unordered_map<std::string, int> & containerMap);
    void savePrologFile(const QString & path, PrologTranslationStack stack) throw (std::runtime_error);

private Q_SLOTS:
    void checkSimpleGraph_varDomain();
    void checkSimpleGraph_prologRules();
    void checkComplexValveGraph_prologRules();
    void checkMultipathMachine_prologRules();

};

RulesGeneratorTest::RulesGeneratorTest()
{
}

void RulesGeneratorTest::checkSimpleGraph_prologRules()
{
    try {
        PrologTranslationStack stack;
        std::shared_ptr<MachineGraph> complexValve = makeMachineGraph();

        GraphRulesGenerator rulesGenerator(complexValve, 3, 0);
        for (const std::shared_ptr<Rule> & rule : rulesGenerator.getRules()) {
            rule->fillTranslationStack(&stack);
            stack.addHeadToRestrictions();
        }

        QTemporaryDir temp;
        if (temp.isValid()) {
            QString generatedFilePath = temp.path() + "/simpleMachine.pl";
            savePrologFile(generatedFilePath, stack);

            QByteArray generatedSHA1;
            QCryptographicHash sha1Calculator(QCryptographicHash::Sha1);

            QFile generatedFile(generatedFilePath);
            if (generatedFile.open(QFile::ReadOnly)) {
                if (sha1Calculator.addData(&generatedFile)) {
                    generatedSHA1 = sha1Calculator.result();
                }
            }
            sha1Calculator.reset();

            QByteArray expectedSHA1;
            QFile expectedFile(":/prolog_examples/simpleMachine.pl");
            if (expectedFile.open(QFile::ReadOnly)) {
                if (sha1Calculator.addData(&expectedFile)) {
                    expectedSHA1 = sha1Calculator.result();
                }
            }

            QVERIFY2(expectedSHA1.size() != 0, "SHA1 value for expected rules file is empty");
            QVERIFY2(generatedSHA1.size() != 0, "SHA1 value for calculated rules file is empty");

            qDebug() << "Expected file:";
            expectedFile.reset();
            QTextStream inExpected(&expectedFile);
            while(!inExpected.atEnd()) {
                qDebug() << inExpected.readLine();
            }

            qDebug() << "Generated file:";
            generatedFile.reset();
            QTextStream inGenerated(&generatedFile);
            while(!inGenerated.atEnd()) {
                qDebug() << inGenerated.readLine();
            }

            QVERIFY2(expectedSHA1 == generatedSHA1, "Generated rules files are not the same");

        } else {
            QFAIL("QT TEST ERROR: error creating temporal directory");
        }
    } catch (std::exception & e) {
        QFAIL(std::string("exception: " + std::string(e.what())).c_str());
    }
}

void RulesGeneratorTest::checkSimpleGraph_varDomain() {
    try {
        StringTranslationStack stack;
        std::shared_ptr<MachineGraph> machine = makeMachineGraph();

        GraphRulesGenerator rulesGenerator(machine,3,2);
        for (const std::shared_ptr<Rule> & rule : rulesGenerator.getRules()) {
            rule->fillTranslationStack(&stack);
            stack.addHeadToRestrictions();
        }

        std::vector<std::string> expectedValues = {"P_4=[-1,1]",
                                                   "R_4=[0,99999]",
                                                   "V_5=[0,3]",
                                                   "C_0=[-799999,-700001][-699999,-600001][-599999,-500001][-499999,-400001][-399999,-300001][-299999,-200001][-199999,-100001][0,0][100001,199999][200001,299999][300001,399999][400001,499999][500001,599999][600001,699999][700001,799999]",
                                                   "C_1=[-799999,-700001][-699999,-600001][-599999,-500001][-499999,-400001][-399999,-300001][-299999,-200001][-199999,-100001][0,0][100001,199999][200001,299999][300001,399999][400001,499999][500001,599999][600001,699999][700001,799999]",
                                                   "C_3=[-799999,-700001][-699999,-600001][-599999,-500001][-499999,-400001][-399999,-300001][-299999,-200001][-199999,-100001][0,0][100001,199999][200001,299999][300001,399999][400001,499999][500001,599999][600001,699999][700001,799999]",
                                                   "C_2=[-799999,-700001][-699999,-600001][-599999,-500001][-499999,-400001][-399999,-300001][-299999,-200001][-199999,-100001][0,0][100001,199999][200001,299999][300001,399999][400001,499999][500001,599999][600001,699999][700001,799999]",
                                                   "T_0_5=[-799999,-700001][-699999,-600001][-599999,-500001][-499999,-400001][-399999,-300001][-299999,-200001][-199999,-100001][0,0][100001,199999][200001,299999][300001,399999][400001,499999][500001,599999][600001,699999][700001,799999]",
                                                   "T_1_5=[-799999,-700001][-699999,-600001][-599999,-500001][-499999,-400001][-399999,-300001][-299999,-200001][-199999,-100001][0,0][100001,199999][200001,299999][300001,399999][400001,499999][500001,599999][600001,699999][700001,799999]",
                                                   "T_5_2=[-799999,-700001][-699999,-600001][-599999,-500001][-499999,-400001][-399999,-300001][-299999,-200001][-199999,-100001][0,0][100001,199999][200001,299999][300001,399999][400001,499999][500001,599999][600001,699999][700001,799999]",
                                                   "T_2_4=[-799999,-700001][-699999,-600001][-599999,-500001][-499999,-400001][-399999,-300001][-299999,-200001][-199999,-100001][0,0][100001,199999][200001,299999][300001,399999][400001,499999][500001,599999][600001,699999][700001,799999]",
                                                   "T_4_3=[-799999,-700001][-699999,-600001][-599999,-500001][-499999,-400001][-399999,-300001][-299999,-200001][-199999,-100001][0,0][100001,199999][200001,299999][300001,399999][400001,499999][500001,599999][600001,699999][700001,799999]"
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
            QString generatedFilePath = temp.path() + "/complexValve.pl";
            savePrologFile(generatedFilePath, stack);

            QByteArray generatedSHA1;
            QCryptographicHash sha1Calculator(QCryptographicHash::Sha1);

            QFile generatedFile(generatedFilePath);
            if (generatedFile.open(QFile::ReadOnly)) {
                if (sha1Calculator.addData(&generatedFile)) {
                    generatedSHA1 = sha1Calculator.result();
                }
            }
            sha1Calculator.reset();

            QByteArray expectedSHA1;
            QFile expectedFile(":/prolog_examples/complexValve.pl");
            if (expectedFile.open(QFile::ReadOnly)) {
                if (sha1Calculator.addData(&expectedFile)) {
                    expectedSHA1 = sha1Calculator.result();
                }
            }

            QVERIFY2(expectedSHA1.size() != 0, "SHA1 value for expected rules file is empty");
            QVERIFY2(generatedSHA1.size() != 0, "SHA1 value for calculated rules file is empty");

            qDebug() << "Expected file:";
            expectedFile.reset();
            QTextStream inExpected(&expectedFile);
            while(!inExpected.atEnd()) {
                qDebug() << inExpected.readLine();
            }

            qDebug() << "Generated file:";
            generatedFile.reset();
            QTextStream inGenerated(&generatedFile);
            while(!inGenerated.atEnd()) {
                qDebug() << inGenerated.readLine();
            }

            QVERIFY2(expectedSHA1 == generatedSHA1, "Generated rules files are not the same");

        } else {
            QFAIL("QT TEST ERROR: error creating temporal directory");
        }
    } catch (std::exception & e) {
        QFAIL(std::string("exception: " + std::string(e.what())).c_str());
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

QTEST_APPLESS_MAIN(RulesGeneratorTest)

#include "tst_rulesgeneratortest.moc"
