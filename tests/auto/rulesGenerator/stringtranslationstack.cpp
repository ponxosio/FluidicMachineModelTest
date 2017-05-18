#include "stringtranslationstack.h"

StringTranslationStack::StringTranslationStack() {

}

StringTranslationStack::~StringTranslationStack() {

}

void StringTranslationStack::pop() {
    stack.pop();
}

void StringTranslationStack::clear() {
    while(!stack.empty()) {
        stack.pop();
    }
}

void StringTranslationStack::addHeadToRestrictions() {
    actualRestriction.push_back(stack.top());
    stack.pop();
}

void StringTranslationStack::stackVariable(const std::string & name) {
    stack.push(std::string(name));
}

void StringTranslationStack::stackNumber(int value) {
    stack.push(std::to_string(value));
}

void StringTranslationStack::stackArithmeticBinaryOperation(int arithmeticOp) {
    std::string right = stack.top();
    stack.pop();
    std::string left = stack.top();
    stack.pop();

    std::string newRestriction = "(" + left + opToStr((BinaryOperation::BinaryOperators)arithmeticOp) + right + ")";
    stack.push(newRestriction);
}

void StringTranslationStack::stackArithmeticUnaryOperation(int unaryOp) {
    std::string operand = stack.top();
    stack.pop();

    std::tuple<std::string, std::string> tuple = unaryOpToStr((RuleUnaryOperation::UnaryOperators) unaryOp);
    std::string newRestriction = "(" + std::get<0>(tuple) + operand + std::get<1>(tuple) + ")";
    stack.push(newRestriction);
}

void StringTranslationStack::stackEquality(int op) {
    std::string right = stack.top();
    stack.pop();
    std::string left = stack.top();
    stack.pop();
    std::string opStr = equalityOPtoStr((Equality::ComparatorOp) op);

    std::string newRestriction = "(" + left + opStr + right + ")";
    stack.push(newRestriction);
}

void StringTranslationStack::stackBooleanConjuction(int booleanOp) {
    std::string right = stack.top();
    stack.pop();
    std::string left = stack.top();
    stack.pop();

    std::string newRestriction = "(" + left + boolOpToStr((Conjunction::BoolOperators)booleanOp) + right + ")";
    stack.push(newRestriction);
}

void StringTranslationStack::stackImplication() {
    std::string right = stack.top();
    stack.pop();
    std::string left = stack.top();
    stack.pop();

    std::string newRestriction = "(" + left + "==>" + right + ")";
    stack.push(newRestriction);
}

void StringTranslationStack::stackVarDomain() {
    std::stringstream stream;
    std::string variable = stack.top();
    stack.pop();

    if((stack.size() % 2) == 0) {
        stream << variable << "=";
        while(!stack.empty()) {
            std::string max = stack.top();
            stack.pop();
            std::string min = stack.top();
            stack.pop();
            stream << "[" << min << "," << max << "]";
        }
        stack.push(stream.str());
    } else {
        clear();
        stack.push("VAR DOMAIN ERROR: NOT EVEN SIZE");
    }
}

std::string StringTranslationStack::opToStr(BinaryOperation::BinaryOperators op) {
    std::string str;
    switch (op) {
    case BinaryOperation::add:
        str = ADD_STR;
        break;
    case BinaryOperation::subtract:
        str = SUBS_STR;
        break;
    case BinaryOperation::multiply:
        str = MULT_STR;
        break;
    case BinaryOperation::divide:
        str = DIV_STR;
        break;
    case BinaryOperation::module:
        str = "mod";
        break;
    default:
        str = "";
        break;
    }
    return str;
}

std::string StringTranslationStack::boolOpToStr(Conjunction::BoolOperators op) {
    std::string str;
    switch (op) {
    case BinaryOperation::add:
        str = AND_STR;
        break;
    case BinaryOperation::subtract:
        str = OR_STR;
        break;
    default:
        str = "";
        break;
    }
    return str;
}

std::tuple<std::string,std::string> StringTranslationStack::unaryOpToStr(RuleUnaryOperation::UnaryOperators op) {
    std::string left = "";
    std::string right = "";

    switch (op) {
    case RuleUnaryOperation::absolute_value:
        left = "|";
        right = "|";
        break;
    default:
        break;
    }
    return std::make_tuple(left, right);
}

std::string StringTranslationStack::equalityOPtoStr(Equality::ComparatorOp op) {
    std::string str = "";
    switch (op) {
    case Equality::equal:
        str = "==";
        break;
    case Equality::bigger:
        str = ">";
        break;
    case Equality::bigger_equal:
        str = ">=";
        break;
    case Equality::lesser:
        str = "<";
        break;
    case Equality::lesser_equal:
        str = "<=";
        break;
    default:
        break;
    }
    return str;
}
