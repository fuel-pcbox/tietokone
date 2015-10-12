#include "common.h"
#include "mem.h"

int main(int ac, char** av)
{
    FILE* biosfp = fopen("roms/bios.bin","rb");

    fread(bios,0x20000,1,biosfp);

    fclose(biosfp);

    log_filter = error | warning | debug | verbose | info;

    handlers.push_back(bioshandler);

    u32 csbase = 0x000f0000;
    u32 eip = 0x0000fff0;

    log_print("CPU", verbose, "Opcode %02x", cpu_readbyte(csbase + eip));
}
