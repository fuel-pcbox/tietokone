#include "cpu.h"
#include "mem.h"

void add_b_rmw_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u8 src = maincpu->get_ea_b(maincpu->rm);
        u8 dst = maincpu->get_ea_b(maincpu->reg);
		u16 tmp = src + dst;
        maincpu->setznp8(src + dst);
		if(tmp & 0x100) maincpu->flags |= 1;
		else maincpu->flags &= ~1;
		if(((src & 0x0f) + (dst & 0x0f)) & 0x10) maincpu->flags |= 0x10;
		else maincpu->flags &= ~0x10;
		if((s16)tmp > 127 || (s16)tmp < -128) maincpu->flags |= 0x800;
		else maincpu->flags &= ~0x800;
        if(!(maincpu->rm & 4)) maincpu->regs[maincpu->rm & 3].b[0] = src + dst;
        else maincpu->regs[maincpu->rm & 3].b[1] = src + dst;
    }
    else
    {
        u8 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
        u8 dst = maincpu->get_ea_b(maincpu->reg);
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

void add_ax_imm(cpu* maincpu)
{
    u16 src = cpu_readword(maincpu->cs + maincpu->ip + 1);
    u16 dst = maincpu->AX.w;
    u32 tmp = src + dst;
    maincpu->setznp16(tmp);
	if(tmp & 0x10000) maincpu->flags |= 1;
	else maincpu->flags &= ~1;
	if(((src & 0x0f) + (dst & 0x0f)) & 0x10) maincpu->flags |= 0x10;
	else maincpu->flags &= ~0x10;
	if((s16)tmp > 0xffff || (s16)tmp < 0x8000) maincpu->flags |= 0x800;
	else maincpu->flags &= ~0x800;
    maincpu->ip+=3;
}

void or_w_rm_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u16 src = maincpu->regs[maincpu->rm].w;
        u16 dst = maincpu->regs[maincpu->reg].w;
        maincpu->setznp16(src | dst);
        maincpu->regs[maincpu->reg].w = src | dst;
    }
    else
    {
        u16 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
        u16 dst = maincpu->regs[maincpu->reg].w;
        maincpu->setznp16(src | dst);
        cpu_writeword(maincpu->ea_seg_base + maincpu->ea_addr, src | dst);
    }
    maincpu->ip+=2;
}

void sub_ax_imm(cpu* maincpu)
{
    u16 src = cpu_readword(maincpu->cs + maincpu->ip + 1);
    u16 dst = maincpu->AX.w;
    u32 tmp = src - dst;
    maincpu->setznp16(tmp);
	if(tmp & 0x10000) maincpu->flags |= 1;
	else maincpu->flags &= ~1;
	if(((src & 0x0f) + (dst & 0x0f)) & 0x10) maincpu->flags |= 0x10;
	else maincpu->flags &= ~0x10;
	if((s16)tmp > 0xffff || (s16)tmp < 0x8000) maincpu->flags |= 0x800;
	else maincpu->flags &= ~0x800;
    maincpu->ip+=3;
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

void xor_b_rm_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u8 src = maincpu->get_ea_b(maincpu->rm);
        u8 dst = maincpu->get_ea_b(maincpu->reg);
        maincpu->setznp8(src ^ dst);
        if(!(maincpu->rm & 4)) maincpu->regs[maincpu->reg & 3].b[0] = src ^ dst;
        else maincpu->regs[maincpu->reg & 3].b[1] = src ^ dst;
    }
    else
    {
        u8 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
        u8 dst = maincpu->get_ea_b(maincpu->reg);
        maincpu->setznp8(src ^ dst);
        cpu_writebyte(maincpu->ea_seg_base + maincpu->ea_addr, src ^ dst);
    }
    maincpu->ip+=2;
}

