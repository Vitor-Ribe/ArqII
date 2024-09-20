#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cstring>
#include <chrono>

using namespace std;

// Função auxiliar do MergeSort paralelizado

void mergest(vector<vector<string>>& arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<vector<string>> L(n1), R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;

    while (i < n1 && j < n2) {
        if (L[i][0] <= R[j][0]) {
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

// Função de ordenação

void mergeSortst(vector<vector<string>>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergeSortst(arr, left, mid);
        mergeSortst(arr, mid + 1, right);

        mergest(arr, left, mid, right);
    }
}

// Função de ordenação com 2 threads

void mergeSortThreadst2(vector<vector<string>>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergeSortst(arr, left, mid);
            }
            #pragma omp section
            {
                mergeSortst(arr, mid + 1, right);
            }
        }

        mergest(arr, left, mid, right);
    }
}

// Função de ordenação com 4 threads

void mergeSortThreadst4(vector<vector<string>>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergeSortst(arr, left, mid / 2);
            }
            #pragma omp section
            {
                mergeSortst(arr, mid / 2 + 1, mid);
            }
            #pragma omp section
            {
                mergeSortst(arr, mid + 1, mid + mid / 2);
            }
            #pragma omp section
            {
                mergeSortst(arr,mid + mid / 2 + 1, right);
            }
        }

        mergest(arr, left, mid, right);
    }
}

// Função de ordenação com 8 threads

void mergeSortThreadst8(vector<vector<string>>& arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        #pragma omp parallel sections
        {
            #pragma omp section
            {
                mergeSortst(arr, left, mid / 4); // 0 a 12.5
            }
            #pragma omp section
            {
                mergeSortst(arr, mid / 4 + 1, mid / 2); //12.5 a 25
            }
            #pragma omp section
            {
                mergeSortst(arr, mid / 2 + 1, mid / 2 + mid / 4); //25 a 37.5
            }
            #pragma omp section
            {
                mergeSortst(arr,mid / 2 + mid / 4 + 1, mid); //37.5 a 50
            }
            #pragma omp section
            {
                mergeSortst(arr, mid + 1, mid + mid / 4); //50 a 62.5
            }
            #pragma omp section
            {
                mergeSortst(arr, mid + mid / 4 + 1, mid + mid / 2);//62.5 a 75
            }
            #pragma omp section
            {
                mergeSortst(arr, mid + mid / 2 + 1, mid + mid / 2 + mid / 4);//75 a 87.5
            }
            #pragma omp section
            {
                mergeSortst(arr,mid + mid / 2 + mid / 4 + 1, right);//87.5 a 100
            }
        }

        mergest(arr, left, mid, right);
    }
}

int main(){
    ifstream arquivoSteam("./reviews.csv");

    vector<vector<string>> dados1;
    vector<vector<string>> dados2;
    string linha;

    if(arquivoSteam.is_open()){
        while(getline(arquivoSteam, linha)){
            stringstream ss(linha);
            string item;
            vector<string> linhaDados;
            try{
                while(getline(ss, item, ','))
                    linhaDados.push_back(item);
                dados1.push_back(linhaDados);
                dados2.push_back(linhaDados);

            }catch (const std::exception& e) {
                std::cerr << "Error: " << e.what() << std::endl;
                return 1;
            }
        }  
    }

    auto start = chrono::high_resolution_clock::now();
    mergeSortst(dados1, 0, dados1.size()-1);
    auto end = chrono::high_resolution_clock::now();

    const chrono::duration<double> duration0 = end - start;
    cout << "Tempo de ordenação sem thread: " << duration0.count() << "s" << endl;
    
    auto ThreadStart = chrono::high_resolution_clock::now();
    mergeSortThreadst4(dados2, 0, dados2.size()-1);
    auto ThreadEnd = chrono::high_resolution_clock::now();

    const chrono::duration<double> duration1 = ThreadEnd - ThreadStart;
    cout << endl << "Tempo de ordenação com thread: " << duration1.count() << "s" << endl;

    return 0;
}