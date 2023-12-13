#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <omp.h>

#define MAX_POINTS 1000
#define MAX_DISTANCE 5.0

typedef struct {
    double x, y, z;
} Point;

// Function to calculate the distance between two points
double calculate_distance(Point *point1, Point *point2) {
    double dx = point1->x - point2->x;
    double dy = point1->y - point2->y;
    double dz = point1->z - point2->z;
    return sqrt(dx * dx + dy * dy + dz * dz);
}

int main() {
    clock_t start_time = clock();

    // Assuming inputfile.dat has three columns of numbers
    FILE *file = fopen("inputfile300.dat", "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    Point points[MAX_POINTS];
    int n = 0;

    // Read data from file
    while (fscanf(file, "%lf %lf %lf", &points[n].x, &points[n].y, &points[n].z) == 3) {
        n++;
        if (n >= MAX_POINTS) {
            printf("Too many points. Increase MAX_POINTS if needed.\n");
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    // Calculate the maximum number of triangles
    int max_triangles = n * (n - 1) * (n - 2) / 6;

    // Initialize histograms dynamically
    double *hist12 = malloc(max_triangles * sizeof(double));
    double *hist23 = malloc(max_triangles * sizeof(double));
    double *hist31 = malloc(max_triangles * sizeof(double));

    if (hist12 == NULL || hist23 == NULL || hist31 == NULL) {
        perror("Error allocating memory");
        return 1;
    }

    // Create a memo cache for each thread
    int max_threads = omp_get_max_threads();
    double ***memo_caches = malloc(max_threads * sizeof(double **));

    for (int i = 0; i < max_threads; i++) {
        memo_caches[i] = malloc(MAX_POINTS * sizeof(double *));
        for (int j = 0; j < MAX_POINTS; j++) {
            memo_caches[i][j] = malloc(MAX_POINTS * sizeof(double));
        }
    }

    int count = 0;

    // Generate triangles and update histograms in parallel
    #pragma omp parallel for shared(hist12, hist23, hist31, count, memo_caches)
    for (int i = 0; i < n - 2; i++) {
        int thread_id = omp_get_thread_num();
        // Each thread gets its own memo cache
        double **memo_cache = memo_caches[thread_id];

        for (int j = i + 1; j < n - 1; j++) {
            for (int k = j + 1; k < n; k++) {
                #pragma omp critical
                {
                    // Calculate distances between points using memoization
                    double dist12 = memo_cache[i][j] > 0 ? memo_cache[i][j] : calculate_distance(&points[i], &points[j]);
                    double dist23 = memo_cache[j][k] > 0 ? memo_cache[j][k] : calculate_distance(&points[j], &points[k]);
                    double dist31 = memo_cache[k][i] > 0 ? memo_cache[k][i] : calculate_distance(&points[k], &points[i]);

                    // Memoize the results
                    memo_cache[i][j] = dist12;
                    memo_cache[j][k] = dist23;
                    memo_cache[k][i] = dist31;
                }

                if (dist12 < MAX_DISTANCE && dist23 < MAX_DISTANCE && dist31 < MAX_DISTANCE) {
                    // Update the histograms
                    int local_count;
                    #pragma omp atomic capture
                    local_count = ++count;

                    hist12[local_count - 1] = dist12;
                    hist23[local_count - 1] = dist23;
                    hist31[local_count - 1] = dist31;
                }
            }
        }
    }

    clock_t end_time = clock();
    double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    // Visualize the histograms
    printf("%d\n", count);
    printf("%f\n", elapsed_time);

    // Free dynamically allocated memory
    free(hist12);
    free(hist23);
    free(hist31);

    // Free memo caches
    for (int i = 0; i < max_threads; i++) {
        for (int j = 0; j < MAX_POINTS; j++) {
            free(memo_caches[i][j]);
        }
        free(memo_caches[i]);
    }
    free(memo_caches);

    return 0;
}
