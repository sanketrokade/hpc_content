#include"stdio.h"
#include"mpi.h"

int main()
{
	int myid, size;
	int myval;
	MPI_Init(NULL,NULL);

	MPI_Comm_size(MPI_COMM_WORLD, &size);

	MPI_Comm_rank(MPI_COMM_WORLD, &myid);

	if(myid==0){
        myval = 100;
		printf("\nmyid: %d \t myval = %d", myid, myval);
        for(int i = 1; i < size; i++){
            MPI_Send(&myval, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
		printf("\nmyid: %d \t Data sent.\n", myid);
	}
	else{	// Process with ID exactly equal to 1
        if(myid == size - 1){
            printf("I left\n");
        }
        else{
               myval = 200;
               MPI_Recv(&myval, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
               printf("\nmyid: %d \t Data received.\n", myid);
               printf("\nmyid: %d \t myval = %d\n", myid, myval);
        }
	}

	MPI_Finalize();
}