void xor_w_rm_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u16 src = maincpu->regs[maincpu->rm].w;
        u16 dst = maincpu->regs[maincpu->reg].w;
        maincpu->setznp16(src ^ dst);
        maincpu->regs[maincpu->reg].w = src ^ dst;
    }
    else
    {
        u16 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
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

void grp2_eb_1(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    switch(modrm & 0x38)
    {
        case 0x20:
        {
            maincpu->fetch_ea_16(modrm);
            if(maincpu->mod == 3)
            {
                u8 src = maincpu->get_ea_b(maincpu->rm);
                maincpu->setznp8(src << 1);
                if(src& 0x80) maincpu->flags |= 1;
                else maincpu->flags &= ~1;
                if((src & 0x80) ^ ((src & 0x40) << 1)) maincpu->flags |= 0x800;
                else maincpu->flags &= ~0x800;
                if(!(maincpu->rm & 4)) maincpu->regs[maincpu->rm & 3].b[0] = src >> 1;
                else maincpu->regs[maincpu->rm & 3].b[1] = src >> 1;
            }
            else
            {
                u8 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
		        maincpu->setznp8(src << 1);
                if(src & 0x80) maincpu->flags |= 1;
                else maincpu->flags &= ~1;
                if((src & 0x80) ^ ((src & 0x40) << 1)) maincpu->flags |= 0x800;
                else maincpu->flags &= ~0x800;
                cpu_writebyte(maincpu->ea_seg_base + maincpu->ea_addr, src << 1);
            }
            break;
        }
        case 0x28:
        {
            maincpu->fetch_ea_16(modrm);
            if(maincpu->mod == 3)
            {
                u8 src = maincpu->get_ea_b(maincpu->rm);
                maincpu->setznp8(src >> 1);
                if(src & 1) maincpu->flags |= 1;
                else maincpu->flags &= ~1;
                if(src & 0x80) maincpu->flags |= 0x800;
                else maincpu->flags &= ~0x800;
                if(!(maincpu->rm & 4)) maincpu->regs[maincpu->rm & 3].b[0] = src >> 1;
                else maincpu->regs[maincpu->rm & 3].b[1] = src >> 1;
            }
            else
            {
                u8 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
		        maincpu->setznp8(src >> 1);
                if(src & 1) maincpu->flags |= 1;
                else maincpu->flags &= ~1;
                if(src & 0x80) maincpu->flags |= 0x800;
                else maincpu->flags &= ~0x800;
                cpu_writebyte(maincpu->ea_seg_base + maincpu->ea_addr, src >> 1);
            }
            break;
        }
    }
    maincpu->ip+=2;
}

void grp2_eb_cl(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    u8 count = maincpu->CX.b[0] & 31;
    switch(modrm & 0x38)
    {
        case 0x20:
        {
            maincpu->fetch_ea_16(modrm);
            if(maincpu->mod == 3)
            {
                u8 src = maincpu->get_ea_b(maincpu->rm);
                maincpu->setznp8(src << count);
                if((src << (count - 1)) & 0x80) maincpu->flags |= 1;
                else maincpu->flags &= ~1;
                if(!(maincpu->rm & 4)) maincpu->regs[maincpu->rm & 3].b[0] = src << count;
                else maincpu->regs[maincpu->rm & 3].b[1] = src << count;
            }
            else
            {
                u8 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
		        maincpu->setznp8(src << count);
                if((src << (count - 1)) & 0x80) maincpu->flags |= 1;
                else maincpu->flags &= ~1;
                cpu_writebyte(maincpu->ea_seg_base + maincpu->ea_addr, src << count);
            }
            break;
        }
        case 0x28:
        {
            maincpu->fetch_ea_16(modrm);
            if(maincpu->mod == 3)
            {
                u8 src = maincpu->get_ea_b(maincpu->rm);
                maincpu->setznp8(src >> count);
                if((src >> (count - 1)) & 1) maincpu->flags |= 1;
                else maincpu->flags &= ~1;
                if(!(maincpu->rm & 4)) maincpu->regs[maincpu->rm & 3].b[0] = src >> count;
                else maincpu->regs[maincpu->rm & 3].b[1] = src >> count;
            }
            else
            {
                u8 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
		        maincpu->setznp8(src >> count);
                if((src >> (count - 1)) & 1) maincpu->flags |= 1;
                else maincpu->flags &= ~1;
                cpu_writebyte(maincpu->ea_seg_base + maincpu->ea_addr, src >> count);
            }
            break;
        }
    }
    maincpu->ip+=2;
}

void grp4_eb(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    switch(modrm & 0x38)
    {
        case 0x00:
        {
            maincpu->fetch_ea_16(modrm);
            if(maincpu->mod == 3)
            {
                u8 src = maincpu->get_ea_b(maincpu->rm);
                maincpu->setznp8(src + 1);
                if(!(maincpu->rm & 4)) maincpu->regs[maincpu->rm & 3].b[0] = src + 1;
                else maincpu->regs[maincpu->rm & 3].b[1] = src + 1;
            }
            else
            {
                u8 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
		        maincpu->setznp8(src + 1);
                cpu_writebyte(maincpu->ea_seg_base + maincpu->ea_addr, src + 1);
            }
            break;
        }
        case 0x08:
        {
            maincpu->fetch_ea_16(modrm);
            if(maincpu->mod == 3)
            {
                u8 src = maincpu->get_ea_b(maincpu->rm);
                maincpu->setznp8(src - 1);
                if(!(maincpu->rm & 4)) maincpu->regs[maincpu->rm & 3].b[0] = src - 1;
                else maincpu->regs[maincpu->rm & 3].b[1] = src - 1;
            }
            else
            {
                u8 src = cpu_readbyte(maincpu->ea_seg_base + maincpu->ea_addr);
		        maincpu->setznp8(src - 1);
                cpu_writebyte(maincpu->ea_seg_base + maincpu->ea_addr, src - 1);
            }
            break;
        }
    }
    maincpu->ip+=2;
}