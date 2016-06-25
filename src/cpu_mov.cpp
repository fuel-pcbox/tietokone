#include "cpu.h"
#include "mem.h"

void mov_b_r_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u8 src = maincpu->regs[maincpu->rm].b[0];
        u8 dst = maincpu->regs[maincpu->reg].b[0];
        maincpu->regs[maincpu->rm].b[0] = dst;
    }
    else
    {
        u8 dst = maincpu->regs[maincpu->reg].b[0];
        cpu_writebyte(maincpu->ea_seg_base + maincpu->ea_addr, dst);
    }
    maincpu->ip+=2;
}

void mov_w_r_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u16 src = maincpu->regs[maincpu->rm].w;
        u16 dst = maincpu->regs[maincpu->reg].w;
        maincpu->regs[maincpu->rm].w = dst;
    }
    else
    {
        u16 dst = maincpu->regs[maincpu->reg].w;
        cpu_writeword(maincpu->ea_seg_base + maincpu->ea_addr, dst);
    }
    maincpu->ip+=2;
}

void mov_r_w_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u16 dst = maincpu->regs[maincpu->rm].w;
        u16 src = maincpu->regs[maincpu->reg].w;
        maincpu->regs[maincpu->rm].w = dst;
    }
    else
    {
        u16 src = maincpu->regs[maincpu->reg].w;
        cpu_writeword(maincpu->ea_seg_base + maincpu->ea_addr, src);
    }
    maincpu->ip+=2;
}

void mov_w_seg_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u16 src = maincpu->regs[maincpu->rm].w;
        u16 dst = maincpu->segs[maincpu->reg].seg;
        maincpu->regs[maincpu->rm].w = dst;
    }
    else
    {
        u16 dst = maincpu->segs[maincpu->reg].seg;
        cpu_writeword(maincpu->ea_seg_base + maincpu->ea_addr, dst);
    }
    maincpu->ip+=2;
}

void mov_seg_w_a16(cpu* maincpu)
{
    u8 modrm = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->fetch_ea_16(modrm);
    if(maincpu->mod == 3)
    {
        u16 src = maincpu->regs[maincpu->rm].w;
        u16 dst = maincpu->segs[maincpu->reg].seg;
        maincpu->segs[maincpu->reg].seg = src;
    }
    else
    {
        maincpu->segs[maincpu->reg].seg = cpu_readword(maincpu->ea_seg_base + maincpu->ea_addr);
    }
    maincpu->ip+=2;
}

void mov_al_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->AX.b[0] = tmp;
    maincpu->ip+=2;
}

void mov_cl_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->CX.b[0] = tmp;
    maincpu->ip+=2;
}

void mov_dl_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->DX.b[0] = tmp;
    maincpu->ip+=2;
}

void mov_bl_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->BX.b[0] = tmp;
    maincpu->ip+=2;
}

void mov_ah_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->AX.b[1] = tmp;
    maincpu->ip+=2;
}

void mov_ch_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->CX.b[1] = tmp;
    maincpu->ip+=2;
}

void mov_dh_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->DX.b[1] = tmp;
    maincpu->ip+=2;
}

void mov_bh_imm(cpu* maincpu)
{
    u8 tmp = cpu_readbyte(maincpu->cs + maincpu->ip + 1);
    maincpu->BX.b[1] = tmp;
    maincpu->ip+=2;
}

void mov_ax_imm(cpu* maincpu)
{
    u16 tmp = cpu_readword(maincpu->cs + maincpu->ip + 1);
    maincpu->AX.w = tmp;
    maincpu->ip+=3;
}

void mov_cx_imm(cpu* maincpu)
{
    u16 tmp = cpu_readword(maincpu->cs + maincpu->ip + 1);
    maincpu->CX.w = tmp;
    maincpu->ip+=3;
}

void mov_dx_imm(cpu* maincpu)
{
    u16 tmp = cpu_readword(maincpu->cs + maincpu->ip + 1);
    maincpu->DX.w = tmp;
    maincpu->ip+=3;
}

void mov_bx_imm(cpu* maincpu)
{
    u16 tmp = cpu_readword(maincpu->cs + maincpu->ip + 1);
    maincpu->BX.w = tmp;
    maincpu->ip+=3;
}

void mov_sp_imm(cpu* maincpu)
{
    u16 tmp = cpu_readword(maincpu->cs + maincpu->ip + 1);
    maincpu->SP.w = tmp;
    maincpu->ip+=3;
}

void mov_bp_imm(cpu* maincpu)
{
    u16 tmp = cpu_readword(maincpu->cs + maincpu->ip + 1);
    maincpu->BP.w = tmp;
    maincpu->ip+=3;
}

void mov_si_imm(cpu* maincpu)
{
    u16 tmp = cpu_readword(maincpu->cs + maincpu->ip + 1);
    maincpu->SI.w = tmp;
    maincpu->ip+=3;
}

void mov_di_imm(cpu* maincpu)
{
    u16 tmp = cpu_readword(maincpu->cs + maincpu->ip + 1);
    maincpu->DI.w = tmp;
    maincpu->ip+=3;
}
