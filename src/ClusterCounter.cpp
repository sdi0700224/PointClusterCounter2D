#include "ClusterCounter.h"
#include "UnionFind.h"
#include <iostream>
#include <omp.h>
#include <unordered_map>
#include <cmath>

struct pair_hash {
    template <class T1, class T2>
    size_t operator()(const pair<T1, T2>& p) const {
        auto hash1 = hash<T1>{}(p.first);
        auto hash2 = hash<T2>{}(p.second);
        return hash1 ^ hash2; // Use XOR for speed
    }
};

int ClusterCounter::countClusters(const vector<Point>& points, double lengthScale) {
    long totalPoints = points.size();
    if (totalPoints == 0) return 0;
    if (lengthScale <= 0) {
        cerr << "Error: lengthScale must be positive." << endl;
        return -1;
    }

    UnionFind unionFind(totalPoints);
    double gridSize = lengthScale;
    double squaredLengthScale = lengthScale * lengthScale;
    
    // Local grids for each thread to avoid critical section on insertion
    vector<unordered_map<pair<int, int>, vector<int>, pair_hash>> localGrids(omp_get_max_threads());

    // Assign points to local grids
    #pragma omp parallel
    {
        int thread_id = omp_get_thread_num();
        auto& localGrid = localGrids[thread_id];

        #pragma omp for
        for (long i = 0; i < totalPoints; ++i) {
            int x = static_cast<int>(floor(points[i].x / gridSize));
            int y = static_cast<int>(floor(points[i].y / gridSize));
            localGrid[{x, y}].push_back(i);
        }
    }

    // Merge local grids into a global grid
    unordered_map<pair<int, int>, vector<int>, pair_hash> grid;
    for (const auto& localGrid : localGrids) {
        for (const auto& cell : localGrid) {
            auto& globalCell = grid[cell.first];
            globalCell.insert(globalCell.end(), cell.second.begin(), cell.second.end());
        }
    }

    // Convert the unordered_map to a vector of pairs for parallel processing
    vector<pair<pair<int, int>, vector<int>>> gridVector(grid.begin(), grid.end());

    // Check points within the same and neighboring grid cells
    #pragma omp parallel for
    for (long index = 0; index < (long)gridVector.size(); ++index) {
        const auto& cell = gridVector[index];
        const auto& cellPoints = cell.second;

        // Parallelize distance checks within the same cell
        #pragma omp parallel for collapse(2) // Parallelize also nested loop
        for (long i = 0; i < (long)cellPoints.size(); ++i) {
            for (long j = i + 1; j < (long)cellPoints.size(); ++j) {
                if (squaredDistance(points[cellPoints[i]], points[cellPoints[j]]) <= squaredLengthScale) {
                    #pragma omp critical
                    unionFind.unite(cellPoints[i], cellPoints[j]);
                }
            }
        }

        int x = cell.first.first;
        int y = cell.first.second;
        for (int deltaX = -1; deltaX <= 1; ++deltaX) {
            for (int deltaY = -1; deltaY <= 1; ++deltaY) {
                if (deltaX == 0 && deltaY == 0) continue; // No delta
                auto neighbor = grid.find({x + deltaX, y + deltaY});
                if (neighbor != grid.end()) {
                    const auto& neighborPoints = neighbor->second;

                    // Parallelize distance checks with neighboring cells
                    #pragma omp parallel for collapse(2)
                    for (long i = 0; i < (long)cellPoints.size(); ++i) {
                        for (long j = 0; j < (long)neighborPoints.size(); ++j) {
                            if (squaredDistance(points[cellPoints[i]], points[neighborPoints[j]]) <= squaredLengthScale) {
                                #pragma omp critical
                                unionFind.unite(cellPoints[i], neighborPoints[j]);
                            }
                        }
                    }
                }
            }
        }
    }

    // Count unique roots
    vector<int> rootCount(totalPoints, 0);
    #pragma omp parallel for
    for (int i = 0; i < totalPoints; ++i) {
        #pragma omp atomic // Use omp atomic for speed here cause cs is just an increment
        rootCount[unionFind.find(i)]++;
    }

    int clusterCount = 0;
    for (int count : rootCount) {
        if (count > 0) {
            clusterCount++;
        }
    }

    return clusterCount;
}
