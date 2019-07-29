#include <string>
#include <vector>
#include <functional>
#include <type_traits>
#include <iostream>

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/adaptor/transformed.hpp>

struct Module
{
    std::string name;
    bool is_local;
};

using ModuleList = std::vector<Module>;
//using ModuleToStringFuncType = std::function<const std::string&(const Module&)>;
//using NamesRange = decltype(boost::adaptors::transform(ModuleList(), &Module::name));

//using RangeType = boost::transformed_range

//using RangeType = typename std::result_of<decltype(boost::adaptors::transform(ModuleList(), boost::bind(&Module::name, _1))>::type;

using NameAccessorType = std::function<const std::string&(const Module&)>;
using RangeType = boost::transformed_range<NameAccessorType, std::vector<Module>>;

int main()
{
    ModuleList v;
    RangeType nr = boost::adaptors::transform(v, NameAccessorType{boost::bind(&Module::name, _1)});
    //RangeType nr = boost::adaptors::transform(v, boost::bind(&Module::name, _1));

    std::function<const std::string&(const Module&)> f = boost::bind(&Module::name, _1);

    auto name_accessor = boost::bind(&Module::name, _1);
    Module m;
    std::cout << name_accessor(m) << std::endl;

    return 0;
}
