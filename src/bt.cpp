#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

int Minimo(const vector<vector<int>>& distancias, const vector<bool> & v, int pos){


}

// https://code-with-me.global.jetbrains.com/ZU3t2oYIEaHkkUu93N6O4A#p=CL&fp=CB515FAE1B2D2DAA042E8BA8B33919512C260C42B173BA51B3B9384B16885C49

// Video Indio:

// BACKTRACKING: Diapositiva 114
// BRANCH AND BOUND: Diapositiva 165

int main (int argc, char * argv []) {
    if (argc != 2) {
        cout << "NUMERO INCORRECTO DE ARGUMENTOS" << endl;
        exit(-1);
    }

    ifstream file;
    file.open(argv[1]);

    if (!file){
        cout << "ERROR ABRIENDO EL ARCHIVO" << endl;
        exit(-1);
    }

    int aux;
    file >> aux;

    const int NUM_NODOS = aux;

    vector<vector<int>> distancias (NUM_NODOS);
    for (int i = 0; i < NUM_NODOS; ++i){
        distancias[i].reserve(NUM_NODOS);
    }

    vector<bool> ya_pertenece(NUM_NODOS);

    for (int i = 0; i < NUM_NODOS; ++i) {
        for (int j = 0; j < NUM_NODOS; ++j) {
            int num;
            file >> num;

            distancias[i][j] = num;
        }
        ya_pertenece[i] = false;
    }

    file.close();

    ya_pertenece[0]=true; // marco el primer nodo como visitado, pues empezamos desde ahí

    for (int i = 0; i < NUM_NODOS; ++i){
        for (int j = 0; j < NUM_NODOS; ++j){
            cout << distancias[i][j] << "\t";
        }
        cout << endl;
    }

    return (0);
}