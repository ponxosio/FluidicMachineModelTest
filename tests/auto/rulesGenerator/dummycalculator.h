#ifndef DUMMYCALCULATOR_H
#define DUMMYCALCULATOR_H

#include <memory>

#include <fluidicmachinemodel/rules/predicate.h>
#include <fluidicmachinemodel/rules/arithmetic/unaryoperation.h>
#include <fluidicmachinemodel/rules/arithmetic/variable.h>

class DummyCalculator
{
public:
    DummyCalculator();
    virtual ~DummyCalculator(){}

    std::shared_ptr<ArithmeticOperable> applyAbs(std::shared_ptr<Variable> var);
};

#endif // DUMMYCALCULATOR_H
