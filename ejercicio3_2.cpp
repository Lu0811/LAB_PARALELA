#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int rango, tamanoComunicador, valorLocal, sumaGlobal = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);
    MPI_Comm_size(MPI_COMM_WORLD, &tamanoComunicador);

    valorLocal = rango + 1;
    int paso = 1;

    while (paso < tamanoComunicador)
    {
        if (rango % (2 * paso) == 0 )
        {
            int emisor = rango + paso;
            if (emisor < tamanoComunicador)
            {
                int valorRecibido;
                MPI_Recv(&valorRecibido, 1, MPI_INT, emisor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                valorLocal += valorRecibido;
            }
        } 
        else if ((rango - paso) % (2 * paso) == 0)
        {
            int receptor = rango - paso;
            MPI_Send(&valorLocal, 1, MPI_INT, receptor, 0, MPI_COMM_WORLD);
            break;
        }
        paso *= 2;
    }

    if (rango == 0)
    {
        cout << "La suma total es " << valorLocal << endl;
    }

    MPI_Finalize();
    return 0;
}

