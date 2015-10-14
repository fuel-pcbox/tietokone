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

#define CS _cs.seg
#define DS _ds.seg
#define ES _es.seg
#define SS _ss.seg
#define FS _fs.seg
#define GS _gs.seg
#define cs _cs.base
#define ds _ds.base
#define es _es.base
#define ss _ss.base
#define seg_fs _fs.base
#define gs _gs.base

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
    } gdt, ldt, idt, tr, _cs, _ds, _es, _ss, _fs, _gs;

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
    void setznp16(u16 val);

    std::function<void(cpu*)> op_table_16d_16a[256];

    void tick();
};

void unknown(cpu* maincpu);

void xor_w_rmw_a16(cpu* maincpu);
void jmp_far_a16(cpu* maincpu);
