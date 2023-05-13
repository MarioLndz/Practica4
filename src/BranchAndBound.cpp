//
// Created by mario on 13/05/23.
//
#include <iostream>
#include <vector>
#include <set>
#include <fstream>
#include <queue>
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

class BB{
private:
    int NUM_NODOS;

    vector<vector<int>> distancias;
    int COTA_GLOBAL;

    vector<int> salidas_minimas;

    priority_queue<int, vector<int>, greater<>> caminos;

    struct nodo {
        vector<int> solucion;
        set<int> sin_visitar;
        int coste;
    };

    nodo origen;

    vector<int> solucion;

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
                caminos.push(i);
                set_origen.insert(i);
            }

        }

        file.close();

        origen.sin_visitar = set_origen;
        origen.solucion = vector<int>{0};
        origen.coste = 0;

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

    void PintaDecisiones (){
        auto aux = caminos;
        int tamanio = aux.size();

        cout << "DECISIONES:\t[\t";
        for (int i = 0; i < tamanio; ++i){
            cout << aux.top() << "\t";
            aux.pop();
        }
        cout << "]" << endl;
    }

    void pvc () {


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

// https://code-with-me.global.jetbrains.com/u4l7RQH69xtukLfXXqQLkw#p=CL&fp=B7CF77FD7E61CC6ADE27EBA1EF1E04CF3DE6AD42766C5FE2F8A2222907289AE9

int main (int argc, char * argv[]){
    if (argc != 2) {
        cout << "NUMERO INCORRECTO DE ARGUMENTOS" << endl;
        exit(-1);
    }
    /*
    priority_queue<int, vector<int>, greater<>> quee;

    quee.push(1);
    quee.push(2);
    quee.push(3);

    for (int i = 0; i < 3; ++i){
        cout << quee.top() << endl;
        quee.pop();
    }*/

    BB branch_bound (argv[1]);
    branch_bound.PintaDistancias();
    branch_bound.PintaDecisiones();
    branch_bound.PintaSalidasMinimas();
    //branch_bound.pvc2();
}
