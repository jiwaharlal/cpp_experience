#define TESTING

#include "matrix.cpp"
#include <iostream>
#include <gtest/gtest.h>

TEST(Matrix, buildGraph)
{
    std::vector<std::vector<int>> edges = {{0, 1, 1}, {0, 2, 2}, {2, 3, 4}};

    auto adj = buildGraph(edges);

    std::cout << adj.size() << std::endl;

    EXPECT_EQ(adj.size(), 4);
    EXPECT_EQ(adj[0].size(), 2);
}

TEST(Matrix, reduceGraph)
{
    std::vector<std::vector<int>> edges = {{0, 1, 1}, {0, 2, 2}, {2, 3, 4}};
    std::vector<int> machines = {0, 3};

    auto adj = buildGraph(edges);
    transformToTree(adj, machines.front());
    auto reduced = reduceGraph(adj, machines);

    AdjWeightList ref_tree = {{{0, 0}, {3, 2}}, {}, {}, {{0, 2}}};

    EXPECT_EQ(reduced[1].size(), 0);
    EXPECT_EQ(reduced, ref_tree);
}

//TEST(Matrix, transformToEdgeMap)
//{
    //std::vector<std::vector<int>> edges = {{0, 1, 1}, {0, 2, 2}, {2, 3, 4}};

    //auto edge_map = transformToEdgeMap(buildGraph(edges));

    //EdgeMap ref_map = {{1, {0, 1}}, {2, {0, 2}}, {4, {2, 3}}};

    //EXPECT_EQ(edge_map, ref_map);
//}
