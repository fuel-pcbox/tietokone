#pragma once

#include "common.h"

struct memhandler
{
    u64 start, end;

    std::function<u8(u64)> rb;
    std::function<u16(u64)> rw;
    std::function<u32(u64)> rl;

    std::function<void(u64,u8)> wb;
    std::function<void(u64,u16)> ww;
    std::function<void(u64,u32)> wl;
};

enum
{
    WATCHPOINT_R = 1,
    WATCHPOINT_W = 2,
    WATCHPOINT_RW = 3
};
    
struct watchpoint
{
    u64 start, end;
    int access;
};

extern bool watchpoint_hit;
extern std::vector<watchpoint> watchpoints;

extern memhandler bioshandler;

extern u8 bios[0x20000];

extern std::vector<memhandler> memhandlers;
extern std::vector<memhandler> iohandlers;

u8 cpu_readbyte(u64 addr);
u16 cpu_readword(u64 addr);
u32 cpu_readlong(u64 addr);
void cpu_writebyte(u64 addr, u8 data);
void cpu_writeword(u64 addr, u16 data);
void cpu_writelong(u64 addr, u32 data);

u8 cpu_ioreadbyte(u64 addr);
u16 cpu_ioreadword(u64 addr);
u32 cpu_ioreadlong(u64 addr);
void cpu_iowritebyte(u64 addr, u8 data);
void cpu_iowriteword(u64 addr, u16 data);
void cpu_iowritelong(u64 addr, u32 data);

void mem_init();
