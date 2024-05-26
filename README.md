# PointClusterCounter2D
This project counts the number of clusters of points in a 2D space using a grid-based spatial hashing and Union-Find data structure. For distance calculation and comparison with threshold squared distance is used, to avoid costly sqrt. Only distance between points of same cell is compared and with points of the neighbour cells. This way we cluster points without having to compare all points of the 2D space that are given as input.

OpenMP is used for parallel computations. A local grid vessel is used for each thread to hash points to cells, so pushbacks do not need to be a critical section. This boosts speed and efficiency and after hashing the local cells are merged into the global grid. Grid is converted to a vector for parallel processing with OpenMP, distances are compared inside cell and with nearby cells and a disjoint set class is updated if points are closer than threshold and belong to different clusters. In the end different clusters are counted and returned.

# Known issues
-Worst case scenario, when distance threshold is big enough for all points to end up in the same cell, remains bad and works only for small inputs.

-Duplicate points are being placed in same cell and same cluster, so cluster count remains correct. No check is made for duplicate points. Adding duplicate
 points though, is leading towards the worst case scenario

-Valgrid may report some memory leaks that are not real with OpenMP use.
