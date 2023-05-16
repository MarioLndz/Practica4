#include <fstream>
#include <iostream>
#include <time.h>
#include <vector>
#include <random>

using namespace std;

int main (int argc, char * argv[]){
    if (argc != 2 && argc != 3){
        cout << "ERROR: FALTA NUMERO DE NODOS A GENERAR" << endl;
        exit (-1);
    }

    int aux = atoi(argv[1]);
    const int NUM_CASOS = aux;
    string fichero = argc == 2 ? "data_pvc.txt" : argv[2];

    srandom(time(NULL));
    std::random_device rd;
    std::default_random_engine eng (rd());
    std::uniform_int_distribution distr (0, 100);

    // Generamos tantos puntos como indique TOPE
    // Las coordenadas estarán entre ]-LIMITE_SUP, LIMITE_SUP[
    vector<int> distancias;

    for (int i = 0; i < NUM_CASOS; ++i){
        for (int j = 0; j < NUM_CASOS; ++j){
            if (i == j) {
                distancias.push_back(-1);
            } else {
                int num = distr(eng);
                distancias.push_back(num);
            }
        }
    }

    ofstream file;
    file.open(fichero.c_str());

    if (!file){
        cout << "ERROR ABRIENDO ARCHIVO" << endl;
        exit (-1);
    }

    file << NUM_CASOS << endl;

    for (auto it = distancias.begin(); it != distancias.end(); ++it){
        file << *it << endl;
    }

    file.close();

    return (0);
}
