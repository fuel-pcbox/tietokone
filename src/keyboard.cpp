#include "keyboard.h"

memhandler kbhandler =
{
    0x60, 0x64,

    [](u64 addr) -> u8
    {
        return 0;
    },
    nullptr, nullptr

    nullptr, nullptr, nullptr
};

void keyboard_init()
{
    iohandlers.push_back(kbhandler);
}