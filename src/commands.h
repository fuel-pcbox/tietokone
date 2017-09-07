/**
 * This file contains the source for the CLI of EMULATOR_NAME
 */

#include "common.h"

class Command {
public:
    Command(
        std::string mStrn,
        std::string mHlp,
        const std::function<void (std::vector<std::string> args)>& f
    ) {
        str = mStrn;
        hlp = mHlp;
        func = f;
        myCommands.push_back(*this);
    }

    std::string toString() {
        return str;
    }

    std::string getHelp() {
        return hlp;
    }

    bool isThis(std::string s) {
        return str.compare(s)==0?true:false;
    }

    void doCommand(std::vector<std::string> v) {
        func(v);
    }

    static std::vector<Command> myCommands;		//set up the commands for the CLI

    static void loadCLI(void) {
        new Command("q", "Quits Tietokone", [&] (std::vector<std::string> args) {
            exit(0x0);
        });
        new Command("hlp", "View the CLI Commands", [&] (std::vector<std::string> args) {
            for(int i = 0; i < (signed) myCommands.size(); i++) {
                printf("%s:\n\t%s\n", myCommands.at(i).toString().c_str(), myCommands.at(i).getHelp().c_str());
            }
        });
    }

private:
    std::string str;
    std::string hlp;
    std::function<void (std::vector<std::string>)> func;
};

std::vector<Command> Command::myCommands;			//initialization for Command::myCommands. Silly static C++ism.