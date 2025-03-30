#include <iostream>
#include <vector>
#include <algorithm>
#include <omp.h>
#include <sys/time.h>
#include <cstdlib>

using namespace std;

bool found = false;
int foundIndex = -1;

double step(){
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return ((double)tp.tv_sec + (double)tp.tv_usec * 1e-6);
}


bool parallelBinarySearch(const vector<int> &arr, int target){
    int left = 0, right = arr.size() - 1;
    bool result = false;
    
    #pragma omp parallel
    {
        while (left <= right && !found){
            int mid = left + (right - left) / 2;
            
            #pragma omp critical
            {
                if (arr[mid] == target){
                    found = true;
                    foundIndex = mid;
                    result = true;
                }
                else if (arr[mid] < target){
                    left = mid + 1;
                }
                else{
                    right = mid - 1;
                }
            }
        }
    }
    return result;
}

int main(){
    const int SIZE = 100000000;
    vector<int> orderedArr(SIZE);
    srand(time(NULL));
    
    for (int i = 0; i < SIZE; ++i){
        orderedArr[i] = rand() % 1000 + 1;
    }
    
    sort(orderedArr.begin(), orderedArr.end());
    
    printf("Arreglo ordenado: ");
    for (int i = 0; i < min(SIZE, 100); ++i){
        printf("%d ", orderedArr[i]);
    }
    printf("\n");
    
    int target = orderedArr[rand() % SIZE];
    
    printf("Buscando %d en el arreglo ordenado...\n", target);
    
    found = false;
    foundIndex = -1;
    
    double startPar = step();
    bool isFoundPar = parallelBinarySearch(orderedArr, target);
    double endPar = step();
    
    if (isFoundPar){
        printf("Elemento encontrado en la posición: %d\n", foundIndex);
    }
    else{
        printf("Elemento no encontrado.\n");
    }
    
    printf("Tiempo paralelo: %f segundos\n", (endPar - startPar));
    return 0;
}

