#include "quicksort.h"

quicksort::quicksort()
{

}

void quicksort::quickSort(std::vector<Vertex> arr, int start, int end)
{
    // base case
        if (start >= end)
            return;

        // partitioning the array
        int p = partition(arr, start, end);

        // Sorting the left part
        quickSort(arr, start, p - 1);

        // Sorting the right part
        quickSort(arr, p + 1, end);
}

int quicksort::partition(std::vector<Vertex> arr, int start, int end)
{
    float pivot = arr.at(start).getX();

        int count = 0;
        for (int i = start + 1; i <= end; i++) {
            if (arr.at(i).getX() <= pivot)
                count++;
        }

        // Giving pivot element its correct position
        int pivotIndex = start + count;
        //std::swap(arr[pivotIndex], arr[start]); //doing this manually
        float temp1 = arr.at(pivotIndex).getX();
        float temp2 = arr.at(start).getX();
        arr.at(start).setXYZ(temp1, arr.at(start).getY(), arr.at(start).getZ());
        arr.at(pivotIndex).setXYZ(temp2, arr.at(pivotIndex).getY(), arr.at(pivotIndex).getZ());

        // Sorting left and right parts of the pivot element
        int i = start, j = end;

        while (i < pivotIndex && j > pivotIndex) {

            while (arr.at(i).getX() <= pivot) {
                i++;
            }

            while (arr.at(j).getX() > pivot) {
                j--;
            }

            if (i < pivotIndex && j > pivotIndex) {
                //std::swap(arr[i++], arr[j--]); //doing this manually, a bit unsure about ++ and -- syntax usage here
                float temp1 = arr.at(i++).getX();
                float temp2 = arr.at(j--).getX();
                arr.at(j).setXYZ(temp1, arr.at(j).getY(), arr.at(j).getZ());
                arr.at(i).setXYZ(temp2, arr.at(i).getY(), arr.at(i).getZ());
            }
        }

        return pivotIndex;
}
