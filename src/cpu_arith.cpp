#include "cpu.h"

void xor_w_rmw_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u16 src = maincpu->regs[maincpu->rm].w;
        u16 dst = maincpu->regs[maincpu->reg].w;
        maincpu->setznp16(src ^ dst);
        maincpu->regs[maincpu->rm].w = src ^ dst;
    }
    else
    {
        u16 src = cpu_readword(maincpu->ea_seg_base + maincpu->ea_addr);
        u16 dst = maincpu->regs[maincpu->reg].w;
        cpu_writeword(maincpu->ea_seg_base + maincpu->ea_addr, src ^ dst);
        maincpu->setznp16(src ^ dst);
    }
    maincpu->ip+=2;
}

void cmp_al_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->setznp8(maincpu->AX.b[0] - tmp);
    maincpu->ip+=2;
}

void test_al_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->setznp8(maincpu->AX.b[0] & tmp);
    maincpu->ip+=2;
}
