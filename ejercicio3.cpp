#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int rango, tam_comunicador, valor_local, suma_global = 0;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);
    MPI_Comm_size(MPI_COMM_WORLD, &tam_comunicador);

    // Asigna un valor local inicial a cada proceso (rango + 1)
    valor_local = rango + 1;

    int paso = 1;
    while (paso < tam_comunicador)
    {
        if (rango % (2 * paso) == 0) {
            int valor_recibido;
            MPI_Recv(&valor_recibido, 1, MPI_INT, rango + paso, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            valor_local += valor_recibido;
        } else if (rango % paso == 0) {
            MPI_Send(&valor_local, 1, MPI_INT, rango - paso, 0, MPI_COMM_WORLD);
            break;
        }
        paso *= 2;
    }

    if (rango == 0)
    {
        cout << "La suma total es " << valor_local << endl;
    }

    MPI_Finalize();
    return 0;
}
