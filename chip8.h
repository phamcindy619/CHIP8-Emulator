#ifndef CHIP8
#define CHIP8

class Chip8
{
    private:
        unsigned short opcode;          // 2 byte opcode
        unsigned char memory[4096];     // 4K memory
        unsigned char V[16];            // 16 8-bit registers
        unsigned short I;               // Index register
        unsigned short pc;              // Program counter
        unsigned char gfx[64 * 32];     // Graphics with 2048 pixels
        
        // Timer registers
        unsigned char delay_timer;
        unsigned char sound_timer;

        unsigned short stack[16];       // Stack with 16 levels
        unsigned short sp;              // Stack pointer
        unsigned char key[16];          // HEX-based keypad

    public:
        void initialize();
        void emulateCycle();
};

#endif
