#include "chip8.h"
#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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
    cout << "Initializing...";

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

    srand(time(NULL));

    cout << "Done\n";
}

bool Chip8::loadApplication(const char* fileName)
{
    return true;
}

// Fetch the opcode
void Chip8::fetch()
{
    opcode = (memory[pc] << 8) | memory[pc + 1];
}

// Decode the instruction
void Chip8::decode()
{
    switch (opcode & 0xF000)
    {
        case 0x0000:
            break;
        // 1NNN: Goto address NNN
        case 0x1000:
            pc = opcode & 0x0FFF;
            break;
        // 2NNN: Call function at address NNN
        case 0x2000:
            stack[sp] = pc;
            sp++;
            pc = opcode & 0x0FFF;
            break;
        // 3XNN: If V[X] == NN, skip next instruction
        case 0x3000:
            if (V[(opcode & 0xF00) >> 8] == 0xFF)
                pc += 2;
            pc += 2;
            break;
        // 4XNN: If V[X] != NN, skip next instruction
        case 0x4000:
            if (V[(opcode & 0xF00) >> 8] != 0xFF)
                pc += 2;
            pc += 2;
            break;
        // 5XY0: If V[X] == V[Y], skip next instruction
        case 0x5000:
            if (V[(opcode & 0xF00) >> 8] == V[(opcode & 0xF0) >> 4])
                pc += 2;
            pc += 2;
            break;
        // 6XNN: Set V[X] to NN
        case 0x6000:
            V[(opcode & 0xF00) >> 8] = opcode & 0xFF;
            pc += 2;
            break;
        // 7XNN: Add NN to V[X]
        case 0x7000:
            V[(opcode & 0xF00) >> 8] += opcode & 0xFF;
            pc += 2;
            break;
        case 0x8000:
            switch(opcode & 0xF)
            {
                // 8XY0: Set V[X] to V[Y]
                case 0x0:
                    V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF0) >> 4];
                    pc += 2;
                    break;
                // 8XY1: Set V[X] to V[X] OR V[Y]
                case 0x1:
                    V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF00) >> 8] | V[(opcode & 0xF0) >> 4];
                    pc += 2;
                    break;
                // 8XY2: Set V[X] to V[X] AND V[Y]
                case 0x2:
                    V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF00) >> 8] & V[(opcode & 0xF0) >> 4];
                    pc += 2;
                    break;
                // 8XY3: Set V[X] to V[X] XOR V[Y]
                case 0x3:
                    V[(opcode & 0xF00) >> 8] = V[(opcode & 0xF00) >> 8] ^ V[(opcode & 0xF0) >> 4];
                    pc += 2;
                    break;
                // 8XY4: Add V[Y] to V[X] and set carry flag, if any
                case 0x4:
                    V[(opcode & 0xF00) >> 8] += V[(opcode & 0xF0) >> 4];
                    break;
                // 8XY5: Subtract V[Y] from V[X] and set borrow flag, if any
                case 0x5:
                    break;
                // 8XY6: Set LSB of V[X] to V[F] and shift V[X] to right by 1
                case 0x6:
                    V[0xF] = V[(opcode & 0xF00) >> 8] & 0x1; 
                    V[(opcode & 0xF00) >> 8] >>= 1;
                    break;
                // 8XY7: Set V[X] to V[Y] - V[X] and set borrow flag, if any
                case 0x7:
                    break;
                // 8XYE: Set MSB of V[X] to V[F] and shift V[X] to left by 1
                case 0xE:
                    V[0xF] = (V[(opcode & 0xF00) >> 8] >> 7) & 0x1;
                    V[(opcode & 0xF00) >> 8] <<= 1;
                    break;
                default:
                    printf("Unknwon opcode: 0x%X\n", opcode);
            }
            break;
        // 9XY0: If V[X] != V[Y], skip next instruction
        case 0x9000:
            if (V[(opcode & 0xF00) >> 8] != V[(opcode & 0xF0) >> 4])
                pc += 2;
            pc += 2;
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
        // CXNN: Set V[X] to random number AND NN
        case 0xC000:
            V[(opcode & 0xF00) >> 8] = (rand() % 256) & (opcode & 0xFF);
            pc += 2;
            break;
        // DXYN: Draw sprite at (V[X], V[Y]) with width of 8 px and height of N px
        case 0xD000:
            break;
        case 0xE000:
            switch (opcode & 0xFF)
            {
                // EX9E: If key stored in V[X] is pressed, skip next instruction
                case 0x9E:
                    if (key[V[(opcode & 0xF00) >> 8]] == 1)
                        pc += 2;
                    pc += 2;
                    break;
                // EXA1: If key stored in V[X] is not pressed, skip next instruction
                case 0xA1:
                    if (key[V[(opcode & 0xF00) >> 8] == 0]
                            pc += 2;
                    pc += 2;
                    break;
                default:
                    printf("Unknown opcode: 0x%X\n", opcode);
            }
            break;
        case 0xF000:
            switch (opcode & 0xFF)
            {
                // FX07: Set V[X] to delay timer value
                case 0x07:
                    V[(opcode & 0xF00) >> 8] = delay_timer;
                    pc += 2;
                    break;
                // FX0A: Wait for key press and then store in V[X]
                case 0x0A:
                    break;
                // FX15: Set delay timer to V[X]
                case 0x15:
                    delay_timer = V[(opcode & 0xF00) >> 8];
                    pc += 2;
                    break;
                // FX18: Set sound timer to V[X]
                case 0x18:
                    sound_timer = V[(opcode & 0xF00) >> 8];
                    pc += 2;
                    break;
                // FX1E: Add V[X] to I
                case 0x1E:
                    I += V[(opcode & 0xF00) >> 8];
                    pc += 2;
                    break;
                // FX29: Set I to location of sprite for character in V[X]
                case 0x29:
                    break;
                case 0x33:
                    break;
                case 0x55:
                    break;
                case 0x65:
                    break;
                default:
                    printf("Unknown opcode: 0x%X\n", opcode);
            }
            break;
        default:
            printf("Unknown opcode: 0x%X\n", opcode);
    }
}

// Execute the instruction
void Chip8::execute()
{
    
}

void Chip8::emulateCycle()
{
    fetch();
    decode();
    execute();

    // Update timers
    if (delay_timer > 0)
        delay_timer--;

    if (sound_timer > 0)
    {
        if (sound_timer == 1)
            printf("BEEP!\n");
        sound_timer--;
    }
}
