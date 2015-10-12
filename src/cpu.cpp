#include "cpu.h"

void cpu::init()
{
    cs = 0xf0000;
    CS = 0xf000;
    ip = 0xfff0;
    cr[0] = 0;
}

void cpu::loadseg(u16 seg, x86seg* s)
{
    if(cr[0] & 1)
    {
        //TODO: Protected mode not implemented yet.
    }
    else
    {
        s->access = (3 << 5) | 2;
        s->base = seg << 4;
        s->seg = seg;
    }
}

void cpu::loadcs(u16 seg)
{
    if(cr[0] & 1)
    {
        //TODO: Protected mode not implemented yet.
    }
    else
    {
        cs = seg << 4;
        _cs.limit = 0xffff;
        _cs.limit_low = 0;
        _cs.limit_high = 0xffff;
        CS = seg;
    }
}
