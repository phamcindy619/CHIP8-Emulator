#ifndef CHIP8_H
#define CHIP8_H

class Chip8
{
    private:
        unsigned short opcode;          // 2 byte opcode
        unsigned char memory[4096];     // 4K memory
        unsigned char V[16];            // 16 8-bit registers
        unsigned short I;               // Index register
        unsigned short pc;              // Program counter
        
        // Timer registers
        unsigned char delay_timer;
        unsigned char sound_timer;

        unsigned short stack[16];               // Stack with 16 levels
        unsigned short sp;                      // Stack pointer

        bool drawFlag;

        // Helper methods
        void fetch();
        void decode();

    public:
        Chip8();
        ~Chip8();
        unsigned char gfx[2048];            // Graphics with 2048 px
        unsigned char key[16];              // HEX-based keypad
        void initialize();
        void emulateCycle();
        bool loadApplication(const char*);
        void setDraw(bool);
        bool getDraw();
};

#endif
