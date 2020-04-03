#include <iostream>

#include "calculation.h"

int main(int argv, char** argc)
{
    std::cout << *argc << std::endl;
    try
    {
        std::cout << "Hello World!" << std::endl;
        std::string path("../csse_covid_19_data/csse_covid_19_time_series/");
        calculation calc;
        calc.download_from_files(
                    std::string(path + "time_series_covid19_confirmed_global.csv"),
                    std::string(path + "time_series_covid19_recovered_global.csv"),
                    std::string(path + "time_series_covid19_deaths_global.csv"));

        std::vector<std::string> locations
        {
            "World",
            "US",
            "Russia",
            "China",
            "Iran",
            "Germany",
            "France",
            "United Kingdom",
            "Spain",
            "Italy",
            "Korea  South"
        };

        std::for_each(locations.begin(), locations.end(), [&](std::string name)
        {
            std::string file_name(name + ".csv");
            std::ofstream fs(file_name, std::ios::out | std::ios::binary);

            if (!fs.is_open())
                throw std::runtime_error("failed to open " + file_name);

            calc.print_head(fs);

            calc.print(fs, name, data::field_confirmed);
            calc.print(fs, name, data::field_recovered);
            calc.print(fs, name, data::field_deadth);

            fs.close();
        });
    }
    catch (std::runtime_error e)
    {
        std::cout << "error: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout << "unknown error" << std::endl;
    }

    return 0;
}
