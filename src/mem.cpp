#include "mem.h"

std::vector<memhandler> handlers;

u8 ram[0xa0000];

u8 bios[0x20000];

memhandler ramhandler =
{
    0x00000, 0x9ffff,
    false,

    [](u32 addr) -> u8
    {
        return ram[addr];
    },

    [](u32 addr) -> u16
    {
        return (ram[addr+1] << 8) | ram[addr];
    },

    [](u32 addr) -> u32
    {
        return (ram[addr+3] << 24) | (ram[addr+2] << 16) | (ram[addr+1] << 8) | ram[addr];
    },

    [](u32 addr, u8 data)
    {
        ram[addr] = data;
    },

    [](u32 addr, u16 data)
    {
        ram[addr] = data & 0xff;
        ram[addr + 1] = (data >> 8) & 0xff;
    },

    [](u32 addr, u32 data)
    {
        ram[addr] = data & 0xff;
        ram[addr + 1] = (data >> 8) & 0xff;
        ram[addr + 2] = (data >> 16) & 0xff;
        ram[addr + 3] = (data >> 24) & 0xff;
    }
};

memhandler bioshandler =
{
    0xe0000, 0xfffff,
    false,

    [](u32 addr) -> u8
    {
        return bios[addr];
    },

    [](u32 addr) -> u16
    {
        return (bios[addr+1] << 8) | bios[addr];
    },

    [](u32 addr) -> u32
    {
        return (bios[addr+3] << 24) | (bios[addr+2] << 16) | (bios[addr+1] << 8) | bios[addr];
    },
    nullptr, nullptr, nullptr
};

u8 cpu_readbyte(u32 addr)
{
    for(int i = 0; i < handlers.size(); i++)
    {
        if(addr>handlers[i].start && addr<handlers[i].end) return handlers[i].rb(addr-handlers[i].start);
    }
}

u16 cpu_readword(u32 addr)
{
    for(int i = 0; i < handlers.size(); i++)
    {
        if(addr>handlers[i].start && (addr+1)<handlers[i].end) return handlers[i].rw(addr-handlers[i].start);
    }
}

u32 cpu_readlong(u32 addr)
{
    for(int i = 0; i < handlers.size(); i++)
    {
        if(addr>handlers[i].start && (addr+3)<handlers[i].end) return handlers[i].rl(addr-handlers[i].start);
    }
}

void cpu_writebyte(u32 addr, u8 data)
{
    for(int i = 0; i < handlers.size(); i++)
    {
        if(addr>handlers[i].start && addr<handlers[i].end)
        {
            handlers[i].wb(addr-handlers[i].start, data);
            return;
        }
    }
}

void cpu_writeword(u32 addr, u16 data)
{
    for(int i = 0; i < handlers.size(); i++)
    {
        if(addr>handlers[i].start && (addr+1)<handlers[i].end)
        {
            handlers[i].ww(addr-handlers[i].start, data);
            return;
        }
    }
}

void cpu_writelong(u32 addr, u32 data)
{
    for(int i = 0; i < handlers.size(); i++)
    {
        if(addr>handlers[i].start && (addr+3)<handlers[i].end)
        {
            handlers[i].wl(addr-handlers[i].start, data);
            return;
        }
    }
}

void mem_init()
{
    handlers.push_back(ramhandler);
    handlers.push_back(bioshandler);
}
