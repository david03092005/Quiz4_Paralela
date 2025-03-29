#include <iostream>
#include <vector>
#include <cmath>
#include <omp.h>
#include <sys/time.h>

using namespace std;

void print_M(vector<vector<int>> &M){
	int n = M.size();	
	for (int i = 0; i < n; i++){
		for (int j = 0; j < n; j++){
			printf(" %d ", M[i][j]);
		}
		printf("\n");
	}
}

// Función de mergesort para ordenar un vector
void merge(vector<int>& arr, int left, int mid, int right, bool flag){
        int n1 = mid - left + 1;
        int n2 = right - mid;
        vector<int> L(n1), R(n2); // Vectores temporales.

        for(int i = 0; i < n1; i++){
                L[i] = arr[left + i];
        }

        for(int j = 0; j < n2; j++){
                R[j] = arr[mid + 1 + j];
        }

        int i = 0, j = 0, k = left;

        while(i < n1 && j < n2){
                if (flag){
			if(L[i] <= R[j]){
                        	arr[k] = L[i];
                        	i++;
                	}
                	else{
                	        arr[k] = R[j];
                	        j++;
               		}
                	k++;
		}
		else{
			if(L[i] > R[j]){
				arr[k] = L[i];
				i++;
			}
			else{
				arr[k] = R[j];
				j++;
			}
			k++;
		}
        }

        while(i < n1){
                arr[k] = L[i];
                i++;
                k++;
        }

        while(j < n2){
                arr[k] = R[j];
                j++;
                k++;
        }
}

void mergeSort(vector<int>& arr, int left, int right, bool flag) {
        if (left < right) {
                int mid = left + (right - left) / 2;
                mergeSort(arr, left, mid, flag);
                mergeSort(arr, mid + 1, right, flag);
                merge(arr, left, mid, right, flag);
        }
}


// Sorts the row-th row of M in ascending order if ascending is true,
// or in descending order if ascending is false.
void sort_row(vector<vector<int>> &M, int row, bool ascending) {
	vector<int> aux(M[row].begin(), M[row].end());
	mergeSort(aux, 0, M.size() - 1, ascending);
	M[row] = aux;
}

// Sorts the column-th column of M in ascending order.
void sort_column(vector<vector<int>> &M, int column, int n) {
	vector<int> aux(n);
	for (int i = 0; i < n; i++){
		aux[i] = M[i][column];
	}
	mergeSort(aux, 0, n - 1, true);
	
      	for (int j = 0; j < n; j++){
		M[j][column] = aux[j];
	}
}

// Sorts M in snake-like order, assuming that M is an n × n matrix.
void shearsort(int n, vector<vector<int>> &M) {
	int repeat = ceil(log2(n)) + 1;
	for (int r = 0; r < repeat; r++){
		#pragma omp parallel for
		for (int i = 0; i < n; i++){
			bool asc = false;
			if (i % 2 == 0){
				asc = true;
			}
			sort_row(M, i, asc);
		}
	
		#pragma omp parallel for
		for (int j = 0; j < n; j++){
			sort_column(M, j, n);
		}		
	}
}

double step(){
        struct timeval tp;
        gettimeofday(&tp, NULL);
        return ((double) tp.tv_sec + (double) tp.tv_usec * 1e-6);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("No hay la cantidad de parametros suficientes (tamano de la matriz n*n)");
        return 1;
    }

    int n = atoi(argv[1]);
    if (n <= 0) {
        printf("Error: El tamaño de la matriz debe ser un número entero positivo.\n");
        return 1;
    }

    srand(time(0));
    vector<vector<int>> matrix(n, vector<int>(n));

    // Llenar la matriz con valores aleatorios
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            	matrix[i][j] = rand() % 100;
	}
    }

    printf("\nMatriz original:\n");
    print_M(matrix);
    double start = step();
    shearsort(n, matrix);
    double end = step();
    printf("Matriz ordenada con ShearSort\n");
    print_M(matrix);

    printf("Tiempo de ejecución : %f\n", end - start);



    return 0;
}
