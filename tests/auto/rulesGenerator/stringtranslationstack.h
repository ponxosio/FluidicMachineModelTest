#ifndef STRINGTRANSLATIONSTACK_H
#define STRINGTRANSLATIONSTACK_H

#define ADD_STR "+"
#define SUBS_STR "-"
#define MULT_STR "*"
#define DIV_STR "/"
#define AND_STR "/\\"
#define OR_STR "\\/"

#include <stack>
#include <string>
#include <sstream>
#include <fluidicmachinemodel/constraintssolverinterface/translationstack.h>
#include <fluidicmachinemodel/rules/conjunction.h>
#include <fluidicmachinemodel/rules/arithmetic/binaryoperation.h>
#include <fluidicmachinemodel/rules/arithmetic/unaryoperation.h>
#include <fluidicmachinemodel/rules/equality.h>

class StringTranslationStack : public TranslationStack
{
public:
    StringTranslationStack();
    virtual ~StringTranslationStack();

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

    inline const std::vector<std::string> & getTranslatedRestriction () {
        return actualRestriction;
    }

    inline virtual RoutingEngine* getRoutingEngine() {
        return nullptr;
    }

 protected:
    std::stack<std::string> stack;
    std::vector<std::string> actualRestriction;

    std::string opToStr(BinaryOperation::BinaryOperators op);
    std::string boolOpToStr(Conjunction::BoolOperators op);
    std::tuple<std::string,std::string> unaryOpToStr(RuleUnaryOperation::UnaryOperators op);
    std::string equalityOPtoStr(Equality::ComparatorOp op);
};

#endif // STRINGTRANSLATIONSTACK_H
