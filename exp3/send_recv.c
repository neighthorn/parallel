#include <mpi.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

// 从0 process发送给1 process
// 1 process接收到之后broadcast给所有process

int main(int argc, char ** argv) {
    MPI_Init(NULL, NULL);

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    int processor_count;
    MPI_Comm_size(MPI_COMM_WORLD, &processor_count);

    const char* hello_world = "hello world!";
    char str[20] = {0};
    int str_length = strlen(hello_world);
    double broadcast_time = 0.0;
    // printf("%d\n", str_length);
    if(rank == 0) {
        memcpy(str, hello_world, str_length);
        // printf("%s  %s\n", hello_world, str);
        // data, count, data_type, destination_rank, tag, communicator 
        MPI_Send(&str, str_length, MPI_BYTE, 1, 0, MPI_COMM_WORLD);
    }
    else if(rank == 1) {
        MPI_Recv(&str, str_length, MPI_BYTE, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received message '%s' from process 0\n", str);
    }

    MPI_Barrier(MPI_COMM_WORLD);
    sleep(1);
    broadcast_time -= MPI_Wtime();
    // data, count, data_type, root_node_to_broadcast, communicator
    MPI_Bcast(str, str_length, MPI_CHAR, 1, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);
    broadcast_time += MPI_Wtime();
    if(rank == 1)
        printf("Process %d finished broadcast message '%s' and the broadcast time is %lf sec.\n", rank, str, broadcast_time);
    else
        printf("Process %d received the broadcast message '%s'.\n", rank, str);
    MPI_Finalize();
    return 0;
}