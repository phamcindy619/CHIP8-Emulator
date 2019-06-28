#ifndef CHIP8_H
#define CHIP8_H

class Chip8
{
    private:
        const static int GFX_SIZE = 64 * 32;   // 2048 pixels
        const static int REG_SIZE = 16;
        const static int MEM_SIZE = 4096;
        const static int STACK_SIZE = 16;
        const static int NUM_OF_KEYS = 16;

        unsigned short opcode;          // 2 byte opcode
        unsigned char memory[MEM_SIZE]; // 4K memory
        unsigned char V[REG_SIZE];      // 16 8-bit registers
        unsigned short I;               // Index register
        unsigned short pc;              // Program counter
        unsigned char gfx[GFX_SIZE];    // Graphics with 2048 pixels
        
        // Timer registers
        unsigned char delay_timer;
        unsigned char sound_timer;

        unsigned short stack[STACK_SIZE];       // Stack with 16 levels
        unsigned short sp;                      // Stack pointer
        unsigned char key[NUM_OF_KEYS];         // HEX-based keypad

    public:
        void initialize();
        void emulateCycle();
        bool loadApplication(const char* fileName);
};

#endif
