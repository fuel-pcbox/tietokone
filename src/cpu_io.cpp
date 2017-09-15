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
    cpu_iowriteword(tmp, maincpu->AX.w);
    maincpu->ip+=2;
}

void in_al_dx(cpu* maincpu)
{
    maincpu->AX.b[0] = cpu_ioreadbyte(maincpu->DX.w);
    maincpu->ip++;
}

void in_ax_dx(cpu* maincpu)
{
    maincpu->AX.w = cpu_ioreadword(maincpu->DX.w);
    maincpu->ip++;
}

void out_al_dx(cpu* maincpu)
{
    cpu_iowritebyte(maincpu->DX.w, maincpu->AX.b[0]);
    maincpu->ip++;
}

void out_ax_dx(cpu* maincpu)
{
    cpu_iowriteword(maincpu->DX.w, maincpu->AX.w);
    maincpu->ip++;
}