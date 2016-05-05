#include "common.h"
#include "mem.h"
#include "cpu.h"
#include "commands.h"
#include "keyboard.h"

std::vector<u64> breakpoints;

std::vector<std::string> splitStrn(const char *str, const char *delim);
char *input(const char *prompt);

int main(int ac, char** av)
{
    if(ac < 2) return 1;
    FILE* biosfp = fopen(av[1],"rb");

    fseek(biosfp,0,SEEK_END);
    long biossize = ftell(biosfp);
    fseek(biosfp,0,SEEK_SET);

    fread(bios+0x10000,biossize,1,biosfp);

    fclose(biosfp);

    log_filter = error | warning | debug | verbose | info;

    mem_init();
    keyboard_init();

    cpu maincpu;

    printf("Tietokone 386 PC Emulator by the Tietokone Team\n");
    Command::loadCLI();

    new Command("start", "Start the emulated processor", [&] (std::vector<std::string> args)
    {
        maincpu.init();
    });


    new Command("run", "Run the emulated processor", [&] (std::vector<std::string> args)
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

    new Command("step", "Single-step the emulated processor", [&] (std::vector<std::string> args)
    {
        maincpu.tick();
    });

    new Command("printregs", "Print the emulated processor's registers", [&] (std::vector<std::string> args)
    {
        printf("EAX=%08x EBX=%08x ECX=%08x EDX=%08x\n",maincpu.AX.l,maincpu.BX.l,maincpu.CX.l,maincpu.DX.l);
        printf("ESP=%08x EBP=%08x ESI=%08x EDI=%08x\n",maincpu.SP.l,maincpu.BP.l,maincpu.SI.l,maincpu.DI.l);
        printf("CS=%04x CS base=%08x CS limit=%08x CS access=%02x\n", segs[1].seg, segs[1].base, segs[1].limit, segs[1].access);
        printf("DS=%04x DS base=%08x DS limit=%08x DS access=%02x\n", segs[3].seg, segs[3].base, segs[3].limit, segs[3].access);
        printf("ES=%04x ES base=%08x ES limit=%08x ES access=%02x\n", segs[0].seg, segs[0].base, segs[0].limit, segs[0].access);
        printf("SS=%04x SS base=%08x SS limit=%08x SS access=%02x\n", segs[2].seg, segs[2].base, segs[2].limit, segs[2].access);
        printf("FS=%04x FS base=%08x FS limit=%08x FS access=%02x\n", segs[4].seg, segs[4].base, segs[4].limit, segs[4].access);
        printf("GS=%04x GS base=%08x GS limit=%08x GS access=%02x\n", segs[5].seg, segs[5].base, segs[5].limit, segs[5].access);
    });

    new Command("peekb", "Reads a byte in the emulated processor's address space", [&] (std::vector<std::string> args)
    {
        if(args.size() < 1) return;
        u8 tmp = cpu_readbyte(strtoull(args[0].c_str(),nullptr,16));
        printf("The byte at address %08x is %02x\n", strtoull(args[0].c_str(),nullptr,16), tmp);
    });

    new Command("pokeb", "Sets a breakpoint for the emulated processor", [&] (std::vector<std::string> args)
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
        breakpoints.erase(breakpoints.begin() + strtoull(args[0].c_str(),nullptr,10) - 1);
    });

    new Command("bplist", "Lists all breakpoints for the emulated processor", [&] (std::vector<std::string> args)
    {
        for(int i = 0;i<breakpoints.size();i++)
        {
          printf("Breakpoint %d at address %08x\n", i + 1, breakpoints[i]);
        }
    });
    
    new Command("wp", "Sets a watchpoint for the emulated processor", [&] (std::vector<std::string> args)
    {
        if(args.size() < 3) return;
        int access = 0;
        if(args[2] == "r") access = WATCHPOINT_R;
        else if(args[2] == "w") access = WATCHPOINT_W; 
        else access = WATCHPOINT_RW;  
        watchpoints.push_back({strtoull(args[0].c_str(),nullptr,16), strtoull(args[1].c_str(),nullptr,16), access});
        printf("Watchpoint %d set\n",watchpoints.size());
    });

    new Command("wpdel", "Deletes a watchpoint for the emulated processor", [&] (std::vector<std::string> args)
    {
        if(args.size() < 1) return;
        watchpoints.erase(watchpoints.begin() + strtoull(args[0].c_str(),nullptr,10) - 1);
    });

    new Command("wplist", "Lists all watchpoints for the emulated processor", [&] (std::vector<std::string> args)
    {
        for(int i = 0;i<watchpoints.size();i++)
        {
          printf("Watchpoint %d from address %08x to address %08x, access %s\n", i + 1, watchpoints[i].start, watchpoints[i].end, (watchpoints[i].access == WATCHPOINT_R) ? "r" : ((watchpoints[i].access == WATCHPOINT_W) ? "w" : "rw"));
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
