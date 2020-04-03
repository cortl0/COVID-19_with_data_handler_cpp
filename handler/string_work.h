#ifndef STRING_WORK_H
#define STRING_WORK_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <stdexcept>
#include <string>

class string_work
{
public:
    static std::list<std::string> split(std::string str, char delimiter);
    static std::string remove_all(std::string str, char c);
    static std::string get_next_line_from_file(std::ifstream& file);
};

#endif // STRING_WORK_H
