#include "prologtranslationstack.h"

PrologTranslationStack::PrologTranslationStack() {

}

PrologTranslationStack::~PrologTranslationStack() {

}

void PrologTranslationStack::pop() {
    stack.pop();
}

void PrologTranslationStack::clear() {
    while(!stack.empty()) {
        stack.pop();
    }
}

void PrologTranslationStack::addHeadToRestrictions() {
    actualRestriction.push_back(stack.top());
    stack.pop();
}

void PrologTranslationStack::stackVariable(const std::string & name) {
    stack.push(std::string(name));
    varTable.insert(name);
}

void PrologTranslationStack::stackNumber(int value) {
    stack.push(std::to_string(value));
}

void PrologTranslationStack::stackArithmeticBinaryOperation(int arithmeticOp) {
    std::string right = stack.top();
    stack.pop();
    std::string left = stack.top();
    stack.pop();

    std::string newRestriction = "(" + left + " " + opToStr((BinaryOperation::BinaryOperators)arithmeticOp) + " " + right + ")";
    stack.push(newRestriction);
}

void PrologTranslationStack::stackArithmeticUnaryOperation(int unaryOp) {
    std::string operand = stack.top();
    stack.pop();

    std::tuple<std::string, std::string> tuple = unaryOpToStr((UnaryOperation::UnaryOperators) unaryOp);
    std::string newRestriction = "(" + std::get<0>(tuple) + operand + std::get<1>(tuple) + ")";
    stack.push(newRestriction);
}

void PrologTranslationStack::stackEquality(int op) {
    std::string right = stack.top();
    stack.pop();
    std::string left = stack.top();
    stack.pop();
    std::string opStr = equalityOPtoStr((Equality::ComparatorOp) op);

    std::string newRestriction = "(" + left + " " + opStr + " " + right + ")";
    stack.push(newRestriction);
}

void PrologTranslationStack::stackBooleanConjuction(int booleanOp) {
    std::string right = stack.top();
    stack.pop();
    std::string left = stack.top();
    stack.pop();

    if ((Conjunction::BoolOperators) booleanOp == Conjunction::predicate_and) {
        std::string newRestriction = "(" + left + " " + boolOpToStr((Conjunction::BoolOperators)booleanOp) + "\n" + right + ")";
        stack.push(newRestriction);
    } else {
        std::string newRestriction = "(\n" + tabulateString(left) + " \n" + boolOpToStr((Conjunction::BoolOperators)booleanOp) + "\n"
                + tabulateString(right) + "\n)";
        stack.push(newRestriction);
    }

}

void PrologTranslationStack::stackImplication() {
    std::string right = stack.top();
    stack.pop();
    std::string left = stack.top();
    stack.pop();

    std::string newRestriction = "(" + left + "==>" + right + ")";
    stack.push(newRestriction);
}

RoutingEngine* PrologTranslationStack::getRoutingEngine() {
    std::unique_ptr<QTemporaryFile> file = std::make_unique<QTemporaryFile>(new QTemporaryFile());

    if (!file->open()) {
        throw(std::runtime_error("impossible to create temporary file."));
    }
    QTextStream fout(file.get());

    fout << ":- use_module(library(clpfd))." << "\n";
    fout << "\n";

    std::string heather = generateMethodHeather();
    fout << QString::fromStdString(heather) << "\n";

    for(auto it = actualRestriction.begin(); it != actualRestriction.end(); ++it) {
        fout << QString::fromStdString(*it) << "," << "\n";
    }
    fout << QString::fromStdString(generateLabelingFoot());

    file->close();

    PrologExecutor* routingEngine = new PrologExecutor(std::move(file), varTable);
    return routingEngine;
}

