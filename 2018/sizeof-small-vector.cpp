#include <bitset>
#include <iostream>
#include <vector>

#include <boost/container/small_vector.hpp>
#include <boost/variant.hpp>

enum class ETravelDirection : std::int8_t
{
    Forward,
    Both
};

enum class ETransitionType : std::int8_t
{
    Split,
    Merge
};

enum class ELaneUsageType : std::int8_t
{
    Regular,
    HOV,
    NonDriveable,
    Parking,
    Bike,
    Bus
};

struct Width
{
    std::int16_t centimeters;
};

using LaneAttributeVariant =
    boost::variant<ETravelDirection, ETransitionType, ELaneUsageType, Width>;

struct Lane
{
    //boost::container::small_vector<LaneAttributeVariant> attrs;
    std::vector<LaneAttributeVariant> attrs;
};

struct PackedLane
{
    ETravelDirection direction;
    ETransitionType transition_type;
    ELaneUsageType usage_type;
    Width width;
};

struct Transitions
{
    bool is_split;
    bool is_merge;
};

struct WellPackedLane
{
    ETravelDirection direction;
    ELaneUsageType usage_type;
    Transitions transitions;
    std::int16_t width_centimeters;
};

struct PackedLaneFloat
{
    ETravelDirection direction;
    ELaneUsageType usage_type;
    Transitions transitions;
    float width;
};

struct Range
{
    float begin;
    float end;
};

struct Marking
{
    Range range;
    ETravelDirection direction;
    ETransitionType transition;
};

int main()
{
    Lane l;
    std::cout << sizeof(l) << std::endl;
    std::cout << sizeof(PackedLane) << std::endl;
    std::cout << sizeof(std::bitset<1>) << std::endl;
    std::cout << sizeof(Transitions) << std::endl;
    std::cout << "WellPackedLane: " << sizeof(WellPackedLane) << std::endl;
    std::cout << "PackedLaneFloat: " << sizeof(PackedLaneFloat) << std::endl;
    std::cout << "Marking: " << sizeof(Marking) << std::endl;
    std::cout << "boost::small_vector<Marking, 8>: " << sizeof(boost::container::small_vector<Marking, 3>) << std::endl;
    std::cout << "std::vector<Marking>: " << sizeof(std::vector<Marking>) << std::endl;

    return 0;
}
