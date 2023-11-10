#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <mpi.h>

#define rmax 5
int count = 0;


int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);
    int rank, size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    FILE *file;
    double *x, *y, *z;
    double d12, d23, d31;
    int N, i, j, k, local_count = 0;
    int counts[rmax + 1][rmax + 1][rmax + 1] = {0};
    int local_counts[rmax+1][rmax+1][rmax+1] = {0};
    clock_t start, end;
    double cpu_time_used;

    file = fopen("inputfile300.dat", "r");
    if (file == NULL) {
        perror("Error opening file");
        MPI_Finalize();
        return 1;
    }

    // Count the number of lines in the file to determine the array size
    N = 0;
    while (fscanf(file, "%*f %*f %*f") != EOF) {
        N++;
    }

    rewind(file);

    x = (double *)malloc(N * sizeof(double));
    y = (double *)malloc(N * sizeof(double));
    z = (double *)malloc(N * sizeof(double));

    for (i = 0; i < N; i++) {
        fscanf(file, "%lf %lf %lf", &x[i], &y[i], &z[i]);
    }

    fclose(file);

    // Divide the work among processes
    int start_i = rank * (N / size);
    int end_i = (rank == size - 1) ? N : start_i + (N / size);

    start = clock();

    for (i = start_i; i < end_i; i++) {
        for (j = 0; j < N; j++) {
            for (k = 0; k < N; k++) {
                d12 = sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2) + pow(z[i] - z[j], 2));
                d23 = sqrt(pow(x[j] - x[k], 2) + pow(y[j] - y[k], 2) + pow(z[j] - z[k], 2));
                d31 = sqrt(pow(x[k] - x[i], 2) + pow(y[k] - y[i], 2) + pow(z[k] - z[i], 2));

                if (d12 > rmax || d23 > rmax || d31 > rmax) {
                    continue;
                } else {
                    local_counts[(int)d12][(int)d23][(int)d31]++;
                    local_count++;
                }
            }
        }
    }

    // Synchronize processes
    MPI_Barrier(MPI_COMM_WORLD);

    // Reduce counts from all processes
    MPI_Reduce(local_counts, counts, (rmax + 1) * (rmax + 1) * (rmax + 1), MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    MPI_Reduce(&local_count, &count, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    if (rank == 0) {
        // Print counts here
        for (i = 0; i <= rmax; i++) {
            for (j = 0; j <= rmax; j++) {
                for (k = 0; k <= rmax; k++) {
                    printf("%d ", counts[i][j][k]);
                }
                printf("\n");
            }
        }

        printf("\nTime taken: %f seconds\n", cpu_time_used);
        printf("Count: %d\n", count);
    }

    free(x);
    free(y);
    free(z);

    MPI_Finalize();

    return 0;
}

