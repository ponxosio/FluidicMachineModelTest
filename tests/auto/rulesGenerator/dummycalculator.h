#ifndef DUMMYCALCULATOR_H
#define DUMMYCALCULATOR_H

#include <memory>

#include <rules/predicate.h>
#include <rules/arithmetic/unaryoperation.h>
#include <rules/arithmetic/variable.h>

class DummyCalculator
{
public:
    DummyCalculator();
    virtual ~DummyCalculator(){}

    std::shared_ptr<ArithmeticOperable> applyAbs(std::shared_ptr<Variable> var);
};

#endif // DUMMYCALCULATOR_H
