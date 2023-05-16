//
// Created by mario on 13/05/23.
//
#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <chrono>
#include <random>
#include <queue>
using namespace std;
using namespace std::chrono;

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

struct nodo {
    vector<int> solucion;
    set<int> sin_visitar;
    int coste;

    nodo(){
        coste = 0;
    }

    nodo(const vector<int> & la_solucion, const set<int> & los_sin_visitar, int el_coste){
        solucion = la_solucion;
        sin_visitar = los_sin_visitar;
        coste = el_coste;
    }

    int NodosVisitados () const{
        return (solucion.size());
    }

    bool operator< (const nodo & otro) const {
        return (this->coste > otro.coste);
    }
};


struct Comparison {
    bool operator() (const pair<nodo,int>& n1, const pair<nodo,int> & n2){
        return (n1.second > n2.second);
    }
};

class BB {
private:
    int NUM_NODOS;

    vector<vector<int>> distancias;
    int COTA_GLOBAL;

    vector<int> salidas_minimas;
    int minimo_coste;

    nodo origen;

    vector<int> solucion;

    priority_queue<pair<nodo,int>, vector<pair<nodo,int>>, Comparison> caminos;

public:
    BB (){
        NUM_NODOS = 0;
        COTA_GLOBAL = 0;
    }

    BB (const char filename []){
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

        set<int> set_origen;

        for (int i = 0; i < NUM_NODOS; ++i) {
            for (int j = 0; j < NUM_NODOS; ++j) {
                int num;
                file >> num;

                distancias[i][j] = num;
            }

            if (i != 0){
                set_origen.insert(i);
            }

        }

        file.close();

        origen = nodo(vector<int>{0}, set_origen, 0);

        // Siempre partimos de la ciudad 0
        //solucion.push_back(0);
        //sin_visitar.erase(0);

        vector<bool> ya_pertenece (NUM_NODOS);
        ya_pertenece.at(0) = true;

        CalculaSalidasMinimas();

        minimo_coste = distancias[0][salidas_minimas.at(0)];
        for (int i = 1; i < NUM_NODOS; ++i){
            if (minimo_coste > distancias[i][salidas_minimas.at(i)]){
                minimo_coste = distancias[i][salidas_minimas.at(i)];
            }
        }


        pair<vector<int>, int> sol_greedy = Greedy (distancias, ya_pertenece, NUM_NODOS);
        COTA_GLOBAL = sol_greedy.second;
        solucion = sol_greedy.first;

    }

    int getNumNodos() const{
        return NUM_NODOS;
    }

    int GetCotaGlobal () const {
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
        for (int i = 0; i < solucion.size(); ++i){
            cout << solucion.at(i) << "\t";
        }
        cout << "]\tCoste: " << coste << endl;
    }

    void pvc (int cota){
        if (cota == 1){
            pvc1();
        } else if (cota == 2){
            pvc2();
        }
    }

    void pvc1(){
        caminos.push(pair<nodo,int>{this->origen, CotaLocal2(this->origen)});

        while (!caminos.empty()){
            nodo prometedor = caminos.top().first;
            caminos.pop();

            if (prometedor.sin_visitar.empty()){
                prometedor.coste += distancias[prometedor.solucion.back()][0];

                if (prometedor.coste < COTA_GLOBAL){
                    solucion = prometedor.solucion;
                    COTA_GLOBAL = prometedor.coste;
                }

            } else {
                if (prometedor.coste < COTA_GLOBAL){
                    for (auto it = prometedor.sin_visitar.begin(); it != prometedor.sin_visitar.end(); ++it){
                        nodo aux = prometedor;

                        aux.sin_visitar.erase(*it);
                        aux.solucion.push_back(*it);
                        aux.coste += distancias[prometedor.solucion.back()][*it];

                        int cota = CotaLocal1(aux);

                        if (cota < COTA_GLOBAL){
                            caminos.push(pair<nodo,int>{aux,cota});
                        }
                    }
                }

            }

        }
    }

    int CotaLocal1(const nodo & n) const{
        int coste = n.coste;
        int num_nodos_sin_visitar = n.sin_visitar.size();

        coste += (minimo_coste*num_nodos_sin_visitar);

        return (coste);
    }

    void pvc2 () {
        caminos.push(pair<nodo,int>{this->origen, CotaLocal2(this->origen)});

        while (!caminos.empty()){
            nodo prometedor = caminos.top().first;
            caminos.pop();

            if (prometedor.sin_visitar.empty()){
                prometedor.coste += distancias[prometedor.solucion.back()][0];

                if (prometedor.coste < COTA_GLOBAL){
                    solucion = prometedor.solucion;
                    COTA_GLOBAL = prometedor.coste;
                }

            } else {
                if (prometedor.coste < COTA_GLOBAL){
                    for (auto it = prometedor.sin_visitar.begin(); it != prometedor.sin_visitar.end(); ++it){
                        nodo aux = prometedor;

                        aux.sin_visitar.erase(*it);
                        aux.solucion.push_back(*it);
                        aux.coste += distancias[prometedor.solucion.back()][*it];

                        int cota = CotaLocal2(aux);

                        if (cota < COTA_GLOBAL){
                            caminos.push(pair<nodo,int>{aux,cota});
                        }
                    }
                }

            }

        }
    }

    int CotaLocal2 (const nodo & n){
        int coste = n.coste;

        coste += GetSalidaMinima(solucion.back());

        for (auto it = n.sin_visitar.begin(); it != n.sin_visitar.end(); ++it){
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
        }
    }

    int SalidaMinima (const vector<int>& fila) const{
        int indice_min = (fila[0] == -1) ? 1 : 0;

        for (int i = indice_min+1; i < NUM_NODOS; ++i){
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
            salidas_minimas.push_back(SalidaMinima(distancias[i]));
        }
    }

    int CalculaCoste () const {
        int coste = 0;
        for (int i = 0; i < NUM_NODOS-1; ++i){
            //cout << distancias[solucion.at(i)][solucion.at(i+1)] << " + ";
            coste += distancias[solucion.at(i)][solucion.at(i+1)];
        }
        coste += distancias[solucion.back()][0];
        //cout << distancias[solucion.back()][0] << " = " << coste << endl;

        return (coste);
    }

};

int main (int argc, char * argv[]){
    if (argc != 2) {
        cout << "NUMERO INCORRECTO DE ARGUMENTOS" << endl;
        exit(-1);
    }

    BB branch_bound (argv[1]);
    const int NODOS = branch_bound.getNumNodos();

    //branch_bound.PintaDistancias();
    //branch_bound.PintaSalidasMinimas();

    high_resolution_clock::time_point t_antes, t_despues;
    duration<double> transcurrido;
    t_antes = high_resolution_clock::now();

    //branch_bound.PintaDistancias();

    branch_bound.pvc(1);

    t_despues = high_resolution_clock::now();
    transcurrido = duration_cast<duration<double>> (t_despues - t_antes);
    cout << NODOS << "\t" << transcurrido.count() << endl;

    //branch_bound.PintaSolucion();
}
