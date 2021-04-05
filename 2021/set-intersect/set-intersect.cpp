#include <vector>
#include <random>
#include <algorithm>
#include <unordered_set>
#include <chrono>
#include <iostream>
#include <cmath>
#include <google/dense_hash_set>
#include <google/sparse_hash_set>
#include <type_traits>

template <typename T>
//using hash_set = std::unordered_set<T>;
using hash_set = google::dense_hash_set<T>;
// using hash_set = google::sparse_hash_set<T>;

// template <typename T>
// using hash_set = google::sparse_hash_set<T>;

template <typename T>
T intersectWithVectors(const T& t1, const T& t2)
{
    std::vector<typename T::value_type> t1_copy(t1.begin(), t1.end());
    std::vector<typename T::value_type> t2_copy(t2.begin(), t2.end());
    std::sort(t1_copy.begin(), t1_copy.end());
    std::sort(t2_copy.begin(), t2_copy.end());

    T intersection;
    std::set_intersection(t1_copy.begin(), t1_copy.end(),
            t2_copy.begin(), t2_copy.end(),
            std::back_inserter(intersection));

    return intersection;
}

template <typename T>
T intersectWithHashSet(const T& t1, const T& t2)
{
    auto t1_copy = [&](){
        if constexpr (std::is_same<hash_set<T>, google::dense_hash_set<T>>::value)
            return hash_set<typename T::value_type>(t1.begin(), t1.end(), typename T::value_type{'a'});
        else
            return hash_set<typename T::value_type>(t1.begin(), t1.end());
    }();

    T intersection;
    //std::copy_if(
            //t1.begin(),
            //t2.end(),
            //std::back_inserter(intersection),
            //[&](const auto& elt){ return t1_copy.find(elt) != t1_copy.end(); });

    for (const auto& v : t2)
    {
        if (t1_copy.find(v) != t1_copy.end())
            intersection.push_back(v);
    }

    //std::sort(intersection.begin(), intersection.end());

    return intersection;
}

template <typename T>
T getRandom(int total_size);

template <>
std::string getRandom<std::string>(int total_size)
{
    int size = std::log2(total_size);
    std::string result(size, ' ');
    std::generate_n(result.begin(), size, [](){ return std::rand() % ('z' - 'a') + 'a'; });

    return result;
}

template <>
int getRandom<int>(int total_size)
{
    return std::rand() % (total_size * 2);
}

template <typename T>
std::vector<T> generateRandomSet(int count)
{
    std::vector<T> result;
    result.reserve(count);
    std::generate_n(std::back_inserter(result), count, [count](){ return getRandom<T>(count); });
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(result.begin(), result.end(), g);

    return result;
}

int main()
{
    auto v1 = generateRandomSet<int>(1000000);
    auto v2 = generateRandomSet<int>(1000000);

    // just warming up
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto intersection = intersectWithVectors(v1, v2);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "intersection with vectors: " << (end - start).count() << std::endl;
    }
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto intersection = intersectWithHashSet(v1, v2);
        auto end = std::chrono::high_resolution_clock::now();
        std::cout << "intersection with hash set: " << (end - start).count() << std::endl;
    }

    // now measure
    long long total_vector = 0;
    long long total_hash = 0;
    for (int i = 0; i != 20; ++i)
    {
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto intersection = intersectWithHashSet(v1, v2);
            auto end = std::chrono::high_resolution_clock::now();
            total_hash += (end - start).count();
        }
        {
            auto start = std::chrono::high_resolution_clock::now();
            auto intersection = intersectWithVectors(v1, v2);
            auto end = std::chrono::high_resolution_clock::now();
            total_vector += (end - start).count();
        }
    }

    std::cout << "Total for vector intersection:   " << total_vector <<
               "\nTotal for hash set intersection: " << total_hash << std::endl;

    return 0;
}
