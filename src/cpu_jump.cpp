#include "cpu.h"

void jump_if_z(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(maincpu->flags & 0x40) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

void jump_if_nz(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(!(maincpu->flags & 0x40)) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

void jmp_r16(cpu* maincpu)
{
    u16 off = cpu_readword(maincpu->cs + maincpu->ip + 1);
    maincpu->ip += 3;
    maincpu->ip += (s16)off;
}

void jmp_far_a16(cpu* maincpu)
{
    u16 off = cpu_readword(maincpu->cs + maincpu->ip + 1);
    u16 seg = cpu_readword(maincpu->cs + maincpu->ip + 3);
    maincpu->ip = off;
    maincpu->loadcs(seg);
}

void jmp_r8(cpu* maincpu)
{
    u8 off = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->ip += 2;
    maincpu->ip += (s8)off;
}
