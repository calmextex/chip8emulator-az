#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct Chip8 {
    uint8_t V[16]; 
    uint16_t I; 
    uint16_t PC; 
    uint16_t SP; 
    uint8_t delay;
    uint8_t sound;
    uint8_t *memory;
    unit8_t *screen;
} Chip8;

Chip8* InitChip8(void) {
    Chip8* s = calloc(sizeof(Chip8), 1);
    s->memory = calloc(4096, 1);
    s->screen = s->memory[0xf00];
    s->PC = 0x200;
    s->SP = 0xfa0;
    return s;
}


void EmulateChip80p(Chip8 *state) {
    uint8_t *op = &state->memory[state->PC];
    int highnib = (*op & 0xf0) >> 4;
    switch (highnib) {
        case 0x00:
            switch (*op) {
                case 0xe0:
                    memset(state->screen, 0, 64 * 32 / 8);
                    state -> PC += 2;
                    break;
                case 0xee:
                    state->PC = (state -> memory[state->SP] << 8) | state->memory[state->SP + 1];
                    state->SP += 2;
                    break;
                default:
                    printf("Unknown opcode: 0x%04x\n", *op);
                    break;
            }
            break;
        case 0x01:
            state->PC = ((*op & 0x0f) << 8) | *(op + 1);
            break;
        case 0x02:
            state-> SP -= 2;
            state-> memory[state->SP] = ((state->PC + 2) & 0xff00) >> 8;
            state-> memory[state->SP + 1] = (state->PC + 2) & 0x00ff;
            state->PC = ((*op & 0x0f) << 8) | *(op + 1);
            break;
        case 0x03:
            uint8_t reg = *op & 0x0f;
            if (state->V[reg] == *(op + 1)) {
                state->PC += 4;
            } else {
                state->PC += 2;
            }
            break;
        case 0x04:
            uint8_t reg = *op & 0x0f;
            if (state->V[reg] != *(op + 1)) {
                state->PC += 4;
            } else {
                state->PC += 2;
            }
            break;
        case 0x05:
            uint8_t reg = *op & 0x0f;
            if (state->V[reg] == (*(op + 1) >> 4)) {
                state->PC += 4;
            } else {
                state->PC += 2;
            }
            break;
        case 0x06:
            uint8_t reg = *op & 0x0f;
            state->V[reg] = *(op + 1);
            state->PC += 2;
            break;
        case 0x07:
            uint8_t reg = *op & 0x0f;
            state->V[reg] += *(op + 1);
            state->PC += 2;
            break;
        case 0x08:
        case 0x09:
            uint8_t reg1 = *op & 0x0f;
            uint8_t reg2 = *(op + 1) >> 4;
            if (state->V[reg1] == state->V[reg2]) {
                state->PC += 4;
            } else {
                state->PC += 2;
            }
        case 0x0a:
            state->I = ((*op & 0x0f) << 8) | *(op + 1);
            state->PC += 2;
        case 0x0b:
            state->PC = (uint16_t)state->V[0] + ((*op & 0x0f) << 8) | *(op + 1);
        case 0x0c:
            uint8_t reg = *op & 0x0f;
            state->V[reg] = rand() & *(op + 1);
            state->PC += 2;
        case 0x0d:
            
        case 0x0e:
        case 0x0f:
    }
}