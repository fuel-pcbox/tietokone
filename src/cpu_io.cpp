#include "cpu.h"
#include "mem.h"

void in_al_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->AX.b[0] = cpu_ioreadbyte(tmp);
    maincpu->ip+=2;
}

void in_ax_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->AX.w = cpu_ioreadword(tmp);
    maincpu->ip+=2;
}

void out_al_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    cpu_iowritebyte(tmp, maincpu->AX.b[0]);
    maincpu->ip+=2;
}

void out_ax_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    cpu_iowritebyte(tmp, maincpu->AX.w);
    maincpu->ip+=2;
}
