#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[]) {
    int rango, tamano_comunicador, valor_local, suma_global;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);          
    MPI_Comm_size(MPI_COMM_WORLD, &tamano_comunicador); 

    valor_local = rango + 1;
    int desplazamiento = 1;

    while (desplazamiento < tamano_comunicador) {
        int companero = rango ^ desplazamiento;

        int valor_recibido;
        MPI_Sendrecv(&valor_local, 1, MPI_INT, companero, 0, &valor_recibido, 1, MPI_INT, companero, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        valor_local += valor_recibido;

        desplazamiento *= 2;
    }

    cout << "Proceso " << rango << " - Suma global: " << valor_local << endl;

    MPI_Finalize();
    return 0;
}
