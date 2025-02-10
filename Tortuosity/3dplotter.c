#include <SDL2/SDL.h>
#include <SDL2/SDL_gpu.h>
#include <stdio.h>
#include <stdlib.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_POINTS 1000

// Function to read 3D points from a CSV file
int read_csv(const char *filename, float points[MAX_POINTS][3], int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    *count = 0;
    while (fscanf(file, "%f,%f,%f", &points[*count][0], &points[*count][1], &points[*count][2]) == 3) {
        (*count)++;
        if (*count >= MAX_POINTS) {
            fprintf(stderr, "Warning: Maximum number of points reached.\n");
            break;
        }
    }

    fclose(file);
    return 1;
}

// Function to project 3D points to 2D screen coordinates
void project_point(float x, float y, float z, int *screenX, int *screenY) {
    // Simple orthographic projection (ignoring z for now)
    *screenX = (int)(x + WINDOW_WIDTH / 2);
    *screenY = (int)(-y + WINDOW_HEIGHT / 2);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <csv_file>\n", argv[0]);
        return 1;
    }

    // Read 3D points from the CSV file
    float points[MAX_POINTS][3];
    int point_count;
    if (!read_csv(argv[1], points, &point_count)) {
        return 1;
    }

    // Initialize SDL and SDL_gpu
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    GPU_Target *screen = GPU_Init(WINDOW_WIDTH, WINDOW_HEIGHT, GPU_DEFAULT_INIT_FLAGS);
    if (!screen) {
        fprintf(stderr, "GPU initialization failed: %s\n", GPU_GetError());
        SDL_Quit();
        return 1;
    }

    // Main loop
    int running = 1;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = 0;
            }
        }

        // Clear the screen
        GPU_Clear(screen);

        // Render the 3D points
        for (int i = 0; i < point_count; i++) {
            int screenX, screenY;
            project_point(points[i][0], points[i][1], points[i][2], &screenX, &screenY);

            // Draw a small circle for each point
            GPU_CircleFilled(screen, screenX, screenY, 3, GPU_MakeColor(255, 255, 255, 255));
        }

        // Update the screen
        GPU_Flip(screen);
    }

    // Clean up
    GPU_Quit();
    SDL_Quit();

    return 0;
}