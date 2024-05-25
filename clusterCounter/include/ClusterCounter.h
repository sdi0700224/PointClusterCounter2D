#pragma once
#include "Point.h"
#include <vector>
using namespace std;

class ClusterCounter {
public:
    static int countClusters(const vector<Point>& points, double lengthScale);
};
