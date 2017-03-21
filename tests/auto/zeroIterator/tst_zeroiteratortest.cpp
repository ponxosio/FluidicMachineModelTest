#include <QString>
#include <QtTest>
#include <sstream>

#include <fluidicmachinemodel/machine_graph_utils/rules_generation/label.h>
#include <fluidicmachinemodel/machine_graph_utils/rules_generation/labelcombinationsiterator.h>

class ZeroIteratorTest : public QObject
{
    Q_OBJECT

public:
    ZeroIteratorTest();

private:
   LabelCombinationsIterator::LabelTypeTubeMap createTubeMap();

private Q_SLOTS:
    void checkZero();
    void checkBiggerSmaller();
};

ZeroIteratorTest::ZeroIteratorTest()
{
}

void ZeroIteratorTest::checkZero()
{
    try {
        LabelCombinationsIterator::LabelTypeTubeMap tubeMap = createTubeMap();
        std::vector<Label::LabelType> types = {Label::zero};

        LabelCombinationsIterator it(tubeMap, types);

        std::vector<std::string> calculated;
        while(it.calculateNext()) {
            const LabelCombinationsIterator::LabelTypeTubeMap & actual = it.getTubesLabelsMap();
            for (auto tuple: actual) {
                stringstream stream;
                const std::tuple<int,int> & tubeId = tuple.first;
                Label label = tuple.second;

                stream << "tube: " << std::to_string(std::get<0>(tubeId)) << ", label: " << label.toStr();
                qDebug() << "tube: " << QString::number(std::get<0>(tubeId)) << ", label: " << QString::fromStdString(label.toStr());

                calculated.push_back(stream.str());
            }
        }

        std::vector<std::string> expedted = {"tube: 0, label: 000",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 000",
                                             "tube: 3, label: 000",
                                             "tube: 0, label: 001",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 000",
                                             "tube: 3, label: 000",
                                             "tube: 0, label: 000",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 001",
                                             "tube: 3, label: 000",
                                             "tube: 0, label: 001",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 001",
                                             "tube: 3, label: 000",
                                             "tube: 0, label: 000",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 000",
                                             "tube: 3, label: 001",
                                             "tube: 0, label: 001",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 000",
                                             "tube: 3, label: 001",
                                             "tube: 0, label: 000",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 001",
                                             "tube: 3, label: 001",
                                             "tube: 0, label: 001",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 001",
                                             "tube: 3, label: 001"};

        QVERIFY2(calculated.size() == expedted.size(),
                 std::string("expected rules size:" + std::to_string(expedted.size()) + ", received: " +
                             std::to_string(calculated.size())).c_str());

        for(int i = 0; i < expedted.size(); i++) {
            QVERIFY2(calculated[i].compare(expedted[i]) == 0,
                     std::string("i: " + std::to_string(i) +".Expected : " + expedted[i] + ", received: " + calculated[i]).c_str());
        }

    } catch (std::exception & e) {
        QFAIL(std::string("exception occured, message: " + std::string(e.what())).c_str());
    }
}

void ZeroIteratorTest::checkBiggerSmaller() {
    try {
        LabelCombinationsIterator::LabelTypeTubeMap tubeMap = createTubeMap();
        std::vector<Label::LabelType> types = {Label::bigger, Label::smaller};

        LabelCombinationsIterator it(tubeMap, types);

        std::vector<std::string> calculated;
        while(it.calculateNext()) {
            const LabelCombinationsIterator::LabelTypeTubeMap & actual = it.getTubesLabelsMap();
            for (auto tuple: actual) {
                stringstream stream;
                const std::tuple<int,int> & tubeId = tuple.first;
                Label label = tuple.second;

                stream << "tube: " << std::to_string(std::get<0>(tubeId)) << ", label: " << label.toStr();
                qDebug() << "tube: " << QString::number(std::get<0>(tubeId)) << ", label: " << QString::fromStdString(label.toStr());

                calculated.push_back(stream.str());
            }
        }

        std::vector<std::string> expedted = {"tube: 0, label: 000",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 000",
                                             "tube: 3, label: 000",
                                             "tube: 0, label: 000",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 000",
                                             "tube: 3, label: 010",
                                             "tube: 0, label: 000",
                                             "tube: 1, label: 000",
                                             "tube: 2, label: 000",
                                             "tube: 3, label: 100"};

        QVERIFY2(calculated.size() == expedted.size(),
                 std::string("expected rules size:" + std::to_string(expedted.size()) + ", received: " +
                             std::to_string(calculated.size())).c_str());

        for(int i = 0; i < expedted.size(); i++) {
            QVERIFY2(calculated[i].compare(expedted[i]) == 0,
                     std::string("i: " + std::to_string(i) +".Expected : " + expedted[i] + ", received: " + calculated[i]).c_str());
        }

    } catch (std::exception & e) {
        QFAIL(std::string("exception occured, message: " + std::string(e.what())).c_str());
    }
}

LabelCombinationsIterator::LabelTypeTubeMap ZeroIteratorTest::createTubeMap() {
    LabelCombinationsIterator::LabelTypeTubeMap tubeMap;

    Label empty;

    Label zero;
    zero.setLabel(Label::zero);

    Label complete;
    complete.setLabel(Label::zero);
    complete.setLabel(Label::bigger);
    complete.setLabel(Label::smaller);

    tubeMap.insert(std::make_pair(std::make_tuple(0,0),zero));
    tubeMap.insert(std::make_pair(std::make_tuple(1,0),empty));
    tubeMap.insert(std::make_pair(std::make_tuple(2,0),zero));
    tubeMap.insert(std::make_pair(std::make_tuple(3,0),complete));

    return tubeMap;
}

QTEST_APPLESS_MAIN(ZeroIteratorTest)

#include "tst_zeroiteratortest.moc"
