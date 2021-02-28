#include "cpu.h"
#include "mem.h"

struct cpu_op_template
{
    u8 opcode;
    std::function<void(cpu*)> func;
};

static cpu_op_template optbl_16d_16a[] =
{
    {0x00, add_b_rmw_a16},
    {0x05, add_ax_imm},
    {0x0b, or_w_rm_a16},
    {0x2d, sub_ax_imm},
    {0x31, xor_w_rmw_a16},
    {0x32, xor_b_rm_a16},
    {0x33, xor_w_rm_a16},
    {0x3c, cmp_al_imm},
	{0x70, jump_if_o},
    {0x71, jump_if_no},
	{0x72, jump_if_c},
    {0x73, jump_if_nc},
    {0x74, jump_if_z},
    {0x75, jump_if_nz},
    {0x76, jump_if_be},
    {0x77, jump_if_a},
    {0x78, jump_if_s},
    {0x79, jump_if_ns},
    {0x7a, jump_if_p},
    {0x7b, jump_if_np},
    {0x88, mov_b_r_a16},
    {0x89, mov_w_r_a16},
    {0x8b, mov_r_w_a16},
    {0x8c, mov_w_seg_a16},
    {0x8e, mov_seg_w_a16},
    {0x90, nop},
    {0x9e, sahf},
    {0x9f, lahf},
    {0xa8, test_al_imm},
    {0xb0, mov_al_imm},
    {0xb1, mov_cl_imm},
    {0xb2, mov_dl_imm},
    {0xb3, mov_bl_imm},
    {0xb4, mov_ah_imm},
    {0xb5, mov_ch_imm},
    {0xb6, mov_dh_imm},
    {0xb7, mov_bh_imm},
    {0xb8, mov_ax_imm},
    {0xb9, mov_cx_imm},
    {0xba, mov_dx_imm},
    {0xbb, mov_bx_imm},
    {0xbc, mov_sp_imm},
    {0xbd, mov_bp_imm},
    {0xbe, mov_si_imm},
    {0xbf, mov_di_imm},
    {0xc7, mov_w_imm_a16},
    {0xd0, grp2_eb_1},
    {0xd2, grp2_eb_cl},
    {0xe4, in_al_imm},
    {0xe5, in_ax_imm},
    {0xe6, out_al_imm},
    {0xe7, out_ax_imm},
    {0xe9, jmp_r16},
    {0xea, jmp_far_a16},
    {0xeb, jmp_r8},
    {0xec, in_al_dx},
    {0xed, in_ax_dx},
    {0xee, out_al_dx},
    {0xef, out_ax_dx},
    {0xf8, clc},
    {0xf9, stc},
    {0xfa, cli},
    {0xfc, cld},
    {0xfe, grp4_eb},
};

void cpu::init()
{
    cs = 0xf0000;
    CS = 0xf000;
    ip = 0xfff0;
    cr[0] = 0;
    zero = 0;
    flags = 0x0002;

    for(int i = 0; i<8; i++)
    {
        regs[i].l = 0;
    }

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

    for(int i = 0; i<256; i++)
    {
        op_table_16d_16a[i] = unknown;
    }

    for(auto& tbl_op : optbl_16d_16a)
    {
        op_table_16d_16a[tbl_op.opcode] = tbl_op.func;
    }
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
        segs[1].limit = 0xffff;
        segs[1].limit_low = 0;
        segs[1].limit_high = 0xffff;
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

u8 cpu::get_ea_b(int which_reg)
{
    if(mod == 3) return (which_reg & 4) ? regs[which_reg & 3].b[1] : regs[which_reg & 3].b[0];
    return cpu_readbyte(ea_seg_base + ea_addr);
}

void cpu::setznp8(u8 val)
{
    if(!val) flags |= 0x40; //Zero flag.
    else flags &= 0xffffffbf;

    if(val & 0x80) flags |= 0x80; //Sign flag.
    else flags &= 0xffffff7f;

    int v = 0;
    for(int i = 0; i < 8; i++)
    {
        if(val & (1 << i)) v ^= 1;
    }
    if(!v) flags |= 0x04;
    else flags &= 0xfffffffb;
}

void cpu::setznp16(u16 val)
{
    if(!val) flags |= 0x40; //Zero flag.
    else flags &= 0xffffffbf;

    if(val & 0x8000) flags |= 0x80; //Sign flag.
    else flags &= 0xffffff7f;

    int v = 0;
    for(int i = 0; i < 16; i++)
    {
        if(val & (1 << i)) v ^= 1;
    }
    if(!v) flags |= 0x04;
    else flags &= 0xfffffffb;
}

void unknown(cpu* maincpu)
{
    log_print("CPU", verbose, "Unknown Opcode %02x CS:%08x IP:%08x", maincpu->opcode, maincpu->cs, maincpu->ip);
}

void cpu::tick()
{
    opcode = cpu_readbyte(cs + ip);
    log_print("CPU", verbose, "Opcode %02x CS:%08x IP:%08x", opcode, cs, ip);
    op_table_16d_16a[opcode](this);
}
