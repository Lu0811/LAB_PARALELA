#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
    int rango, tamano_comunicador, valor_local, suma_global = 0;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rango);          
    MPI_Comm_size(MPI_COMM_WORLD, &tamano_comunicador); 

    valor_local = rango + 1;

    while (paso < tamano_comunicador)
    {
        if (rango % (2 * paso) == 0)
        {
            int emisor = rango + paso;
            if (emisor < tamano_comunicador) 
                int valor_recibido;
                MPI_Recv(&valor_recibido, 1, MPI_INT, emisor, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                valor_local += valor_recibido; 
            }
        }
       
        else if ((rango - paso) % (2 * paso) == 0)
        {
            int receptor = rango - paso;
            MPI_Send(&valor_local, 1, MPI_INT, receptor, 0, MPI_COMM_WORLD);
            break; 
        paso *= 2;
    }

    if (rango == 0)
    {
        cout << "La suma total es " << valor_local << endl;
    }

    MPI_Finalize();
    return 0;
}
