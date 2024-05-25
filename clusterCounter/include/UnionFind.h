#pragma once
#include <vector>
using namespace std;

class UnionFind {
private:
    vector<int> parent;
    vector<int> rank;

public:
    UnionFind(int n);
    int find(int p);
    void unite(int p, int q);
};
