#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <omp.h> // Biblioteca OpenMP

using namespace std;

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    vector<int> L(n1), R(n2);

    for(int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for(int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;

    while(i < n1 && j < n2) {
        if(L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while(i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while(j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    const int threshold = 20000; // Defina um threshold adequado
    if(left < right) {
        int mid = left + (right - left) / 2;

        if (right - left > threshold) {
            #pragma omp parallel sections
            {
                #pragma omp section
                mergeSort(arr, left, mid);

                #pragma omp section
                mergeSort(arr, mid + 1, right);
            }
        } else {
            mergeSort(arr, left, mid);
            mergeSort(arr, mid + 1, right);
        }

        merge(arr, left, mid, right);
    }
}

int main() {
    ifstream arquivoNum("./Teste.txt");
    vector<int> Numbers;
    string item;

    if(arquivoNum.is_open()) {
        while(getline(arquivoNum, item, ' ')) {
            Numbers.push_back(stoi(item));
        }
    }

    omp_set_num_threads(omp_get_num_procs()); // Use todos os núcleos disponíveis

    double start_time = omp_get_wtime();
    mergeSort(Numbers, 0, Numbers.size() - 1);
    double end_time = omp_get_wtime();

    cout << "Tempo de ordenacao com OMP: " << end_time - start_time << " segundos" << endl;
    cout << Numbers[0] << " " << Numbers[65] << "," << Numbers[4567] << endl;

    return 0;
}
