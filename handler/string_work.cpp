#include "string_work.h"

std::list<std::string> string_work::split(std::string str, char delimiter)
{
    std::list<std::string> l;
    std::string s;
    std::for_each(str.begin(), str.end(), [&](char c)
    {
        if(c == delimiter)
        {
            l.push_back(s);
            s = std::string();
            return;
        }
        s.push_back(c);
    });
    l.push_back(s);
    return l;
}

std::string string_work::remove_all(std::string str, char ch)
{
    std::string s;
    std::for_each(str.begin(), str.end(), [&](char c)
        {
            if (c != ch)
                s += c;
        });
    return s;
}

std::string string_work::get_next_line_from_file(std::ifstream& file)
{
    static int count_debug = 0;

    static char c[8096];
    int count = 0;
    if (file.is_open())
        while (!file.eof())
        {
            if (count > 8094)
            {
                //log->error("update_manager::get_next_line_from_file()", "line too long in sequence.upd");
                throw std::runtime_error("std::string string_work::get_next_line_from_file(std::ifstream& file) - line too long");
            }
            file.read(c + count, 1);
            if ('\n' == c[count])
                break;
            if (file.eof())
                break;
            count++;
        }
    c[count] = '\0';

    std::string s(c);
/*    if(s.size() > 1)
    s.resize(s.size() - 1);*/

    std::cout << std::to_string(count_debug++) << "\t" << s << std::endl;

    return s;
}
