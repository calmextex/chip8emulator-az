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
    uint8_t *screen;
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
            int lastnib = *(op + 1) & 0x0f;
            uint8_t reg1 = *op & 0x0f;
            uint8_t reg2 = *(op + 1) >> 4;

            switch (lastnib) {
                case 0x00:
                    state->V[reg1] = state->V[reg2];
                    break;
                case 0x01:
                    state->V[reg1] |= state->V[reg2];
                    break;
                case 0x02:
                    state->V[reg1] &= state->V[reg2];
                    break;
                case 0x03:
                    state->V[reg1] ^= state->V[reg2];
                    break;
                case 0x04:
                    if (state->V[reg1] + state->V[reg2] > 0xff) {
                        state->V[0xf] = 1;
                    } else {
                        state->V[0xf] = 0;
                    }
                    state->V[reg1] += state->V[reg2];
                    break;
                case 0x05:
                    if (state->V[reg1] > state->V[reg2]) {
                        state->V[0xf] = 1;
                    } else {
                        state->V[0xf] = 0;
                    }
                    state->V[reg1] -= state->V[reg2];
                    break;
                case 0x06:
                    state->V[0xf] = state->V[reg1] & 0x01;
                    state->V[reg1] >>= 1;
                    break;
                case 0x07:
                    if (state->V[reg2] > state->V[reg1]) {
                        state->V[0xf] = 1;
                    } else {
                        state->V[0xf] = 0;
                    }
                    state->V[reg1] = state->V[reg2] - state->V[reg1];
                    break;
                case 0x0e:
                    state->V[0xf] = state->V[reg1] >> 7;
                    state->V[reg1] <<= 1;
                    break;
                default:
                    printf("Unknown opcode: 0x%04x\n", *op);
                    break;
            }
            state->PC += 2;
            
        case 0x09:
            uint8_t reg1 = *op & 0x0f;
            uint8_t reg2 = *(op + 1) >> 4;
            if (state->V[reg1] == state->V[reg2]) {
                state->PC += 4;
            } else {
                state->PC += 2;
            }
            break;
        case 0x0a:
            state->I = ((*op & 0x0f) << 8) | *(op + 1);
            state->PC += 2;
            break;
        case 0x0b:
            state->PC = (uint16_t)state->V[0] + ((*op & 0x0f) << 8) | *(op + 1);
            break;
        case 0x0c:
            uint8_t reg = *op & 0x0f;
            state->V[reg] = rand() & *(op + 1);
            state->PC += 2;
            break;
        case 0x0d:
            // Drawing sprites
            uint8_t x = state->V[*op & 0x0f];
            uint8_t y = state->V[*(op + 1) >> 4];
            uint8_t height = *op & 0x0f;
            
            state->V[0xf] = 0;
            for (int i = 0; i < height; i++) {
                uint8_t *sprite = &state->memory[state->I + i];
                int spritebit = 7;
                for (int j = (int) x; j < (int) x + 8; j++) {
                    uint8_t screenbit = (*sprite >> spritebit) & 0x01;
                    if (screenbit) {
                        uint8_t *screenbyte = &state->screen[(y + i) * 8 + j / 8];
                        uint8_t desscreenbyt = *screenbyte;
                        uint8_t destructbit = desscreenbyt & (0x80 >> (j % 8));
                        screenbit = screenbit << (7 - j % 8);
                        if (spritebit & destructbit) {
                            state->V[0xf] = 1;
                        }
                        destructbit ^= screenbit;
                        desscreenbyt = (desscreenbyt & ~(0x80 >> (j % 8))) | destructbit;
                        *screenbyte = desscreenbyt;
                    }
                    spritebit--;
                }
            }
            state->PC += 2;
            break;
        case 0x0e:
            uint8_t reg = *op & 0x0f;
            switch (*(op + 1)) {
                case 0x9e:
                    if (state->V[reg] == 1) {
                        state->PC += 4;
                    } else {
                        state->PC += 2;
                    }
                    break;
                case 0xa1:
                    if (state->V[reg] != 1) {
                        state->PC += 4;
                    } else {
                        state->PC += 2;
                    }
                    break;
                default:
                    printf("Unknown opcode: 0x%04x\n", *op);
                    break;
            }
            break;
        case 0x0f:
            uint8_t reg = *op & 0x0f;
            switch (*(op + 1)) {
                case 0x07:
                    state->V[reg] = state->delay;
                    break;
                case 0x0a:
                    // Wait for key press
                    break;
                case 0x15:
                    state->delay = state->V[reg];
                    break;
                case 0x18:
                    state->sound = state->V[reg];
                    break;
                case 0x1e:
                    state->I += state->V[reg];
                    break;
                case 0x29:
                    state->I = state->V[reg] * 5;
                    break;
                case 0x33:
                    state->memory[state->I] = state->V[reg] / 100;
                    state->memory[state->I + 1] = (state->V[reg] / 10) % 10;
                    state->memory[state->I + 2] = state->V[reg] % 10;
                    break;
                case 0x55:
                    for (int i = 0; i <= reg; i++) {
                        state->memory[state->I + i] = state->V[i];
                    }
                    break;
                case 0x65:
                    for (int i = 0; i <= reg; i++) {
                        state->V[i] = state->memory[state->I + i];
                    }
                    break;
                default:
                    printf("Unknown opcode: 0x%04x\n", *op);
                    break;
            }
    }
}

int main(int argc, char **argv) {
    Chip8 *state = InitChip8();
    FILE *f = fopen(argv[1], "rb");
    fread(state->memory + 0x200, 1, 4096 - 0x200, f);
    fclose(f);
    while (1) {
        EmulateChip80p(state);
    }
    return 0;
}