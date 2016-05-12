#include "cpu.h"

void add_b_rmw_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u8 src = maincpu->regs[maincpu->rm].b[0];
        u8 dst = maincpu->regs[maincpu->reg].b[0];
		u16 tmp = src + dst;
        maincpu->setznp8(src + dst);
		if(tmp & 0x100) maincpu->flags |= 1;
		else maincpu->flags &= ~1;
		if(((src & 0x0f) + (dst & 0x0f)) & 0x10) maincpu->flags |= 0x10;
		else maincpu->flags &= ~0x10;
		if((s16)tmp > 127 || (s16)tmp < -128) maincpu->flags |= 0x800;
		else maincpu->flags &= ~0x800;
        maincpu->regs[maincpu->rm].b[0] = src + dst;
    }
    else
    {
        u8 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
        u8 dst = maincpu->regs[maincpu->reg].b[0];
		u16 tmp = src + dst;
        maincpu->setznp8(src + dst);
		if(tmp & 0x100) maincpu->flags |= 1;
		else maincpu->flags &= ~1;
		if(((src & 0x0f) + (dst & 0x0f)) & 0x10) maincpu->flags |= 0x10;
		else maincpu->flags &= ~0x10;
		if((s16)tmp > 127 || (s16)tmp < -128) maincpu->flags |= 0x800;
		else maincpu->flags &= ~0x800;
        cpu_writebyte(maincpu->ea_seg_base + maincpu->ea_addr, src + dst);
    }
    maincpu->ip+=2;
}

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
		maincpu->setznp16(src ^ dst);
        cpu_writeword(maincpu->ea_seg_base + maincpu->ea_addr, src ^ dst);
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
