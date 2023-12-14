#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define MAX_POINTS 3000
#define MAX_DISTANCE 25.0

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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <input_file>\n", argv[0]);
        return 1;
    }

    clock_t start_time = clock();

    // Open the input file
    FILE *file = fopen(argv[1], "r");
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
    //double *hist12 = malloc(max_triangles * sizeof(double));
    //double *hist23 = malloc(max_triangles * sizeof(double));
    //double *hist31 = malloc(max_triangles * sizeof(double));

    //if (hist12 == NULL || hist23 == NULL || hist31 == NULL) {
    //    perror("Error allocating memory");
    //    return 1;
    //}

    int count = 0;

    // Generate triangles and update histograms
    for (int i = 0; i < n - 2; i++) {
        for (int j = i + 1; j < n - 1; j++) {
            double dist12 = calculate_distance(&points[i], &points[j]);
            if (dist12 >= MAX_DISTANCE){
		continue;
	    }
            for (int k = j + 1; k < n; k++) {
                // Calculate distances between points
                double dist23 = calculate_distance(&points[j], &points[k]);
                double dist31 = calculate_distance(&points[k], &points[i]);

                if (dist12 < MAX_DISTANCE && dist23 < MAX_DISTANCE && dist31 < MAX_DISTANCE) {
                    // Update the histograms
                   // hist12[count] = dist12;
                    //hist23[count] = dist23;
                    //hist31[count] = dist31;
                    count++;
                }
            }
        }
    }
 
    clock_t end_time = clock();
    double elapsed_time = ((double) (end_time - start_time)) / CLOCKS_PER_SEC;

    // Save results to a CSV file
    FILE *csv_file = fopen("results.csv", "a");
    if (csv_file == NULL) {
        perror("Error creating CSV file");
        return 1;
    }

    // Write data
    fprintf(csv_file, "%d, %d, %f\n", n, count, elapsed_time);

    // Close CSV file
    fclose(csv_file);
    /*
    // Free dynamically allocated memory
    free(hist12);
    free(hist23);
    free(hist31);
    */
    return 0;
}
