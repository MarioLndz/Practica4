//
// Created by mario on 6/05/23.
//
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <random>

using namespace std;

void ReduceFilas(vector<vector<int>> & reducida, vector<int> & minimo_fila, int num_nodos){

    int min=0;


    // calculamos mínimo de cada fila

    for (int i = 0; i < num_nodos; ++i) {
        min = 100;
        for (int j = 0; j < num_nodos; ++j) {
            if (reducida[i][j]!=(-1) && reducida[i][j] < min)
                min = reducida[i][j];
        }
        if(min==100) {// en el caso de que todos sean -1
            minimo_fila[i] = 0;
        }
        else{
            minimo_fila[i]=min;  // vamos guardando el minimo de columna en el vector minimo
        }

        minimo_fila[i] = min;  // vamos guardando el minimo de fila en el vector minimo
    }

    // una vez tenemos el mínimo de cada fila se lo restamos a dicha fila
    for (int i = 0; i < num_nodos; ++i){
        for (int j = 0; j < num_nodos; ++j){
            if (i != j) {
                reducida[i][j] -= minimo_fila[i];
            }
        }
    }

}

void ReduceColumnas(vector<vector<int>> & reducida, vector<int> & minimo_col, int num_nodos){

    int min=0;

    for (int i = 0; i < num_nodos; ++i) {
        min=100;
        for (int j = 0; j < num_nodos; ++j) {
            if(reducida[j][i]!=(-1) && reducida[j][i]<min)
                min=reducida[j][i];
        }
        if(min==100) {
            minimo_col[i] = 0;
        }
        else{
            minimo_col[i]=min;  // vamos guardando el minimo de columna en el vector minimo
        }


    }

    // restamos el valor minimo de cada columna a dicha columna

    for (int i = 0; i < num_nodos; ++i) {
        for (int j = 0; j < num_nodos; ++j) {
            if (i!=j) {
                reducida[j][i] -= minimo_col[i];
            }
        }
    }

}

void EliminaFilayColumna(vector<vector<int>> & reducida, int fila, int columna, int num_nodos){   // hace -1 todos los valores de la fila y la columna, ademas de la posición [col][fila]

    for (int i = 0; i < num_nodos; ++i) {
        reducida[fila][i]=-1;
    }
    for (int i = 0; i < num_nodos; ++i) {
        reducida[i][columna]=-1;
    }
    reducida[columna][fila]=-1;
}

