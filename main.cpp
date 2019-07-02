#include <stdio.h>
#include "chip8.h"
#include <SDL2/SDL.h>

using namespace std;

// Dimensions of window to display
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 512;


int main(int argc, char **argv)
{
    Chip8 myChip;
    
    myChip.initialize();
    
    // Load the ROM
    myChip.loadApplication(argv[1]);

    return 0;
}

void setUpGraphics()
{
    // Initialize SDL
    SDL_Window* window;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        printf("SDL could not be initialized\n");
    
    !// Create window
    window = SDL_CreateWindow("CHIP-8 Emulator", 
                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
                        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL)
        printf("Window could not be created\n");

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RendererSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Create texture
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                        SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTURE_ACCESS_STREAMING,
                        64, 32);
}

void destroyGraphics()
{
    
}
