#pragma once

#define CONCAT_IMPL( x, y ) x##y
#define MACRO_CONCAT( x, y ) CONCAT_IMPL( x, y )

#define RUN_CODE(code) \
    } \
    static int MACRO_CONCAT(x, __COUNTER__) = []{ code; return 0; }(); \
    void MACRO_CONCAT(fun, __COUNTER__) () {
