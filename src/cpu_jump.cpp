#include "cpu.h"
#include "mem.h"

void jump_if_o(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(maincpu->flags & 0x800) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

void jump_if_no(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(!(maincpu->flags & 0x800)) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

void jump_if_c(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(maincpu->flags & 0x1) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

void jump_if_nc(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(!(maincpu->flags & 0x1)) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

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

void jump_if_be(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if((maincpu->flags & 0x40) || (maincpu->flags & 1)) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

void jump_if_a(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(!((maincpu->flags & 0x40) || (maincpu->flags & 1))) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

void jump_if_s(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(maincpu->flags & 0x80) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

void jump_if_ns(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(!(maincpu->flags & 0x80)) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

void jump_if_p(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(maincpu->flags & 0x04) maincpu->ip += (s8)tmp;
    maincpu->ip+=2;
}

void jump_if_np(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    if(!(maincpu->flags & 0x04)) maincpu->ip += (s8)tmp;
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
