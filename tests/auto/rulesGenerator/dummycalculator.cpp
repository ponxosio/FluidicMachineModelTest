#include "dummycalculator.h"

DummyCalculator::DummyCalculator()
{

}

std::shared_ptr<ArithmeticOperable> DummyCalculator::applyAbs(std::shared_ptr<Variable> var) {
    return std::make_shared<UnaryOperation>(var, UnaryOperation::absolute_value);
}
