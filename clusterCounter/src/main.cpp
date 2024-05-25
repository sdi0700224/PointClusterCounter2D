#include "Point.h"
#include "ClusterCounter.h"
#include <iostream>
#include <vector>
#include <chrono>
#include <cstdlib>
#include <stdexcept>
using namespace std;
using namespace chrono;

void printUsage();

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printUsage();
        return EXIT_FAILURE;
    }

    int numPoints;
    double lengthScale;

    try {
        numPoints = stoi(argv[1]);
        lengthScale = stod(argv[2]);

        if (numPoints <= 0 || lengthScale <= 0) {
            throw invalid_argument("Arguments must be positive values.");
        }
    } catch (exception &e) {
        cerr << "Error: " << e.what() << endl;
        printUsage();
        return EXIT_FAILURE;
    }

    unsigned int seed = 42; // Fixed seed for reproducibility
    srand(seed);

    vector<Point> points(numPoints);
    for (int i = 0; i < numPoints; ++i) {
        points[i] = {static_cast<double>(rand()) / RAND_MAX * 1000.0,
                     static_cast<double>(rand()) / RAND_MAX * 1000.0};
    }

    auto start = high_resolution_clock::now();
    int clusters = ClusterCounter::countClusters(points, lengthScale);
    auto end = high_resolution_clock::now();
    duration<double> execTime = end - start;

    if (clusters > 0) {
        cout << "Number of clusters: " << clusters << endl;
    } else {
        cerr << "Error: Number of clusters is non-positive (" << clusters << ")." << endl;
    }
    cout << "Execution time: " << execTime.count() << " seconds" << endl;

    return EXIT_SUCCESS;
}

void printUsage() {
    cout << "Usage: ./program <numPoints> <lengthScale>" << endl;
    cout << "  numPoints    : Number of points (positive integer)" << endl;
    cout << "  lengthScale  : Length scale (positive double)" << endl;
}
