#ifndef DATA_H
#define DATA_H

#include <algorithm>
#include <list>
#include <vector>
#include <string>

class data
{
public:
    std::vector<long long> confirmed;
    std::vector<long long> recovered;
    std::vector<long long> deadths;
    enum field
    {
        field_confirmed = 0,
        field_recovered = 1,
        field_deadth = 2
    };
    data();
    void set_data(std::list<std::string>, field);
};

#endif // DATA_H
