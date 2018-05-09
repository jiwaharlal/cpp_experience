#pragma once

class ICalculator
{
public:
   virtual ~ICalculator() {}
   virtual int add(int lhs, int rhs) = 0;
};

typedef ICalculator* (*tCreateCalculatorFn)();
typedef void (*tDestroyCalculatorFn)(ICalculator*);
