#include "cpu.h"
#include "mem.h"

void cpu::init()
{
    cs = 0xf0000;
    CS = 0xf000;
    ip = 0xfff0;
    cr[0] = 0;

    modadd[0][0] = &BX.w;
    modadd[0][1] = &BX.w;
    modadd[0][2] = &BP.w;
    modadd[0][3] = &BP.w;
    modadd[0][4] = &SI.w;
    modadd[0][5] = &DI.w;
    modadd[0][6] = &BP.w;
    modadd[0][7] = &BX.w;
    modadd[1][0] = &SI.w;
    modadd[1][1] = &DI.w;
    modadd[1][2] = &SI.w;
    modadd[1][3] = &DI.w;
    modadd[1][4] = &zero;
    modadd[1][5] = &zero;
    modadd[1][6] = &zero;
    modadd[1][7] = &zero;

    modseg[0] = &ds;
    modseg[1] = &ds;
    modseg[2] = &ss;
    modseg[3] = &ss;
    modseg[4] = &ds;
    modseg[5] = &ds;
    modseg[6] = &ss;
    modseg[7] = &ds;
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

void cpu::fetch_ea_16(u8 modrm)
{
    mod = (modrm >> 6) & 3;
    reg = (modrm >> 3) & 7;
    rm = modrm & 7;
    if(mod == 3) return;
    else
    {
        if(!mod && rm == 6)
        {
            ea_addr = cpu_readword(cs + ip + 2);
            ea_seg_base = ds;
            ip+=2;
        }
        else
        {
            switch(mod)
            {
                case 0:
                {
                    ea_addr = 0;
                    break;
                }
                case 1:
                {
                    ea_addr = cpu_readbyte(cs + ip + 2);
                    ip++;
                    break;
                }
                case 2:
                {
                    ea_addr = cpu_readword(cs + ip + 2);
                    ip+=2;
                    break;
                }
            }

            ea_addr += *modadd[0][rm] + *modadd[1][rm];
            ea_seg_base = *modseg[rm];
        }
    }
}

void cpu::setznp16(u16 val)
{
    if(!val) flags |= 0x40; //Zero flag.
    else flags &= 0xffffffbf;

    if(val & 0x8000) flags |= 0x80; //Sign flag.
    else flags &= 0xffffff7f;

    int v = 0;
    for(int i = 0;i < 16; i++)
    {
        if(val & (1 << i)) v ^= 1;
    }
    if(!v) flags |= 0x04;
    else flags &= 0xfffffffb;
}

void cpu::tick()
{
    u8 opcode = cpu_readbyte(cs + ip);
    log_print("CPU", verbose, "Opcode %02x CS:%08x IP:%08x", opcode, cs, ip);
    switch(opcode)
    {
        case 0x31:
        {
            u8 modrm = cpu_readbyte(cs + ip + 1);
            fetch_ea_16(modrm);
            if(mod == 3)
            {
                u16 src = regs[rm].w;
                u16 dst = regs[reg].w;
                setznp16(src ^ dst);
                regs[rm].w = src ^ dst;
            }
            else
            {
                u16 src = cpu_readword(ea_seg_base + ea_addr);
                u16 dst = regs[reg].w;
                cpu_writeword(ea_seg_base + ea_addr, src ^ dst);
                setznp16(src ^ dst);
            }
            ip+=2;
            break;
        }
        case 0xea:
        {
            u16 off = cpu_readword(cs + ip + 1);
            u16 seg = cpu_readword(cs + ip + 3);
            ip = off;
            loadcs(seg);
            break;
        }
        default:
        {
            log_print("CPU", verbose, "Unknown Opcode %02x CS:%08x IP:%08x", cpu_readbyte(cs + ip), cs, ip);
        }
    }
}
