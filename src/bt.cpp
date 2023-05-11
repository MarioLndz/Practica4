#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

pair<vector<int>, int> Greedy (const vector<vector<int>>& distancias, vector<bool> ya_pertenece, const int NUM_NODOS){
    vector<int> visita;

    visita.push_back(0);  // la empresa está en 0
    int pos_actual = 0;   // posicion en la que estamos

    int indice_minimo = 1;   // para calcular la siguiente posicion
    int min_dist = distancias[pos_actual][indice_minimo];

    int distancia_solucion = 0;

    while (visita.size() < NUM_NODOS){
        min_dist = 500;
        for (int i = 1; i < NUM_NODOS; ++i){
            int num = distancias[pos_actual][i];

            if((!ya_pertenece[i]) && (num < min_dist) && (i != pos_actual)){
                min_dist = num;
                indice_minimo = i;
            }
        }
        distancia_solucion += min_dist;

        pos_actual = indice_minimo;
        visita.push_back(pos_actual);
        ya_pertenece[pos_actual] = true;
    }

    distancia_solucion += distancias[visita.at(visita.size()-1)][visita.at(0)];

    return (pair<vector<int>,int>(visita, distancia_solucion));

}

class BT{
private:
    int NUM_NODOS;
    vector<bool> ya_pertenece;

    vector<vector<int>> distancias;
    vector<int> solucion;

    int COTA_GLOBAL;

    vector<int> salidas_minimas;

public:
    BT (){
        NUM_NODOS = 0;
        COTA_GLOBAL = 0;
    }

    BT (const char filename []){
        load(filename);
    }

    void load (const char filename[]){
        ifstream file;
        file.open(filename);

        if (!file){
            cout << "ERROR ABRIENDO EL ARCHIVO" << endl;
            exit(-1);
        }

        int nodos_aux;
        file >> nodos_aux;

        NUM_NODOS = nodos_aux;

        reservaMemoria(NUM_NODOS);

        for (int i = 0; i < NUM_NODOS; ++i) {
            for (int j = 0; j < NUM_NODOS; ++j) {
                int num;
                file >> num;

                distancias[i][j] = num;
            }
        }

        file.close();

        // Siempre partimos de la ciudad 0
        ya_pertenece.at(0) = true;
        solucion.push_back(0);

        CalculaSalidasMinimas();

        pair<vector<int>, int> sol_greedy = Greedy (distancias, ya_pertenece, NUM_NODOS);
        COTA_GLOBAL = sol_greedy.second;

    }

    int GetSize (){
        return (NUM_NODOS);
    }

    int GetCotaGlobal () {
        return (COTA_GLOBAL);
    }

    void PintaDistancias () const {
        cout << "MATRIZ DE DISTANCIAS:\t";
        for (int i = 0; i < NUM_NODOS; ++i){
            for (int j = 0; j < NUM_NODOS; ++j){
                cout << distancias[i][j] << "\t";
            }
            cout << endl;
        }
    }

    void PintaSalidasMinimas () const {
        cout << "SALIDAS MINIMAS:\t";
        for (int i = 0; i < NUM_NODOS; ++i){
            cout << salidas_minimas.at(i) << "\t";
        }
        cout << endl;
    }

    void IniciaComp (int k){

    }

private:
    void reservaMemoria (const int & nodos){
        distancias.resize(nodos);
        ya_pertenece.resize(nodos);

        for (int i = 0; i < nodos; ++i){
            distancias[i].resize(nodos);
            ya_pertenece.at(i) = false;
        }
    }

    int SalidaMinima (const vector<int>& fila, const int NUM_NODOS){
        int indice_min = (fila[0] == -1) ? 1 : 0;

        for (int i = indice_min+1; i < NUM_NODOS; ++i){
            //cout <<
            if (fila[i] != -1){   // Si no es la diagonal
                if (fila[i] < fila[indice_min]){  // Nuevo minimo
                    indice_min = i;
                }
            }
        }

        return (indice_min);

    }

    void CalculaSalidasMinimas (){
        for (int i = 0; i < NUM_NODOS; ++i){
            salidas_minimas.push_back(SalidaMinima(distancias[i], NUM_NODOS));
        }
    }
};
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

    int nodos_aux;
    file >> nodos_aux;

    const int NUM_NODOS = nodos_aux;

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

    BT backtracking (argv[1]);

    ya_pertenece[0]=true; // marco el primer nodo como visitado, pues empezamos desde ahí
    /*
    for (int i = 0; i < NUM_NODOS; ++i){
        for (int j = 0; j < NUM_NODOS; ++j){
            cout << distancias[i][j] << "\t";
        }
        cout << endl;
    }*/
    backtracking.PintaDistancias();
    backtracking.PintaSalidasMinimas();
    cout << "COTA GLOBAL INICIAL (GREEDY):\t" << backtracking.GetCotaGlobal() << endl;

    // ------------- ALGORITMO GREEDY ------------- //
    //pair<vector<int>, int> sol_greedy = Greedy(distancias, ya_pertenece, NUM_NODOS);

    //int cota_global = sol_greedy.second;

    /*
    for (auto it = sol_greedy.first.begin(); it != sol_greedy.first.end(); ++it){
        cout << *it << "\t";
    }
    cout << endl;
     */

    //cout << "COTA GLOBAL INICIAL:\t"<< cota_global << endl;


    /*
    // ------------- BACKTRACKING ------------- //
    vector<int> solucion;

    vector<int> solucion_aux;
    solucion_aux.push_back(0);
    ya_pertenece[0] = true;

    int nodo_actual = 1;
    solucion_aux.push_back(nodo_actual);
    ya_pertenece[1] = true;

    int cota_local = 0;
    int num_soluciones = solucion_aux.size()-1;

    // Sumamos las distancias de los nodos ya elegidos
    for (int i = 0; i < num_soluciones; ++i){
        cota_local += distancias[solucion_aux.at(i)][solucion_aux.at(i+1)];
    }

    // Coste mínimo de las salidas de cada nodo
    cota_local += salidas_minimas[nodo_actual]; //Salir del nodo actual
    for (int i = 1; i < NUM_NODOS; ++i){        // Salir del resto de nodos
        if (!ya_pertenece[i]){
            cota_local += salidas_minimas[i];
        }
    }
    cout << "COTA LOCAL DEL NODO:\t" << cota_local << endl;
    /*
    for (int i = 1; i < NUM_NODOS; ++i){
        bool factible = true;
        int nodo = i;

        while (factible) {

        }
    }*/



    return (0);
}