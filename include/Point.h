#pragma once

struct Point {
    double x, y;
};

inline double squaredDistance(const Point& a, const Point& b) {
    return (a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}
