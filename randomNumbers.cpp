#include <iostream>
#include <time.h>
#include <vector>
#include <algorithm>
#include <random>
#include <fstream>

using namespace std;

#define MIN 1
#define MAX 100000000
#define QTDE 10000000  //precisa ser menor que MAX

void shuffle(int *array) {
    for (int i = MAX - MIN - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = array[j];
        array[j] = array[i];
        array[i] = tmp;
    }
}

int main(void) {
    srand(time(0));
    vector<int> numeros;

    ofstream Myfile("Teste.txt");




    int interations = 0;

    auto rng = default_random_engine{};

    
    for (int i = 0; i < QTDE; i++) {
        numeros.push_back(i);
    }
    shuffle(begin(numeros), end(numeros), rng);

    for(int i = 0; i < numeros.size(); i++){
        Myfile << numeros[i] << " ";
    }

    Myfile.close();
    for (int i = 0; i < numeros.size(); i++) {
        cout << numeros[i] << endl;
        interations += 1;

    }



    cout << interations << endl;
    return 0;
}