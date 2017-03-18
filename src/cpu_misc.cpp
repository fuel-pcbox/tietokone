#include "cpu.h"
#include "mem.h"

void nop(cpu* maincpu)
{
    maincpu->ip++;
}

void sahf(cpu* maincpu)
{
    maincpu->flags &= 0xffffff2a;
    maincpu->flags |= maincpu->AX.b[1] & 0xd5;
    maincpu->ip+=1;
}

void lahf(cpu* maincpu)
{
    maincpu->AX.b[1] &= 0x2a;
    maincpu->AX.b[1] |= maincpu->flags & 0xd7;
    maincpu->ip+=1;
}

void cli(cpu* maincpu)
{
    maincpu->flags &= 0xfffffdff;
    maincpu->ip+=1;
}

void cld(cpu* maincpu)
{
    maincpu->flags &= 0xfffffbff;
    maincpu->ip+=1;
}
