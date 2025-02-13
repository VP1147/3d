#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_POINTS 100000

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

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow(
        "3D Plotter",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN
    );
    if (!window) {
        fprintf(stderr, "Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        fprintf(stderr, "Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
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
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Render the 3D points
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White points
        for (int i = 0; i < point_count; i++) {
            int screenX, screenY;
            project_point(points[i][0], points[i][1], points[i][2], &screenX, &screenY);

            // Draw a small rectangle for each point
            SDL_Rect pointRect = {screenX - 2, screenY - 2, 4, 4};
            SDL_RenderFillRect(renderer, &pointRect);
        }

        // Update the screen
        SDL_RenderPresent(renderer);
    }

    // Clean up
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
