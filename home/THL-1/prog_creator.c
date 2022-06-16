#include <stdio.h>
#include <stdint.h>

#include "control.h"

#define INSTR(x, op) ((x << 4) | op)

int main() {

    /* Initializing variables */
    auto char *header = "THL1";
    auto uint8_t data[1 << 4];

    auto FILE *prog = fopen("fib.thl", "wb");

    /* Program */
    data[0x0] = INSTR(LDA,  0xE);
    data[0x1] = INSTR(OUT,  0x0);
    data[0x2] = INSTR(ADD,  0xD);
    data[0x3] = INSTR(JC,   0xA);
    data[0x4] = INSTR(STA,  0xF);
    data[0x5] = INSTR(LDA,  0xE);
    data[0x6] = INSTR(STA,  0xD);
    data[0x7] = INSTR(LDA,  0xF);
    data[0x8] = INSTR(STA,  0xE);
    data[0x9] = INSTR(JMP,  0x0);
    data[0xA] = INSTR(HALT, 0x0);
    data[0xB] = 0x0;
    data[0xC] = 0x0;
    data[0xD] = 0x0; // z - pre-prev
    data[0xE] = 0x1; // y - prev
    data[0xF] = 0x0; // x - current

    /* I/O flow */
    fwrite(header, 1, 0x4, prog);
    fwrite(data, 1, sizeof(data), prog);

    /* Main part */
    fclose(prog);

    /* Returning value */
    return 0;
}
