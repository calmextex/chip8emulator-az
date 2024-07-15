#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


void Chip8Disassembler(uint8_t *buffer, int pc) {
    // Citation: http://www.emulator101.com/chip-8-disassembler.html
    // Citation: https://en.wikipedia.org/wiki/CHIP-8 for opcode information
    // Starter code and suggestions for opcodes taken from the Emulator 101 breakdown of the Chip-8. x06, x0a code implementations taken from site. The rest are my own.

    uint8_t *code = &buffer[pc];
    uint8_t firstnib = (code[0] >> 4);

    printf("%04x %02x %02x ", pc, code[0], code[1]);
    switch (firstnib)
    {
        case 0x00: 
            switch (code[1]) {
                // there are two 00 opcodes, 0xe0 and 0xee according to the wikipedia page. One set to clear screen and second for return
                case 0xe0: printf("%-10s\n", "CLS"); break;
                case 0xee: printf("%-10s\n", "RET"); break;
                default: printf("Unknown opcode: 0"); break;
            }
            break;
        case 0x01: 
            {
                uint8_t addr = code[0] & 0x0f;
                print("%-10s $%01x%02x", "JUMP", addr, code[1]);
            }
            break;
        case 0x02: 
            {
                uint8_t addr = code[0] & 0x0f;
                printf("%-10s $%01x%02x", "CALL", addr, code[1]);
            }
            break;
        case 0x03: 
            {
                uint8_t reg = code[0] & 0x0f;
                printf("%-10s V%01x, #$%02x", "SKIP.EQUAL", reg, code[1]);
            }
            break;
        case 0x04: 
            {
                uint8_t reg = code[0] & 0x0f;
                printf("%-10s V%01x, #$%02x", "SKIP.NOTEQUAL", reg, code[1]);
            }
            break;
        case 0x05:
            {
                uint8_t reg1 = code[0] & 0x0f;
                // shift bits 4 places to the right of the element in code[1]
                printf("%-10s V%01x, V%01x", "SKIP.EQUAL", reg1, code[1] >> 4);
            }
            break;
        case 0x06:
            {
                // sets VX to NN
                uint8_t reg = code[0] & 0x0f;
                printf("%-10s V%01x, #$%02x\n", "MVI", reg, code[1]);
            }
            break;
        case 0x07: 
            {   
                // adds NN to VX wihout changing the carry flag
                uint8_t reg = code[0] & 0x0f;
                printf("%-10s V%01x, #$%02x\n", "ADD", reg, code[1]);
            }
            break;
        case 0x08: 
            {
                uint8_t lastnib = code[1]>>4;
                switch(lastnib)
                {
                    case 0:
                        {
                            uint8_t reg = code[0] & 0x0f;
                            printf("%-10s V%01x, V%01x\n", "MOV", reg, code[1] >> 4);
                        }
                        break;
                    case 1:
                        {
                            uint8_t reg = code[0] & 0x0f;
                            printf("%-10s V%01x, V%01x\n", "OR", reg, code[1] >> 4);
                        }
                        break;
                    case 2:
                        {
                            uint8_t reg = code[0] & 0x0f;
                            printf("%-10s V%01x, V%01x\n", "AND", reg, code[1] >> 4);
                        }
                        break;
                    case 3:
                        {
                            uint8_t reg = code[0] & 0x0f;
                            printf("%-10s V%01x, V%01x\n", "XOR", reg, code[1] >> 4);
                        }
                        break;
                    case 4:
                        {
                            uint8_t reg = code[0] & 0x0f;
                            printf("%-10s V%01x, V%01x\n", "ADD", reg, code[1] >> 4);
                        }
                        break;
                    case 5:
                        {
                            uint8_t reg = code[0] & 0x0f;
                            printf("%-10s V%01x, V%01x, V%01x", "SUB", reg, reg, code[1] >> 4);
                        }
                        break;
                    case 6:
                        {
                            uint8_t reg = code[0] & 0x0f;
                            printf("%-10s V%01x, V%01x\n", "SHR", reg, code[1] >> 4);
                        }
                        break;
                    case 7:
                        {
                            uint8_t reg = code[0] & 0x0f;
                            printf("%-10s V%01x, V%01x, V%01x", "SUBN", reg, reg, code[1] >> 4);
                        }
                        break;
                    case 0xe:
                        {
                            uint8_t reg = code[0] & 0x0f;
                            printf("%-10s V%01x, V%01x\n", "SHL", reg, code[1] >> 4);
                        }
                        break;
                    default: printf("Unknown opcode: 8"); break;
                }
            }
            break;
        case 0x09:
            {
                uint8_t reg = code[0] & 0x0f;
                printf("%-10s V%01x, V%01x", "SKIP.NOTEQUAL", reg, code[1] >> 4);
            }
            break;
        case 0x0a:
            {
                uint8_t addr = code[0] & 0x0f;
                printf("%-10s I, #$01x%02x", "MVI", addr, code[1]);
            }
            break;
        case 0x0b: 
            {
                uint8_t addr = code[0] & 0x0f;
                printf("%-10s $%01x%02x(V0)", "JUMP", addr, code[1]);
            }
            break;
        case 0x0c:
            {
                uint8_t reg = code[0] & 0x0f;
                printf("%-10s V%01x, #$%02x", "RAND", reg, code[1]);

            }
            break;
        case 0x0d:
            {
                uint8_t reg = code[0] & 0x0f;
                uint8_t reg2 = code[1] & 0x0f;
                printf("%-10s V%01x, V%01x, #$%01x", "SPRITE", reg, code[1] >> 4, reg2);
            }
            break;
        case 0x0e:
            switch(code[1])
            {
                case 0x9e:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s V%01x", "SKIP.KEY", reg);
                    }
                    break;
                case 0xa1:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s V%01x", "SKIP.NOKEY", reg);
                    }
                    break;
                default: printf("Unknown opcode: E"); break;
            }
            break;
        case 0x0f:
            switch(code[1])
            {
                case 0x07:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s V%01x, DELAY", "MOV", reg);
                    }
                    break;
                case 0x0a:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s V%01x", "KEY", reg);
                    }
                    break;
                case 0x15:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s DELAY, V%01x", "MOV", reg);
                    }
                    break;
                case 0x18:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s SOUND, V%01x", "MOV", reg);
                    }
                    break;
                case 0x1e:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s I, V%01x", "ADD", reg);
                    }
                    break;
                case 0x29:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s I, V%01x", "SPRITECHAR", reg);
                    }
                    break;
                case 0x33:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s (I), V%01x", "BCD", reg);
                    }
                    break;
                case 0x55:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s (I), V0-V%01x", "MOV", reg);
                    }
                    break;
                case 0x65:
                    {
                        uint8_t reg = code[0] & 0x0f;
                        printf("%-10s V0-V%01x, (I)", "MOV", reg);
                    }
                    break;
                default: printf("Unknown opcode: F"); break;
            }

}


int main(int argc, char**argv) {
    // taken from skeleton code in Emulator 101 breakdown of the Chip-8
    if (argc < 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }
    FILE *file = fopen(argv[1], "rb");
    if (file == NULL) {
        fprintf(stderr, "File: %s not found\n", argv[1]);
        return 2;
    }

    fseek(file, 0L, SEEK_END);
    int fsize = ftell(file);
    fseek(file, 0L, SEEK_SET);

    unsigned char *buffer = malloc(fsize + 0x200);
    if (buffer == NULL) {
        fprintf(stderr, "Memory error\n");
        return 3;
    }
    fread(buffer + 0x200, fsize, 1, file);
    fclose(file);

    int pc = 0x200;
    while (pc < (fsize + 0x200)) {
        Chip8Disassembler(buffer, pc);
        pc += 2;
        printf("\n");
    }
    free(buffer);
    return 0;
}