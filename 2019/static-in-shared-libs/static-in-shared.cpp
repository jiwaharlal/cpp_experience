#include <iostream>

#include <boost/shared_ptr.hpp>
#include <boost/dll/import.hpp>

#include "printer.hpp"
#include "printer-api.hpp"

int main(int argc, char** argv)
{
    printer p;

    PrinterBase pb;
    std::cout << "in base: " << pb.i << std::endl;


    if (argc != 1)
    {
        boost::shared_ptr<printer_api> plugin;            // variable to hold a pointer to plugin variable
        std::cout << "Loading the plugin" << std::endl;

        plugin = boost::dll::import<printer_api>(          // type of imported symbol is located between `<` and `>`
            argv[1],                     // path to the library and library name
            "plugin",                                       // name of the symbol to import
            boost::dll::load_mode::append_decorations              // makes `libmy_plugin_sum.so` or `my_plugin_sum.dll` from `my_plugin_sum`
        );

        std::cout << "seems loaded\n";

        if (plugin)
        {
            try
            {
                plugin->print();
            }
            catch (const std::exception& e)
            {
                std::cout << "Exception caught: " << e.what() << '\n';
            }
        }
        else
        {
            std::cout << "Error loading plugin\n";
        }
    }

    return 0;
}
