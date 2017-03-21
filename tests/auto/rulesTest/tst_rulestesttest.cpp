#include <QString>
#include <QtTest>

#include <sstream>

#include <fluidicmachinemodel/rules/arithmetic/arithmeticoperable.h>
#include <fluidicmachinemodel/rules/rule.h>
#include <fluidicmachinemodel/rules/predicate.h>
#include <fluidicmachinemodel/rules/equality.h>
#include <fluidicmachinemodel/rules/conjunction.h>
#include <fluidicmachinemodel/rules/variabledomain.h>
#include <fluidicmachinemodel/rules/arithmetic/variable.h>
#include <fluidicmachinemodel/rules/arithmetic/integernumber.h>
#include <fluidicmachinemodel/rules/arithmetic/binaryoperation.h>

#include "stringtranslationstack.h"

class RulesTestTest : public QObject
{
    Q_OBJECT

public:
    RulesTestTest();

private Q_SLOTS:
    void arithmeticOperable();
    void varDomain();
};

RulesTestTest::RulesTestTest()
{
}

void RulesTestTest::arithmeticOperable()
{
    std::shared_ptr<ArithmeticOperable> e1 = std::make_shared<Variable>("e1");
    std::shared_ptr<ArithmeticOperable> e2 = std::make_shared<Variable>("e2");
    std::shared_ptr<ArithmeticOperable> e3 = std::make_shared<Variable>("e3");

    std::shared_ptr<ArithmeticOperable> abs_e2 = std::make_shared<UnaryOperation>(e2, UnaryOperation::absolute_value);
    std::shared_ptr<ArithmeticOperable> abs_e3 = std::make_shared<UnaryOperation>(e3, UnaryOperation::absolute_value);
    std::shared_ptr<ArithmeticOperable> add_e2_e3 = std::make_shared<BinaryOperation>(abs_e2, BinaryOperation::add, abs_e3);

    std::shared_ptr<Predicate> eq_e1_add_e2_e3 = std::make_shared<Equality>(e1, Equality::equal, add_e2_e3);

    StringTranslationStack* stringStack = new StringTranslationStack();

    eq_e1_add_e2_e3->fillTranslationStack(stringStack);
    stringStack->addHeadToRestrictions();
    QVERIFY2(stringStack->getTranslatedRestriction().compare("(e1==((|e2|)+(|e3|)))") == 0,
             std::string("rule is not translated ok, (e1==(|e2|+|e3|)) != " + stringStack->getTranslatedRestriction()).c_str());

    stringStack->clear();

    std::shared_ptr<IntegerNumber> n0 = std::make_shared<IntegerNumber>(0);
    std::shared_ptr<IntegerNumber> n1 = std::make_shared<IntegerNumber>(1);
    std::shared_ptr<IntegerNumber> n3 = std::make_shared<IntegerNumber>(3);
    std::shared_ptr<IntegerNumber> n4 = std::make_shared<IntegerNumber>(4);

    VariableDomain::DomainTuple n0n1 = std::make_tuple(n0, n1);
    VariableDomain::DomainTuple n3n4 = std::make_tuple(n3,n4);
    std::vector<VariableDomain::DomainTuple> v = {n0n1, n3n4};

    std::shared_ptr<VariableDomain> domain = std::make_shared<VariableDomain>(std::dynamic_pointer_cast<Variable>(e1), v);

    domain->fillTranslationStack(stringStack);
    stringStack->addHeadToRestrictions();
    QVERIFY2(stringStack->getTranslatedRestriction().compare("e1=[0,1][3,4]") == 0,
             std::string("rule is not translated ok, e1=[3,4][0,1] != " + stringStack->getTranslatedRestriction()).c_str());

    delete stringStack;
}

void RulesTestTest::varDomain() {
    StringTranslationStack* stringStack = new StringTranslationStack();
    try {
        std::shared_ptr<Variable> var = std::make_shared<Variable>("test");
        std::vector<VariableDomain::DomainTuple> domainVector;
        domainVector.push_back(std::make_tuple(std::make_shared<IntegerNumber>(-100),std::make_shared<IntegerNumber>(-99)));
        domainVector.push_back(std::make_tuple(std::make_shared<IntegerNumber>(0),std::make_shared<IntegerNumber>(100)));
        domainVector.push_back(std::make_tuple(std::make_shared<IntegerNumber>(102),std::make_shared<IntegerNumber>(300)));

        VariableDomain domain(var, domainVector);
        domain.fillTranslationStack(stringStack);
        stringStack->addHeadToRestrictions();

        QVERIFY2(stringStack->getTranslatedRestriction().compare("test=[-100,-99][0,100][102,300]") == 0,
                 std::string("expected: \"test=[300,102][100,0][-99,-100]\", received: " + stringStack->getTranslatedRestriction()).c_str());

    } catch (std::exception & e) {
        QFAIL(std::string("Exception: " + std::string(e.what())).c_str());
    }
    delete stringStack;
}

QTEST_APPLESS_MAIN(RulesTestTest)

#include "tst_rulestesttest.moc"
