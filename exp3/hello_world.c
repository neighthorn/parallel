#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
	// MPI global and inteval variables are constructed
	MPI_Init(NULL, NULL);
	
	int process_num; 
	// MPI_Comm_size 返回communicator包含的进程数量(可以工作的进程数量),存储在process_num中
	MPI_Comm_size(MPI_COMM_WORLD, &process_num);

	int rank_num;
	// 当前进程的秩(类似进程号)
	MPI_Comm_rank(MPI_COMM_WORLD, &rank_num);

	char processor_name[MPI_MAX_PROCESSOR_NAME];
	int name_length;
	MPI_Get_processor_name(processor_name, &name_length);

	printf("Hello world from processor %s, rank %d out of %d processors\n", processor_name, rank_num, process_num);

	// 释放MPI资源
	MPI_Finalize();
	return 0;
}
