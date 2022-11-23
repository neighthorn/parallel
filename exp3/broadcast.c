#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv) {
    int num_elements = atoi(argv[1]);
    int num_trials = atoi(argv[2]);

    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    double broadcast_time = 0.0;
    int i;
    int *data = (int*) malloc(sizeof(int) * num_elements);
    
    for(i = 0; i < num_trials; ++i) {
        MPI_Barrier(MPI_COMM_WORLD);
        broadcast_time -= MPI_Wtime();
        MPI_Bcast(data, num_elements, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        broadcast_time += MPI_Wtime();
    }
    
    return 0;
}