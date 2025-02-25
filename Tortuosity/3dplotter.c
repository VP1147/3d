#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600
#define MAX_POINTS 1000

// Function to read 3D points from a .dat file
int read_dat(const char *filename, float points[MAX_POINTS][3], int *count) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return 0;
    }

    *count = 0;
    while (fscanf(file, "%f %f %f", &points[*count][0], &points[*count][1], &points[*count][2]) == 3) {
        (*count)++;
        if (*count >= MAX_POINTS) {
            fprintf(stderr, "Warning: Maximum number of points reached.\n");
            break;
        }
    }

    fclose(file);
    return 1;
}

// Function to rotate a point around the X axis
void rotate_x(float *y, float *z, float angle) {
    float y1 = *y * cos(angle) - *z * sin(angle);
    float z1 = *y * sin(angle) + *z * cos(angle);
    *y = y1;
    *z = z1;
}

// Function to rotate a point around the Y axis
void rotate_y(float *x, float *z, float angle) {
    float x1 = *x * cos(angle) - *z * sin(angle);
    float z1 = *x * sin(angle) + *z * cos(angle);
    *x = x1;
    *z = z1;
}

// Function to project 3D points to 2D screen coordinates
void project_point(float x, float y, float z, int *screenX, int *screenY) {
    // Simple perspective projection
    float scale = 200.0f / (z + 200.0f); // Adjust the scale for perspective
    *screenX = (int)(x * scale + WINDOW_WIDTH / 2);
    *screenY = (int)(-y * scale + WINDOW_HEIGHT / 2);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <dat_file>\n", argv[0]);
        return 1;
    }

    // Read 3D points from the .dat file
    float points[MAX_POINTS][3];
    int point_count;
    if (!read_dat(argv[1], points, &point_count)) {
        return 1;
    }

    // Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow(
        "3D Plotter with Mouse Interactivity",
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

    // Variables for mouse interaction
    int mouseX, mouseY;
    int prevMouseX = 0, prevMouseY = 0;
    float rotateAngleX = 0.0f, rotateAngleY = 0.0f;
    int isMouseDown = 0;

    // Main loop
    int running = 1;
    while (running) {
        // Handle events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = 0;
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        isMouseDown = 1;
                        SDL_GetMouseState(&prevMouseX, &prevMouseY);
                    }
                    break;
                case SDL_MOUSEBUTTONUP:
                    if (event.button.button == SDL_BUTTON_LEFT) {
                        isMouseDown = 0;
                    }
                    break;
                case SDL_MOUSEMOTION:
                    if (isMouseDown) {
                        SDL_GetMouseState(&mouseX, &mouseY);
                        rotateAngleX += (mouseY - prevMouseY) * 0.01f;
                        rotateAngleY += (mouseX - prevMouseX) * 0.01f;
                        prevMouseX = mouseX;
                        prevMouseY = mouseY;
                    }
                    break;
            }
        }

        // Clear the screen
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Black background
        SDL_RenderClear(renderer);

        // Render the 3D points
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // White points
        for (int i = 0; i < point_count; i++) {
            float x = points[i][0];
            float y = points[i][1];
            float z = points[i][2];

            // Apply rotations
            rotate_x(&y, &z, rotateAngleX);
            rotate_y(&x, &z, rotateAngleY);

            // Project to 2D
            int screenX, screenY;
            project_point(x, y, z, &screenX, &screenY);

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