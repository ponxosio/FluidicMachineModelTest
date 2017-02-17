#include <QString>
#include <QtTest>

#include <sstream>

#include <util/sequence.h>

class SequencetestTest : public QObject
{
    Q_OBJECT

public:
    SequencetestTest();

private Q_SLOTS:
    void testStartEndSequence();
    void testForbiddenValues();
    void testSequenceSet();
    void testSequenceVector();
    void testSequenceVector_gap();
};

SequencetestTest::SequencetestTest()
{
}

void SequencetestTest::testStartEndSequence()
{
    std::stringstream stream;
    Sequence seq(-1,1);

    do {
        stream << seq.getActualValue() << ";";
    } while (seq.advanceSequence());

    QVERIFY2(stream.str().compare("-1;0;1;") == 0, std::string("Expected -1;0;1;, received " + stream.str()).c_str());
    stream.str("");
    stream.clear();

    seq.reset();

    do {
        stream << seq.getActualValue() << ";";
    } while (seq.advanceSequence());
    QVERIFY2(stream.str().compare("-1;0;1;") == 0,
             std::string("Sequence after reset not working, Expected -1;0;1;, received " + stream.str()).c_str());
}

void SequencetestTest::testForbiddenValues() {
    std::stringstream stream;

    std::vector<int> zeros = {0};
    Sequence seq(-1,1, &zeros);

    do {
        stream << seq.getActualValue() << ";";
    } while (seq.advanceSequence());

    QVERIFY2(stream.str().compare("-1;1;") == 0, std::string("Expected -1;1;, received " + stream.str()).c_str());
    stream.str("");
    stream.clear();

    seq.reset();

    do {
        stream << seq.getActualValue() << ";";
    } while (seq.advanceSequence());
    QVERIFY2(stream.str().compare("-1;1;") == 0,
             std::string("Sequence after reset not working, Expected -1;1;, received " + stream.str()).c_str());
}

void SequencetestTest::testSequenceSet() {
    std::stringstream stream;

    std::set<int> set = {0,2,3,4};
    Sequence seq(set);

    do {
        stream << seq.getActualValue() << ";";
    } while (seq.advanceSequence());

    QVERIFY2(stream.str().compare("0;2;3;4;") == 0, std::string("Expected 0;2;3;4;, received " + stream.str()).c_str());
    stream.str("");
    stream.clear();

    seq.reset();

    do {
        stream << seq.getActualValue() << ";";
    } while (seq.advanceSequence());
    QVERIFY2(stream.str().compare("0;2;3;4;") == 0,
             std::string("Sequence after reset not working, Expected 0;2;3;4;, received " + stream.str()).c_str());
}

void SequencetestTest::testSequenceVector() {
    std::stringstream stream;

    std::vector<int> set = {0,1,2,3};
    Sequence seq(set);

    do {
        stream << seq.getActualValue() << ";";
    } while (seq.advanceSequence());

    QVERIFY2(stream.str().compare("0;1;2;3;") == 0, std::string("Expected 0;1;2;3;, received " + stream.str()).c_str());
    stream.str("");
    stream.clear();

    seq.reset();

    do {
        stream << seq.getActualValue() << ";";
    } while (seq.advanceSequence());
    QVERIFY2(stream.str().compare("0;1;2;3;") == 0,
             std::string("Sequence after reset not working, Expected 0;1;2;3;, received " + stream.str()).c_str());
}

void SequencetestTest::testSequenceVector_gap() {
    std::stringstream stream;

    std::vector<int> set = {7,1,0,8};
    Sequence seq(set);

    do {
        stream << seq.getActualValue() << ";";
    } while (seq.advanceSequence());

    QVERIFY2(stream.str().compare("0;1;7;8;") == 0, std::string("Expected 0;1;7;8;, received " + stream.str()).c_str());
    stream.str("");
    stream.clear();

    seq.reset();

    do {
        stream << seq.getActualValue() << ";";
    } while (seq.advanceSequence());
    QVERIFY2(stream.str().compare("0;1;7;8;") == 0,
             std::string("Sequence after reset not working, Expected 0;1;7;8;, received " + stream.str()).c_str());
}

QTEST_APPLESS_MAIN(SequencetestTest)

#include "tst_sequencetesttest.moc"
