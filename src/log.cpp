#include <cstdarg>

#include "log.h"

uint8_t log_filter;
std::vector<std::string> comp_filter;

void log_print(std::string *component, uint8_t level, std::string *msg, ...)
{
    if(!(level & log_filter)) return;

    std::string level_str;
    switch(level)
    {
    case log_level::error:
    {
        level_str = "Error";
        break;
    }
    case log_level::warning:
    {
        level_str = "Warning";
        break;
    }
    case log_level::debug:
    {
        level_str = "Debug";
        break;
    }
    case log_level::verbose:
    {
        level_str = "Verbose";
        break;
    }
    case log_level::info:
    {
        level_str = "Info";
        break;
    }
    }

	for(int i = 0;i<comp_filter.size();i++)
	{
		if(component == comp_filter[i]) return;
	}

  std::string final_msg = "[" + component + " | " + level_str + "] " + msg + "\n";

  va_list args;
  va_start(args, msg.c_str());

  vprintf(final_msg.c_str(), args);

  va_end(args);
}
