#include <cstdint>
#include <cstdarg>

#include "log.h"

log_level log_filter;
std::vector<std::string> comp_filter;

void log_print(std::string component, log_level level, std::string msg, ...)
{
    if(!((int)level & log_filter)) return;

	va_list args;
	va_start(args, msg);

    std::string level_str;
    switch(level)
    {
    case error:
    {
        level_str = "Error";
        break;
    }
    case warning:
    {
        level_str = "Warning";
        break;
    }
    case debug:
    {
        level_str = "Debug";
        break;
    }
    case verbose:
    {
        level_str = "Verbose";
        break;
    }
    case info:
    {
        level_str = "Info";
        break;
    }
    default:
    {
        return;
        break;
    }
    }

    for(unsigned i = 0; i<comp_filter.size(); i++)
    {
        if(component == comp_filter[i]) return;
    }

    std::string final_msg = "[" + component + " | " + level_str + "] " + msg + "\n";

    vprintf(final_msg.c_str(), args);

    va_end(args);
}
