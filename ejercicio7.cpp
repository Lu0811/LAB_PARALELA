#include <mpi.h>
#include <iostream>
#include <ctime>
using namespace std;

int main(int argc, char* argv[]) {
    int rango, tamano_comunicador;
    const int LIMITE_PING_PONG = 10000;
    int contador_ping_pong = 0;
    int mensaje = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);
    MPI_Comm_size(MPI_COMM_WORLD, &tamano_comunicador);

    if (tamano_comunicador != 2) {
        if (rango == 0) {
            cerr << "Este programa requiere exactamente 2 procesos." << endl;
        }
        MPI_Finalize();
        return 1;
    }

    clock_t inicio_clock, fin_clock;
    inicio_clock = clock();

    double inicio_mpi, fin_mpi;
    inicio_mpi = MPI_Wtime();

    while (contador_ping_pong < LIMITE_PING_PONG) {
        if (rango == 0) {
            MPI_Send(&mensaje, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(&mensaje, 1, MPI_INT, 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        } else if (rango == 1) {
            MPI_Recv(&mensaje, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            MPI_Send(&mensaje, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
        contador_ping_pong++;
    }

    fin_clock = clock();
    fin_mpi = MPI_Wtime();

    if (rango == 0) {
        double tiempo_clock = double(fin_clock - inicio_clock) / CLOCKS_PER_SEC;
        double tiempo_mpi = fin_mpi - inicio_mpi;

        cout << "Tiempo medido con clock(): " << tiempo_clock << " segundos" << endl;
        cout << "Tiempo medido con MPI_Wtime(): " << tiempo_mpi << " segundos" << endl;
    }

    MPI_Finalize();
    return 0;
}
