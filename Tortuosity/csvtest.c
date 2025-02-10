#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 1024

// Function to parse numbers from a CSV file
double *parse_csv_numbers(const char *filename, int *count) {
    FILE *file;
    char line[MAX_LINE_LENGTH];
    double *numbers = NULL;
    int capacity = 10; // Initial capacity
    int size = 0;

    // Open the CSV file
    file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Allocate initial memory for the numbers array
    numbers = (double *)malloc(capacity * sizeof(double));
    if (numbers == NULL) {
        perror("Error allocating memory");
        fclose(file);
        return NULL;
    }

    // Read the file line by line
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        char *token;

        // Split the line into tokens based on commas
        token = strtok(line, ",");
        while (token != NULL) {
            // Convert the token to a number and store it in the array
            numbers[size] = atof(token);
            size++;

            // Resize the array if necessary
            if (size >= capacity) {
                capacity *= 2;
                numbers = (double *)realloc(numbers, capacity * sizeof(double));
                if (numbers == NULL) {
                    perror("Error reallocating memory");
                    fclose(file);
                    return NULL;
                }
            }

            // Get the next token
            token = strtok(NULL, ",");
        }
    }

    // Close the file
    fclose(file);

    // Resize the array to the actual number of elements
    numbers = (double *)realloc(numbers, size * sizeof(double));
    if (numbers == NULL && size > 0) {
        perror("Error finalizing memory allocation");
        return NULL;
    }

    // Set the count of numbers
    *count = size;

    return numbers;
}

int main() {
    const char *filename = "entrada.csv";
    int count;
    double *numbers;

    // Parse the CSV file
    numbers = parse_csv_numbers(filename, &count);
    if (numbers == NULL) {
        fprintf(stderr, "Failed to parse CSV file.\n");
        return EXIT_FAILURE;
    }

    // Print the parsed numbers
    printf("Parsed numbers:\n");
    for (int i = 0; i < count; i++) {
        printf("%f\n", numbers[i]);
    }

    // Free the allocated memory
    free(numbers);

    return EXIT_SUCCESS;
}