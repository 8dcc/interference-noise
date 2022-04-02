#include <stdio.h>
#include <stdlib.h>  // For random numbers
#include "SDL.h"

#define WINDOW_W 1200		// Window width in pixels
#define WINDOW_H 750		// Window height in pixels
#define NOISE_SIZE 5		// Noise size in pixels

#define MAX_INTENSITY 256	// Max random number for colors. From 2 to 256.

#define FPS 60				// Will wait 1000ms/FPS between frames
#define DELAY 50			// Will set this as delay instead of the fps if the value is not 0 and the space is pressed

#define DEBUG_PRINT 1		// 1 is enabled

int main(int argc, char* argv[]) {
	int noise_grid_colors[WINDOW_H/NOISE_SIZE][WINDOW_W/NOISE_SIZE][3];
	
	// Clear the array
	for (int y = 0; y < WINDOW_H/NOISE_SIZE; y++) {
		for (int x = 0; x < WINDOW_W/NOISE_SIZE; x++) {
			for (int color = 0; color < 3; color++) {
				noise_grid_colors[y][x][color] = 0;
			}
		}
	}

	// Start SDL
	if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) != 0) {
		SDL_Log("Unable to start: %s\n", SDL_GetError());
		return 1;
	}
	if (DEBUG_PRINT) printf("SLD started!\n");
	
	// Create window
	SDL_Window* fuckwindows = SDL_CreateWindow("TV noise (Grayscale)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, 0); 
	if (!fuckwindows) {
		printf("Error creating a window: %s\n", SDL_GetError());
		SDL_Quit();
		return 1;
	}
	if (DEBUG_PRINT) printf("Window created!\n");

	// Create renderer
	Uint32 render_flags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
	SDL_Renderer* fuckrenderers = SDL_CreateRenderer(fuckwindows, -1, render_flags);
	if (!fuckrenderers) {
		printf("Error creating a renderer: %s\n", SDL_GetError());
		SDL_DestroyWindow(fuckwindows);
		SDL_Quit();
		return 1;
	}
	if (DEBUG_PRINT) printf("Renderer created!\n\n");
	printf("Press space to start the simulation.\n");

	// Main loop
	int running = 0, space_pressed = 1, random_number = 0; 

	SDL_Rect current_rect;
	SDL_Event fuckevents;	// Create an event for the keys and shit
	while (running == 0) {
		// Events
		while (SDL_PollEvent(&fuckevents)) {
			switch (fuckevents.type) {
				case SDL_QUIT:							running = 1;			break;
				case SDL_KEYDOWN:						// Check the pressed key
					switch (fuckevents.key.keysym.scancode) {
						case SDL_SCANCODE_ESCAPE:		running = 1;			break;
						case SDL_SCANCODE_SPACE:		space_pressed = 0;		break;
						default:						break;
					}
					break;
				case SDL_KEYUP:							// Check the released key
					switch (fuckevents.key.keysym.scancode) {
						case SDL_SCANCODE_SPACE:		space_pressed = 1;		break;
						default:						break;
					}
					break;
				default:								break;
			}
		}

		// Clear window
		SDL_SetRenderDrawColor(fuckrenderers, 0, 0, 0, 255);
		SDL_RenderClear(fuckrenderers);

		// Change array colors if the space is pressed
		if (space_pressed == 0) {
			for (int y = 0; y < WINDOW_H/NOISE_SIZE; y++) {
				for (int x = 0; x < WINDOW_W/NOISE_SIZE; x++) {
					random_number = rand() % MAX_INTENSITY;
					for (int color = 0; color < 3; color++) {
						noise_grid_colors[y][x][color] = random_number;
						noise_grid_colors[y][x][color] = random_number;
						noise_grid_colors[y][x][color] = random_number;
					}
				}
			}
		}

		// Draw rects
		for (int y = 0; y < WINDOW_H/NOISE_SIZE; y++) {
			for (int x = 0; x < WINDOW_W/NOISE_SIZE; x++) {
				SDL_SetRenderDrawColor(fuckrenderers, noise_grid_colors[y][x][0], noise_grid_colors[y][x][1], noise_grid_colors[y][x][2], 255);

				current_rect.x = x*NOISE_SIZE;
				current_rect.y = y*NOISE_SIZE;
				current_rect.w = NOISE_SIZE;
				current_rect.h = NOISE_SIZE;
				SDL_RenderFillRect(fuckrenderers, &current_rect);
			}
		}
			
		// Send to window
		SDL_RenderPresent(fuckrenderers);
		if (space_pressed == 0 && DELAY != 0) SDL_Delay(DELAY);
		else SDL_Delay(1000/FPS);
	}

	// End of the program
	printf("Reached end of the program!\n");
	SDL_DestroyRenderer(fuckrenderers);
	SDL_DestroyWindow(fuckwindows);
	SDL_Quit();

	return 0;
}
