#include "chip8.h"
#include <fstream>

using namespace std;

unsigned char chip8_fontset[80] =
{ 
     0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
     0x20, 0x60, 0x20, 0x20, 0x70, // 1
     0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
     0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
     0x90, 0x90, 0xF0, 0x10, 0x10, // 4
     0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
     0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
     0xF0, 0x10, 0x20, 0x40, 0x40, // 7
     0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
     0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
     0xF0, 0x90, 0xF0, 0x90, 0x90, // A
     0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
     0xF0, 0x80, 0x80, 0x80, 0xF0, // C
     0xE0, 0x90, 0x90, 0x90, 0xE0, // D
     0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
     0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void Chip8::initialize()
{
    pc = 0x200;     // App will be loaded at 0x200
    opcode = 0;     // Reset opcode
    I = 0;          // Reset index register
    sp = 0;         // Reset stack pointer

    // Clear display screen
    for (int i = 0; i < GFX_SIZE; i++)
        gfx[i] = 0;

    // Clear stack
    for (int i = 0; i < STACK_SIZE; i++)
        stack[i] = 0;

    // Clear registers
    for (int i = 0; i < REG_SIZE; i++)
        V[i] = 0;

    // Clear memory
    for (int i = 0; i < MEM_SIZE; i++)
        memory[i] = 0;

    // Load font set into memory
    for (int i = 0; i < 80; i++)
        memory[i] = chip8_fontset[i];

    // Reset timers
    delay_timer = 0;
    sound_timer = 0;
}

bool Chip8::loadApplication(const char* fileName)
{
    return true;
}

void Chip8::emulateCycle()
{
    // Fetch
    opcode = (memory[pc] << 8) | memory[pc + 1];

    // Decode
    switch (opcode & 0xF000)
    {
        // 1NNN: Goto address NNN
        case 0x1000:
            pc = opcode & 0x0FFF;
            break;
        case 0x2000:
            break;
        case 0x3000:
            break;
        case 0x4000:
            break;
        case 0x5000:
            break;
        case 0x6000:
            break;
        case 0x7000:
            break;
        case 0x8000:
            break;
        case 0x9000:
            break;
        // ANNN: Sets I to address NNN
        case 0xA000:
            I = opcode & 0x0FFF;
            pc += 2;
            break;
        // BNNN: Goto address NNN + V0
        case 0xB000:
            pc = (opcode & 0x0FFF) + V[0];
            break;
        case 0xC000:
            break;
        case 0xD000:
            break;
        case 0xE000:
            break;
        case 0xF000:
            break;
        default:
            printf("Unknown opcode: 0x%X\n", opcode);

    }

    // Execute
}
