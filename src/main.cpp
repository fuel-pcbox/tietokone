#include "common.h"
#include "mem.h"
#include "cpu.h"

int main(int ac, char** av)
{
    FILE* biosfp = fopen("roms/bios.bin","rb");

    fread(bios,0x20000,1,biosfp);

    fclose(biosfp);

    log_filter = error | warning | debug | verbose | info;

    handlers.push_back(bioshandler);

    cpu maincpu;

    maincpu.init();

    log_print("CPU", verbose, "Opcode %02x", cpu_readbyte(maincpu._cs.base + maincpu.ip));
}
