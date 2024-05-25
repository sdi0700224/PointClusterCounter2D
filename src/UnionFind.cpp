#include "UnionFind.h"

UnionFind::UnionFind(int n) : parent(n), rank(n, 0) {
    for (int i = 0; i < n; ++i) {
        parent[i] = i;
    }
}

int UnionFind::find(int p) {
    if (parent[p] != p) {
        parent[p] = find(parent[p]);
    }
    return parent[p];
}

void UnionFind::unite(int p, int q) {
    int rootP = find(p);
    int rootQ = find(q);
    if (rootP != rootQ) {
        if (rank[rootP] < rank[rootQ]) {
            parent[rootP] = rootQ;
        } else if (rank[rootP] > rank[rootQ]) {
            parent[rootQ] = rootP;
        } else {
            parent[rootQ] = rootP;
            rank[rootP]++;
        }
    }
}
