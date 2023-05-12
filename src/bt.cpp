#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>
#include <set>

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
    set<int> sin_visitar;

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
        //solucion.push_back(0);
        //sin_visitar.erase(0);

        vector<bool> ya_pertenece (NUM_NODOS);
        ya_pertenece.at(0) = true;

        CalculaSalidasMinimas();

        pair<vector<int>, int> sol_greedy = Greedy (distancias, ya_pertenece, NUM_NODOS);
        COTA_GLOBAL = sol_greedy.second;
        solucion = sol_greedy.first;

    }

    int GetCotaGlobal () {
        return (COTA_GLOBAL);
    }

    void PintaDistancias () const {
        cout << "MATRIZ DE DISTANCIAS:\n";
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

    void PintaSolucion(int coste=-1) const {
        if (coste == -1){
            coste = CalculaCoste();
        }

        cout << "SOLUCION:\t[\t";
        for (int i = 0; i < NUM_NODOS; ++i){
            cout << solucion.at(i) << "\t";
        }
        cout << "]\tCoste: " << coste << endl;
    }

    void pvc2 (){
        vector<int> solucion_aux;
        pvc2(0, solucion_aux);
    }
    void pvc2 (int k, vector<int> & solucion_aux, int coste=0){
        sin_visitar.erase(k);
        solucion_aux.push_back(k);

        if (sin_visitar.empty()){
            coste += distancias[k][0];
            if (coste < COTA_GLOBAL){
                COTA_GLOBAL = coste;
                solucion = solucion_aux;
                //PintaSolucion(coste);
            }
        } else {
            int cota_local = CotaLocal2(coste);
            if (cota_local < COTA_GLOBAL){
                set<int> sin_visitar_aux = sin_visitar;

                for (auto it = sin_visitar_aux.begin(); it != sin_visitar_aux.end(); ++it){
                    coste += distancias[k][*it];
                    pvc2(*it, solucion_aux, coste);

                    coste -= distancias[k][*it];
                    sin_visitar.insert(*it);
                    solucion_aux.pop_back();
                }
            }
        }
    }

    int CotaLocal2 (int coste){
        coste += GetSalidaMinima(solucion.back());
        for (auto it = sin_visitar.begin(); it != sin_visitar.end(); ++it){
            coste += GetSalidaMinima(*it);
        }

        return (coste);
    }

    int GetSalidaMinima (int k){
        return (distancias[k][salidas_minimas.at(k)]);
    }

private:
    void reservaMemoria (const int & nodos){
        distancias.resize(nodos);

        for (int i = 0; i < nodos; ++i){
            distancias[i].resize(nodos);
            sin_visitar.insert(i);
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

    int CalculaCoste () const {
        int coste = 0;
        for (int i = 0; i < NUM_NODOS-1; ++i){
            coste += distancias[solucion.at(i)][solucion.at(i+1)];
        }
        coste += distancias[solucion.back()][0];

        return (coste);
    }
};
// https://code-with-me.global.jetbrains.com/xD3Sv4W8E5CUM3YfIP-yzQ#p=CL&fp=0B1879F530729FEAB4D59B9AF4A2A51073F71AE310EE667474671678948C5AAF

// Video Indio:

// BACKTRACKING: Diapositiva 114
// BRANCH AND BOUND: Diapositiva 165

int main (int argc, char * argv []) {
    if (argc != 2) {
        cout << "NUMERO INCORRECTO DE ARGUMENTOS" << endl;
        exit(-1);
    }

    BT backtracking (argv[1]);

    backtracking.PintaDistancias();
    backtracking.PintaSalidasMinimas();
    cout << "COTA GLOBAL INICIAL (GREEDY):\t" << backtracking.GetCotaGlobal() << endl;

    backtracking.pvc2();

    backtracking.PintaSolucion();


    return (0);
}