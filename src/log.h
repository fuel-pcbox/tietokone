#pragma once

#include <cstdio>
#include <string>
#include <vector>

enum log_level
{
    error = 1,
    warning = 2,
    debug = 4,
    verbose = 8,
    info = 16,
};

void log_print(std::string component, log_level level, std::string msg, ...);

extern log_level log_filter;
extern std::vector<std::string> comp_filter;
