#include "calculator.hpp"

#include "LoggerHolder.hpp"
#include "../calculatorCommon/ILogger.hpp"
#include <sstream>

int Calculator::add(int lhs, int rhs)
{
   //if (LoggerHolder::logger())
   //{
      //std::stringstream ss;
      //ss << "Calculator::add(), Adding " << lhs << " + " << rhs;
      //LoggerHolder::logger()->log_info( ss.str().c_str() );
   //}

   LOG_INFO("Calculator::add(), Adding " << lhs << " + " << rhs);

   return lhs + rhs;
}

//extern "C" ICalculator* create()
//{
   //return new Calculator;
//}

//extern "C" void destroy(ICalculator* calculator)
//{
   //delete calculator;
//}
