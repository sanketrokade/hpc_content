#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[20];
    int prn;
    int age;
    char email[40];
    double salary;
    int marks[5];
} student;

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "World size must be greater than 1 for this example\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    student st;
    MPI_Datatype struct_type;

    // Create the datatype for my_struct
    int blocklengths[6] = {20, 1, 1, 40, 1, 5};
    MPI_Aint displacements[6];
    MPI_Datatype types[6] = {MPI_CHAR, MPI_INT, MPI_INT, MPI_CHAR, MPI_DOUBLE, MPI_INT};

    displacements[0] = offsetof(student, name);
    displacements[1] = offsetof(student, prn);
    displacements[2] = offsetof(student, age);
    displacements[3] = offsetof(student, email);
    displacements[4] = offsetof(student, salary);
    displacements[5] = offsetof(student, marks);

    MPI_Type_create_struct(6, blocklengths, displacements, types, &struct_type);
    MPI_Type_commit(&struct_type);

    if (rank == 0) {
        strcpy(st.name,"Abhishek Raj");
        st.prn = 001;
        st.age = 24;
        strcpy(st.email, "abhi@abhi.com");
        st.salary = 10000.011111;
        st.marks[0] = 40;
        st.marks[1] = 39;
        st.marks[2] = 40;
        st.marks[3] = 40;
        st.marks[4] = 38;

        MPI_Send(&st, 1, struct_type, 1, 0, MPI_COMM_WORLD);
        printf("Process 0 send data: \n");
        printf("Name : %s\n", st.name);
        printf("PRN : %d\n", st.prn);
        printf("Age : %d\n", st.age);
        printf("Email : %s\n", st.email);
        printf("Salary : %lf\n", st.salary);
        printf("Marks : ");
        for(int i = 0; i < 5; i++) printf("%d ", st.marks[i]);
        printf("\n");
    } else if (rank == 1) {
        MPI_Recv(&st, 1, struct_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        printf("Process 1 received data: \n");
        printf("Name : %s\n", st.name);
        printf("PRN : %d\n", st.prn);
        printf("Age : %d\n", st.age);
        printf("Email : %s\n", st.email);
        printf("Salary : %lf\n", st.salary);
        printf("Marks : ");
        for(int i = 0; i < 5; i++) printf("%d ", st.marks[i]);
        printf("\n");
    }

    MPI_Type_free(&struct_type);
    MPI_Finalize();
    return 0;
}
