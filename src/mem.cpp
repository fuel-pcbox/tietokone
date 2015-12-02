#include "mem.h"

std::vector<memhandler> memhandlers;
std::vector<memhandler> iohandlers;

u8 ram[0xa0000];

u8 bios[0x20000];

memhandler ramhandler =
{
    0x00000, 0x9ffff,

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
    for(int i = 0; i < memhandlers.size(); i++)
    {
        if(addr>memhandlers[i].start && addr<memhandlers[i].end) return memhandlers[i].rb(addr-memhandlers[i].start);
    }
}

u16 cpu_readword(u32 addr)
{
    for(int i = 0; i < memhandlers.size(); i++)
    {
        if(addr>memhandlers[i].start && (addr+1)<memhandlers[i].end) return memhandlers[i].rw(addr-memhandlers[i].start);
    }
}

u32 cpu_readlong(u32 addr)
{
    for(int i = 0; i < memhandlers.size(); i++)
    {
        if(addr>memhandlers[i].start && (addr+3)<memhandlers[i].end) return memhandlers[i].rl(addr-memhandlers[i].start);
    }
}

void cpu_writebyte(u32 addr, u8 data)
{
    for(int i = 0; i < memhandlers.size(); i++)
    {
        if(addr>memhandlers[i].start && addr<memhandlers[i].end)
        {
            memhandlers[i].wb(addr-memhandlers[i].start, data);
            return;
        }
    }
}

void cpu_writeword(u32 addr, u16 data)
{
    for(int i = 0; i < memhandlers.size(); i++)
    {
        if(addr>memhandlers[i].start && (addr+1)<memhandlers[i].end)
        {
            memhandlers[i].ww(addr-memhandlers[i].start, data);
            return;
        }
    }
}

void cpu_writelong(u32 addr, u32 data)
{
    for(int i = 0; i < memhandlers.size(); i++)
    {
        if(addr>memhandlers[i].start && (addr+3)<memhandlers[i].end)
        {
            memhandlers[i].wl(addr-memhandlers[i].start, data);
            return;
        }
    }
}


u8 cpu_ioreadbyte(u32 addr)
{
  for(int i = 0; i < iohandlers.size(); i++)
  {
    if(addr>iohandlers[i].start && addr<iohandlers[i].end) return iohandlers[i].rb(addr-iohandlers[i].start);
  }
}

u16 cpu_ioreadword(u32 addr)
{
  for(int i = 0; i < iohandlers.size(); i++)
  {
    if(addr>iohandlers[i].start && (addr+1)<iohandlers[i].end) return iohandlers[i].rw(addr-iohandlers[i].start);
  }
}

u32 cpu_ioreadlong(u32 addr)
{
  for(int i = 0; i < iohandlers.size(); i++)
  {
    if(addr>iohandlers[i].start && (addr+3)<iohandlers[i].end) return iohandlers[i].rl(addr-iohandlers[i].start);
  }
}

void cpu_iowritebyte(u32 addr, u8 data)
{
  for(int i = 0; i < iohandlers.size(); i++)
  {
    if(addr>iohandlers[i].start && addr<iohandlers[i].end)
    {
      iohandlers[i].wb(addr-iohandlers[i].start, data);
      return;
    }
  }
}

void cpu_iowriteword(u32 addr, u16 data)
{
  for(int i = 0; i < iohandlers.size(); i++)
  {
    if(addr>iohandlers[i].start && (addr+1)<iohandlers[i].end)
    {
      iohandlers[i].ww(addr-iohandlers[i].start, data);
      return;
    }
  }
}

void cpu_iowritelong(u32 addr, u32 data)
{
  for(int i = 0; i < iohandlers.size(); i++)
  {
    if(addr>iohandlers[i].start && (addr+3)<iohandlers[i].end)
    {
      iohandlers[i].wl(addr-iohandlers[i].start, data);
      return;
    }
  }
}

void mem_init()
{
    memhandlers.push_back(ramhandler);
    memhandlers.push_back(bioshandler);
}
