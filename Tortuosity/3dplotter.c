#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 10000

int main() {
    FILE *dataFile, *gnuplotPipe;
    double x[MAX_POINTS], y[MAX_POINTS], z[MAX_POINTS];
    int numPoints = 0;

    // Open the data file
    dataFile = fopen("raio3d1.dat", "r");
    if (dataFile == NULL) {
        perror("Error opening data file");
        return EXIT_FAILURE;
    }

    // Read the data from the file
    while (fscanf(dataFile, "%lf %lf %lf", &x[numPoints], &y[numPoints], &z[numPoints]) == 3) {
        numPoints++;
        if (numPoints >= MAX_POINTS) {
            fprintf(stderr, "Warning: Maximum number of points reached.\n");
            break;
        }
    }
    fclose(dataFile);

    // Open a pipe to GNUplot
    gnuplotPipe = popen("gnuplot -persist", "w");
    if (gnuplotPipe == NULL) {
        perror("Error opening GNUplot pipe");
        return EXIT_FAILURE;
    }

    // Send commands to GNUplot
    fprintf(gnuplotPipe, "set title '3D Trajectory Visualization'\n");
    fprintf(gnuplotPipe, "set xlabel 'X (m)'\n");
    fprintf(gnuplotPipe, "set ylabel 'Y (m)'\n");
    fprintf(gnuplotPipe, "set zlabel 'Z (m)'\n");
    fprintf(gnuplotPipe, "splot '-' with linespoints title 'Trajectory'\n");

    // Send data to GNUplot
    for (int i = 0; i < numPoints; i++) {
        fprintf(gnuplotPipe, "%lf %lf %lf\n", x[i], y[i], z[i]);
    }
    fprintf(gnuplotPipe, "e\n");  // End of data marker

    // Close the GNUplot pipe
    fflush(gnuplotPipe);
    pclose(gnuplotPipe);

    return EXIT_SUCCESS;
}