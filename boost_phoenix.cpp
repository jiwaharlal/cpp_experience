#include <boost/phoenix.hpp>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <iterator>
#include <boost/range/algorithm.hpp>
#include <boost/fusion/sequence.hpp>
#include <boost/fusion/include/define_struct.hpp>
#include <boost/fusion/include/adapt_adt.hpp>
#include <boost/fusion/include/copy.hpp>
#include <boost/fusion/include/transform.hpp>

BOOST_FUSION_DEFINE_STRUCT(
    (), RoadLink,
    (int32_t, id)
    (std::string, name)
)

struct IpcRoadLink
{
public:
    const int32_t& getId() const { return m_id; }
    void setId(const int32_t& id) { m_id = id; }

    const std::string& getName() const { return m_name; }
    void setName(const std::string& name) { m_name = name; }

private:
    int32_t m_id;
    std::string m_name;
};

BOOST_FUSION_ADAPT_ADT(
    IpcRoadLink,
    (obj.getId(), obj.setId(val))
    (obj.getName(), obj.setName(val))
)

//struct RoadLink
//{
    //int32_t id;
    //std::string name;
//};

struct Converter
{
    template <typename T>
    T operator ()(const T& value) const &&
    {
        std::cout << "Converting temporary" << std::endl;
        return value + value;
    }

    template <typename T>
    T operator ()(const T& value) const &
    {
        std::cout << "Converting" << std::endl;
        return value + value;
    }

    template <typename T1, typename T2>
    void operator ()(const T1& in, T2& out) const &&
    {
        std::cout << "Converting 2" << std::endl;
        out = in + in;
    }
};

struct A
{
    void print() const &
    {
        std::cout << "standart print" << std::endl;
    }

    void print() const &&
    {
        std::cout << "temporary print" << std::endl;
    }

    void print() &
    {
    }

    void print() &&
    {
    }
};


void testBindMember()
{
    A().print();

    A a;
    a.print();

    std::vector<RoadLink> links =
    { {1, "Bulakhovskogo"},
        {2, "Dvinskaya"},
        {3, "Irpenskaya"} };

    const RoadLink& link = links[1];
    boost::fusion::for_each(link, std::cout << boost::phoenix::placeholders::arg1 << std::endl);

    IpcRoadLink ipc_link;

    //boost::fusion::copy(link, ipc_link);

    boost::fusion::copy(boost::fusion::transform(link, Converter()), ipc_link);

    //boost::fusion::transform(link, ipc_link, Converter());

    std::cout << ipc_link.getId() << " : " << ipc_link.getName() << std::endl;

    //boost::fusion::for_each(ipc_link, std::cout << boost::phoenix::placeholders::arg1 << std::endl);

    std::vector<std::string> names;

    boost::transform(links, std::back_inserter(names), boost::phoenix::bind(&RoadLink::name, boost::phoenix::placeholders::arg1));
    //boost::transform(links, std::back_inserter(names), &boost::fusion::at<boost::mpl::int_<1>><IpcRoadLink>);

    boost::copy(names, std::ostream_iterator<std::string>(std::cout, "\n"));
}

int main()
{
   using namespace boost::phoenix::placeholders;

   std::vector<int> v;
   for (int i = 0; i < 15; i++)
   {
      v.push_back(rand() % 100);
   }

   boost::find_if(v, arg1 == 15);

   boost::copy(v, std::ostream_iterator<int>(std::cout, ", "));
   std::cout << std::endl;

   testBindMember();

   return 0;
}
