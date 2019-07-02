#include <stdio.h>
#include "chip8.h"
#include <SDL2/SDL.h>

using namespace std;

int main()
{
    Chip8 myChip;
    
    // Dimensions of window to display
    const int SCREEN_WIDTH = 1024;
    const int SCREEN_HEIGHT = 512;

    // Initialize SDL
    SDL_Window* window;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        printf("SDL could not be initialized\n");
    
    // Create window
    window = SDL_CreateWindow("CHIP-8 Emulator", 
                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
        printf("Window could not be created\n");
    
    return 0;
}
