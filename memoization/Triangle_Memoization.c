#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define MAX_POINTS 3000
#define MAX_DISTANCE 25.0

typedef struct {
    double x, y, z;
    int index;  // Added index field
} Point;

// Function to calculate the distance between two points with memoization
double calculate_distance_memoized(Point *point1, Point *point2, double memo[MAX_POINTS][MAX_POINTS]) {
    int index1 = point1->index;
    int index2 = point2->index;

    // Check index boundaries
    if (index1 < 0 || index1 >= MAX_POINTS || index2 < 0 || index2 >= MAX_POINTS) {
        printf("Error: Invalid indices %d and %d\n", index1, index2);
        exit(1);
    }

    if (memo[index1][index2] != 0.0) {
        // Return memoized result if available
        printf("Debug: Returning memo[%d][%d]\n", index1, index2);
        return memo[index1][index2];
    }

    double dx = point1->x - point2->x;
    double dy = point1->y - point2->y;
    double dz = point1->z - point2->z;

    double distance = sqrt(dx * dx + dy * dy + dz * dz);

    printf("Debug: indices %d and %d\n", index1, index2);

    // Memoize the result
    memo[index1][index2] = distance;
    memo[index2][index1] = distance;

    return distance;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    clock_t start_time = clock();

    FILE *file = fopen(argv[1], "r");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }

    Point points[MAX_POINTS];
    int n = 0;

    while (fscanf(file, "%lf %lf %lf", &points[n].x, &points[n].y, &points[n].z) == 3) {
        points[n].index = n;  // Assign index to the point
        n++;
        if (n >= MAX_POINTS) {
            printf("Too many points. Increase MAX_POINTS if needed.\n");
            fclose(file);
            return 1;
        }
    }

    fclose(file);

    double memo[MAX_POINTS][MAX_POINTS];
    memset(memo, 0, sizeof(memo));

    int count = 0;

    for (int i = 0; i < n - 2; i++) {
        for (int j = i + 1; j < n - 1; j++) {
            double dist12 = calculate_distance_memoized(&points[i], &points[j], memo);
            if (dist12 >= MAX_DISTANCE) {
                continue;
            }
            for (int k = j + 1; k < n; k++) {
                double dist23 = calculate_distance_memoized(&points[j], &points[k], memo);
                double dist31 = calculate_distance_memoized(&points[k], &points[i], memo);

                if (dist12 < MAX_DISTANCE && dist23 < MAX_DISTANCE && dist31 < MAX_DISTANCE) {
                    count++;
                }
            }
        }
    }

    clock_t end_time = clock();
    double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    FILE *csv_file = fopen("results.csv", "a");
    if (csv_file == NULL) {
        perror("Error creating CSV file");
        return 1;
    }

    fprintf(csv_file, "%d, %d, %f\n", n, count, elapsed_time);

    fclose(csv_file);

    return 0;
}
