#include <stdio.h>
#include "chip8.h"
#include <SDL2/SDL.h>
#include <chrono>
#include <thread>

using namespace std;

// Dimensions of window to display
const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 512;

// Keypad map
uint8_t keyMap[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v
};


int main(int argc, char **argv)
{
    Chip8 myChip = Chip8();
    bool quit = false;

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

    // Create renderer
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);

    // Create texture
    SDL_Texture* texture = SDL_CreateTexture(renderer,
                        SDL_PIXELFORMAT_ARGB8888,
                        SDL_TEXTUREACCESS_STREAMING,
                        64, 32);
    
    // Temp buffer
    uint32_t pixels[2048];

    myChip.initialize();
    
    // Load the ROM
    myChip.loadApplication(argv[1]);

    // Program loop
    while (!quit)
    {
        myChip.emulateCycle();

        // Process events
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            // Quit emulator
            if (event.type == SDL_QUIT)
                quit = true;
            else if (event.type == SDL_KEYDOWN)
            {
                // Quit emulator
                if (event.key.keysym.sym == SDLK_ESCAPE)
                    quit = true;
                // Reload ROM
                else if (event.key.keysym.sym == SDLK_F1)
                    myChip.loadApplication(argv[1]);

                // Store key state as pressed
                for (int i = 0; i < 16; i++)
                {
                    if (event.key.keysym.sym == keyMap[i])
                        myChip.key[i] = 1;
                }
            }
            else if (event.type = SDL_KEYUP)
            {
                // Store key state as unpressed
                for (int i = 0; i < 16; i++)
                {
                    if (event.key.keysym.sym == keyMap[i])
                        myChip.key[i] = 0;
                }
                
            }
        }

        // Redraw SDL screen if draw flag is true
        if (myChip.getDraw())
        {
            printf("Drawing...\n");
            // Store px in temp buffer
            for (int i = 0; i < 2048; i++)
            {
                uint8_t px = myChip.gfx[i];
                pixels[i] = (0xFFFFFF * px) | 0xFF000000;
            }

            // Update texture
            SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));
        
            // Clear and render
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);

            // Reset draw flag
            myChip.setDraw(false);
        }

        std::this_thread::sleep_for(std::chrono::microseconds(1200));
    }
}
