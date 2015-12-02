#pragma once

#include "common.h"

struct memhandler
{
    u32 start, end;

    std::function<u8(u32)> rb;
    std::function<u16(u32)> rw;
    std::function<u32(u32)> rl;

    std::function<void(u32,u8)> wb;
    std::function<void(u32,u16)> ww;
    std::function<void(u32,u32)> wl;
};

extern memhandler bioshandler;

extern u8 bios[0x20000];

extern std::vector<memhandler> memhandlers;
extern std::vector<memhandler> iohandlers;

u8 cpu_readbyte(u32 addr);
u16 cpu_readword(u32 addr);
u32 cpu_readlong(u32 addr);
void cpu_writebyte(u32 addr, u8 data);
void cpu_writeword(u32 addr, u16 data);
void cpu_writelong(u32 addr, u32 data);

u8 cpu_ioreadbyte(u32 addr);
u16 cpu_ioreadword(u32 addr);
u32 cpu_ioreadlong(u32 addr);
void cpu_iowritebyte(u32 addr, u8 data);
void cpu_iowriteword(u32 addr, u16 data);
void cpu_iowritelong(u32 addr, u32 data);

void mem_init();
