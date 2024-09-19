#include <iostream>
#include <vector>
#include <omp.h>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono>


using namespace std;

void merge(vector<int>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<int> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);

        mergeSort(arr, mid + 1, right);


        merge(arr, left, mid, right);
    }
}

void mergeSortThread(vector<int>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergeSort(arr, left, mid);
            }
            #pragma omp section
            {
                mergeSort(arr, mid + 1, right);
            }
        }

        merge(arr, left, mid, right);
    }
}

int main() {
    ifstream arquivoNum("./Teste.txt");

    vector<int> arr;
    vector<int> arr2;
    string item;
    string valor;
    stringstream ss(valor);

    if(arquivoNum.is_open()){
        while(getline(arquivoNum, item, ' ')){
            arr.push_back(stoi(item));
            arr2.push_back(stoi(item));
        }
    }
    int n = arr.size();


    auto notThreadStart = chrono::high_resolution_clock::now();
    mergeSort(arr, 0, n - 1);
    auto notThreadEnd = chrono::high_resolution_clock::now();

    const chrono::duration<double> duration1 = notThreadEnd - notThreadStart;
    cout << "Tempo de ordenação sem thread: " << duration1.count() << "s" << endl;

    auto ThreadStart = chrono::high_resolution_clock::now();
    mergeSortThread(arr2, 0, n - 1);
    auto ThreadEnd = chrono::high_resolution_clock::now();

    const chrono::duration<double> duration2 = ThreadEnd - ThreadStart;
    cout << "Tempo de ordenação com thread: " << duration2.count() << "s" << endl;;


    return 0;
}