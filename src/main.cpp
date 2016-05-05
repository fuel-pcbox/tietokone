#include "common.h"
#include "mem.h"
#include "cpu.h"
#include "commands.h"

std::vector<u64> breakpoints;

std::vector<std::string> splitStrn(const char *str, const char *delim);
char *input(const char *prompt);

int main(int ac, char** av)
{
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
        printf("CS=%04x DS=%04x ES=%04x SS=%04x FS=%04x GS=%04x\n",maincpu.CS,maincpu.DS,maincpu.ES,maincpu.SS,maincpu.FS,maincpu.GS);
    });

    new Command("peekb", "Reads a byte in the emulated processor's address space", [&] (std::vector<std::string> args)
    {
        u8 tmp = cpu_readbyte(strtoull(args[0].c_str(),nullptr,16));
        printf("The byte at address %08x is %02x\n", strtoull(args[0].c_str(),nullptr,16), tmp);
    });

    new Command("pokeb", "Sets a breakpoint for the emulated processor", [&] (std::vector<std::string> args)
    {
        cpu_writebyte(strtoull(args[0].c_str(),nullptr,16), strtoull(args[1].c_str(),nullptr,16) & 0xff);
    });

    new Command("bp", "Sets a breakpoint for the emulated processor", [&] (std::vector<std::string> args)
    {
        breakpoints.push_back(strtoull(args[0].c_str(),nullptr,16));
        printf("Breakpoint %d set\n",breakpoints.size());
    });

    new Command("bpdel", "Deletes a breakpoint for the emulated processor", [&] (std::vector<std::string> args)
    {
        breakpoints.erase(breakpoints.begin() + strtoull(args[0].c_str(),nullptr,10) - 1);
    });

    new Command("bplist", "Lists all breakpoints for the emulated processor", [&] (std::vector<std::string> args)
    {
        for(int i = 0;i<breakpoints.size();i++)
        {
          printf("Breakpoint %d at address %08x\n", i + 1, breakpoints[i]);
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
