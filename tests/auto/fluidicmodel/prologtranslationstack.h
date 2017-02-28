#ifndef PROLOGTRANSLATIONSTACK_H
#define PROLOGTRANSLATIONSTACK_H

#define ADD_STR "+"
#define SUBS_STR "-"
#define MULT_STR "*"
#define DIV_STR "//"
#define AND_STR "#/\\"
#define OR_STR "#\\/"
#define MOD_STR "rem"
#define ABS_LEFT_STR "abs("
#define ABS_RIGHT_STR ")"
#define DOMAIN_LEFT ""
#define DOMAIN_RIGHT ""
#define DOMAIN_MIDDLE ".."
#define DOMAIN_JOIN "\\/"
#define DOMAIN_EQ "in"
#define NOT_EQUALS_STR "#\\="
#define EQUALS_STR "#="
#define BIGGER_STR "#>"
#define LESSER_STR "#<"
#define BIGGER_EQ_STR "#>="
#define LESSER_EQ_STR "#=<"

#include <stack>
#include <string>
#include <sstream>
#include <set>

#include <QString>
#include <QFile>
#include <QTemporaryFile>
#include <QTextStream>

#include <constraintssolverinterface/translationstack.h>
#include <machine_graph_utils/variablenominator.h>
#include <rules/conjunction.h>
#include <rules/arithmetic/binaryoperation.h>
#include <rules/arithmetic/unaryoperation.h>
#include <rules/equality.h>

#include "prologexecutor.h"

class PrologTranslationStack : public TranslationStack
{
public:
    PrologTranslationStack();
    virtual ~PrologTranslationStack();

    virtual void pop();
    virtual void clear();
    virtual void addHeadToRestrictions();
    virtual void stackVariable(const std::string & name);
    virtual void stackNumber(int value);
    virtual void stackArithmeticBinaryOperation(int arithmeticOp);
    virtual void stackArithmeticUnaryOperation(int unaryOp);
    virtual void stackEquality(int op);
    virtual void stackBooleanConjuction(int booleanOp);
    virtual void stackImplication();
    virtual void stackVarDomain();

    virtual RoutingEngine* getRoutingEngine();

    std::string generateMethodHeather();
    std::string generateLabelingFoot();
    inline const std::vector<std::string> & getTranslatedRestriction () {
        return actualRestriction;
    }
    inline const std::set<std::string> & getVarTable() {
        return varTable;
    }

 protected:
    std::stack<std::string> stack;
    std::vector<std::string> actualRestriction;
    std::set<std::string> varTable;

    std::string opToStr(BinaryOperation::BinaryOperators op);
    std::string boolOpToStr(Conjunction::BoolOperators op);
    std::tuple<std::string,std::string> unaryOpToStr(UnaryOperation::UnaryOperators op);
    std::string equalityOPtoStr(Equality::ComparatorOp op);
    std::string tabulateString(const std::string & str);
    std::string savePrologFile() throw (std::runtime_error);

};

#endif // PROLOGTRANSLATIONSTACK_H
