#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<math.h>
#include<sys/time.h>
int main()
{
    MPI_Init(NULL, NULL);
    const long long N = 99999999999;
	long long i, j;
	double area, pi;
	double dx, y, x;
	double exe_time;
	struct timeval stop_time, start_time;
	dx = 1.0/N;
	x = 0.0;
	area = 0.0;
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    long long chunksize = N / size;
    long long start = rank * chunksize;
    long long end = start + chunksize;
    if(rank == size - 1) end = N;
    if(rank == 0)
        gettimeofday(&start_time, NULL);
    for(i=start;i<end;i++){
        x = i*dx;
        y = sqrt(1-x*x);
        area += y*dx;
    }
    double finalarea;
    MPI_Reduce(&area, &finalarea, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);
    if(rank == 0){
        gettimeofday(&stop_time, NULL);
        exe_time = (stop_time.tv_sec+(stop_time.tv_usec/1000000.0)) - (start_time.tv_sec+(start_time.tv_usec/1000000.0));
        pi = 4.0*finalarea;
        printf("\n Value of pi is = %.16lf\n Execution time is = %lf seconds\n", pi, exe_time);
    }
    MPI_Finalize();
    return 0;
}
