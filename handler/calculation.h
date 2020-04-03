#ifndef CALCULATION_H
#define CALCULATION_H

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <list>
#include <vector>

#include "string_work.h"
#include "place.h"

class calculation
{
public:
    std::vector<std::string> dates;
    std::shared_ptr<place> wrld;
    calculation();
    void download_from_files(
            std::string file_confirmed_path,
            std::string file_recovered_path,
            std::string file_deadths_path);
    void fill(std::string& file_name, data::field fld);
    void print_head(std::ofstream& fs);
    void print(std::ofstream& fs, std::string& location, data::field);
};

#endif // CALCULATION_H
