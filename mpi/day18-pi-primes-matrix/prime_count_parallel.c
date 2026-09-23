#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<sys/time.h>
#include<mpi.h>

#define N 100000

int main()
{
    MPI_Init(NULL, NULL);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
	int i, j;
	int count, flag;
	double exe_time;
	struct timeval stop_time, start_time;
    int chunksize = N / size;
    int start = rank * chunksize;
    int end = start + chunksize;
    if(rank == size - 1) end = N;
	count = 0;
    if(rank == 0){
        count = 1;
        start = 3;
    }
	gettimeofday(&start_time, NULL);
	for(i=start;i<end;i++)
	{
	 	flag = 0;
		for(j=2;j<i;j++)
	    {
		    if((i%j) == 0)
		    {
			    flag = 1;
			    break;
		    }
	    }
        if(flag == 0)
        {
        	count++;
        }
	}
    int total_count = 0;
    MPI_Reduce(&count, &total_count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if(rank == 0){
        gettimeofday(&stop_time, NULL);
        exe_time = (stop_time.tv_sec+(stop_time.tv_usec/1000000.0)) - (start_time.tv_sec+(start_time.tv_usec/1000000.0));
        printf("\n Number of prime numbers = %d \n Execution time is = %lf seconds\n", total_count, exe_time);
    }
    MPI_Finalize();

}
