#pragma once

#include "common.h"

#define AX regs[0]
#define CX regs[1]
#define DX regs[2]
#define BX regs[3]
#define SP regs[4]
#define BP regs[5]
#define SI regs[6]
#define DI regs[7]

#define CS segs[1].seg
#define DS segs[3].seg
#define ES segs[0].seg
#define SS segs[2].seg
#define FS segs[4].seg
#define GS segs[5].seg
#define cs segs[1].base
#define ds segs[3].base
#define es segs[0].base
#define ss segs[2].base
#define seg_fs segs[4].base
#define gs segs[5].base

struct cpu
{
    union x86reg
    {
        u32 l;
        u16 w;
        u8 b[2];
    } regs[8];

    u32 flags;
    u32 ip;
    u16 zero;

    struct x86seg
    {
        u32 base,limit;
        u8 access;
        u16 seg;
        u32 limit_low, limit_high;
    } gdt, ldt, idt, tr, segs[6];

    void init();

    void loadseg(u16 seg, x86seg* s);
    void loadcs(u16 seg);

    u32 cr[5]; //CR1 is unused.
    u32 dr[8];

    u16* modadd[2][8];
    u32* modseg[8];

    u8 opcode;

    int mod;
    int reg;
    int rm;

    u16 ea_seg_base;
    u32 ea_addr;

    void fetch_ea_16(u8 modrm);
    void setznp8(u8 val);
    void setznp16(u16 val);

    std::function<void(cpu*)> op_table_16d_16a[256];

    void tick();
};

void unknown(cpu* maincpu);

void xor_w_rmw_a16(cpu* maincpu);
void cmp_al_imm(cpu* maincpu);
void jump_if_z(cpu* maincpu);
void jump_if_nz(cpu* maincpu);
void mov_b_r_a16(cpu* maincpu);
void mov_w_r_a16(cpu* maincpu);
void mov_r_w_a16(cpu* maincpu);
void mov_w_seg_a16(cpu* maincpu);
void mov_seg_w_a16(cpu* maincpu);
void nop(cpu* maincpu);
void test_al_imm(cpu* maincpu);
void mov_al_imm(cpu* maincpu);
void mov_cl_imm(cpu* maincpu);
void mov_dl_imm(cpu* maincpu);
void mov_bl_imm(cpu* maincpu);
void mov_ah_imm(cpu* maincpu);
void mov_ch_imm(cpu* maincpu);
void mov_dh_imm(cpu* maincpu);
void mov_bh_imm(cpu* maincpu);
void mov_ax_imm(cpu* maincpu);
void mov_cx_imm(cpu* maincpu);
void mov_dx_imm(cpu* maincpu);
void mov_bx_imm(cpu* maincpu);
void mov_sp_imm(cpu* maincpu);
void mov_bp_imm(cpu* maincpu);
void mov_si_imm(cpu* maincpu);
void mov_di_imm(cpu* maincpu);
void in_al_imm(cpu* maincpu);
void in_ax_imm(cpu* maincpu);
void out_al_imm(cpu* maincpu);
void out_ax_imm(cpu* maincpu);
void jmp_r16(cpu* maincpu);
void jmp_far_a16(cpu* maincpu);
void jmp_r8(cpu* maincpu);
void cli(cpu* maincpu);
void cld(cpu* maincpu);
