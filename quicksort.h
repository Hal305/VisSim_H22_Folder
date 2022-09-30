#ifndef QUICKSORT_H
#define QUICKSORT_H

#include <bits/stdc++.h>
#include <vector>
#include <Vertex.h>

class quicksort
{
public:
    quicksort();
    void quickSort(std::vector<Vertex> arr, int start, int end);
    int partition(std::vector<Vertex> arr, int start, int end);
};

#endif // QUICKSORT_H

//taken from: https://www.geeksforgeeks.org/cpp-program-for-quicksort/
