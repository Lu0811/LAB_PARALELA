#include <mpi.h>
#include <iostream>
#include <vector>
#define CANTIDAD_CATEGORIAS 5

using namespace std;

struct RangoCategorias {
    double limite_inferior;
    double limite_superior;
    double tamano_intervalo;
};

RangoCategorias inicializar_rangos(double limite_inferior, double limite_superior, int cantidad_categorias) {
    RangoCategorias rango;
    rango.limite_inferior = limite_inferior;
    rango.limite_superior = limite_superior;
    rango.tamano_intervalo = (limite_superior - limite_inferior) / cantidad_categorias;
    return rango;
}

int asignar_categoria(double valor, const RangoCategorias& rango) {
    int indice_categoria = (int)((valor - rango.limite_inferior) / rango.tamano_intervalo);
    if (indice_categoria >= CANTIDAD_CATEGORIAS) {
        indice_categoria = CANTIDAD_CATEGORIAS - 1;
    }
    return indice_categoria;
}

void procesar_datos_locales(const vector<double>& datos_locales, int* histograma_local, const RangoCategorias& rango) {
    for (double valor : datos_locales) {
        int categoria = asignar_categoria(valor, rango);
        histograma_local[categoria]++;
    }
}

void imprimir_histograma(const int* histograma_global) {
    cout << "Resultado del Histograma:" << endl;
    for (int i = 0; i < CANTIDAD_CATEGORIAS; i++) {
        cout << "Categoria " << i << ": " << histograma_global[i] << endl;
    }
}

int main(int argc, char* argv[]) {
    double conjunto_datos[] = {1.3, 2.9, 0.4, 0.3, 1.3, 4.4, 1.7, 0.4, 3.2, 0.3, 4.9, 2.4, 3.1, 4.4, 3.9, 0.4, 4.2, 4.5, 4.9, 0.9};
    int cantidad_datos = 20;
    int histograma_local[CANTIDAD_CATEGORIAS] = {0};
    int histograma_global[CANTIDAD_CATEGORIAS] = {0};

    MPI_Init(&argc, &argv);
    int tamano_mundo, rango_mundo;
    MPI_Comm_size(MPI_COMM_WORLD, &tamano_mundo);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango_mundo);

    RangoCategorias rango = inicializar_rangos(0.0, 5.0, CANTIDAD_CATEGORIAS);

    int cantidad_datos_local = cantidad_datos / tamano_mundo;
    vector<double> datos_locales(conjunto_datos + rango_mundo * cantidad_datos_local,
                                 conjunto_datos + (rango_mundo + 1) * cantidad_datos_local);

    procesar_datos_locales(datos_locales, histograma_local, rango);

    MPI_Reduce(histograma_local, histograma_global, CANTIDAD_CATEGORIAS, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rango_mundo == 0) {
        imprimir_histograma(histograma_global);
    }

    MPI_Finalize();
    return 0;
}