void PrologTranslationStack::stackVarDomain() {
    std::stringstream stream;
    std::string variable = stack.top();
    stack.pop();

    if((stack.size() % 2) == 0) {
        stream << variable << " " << DOMAIN_EQ << " ";
        while(stack.size() > 2) {
            std::string max = stack.top();
            stack.pop();
            std::string min = stack.top();
            stack.pop();
            stream << DOMAIN_LEFT << min << " " << DOMAIN_MIDDLE << " " << max << DOMAIN_RIGHT << " " << DOMAIN_JOIN << " ";
        }
        std::string max = stack.top();
        stack.pop();
        std::string min = stack.top();
        stack.pop();
        stream << DOMAIN_LEFT << min << " " << DOMAIN_MIDDLE << " " << max << DOMAIN_RIGHT;

        stack.push(stream.str());
    } else {
        clear();
        stack.push("VAR DOMAIN ERROR: NOT EVEN SIZE");
    }
}

std::string PrologTranslationStack::generateMethodHeather() {
    std::stringstream stream;
    stream << "stackAutoPredicate(";

    if (!varTable.empty()) {
        auto it = varTable.begin();
        stream << *it;
        ++it;

        for(; it != varTable.end(); ++it) {
            stream << "," << *it;
        }
    }
    stream << "):-";

    return stream.str();
}

std::string PrologTranslationStack::generateLabelingFoot() {
    std::stringstream streamMin;
    std::stringstream streamName;
    streamMin << "once(labeling([ff,min(";
    streamName << "[";

    bool lastWasPump = false;
    bool lastWasValve = false;
    for(const std::string & var: varTable) {
        VariableNominator::VariableType type = VariableNominator::getVariableType(var);
        if (type == VariableNominator::pump) {
            if (lastWasPump) {
                streamMin << " + ";
                streamName << ",";
            } else {
                lastWasPump = true;
            }
            streamMin << "abs(" << var << ")";
            streamName << var;
        } else if (type == VariableNominator::valve) {
            if (lastWasPump) {
                streamMin << "), min(";
                streamName << ",";
                lastWasPump = false;
            }

            if (lastWasValve) {
                streamMin << " + ";
                streamName << ",";
            } else {
                lastWasValve = true;
            }
            streamMin  << var;
            streamName << var;
        }
    }
    streamMin << ")]";
    streamName << "])).";

    return streamMin.str() + "," + streamName.str();
}

std::string PrologTranslationStack::opToStr(BinaryOperation::BinaryOperators op) {
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
        str = MOD_STR;
        break;
    default:
        str = "";
        break;
    }
    return str;
}

std::string PrologTranslationStack::boolOpToStr(Conjunction::BoolOperators op) {
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

std::tuple<std::string,std::string> PrologTranslationStack::unaryOpToStr(UnaryOperation::UnaryOperators op) {
    std::string left = "";
    std::string right = "";

    switch (op) {
    case UnaryOperation::absolute_value:
        left = ABS_LEFT_STR;
        right = ABS_RIGHT_STR;
        break;
    default:
        break;
    }
    return std::make_tuple(left, right);
}

std::string PrologTranslationStack::equalityOPtoStr(Equality::ComparatorOp op) {
    std::string str = "";
    switch (op) {
    case Equality::not_equal:
        str = NOT_EQUALS_STR;
        break;
    case Equality::equal:
        str = EQUALS_STR;
        break;
    case Equality::bigger:
        str = BIGGER_STR;
        break;
    case Equality::bigger_equal:
        str = BIGGER_EQ_STR;
        break;
    case Equality::lesser:
        str = LESSER_STR;
        break;
    case Equality::lesser_equal:
        str = LESSER_EQ_STR;
        break;
    default:
        break;
    }
    return str;
}

std::string PrologTranslationStack::tabulateString(const std::string & str) {
    std::string formattedStr = "";

    std::size_t pos = str.find('\n');
    if (pos == std::string::npos) {
        formattedStr = "\t" + str;
    } else {
        std::string chunkBefore = str.substr(0, pos + 1);
        std::string chunkAfter = str.substr(pos +1);
        formattedStr = "\t" + chunkBefore + tabulateString(chunkAfter);
    }
    return formattedStr;
}
