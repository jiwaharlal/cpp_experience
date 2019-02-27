#include <boost/config.hpp>

#include "printer-base.hpp"
#include "printer-api.hpp"

class printer2 : public PrinterBase, public printer_api
{
public:
    explicit printer2();

    virtual void print();
};

// Exporting `my_namespace::plugin` variable with alias name `plugin`
// (Has the same effect as `BOOST_DLL_ALIAS(my_namespace::plugin, plugin)`)
extern "C" BOOST_SYMBOL_EXPORT printer2 plugin;
printer2 plugin;
