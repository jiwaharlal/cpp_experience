%module swigdemo
%include <stl.i>
%{
#include "mul.hpp"
%}

%include "mul.hpp"
%catches(std::runtime_error, ...) concat(const std::string&, const std::string&);

%template(intvector) std::vector<int>;
%template(IntToStringMap) std::map<int, std::string>;

/*%template(charstring) std::string<char>;*/
