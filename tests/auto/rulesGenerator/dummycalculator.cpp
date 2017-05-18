#include "dummycalculator.h"

DummyCalculator::DummyCalculator()
{

}

std::shared_ptr<ArithmeticOperable> DummyCalculator::applyAbs(std::shared_ptr<Variable> var) {
    return std::make_shared<RuleUnaryOperation>(var, RuleUnaryOperation::absolute_value);
}