int main (int argc, char * argv []) {
    if (argc != 2) {
        cout << "NUMERO INCORRECTO DE ARGUMENTOS" << endl;
        exit(-1);
    }

    ifstream file;
    file.open(argv[1]);

    if (!file) {
        cout << "ERROR ABRIENDO EL ARCHIVO" << endl;
        exit(-1);
    }

    int aux;
    file >> aux;

    const int NUM_NODOS = aux;

    vector<vector<int>> distancias(NUM_NODOS);
    for (int i = 0; i < NUM_NODOS; ++i) {
        distancias[i].reserve(NUM_NODOS);
    }

    for (int i = 0; i < NUM_NODOS; ++i) {
        for (int j = 0; j < NUM_NODOS; ++j) {
            int num;
            file >> num;
            distancias[i][j] = num;
        }
    }

    file.close();

    /*
    for (int i = 0; i < NUM_NODOS; ++i){
        for (int j = 0; j < NUM_NODOS; ++j){
            cout << distancias[i][j] << "\t";
        }
        cout << endl;
    }
     */

    // reducimos la matriz de distancias

    vector<vector<int>> reducida(NUM_NODOS);
    for (int i = 0; i < NUM_NODOS; ++i) {
        reducida[i].reserve(NUM_NODOS);
    }

    for (int i = 0; i < NUM_NODOS; ++i)
        for (int j = 0; j < NUM_NODOS; ++j)
            reducida[i][j]=distancias[i][j];


    vector<int> minimo_fila(NUM_NODOS);

    ReduceFilas(reducida,minimo_fila,NUM_NODOS);

    cout << "FILAS MINIMAS:\t";
    for(int i=0;i<NUM_NODOS;i++){
        cout << minimo_fila[i] << "\t";
    }
    cout << endl;

    for (int i = 0; i < NUM_NODOS; ++i){
        for (int j = 0; j < NUM_NODOS; ++j){
            cout << reducida[i][j] << "\t";
        }
        cout << endl;
    }

    // Tenemos que hacer lo mismo pero con las columnas

    vector<int> minimo_col (NUM_NODOS);   // vamos a guadar el minimo de todas las columnas

    ReduceColumnas(reducida,minimo_col,NUM_NODOS);

    cout << "COLUMNAS MINIMAS:\t";
    for(int i=0;i<NUM_NODOS;i++){
        cout << minimo_col[i] << "\t";
    }
    cout << endl;

    for (int i = 0; i < NUM_NODOS; ++i){
        for (int j = 0; j < NUM_NODOS; ++j){
            cout << reducida[i][j] << "\t";
        }
        cout << endl;
    }

    // calculamos el coste de la reducida

    int coste_reducida=0;

    for(int i=0;i<NUM_NODOS;++i){
        coste_reducida+=minimo_fila[i];
        coste_reducida+=minimo_col[i];
    }


    // ya tenemos nuestra matriz reducida y su coste, para probar que funciona:

    for (int i = 0; i < NUM_NODOS; ++i){
        for (int j = 0; j < NUM_NODOS; ++j){
            cout << distancias[i][j] << "\t";
        }
        cout << endl;
    }

    cout << endl << endl << "COSTE:" << coste_reducida << endl << endl;


    // tenemos que elegir cual será el siguiente nodo al que vayamos y para ello tendremos que ir
    // considerando todos los nodos restantes uno a uno

    vector<vector<vector<int>>> matrices_reducidas (NUM_NODOS);
    for (int i = 0; i < NUM_NODOS; ++i) {
        for(int j=0;j<NUM_NODOS;j++) {
            matrices_reducidas[i][j].reserve(NUM_NODOS);
        }
    }

    vector<int> costes(NUM_NODOS);

    for(int i=1;i<NUM_NODOS;++i){   // buble para pasar por todos los nodos restantes, empezamos desde 1

        // para acda nodo tendremos que poner a -1 la fila 0 y la columna i y
        // posteirormente realizar la reducción por filas y por columnas

        vector<vector<int>> reducida2(NUM_NODOS);
        for (int z = 0; z < NUM_NODOS; ++z) {
            reducida2[z].reserve(NUM_NODOS);  // aquí da problemas
        }
        cout << "p";
        for (int z = 0; z < NUM_NODOS; ++z)
            for (int j = 0; j < NUM_NODOS; ++j)
                reducida2[z][j]=reducida[z][j];

        EliminaFilayColumna(reducida2,0,i,NUM_NODOS);

        vector<int> minimo_fila2 (NUM_NODOS);   // vamos a guadar el minimo de todas las filas
        ReduceColumnas(reducida2,minimo_fila2,NUM_NODOS);

        vector<int> minimo_col2 (NUM_NODOS);   // vamos a guadar el minimo de todas las columnas
        ReduceColumnas(reducida2,minimo_col2,NUM_NODOS);

        int coste_reducida2=0;
        for(int z=0;z<NUM_NODOS;++z) {
            coste_reducida2 += minimo_fila2[z];
            coste_reducida2 += minimo_col2[z];
        }
        coste_reducida2 += coste_reducida;
        coste_reducida2 += reducida[0][i];



        for(int z=0;z<NUM_NODOS-1;++z){
            for(int j=0;j<NUM_NODOS-1;++j){
                matrices_reducidas[i-1][z][j]=reducida2[z][j];
            }
        }

        costes[i-1]=coste_reducida2;

    }

    // una vez tenemos todas las matrices y todos los costes nos tenemos que quedar con la que tenga menor coste

    for(int i=0;i<NUM_NODOS-1;++i){
        cout << costes[i] << "\t";
    }


    return (0);
}