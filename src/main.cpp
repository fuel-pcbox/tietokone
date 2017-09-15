#include "common.h"
#include "mem.h"
#include "cpu.h"
#include "commands.h"
#include "keyboard.h"

std::vector<u64> breakpoints;

std::vector<std::string> splitStrn(const char *str, const char *delim);
char *input(const char *prompt);

enum class machinetype
{
	ibm5150, ibm5155, ibm5160, genxt, ibm5162, ibm5170,
};

int main(int ac, char** av)
{
    cpu maincpu;
	machinetype machine;

    printf("Tietokone PC Emulator by the Tietokone Team\n");
    Command::loadCLI();

    new Command("start", "Starts the emulated processor", [&] (std::vector<std::string> args)
    {
        maincpu.init();

		log_filter = all;

		mem_init();
		keyboard_init();
    });

	new Command("machine", "Sets the machine type", [&](std::vector<std::string> args)
	{
		if(args.size() < 1) return;

		if(args[0] == "ibm5150") machine = machinetype::ibm5150;
		else if(args[0] == "ibm5155") machine = machinetype::ibm5155;
		else if(args[0] == "ibm5160") machine = machinetype::ibm5160;
		else if(args[0] == "genxt") machine = machinetype::genxt;
		else if(args[0] == "ibm5162") machine = machinetype::ibm5162;
		else if(args[0] == "ibm5170") machine = machinetype::ibm5170;
		
		switch (machine)
		{
			case machinetype::ibm5150:
			{
				maincpu.type = cputype::i8088;
				FILE* fp = fopen("roms/machines/ibmpc/pc102782.bin", "rb");
				if (!fp)
				{
					printf("Error loading IBM 5150 BIOS\n");
					return;
				}
				fread(bios + 0x1e000, 0x2000,1,fp);
				fclose(fp);

				fp = fopen("roms/machines/ibmpc/ibm-basic-1.10.rom", "rb");
				if (!fp)
				{
					printf("Error loading IBM 5150 ROM BASIC\n");
					return;
				}
				fread(bios + 0x16000, 0x8000, 1, fp);
				fclose(fp);
				break;
			}
			case machinetype::ibm5160:
			{
				maincpu.type = cputype::i8088;
				FILE* fp = fopen("roms/machines/ibmxt/xt.rom", "rb");
				if (!fp)
				{
					printf("Error loading IBM 5160 BIOS\n");
					return;
				}
				fread(bios + 0x10000, 0x10000, 1, fp);
				fclose(fp);
				break;
			}
			case machinetype::genxt:
			{
				maincpu.type = cputype::i8088;
				FILE* fp = fopen("roms/machines/genxt/pcxt.rom", "rb");
				if (!fp)
				{
					printf("Error loading Generic XT BIOS\n");
					return;
				}
				fread(bios + 0x1E000, 0x2000, 1, fp);
				fclose(fp);
				break;
			}
		}
	});

    new Command("run", "Runs the emulated processor", [&] (std::vector<std::string> args)
    {
        for(int i = 0; i<50; i++)
        {
            for(auto bp : breakpoints)
            {
                if((maincpu.cs + maincpu.ip) == bp) return;
            }
            maincpu.tick();
            if(watchpoint_hit)
            {
                watchpoint_hit = false;
                return;
            }
        }
    });

    new Command("step", "Single-steps the emulated processor", [&] (std::vector<std::string> args)
    {
        maincpu.tick();
    });

    new Command("printregs", "Print the emulated processor's registers", [&] (std::vector<std::string> args)
    {
        printf("EAX=%08x EBX=%08x ECX=%08x EDX=%08x\n",maincpu.AX.l,maincpu.BX.l,maincpu.CX.l,maincpu.DX.l);
        printf("ESP=%08x EBP=%08x ESI=%08x EDI=%08x\n",maincpu.SP.l,maincpu.BP.l,maincpu.SI.l,maincpu.DI.l);
        printf("CS=%04x CS base=%08x CS limit=%08x CS access=%02x\n", maincpu.segs[1].seg, maincpu.segs[1].base, maincpu.segs[1].limit, maincpu.segs[1].access);
        printf("DS=%04x DS base=%08x DS limit=%08x DS access=%02x\n", maincpu.segs[3].seg, maincpu.segs[3].base, maincpu.segs[3].limit, maincpu.segs[3].access);
        printf("ES=%04x ES base=%08x ES limit=%08x ES access=%02x\n", maincpu.segs[0].seg, maincpu.segs[0].base, maincpu.segs[0].limit, maincpu.segs[0].access);
        printf("SS=%04x SS base=%08x SS limit=%08x SS access=%02x\n", maincpu.segs[2].seg, maincpu.segs[2].base, maincpu.segs[2].limit, maincpu.segs[2].access);
        printf("FS=%04x FS base=%08x FS limit=%08x FS access=%02x\n", maincpu.segs[4].seg, maincpu.segs[4].base, maincpu.segs[4].limit, maincpu.segs[4].access);
        printf("GS=%04x GS base=%08x GS limit=%08x GS access=%02x\n", maincpu.segs[5].seg, maincpu.segs[5].base, maincpu.segs[5].limit, maincpu.segs[5].access);
        printf("EFLAGS=%08x\n", maincpu.flags);
    });

    new Command("peekb", "Reads a byte in the emulated processor's address space", [&] (std::vector<std::string> args)
    {
        if(args.size() < 1) return;
        u8 tmp = cpu_readbyte(strtoull(args[0].c_str(),nullptr,16));
        printf("The byte at address %llu is %02x\n", strtoull(args[0].c_str(),nullptr,16), tmp);
    });

    new Command("pokeb", "Writes a byte in the emulated processor's address space", [&] (std::vector<std::string> args)
    {
        if(args.size() < 2) return;
        cpu_writebyte(strtoull(args[0].c_str(),nullptr,16), strtoull(args[1].c_str(),nullptr,16) & 0xff);
    });

    new Command("bp", "Sets a breakpoint for the emulated processor", [&] (std::vector<std::string> args)
    {
        if(args.size() < 1) return;
        breakpoints.push_back(strtoull(args[0].c_str(),nullptr,16));
        printf("Breakpoint %d set\n",breakpoints.size());
    });

    new Command("bpdel", "Deletes a breakpoint for the emulated processor", [&] (std::vector<std::string> args)
    {
        if(args.size() < 1) return;
        breakpoints.erase(breakpoints.begin() + strtoull(args[0].c_str(),nullptr,16) - 1);
    });

    new Command("bplist", "Lists all breakpoints for the emulated processor", [&] (std::vector<std::string> args)
    {
        for(unsigned i = 0;i<breakpoints.size();i++)
        {
          printf("Breakpoint %d at address %llux\n", i + 1, breakpoints[i]);
        }
    });
    
    new Command("wp", "Sets a watchpoint for the emulated processor", [&] (std::vector<std::string> args)
    {
        if(args.size() < 4) return;
        int access = 0;
        int addrspace = 0;
        if(args[2] == "mem") addrspace = 0;
        else if(args[2] == "io") addrspace = 1;
        
        if(args[3] == "r") access = WATCHPOINT_R;
        else if(args[3] == "w") access = WATCHPOINT_W; 
        else access = WATCHPOINT_RW;
        
        if(addrspace == 0)
        {
            memwatchpoints.push_back({strtoull(args[0].c_str(),nullptr,16), strtoull(args[1].c_str(),nullptr,16), access});
            printf("Memory Watchpoint %d set\n",memwatchpoints.size());
        }
        else if(addrspace == 1)
        {
            iowatchpoints.push_back({strtoull(args[0].c_str(),nullptr,16), strtoull(args[1].c_str(),nullptr,16), access});
            printf("I/O Watchpoint %d set\n",iowatchpoints.size());
        }
    });

    new Command("wpdel", "Deletes a watchpoint for the emulated processor", [&] (std::vector<std::string> args)
    {
        if(args.size() < 2) return;
        int addrspace = 0;
        if(args[1] == "mem") addrspace = 0;
        else if(args[1] == "io") addrspace = 1;
        if(addrspace == 0) memwatchpoints.erase(memwatchpoints.begin() + strtoull(args[0].c_str(),nullptr,16) - (uint64_t)1);
        else if (addrspace == 1) iowatchpoints.erase(memwatchpoints.begin() + strtoull(args[0].c_str(),nullptr,16) - (uint64_t)1);
    });

    new Command("wplist", "Lists all watchpoints for the emulated processor", [&] (std::vector<std::string> args)
    {
        for(unsigned i = 0;i<memwatchpoints.size();i++)
        {
          printf("Memory Watchpoint %d from address %llux to address %llux, access %s\n", i + 1, memwatchpoints[i].start, memwatchpoints[i].end, (memwatchpoints[i].access == WATCHPOINT_R) ? "r" : ((memwatchpoints[i].access == WATCHPOINT_W) ? "w" : "rw"));
        }
        
        for(unsigned i = 0;i<iowatchpoints.size();i++)
        {
          printf("I/O Watchpoint %d from address %llux to address %llux, access %s\n", i + 1, iowatchpoints[i].start, iowatchpoints[i].end, (iowatchpoints[i].access == WATCHPOINT_R) ? "r" : ((iowatchpoints[i].access == WATCHPOINT_W) ? "w" : "rw"));
        }
    });

    char *prompt;
    printf("Use \"hlp\" to see the list of available commands.\n");
    do
    {
        prompt = input("> ");
        std::vector<std::string> data = splitStrn(prompt, " ");
        if(data.size() > 0) {               // ensure we don't cause an index out of bounds exception.
            for(int i = 0; i < (signed) Command::myCommands.size(); i++)
            {
                if(Command::myCommands.at(i).isThis(data.at(0)))
                {
                    std::vector<std::string> sub(data.begin() + 1, data.end());
                    Command::myCommands.at(i).doCommand(sub);
                }
            }
        }
    }
    while(true);
}

char *input(const char *prompt)
{
    int size = 256;											// we will allocate 256 chars for this purpose.
    char * data = (char *) malloc(size * sizeof(char));		// size * 1 byte. (platform dependent). May need to be replaced with wchar later.
    printf("%s", prompt);									// display the prompt...
    std::cin.getline(data, size);							// read in to data.
    if(std::cin.fail() || std::cin.bad())
    {
        printf("input stream error; resetting.\n");
        std::cin.clear();										// clear the stream error flags
        std::cin.ignore(INT_MAX, '\n');							// remove any newlines in the stream.
    }
    return data;
}

/**
 * Tokenizes a string according to a specified delimiter.
 *
 * @author ryanbwork@stackoverflow
 * @link http://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
 *
 * Special thanks to reenigne for helping debug this function.
 */
std::vector<std::string> splitStrn(const char *str, const char *delim)
{
    std::string myData(str);
    std::string token;
    std::vector<std::string> collection;
    while ( token != myData)
    {
        token = myData.substr(0, myData.find_first_of(delim));
        myData = myData.substr(myData.find_first_of(delim) + 1);
        collection.push_back(token);
    }
    return collection;
}
