#include "cpu.h"

void nop(cpu* maincpu)
{
    maincpu->ip++;
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
