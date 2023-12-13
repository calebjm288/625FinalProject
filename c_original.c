#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <string.h>
#include <stdint.h>

#include "sys/types.h"
#include "sys/sysinfo.h"


#define rmax 25


typedef struct {
        uint32_t virtualMem;
        uint32_t physicalMem;
} processMem_t;

int parseLine(char *line) {
        // This assumes that a digit will be found and the line ends in " Kb".
        int i = strlen(line);
        const char *p = line;
        while (*p < '0' || *p > '9') p++;
        line[i - 3] = '\0';
        i = atoi(p);
        return i;
}

void GetProcessMemory(processMem_t* processMem) {
        FILE *file = fopen("/proc/self/status", "r");
        char line[128];

        while (fgets(line, 128, file) != NULL) {
                //printf("%s", line);
                if (strncmp(line, "VmSize:", 7) == 0) {
                        processMem->virtualMem = parseLine(line);
                }

                if (strncmp(line, "VmRSS:", 6) == 0) {
                        processMem->physicalMem = parseLine(line);
                }
        }
        fclose(file);
}

processMem_t  checkMemoryMain() {
        processMem_t myMem;

        GetProcessMemory(&myMem);

        return myMem;
}



int main() {
    FILE *file;
    double *x, *y, *z;
    double d12, d23, d31;
    int N, i, j, k, count = 0;
    int counts[rmax+1][rmax+1][rmax+1] = {0};
    clock_t start, end;
    double cpu_time_used;
	processMem_t myMem;

    file = fopen("inputfile100.dat", "r");
    if (file == NULL) {
        perror("Error opening file");
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

    start = clock();

    for (i = 0; i < N; i++) {
        for (j = i+1; j < N; j++) {
			d12 = sqrt(pow(x[i] - x[j], 2) + pow(y[i] - y[j], 2) + pow(z[i] - z[j], 2));
			if (d12 > rmax) { continue; }
            for (k = j+1; k < N; k++) {
                d23 = sqrt(pow(x[j] - x[k], 2) + pow(y[j] - y[k], 2) + pow(z[j] - z[k], 2));
                d31 = sqrt(pow(x[k] - x[i], 2) + pow(y[k] - y[i], 2) + pow(z[k] - z[i], 2));

                if (d23 > rmax || d31 > rmax) {
                    continue;
                } else {
                    counts[(int)d12][(int)d23][(int)d31]++;
                    count++;
                }
            }
        }
    }
	
	myMem = checkMemoryMain();
    end = clock();
    cpu_time_used = ((double)(end - start)) / CLOCKS_PER_SEC;

    // Print counts here
    for (i = 0; i <= rmax; i++) {
        for (j = 0; j <= rmax; j++) {
            for (k = 0; k <= rmax; k++) {
                printf("%d ", counts[i][j][k]);
            }
            printf("\n");
        }
    }

    //printf("\nTime taken: %f seconds\n", cpu_time_used);
    //printf("Count: %d\n", count);
	printf("DATA, %u, %u, %f, %d\n", myMem.virtualMem, myMem.physicalMem, cpu_time_used, count);

    free(x);
    free(y);
    free(z);

    return 0;
}

