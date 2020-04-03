#include "calculation.h"

calculation::calculation() : wrld(new place("World"))
{

}

void calculation::download_from_files(std::string file_confirmed_path, std::string file_recovered_path, std::string file_deadths_path)
{
    fill(file_confirmed_path, data::field_confirmed);
    fill(file_recovered_path, data::field_recovered);
    fill(file_deadths_path, data::field_deadth);
}

void quotation_marks_handler(std::string& line)
{
    auto n = line.find("\"");
    if(std::string::npos != n)
    {
        std::cout << std::to_string(std::string::npos) << std::endl;
        std::cout << std::to_string(n) << std::endl;

        //        auto m = line.find('\"', n + 1);
        //        if(std::string::npos != m)
        //        {
        auto k = line.find(',', n + 1);
        if(std::string::npos != k)
        {
            auto it = line.begin();
            std::advance(it, k);
            *it = ' ';
        }
        //        }
    }
}

void calculation::fill(std::string& file_name, data::field fld)
{
    std::ifstream fs(file_name, std::ios::in | std::ios::binary);
    if (!fs.is_open())
        throw std::runtime_error("failed to open " + file_name);

    std::string line = string_work::get_next_line_from_file(fs);

    if (!line.size())
        return;

    std::list<std::string> lines = string_work::split(line, ',');

    if (lines.size() < 5)
        throw std::runtime_error("(lines.size() < 5) " + file_name);

    auto it_str = lines.begin();
    if(("Province/State" != *it_str++) ||
            ("Country/Region" != *it_str++) ||
            ("Lat" != *it_str++) ||
            ("Long" != *it_str++))
        throw std::runtime_error("wrong file format " + file_name);

    lines.erase(lines.begin(), it_str);

    std::vector<std::string >::size_type count = 0;
    if(dates.size() == 0)
    {
        dates.resize(lines.size());
        std::for_each(lines.begin(), lines.end(), [&](std::string s)
        {
            dates[count++] = s;
        });
    }
    else
    {
        if(dates.size() != lines.size())
            throw std::runtime_error("wrong file format " + file_name + " dates wrong");
        std::for_each(lines.begin(), lines.end(), [&](std::string s)
        {
            if(dates[count++] != s)
                throw std::runtime_error("wrong file format " + file_name + " dates wrong");
        });
    }

    while (true)
    {
        line = string_work::get_next_line_from_file(fs);

        if (line.size() == 0)
            break;

        std::cout << line << std::endl;
        quotation_marks_handler(line);
        std::cout << line << std::endl;

        lines = string_work::split(line, ',');

        if(dates.size() != lines.size() - 4)
            throw std::runtime_error("wrong file format " + file_name + " wrong data line");

        it_str = lines.begin();
        std::string province_name = *it_str++;
        std::string country_name = *it_str++;
        double latitude = std::stod(*it_str++);
        double longitude = std::stod(*it_str++);

        std::cout << std::to_string(lines.size()) << std::endl;

        lines.erase(lines.begin(), it_str);

        std::cout << std::to_string(lines.size()) << std::endl;

        std::cout << std::to_string(dates.size()) << std::endl;

        std::map<std::string, std::shared_ptr<place>>::iterator it_country = wrld->entry.find(country_name);
        if(it_country == wrld->entry.end())
            wrld->entry.insert(std::pair<std::string, std::shared_ptr<place>>(country_name, std::shared_ptr<place>(new place(country_name))));
        it_country = wrld->entry.find(country_name);
        if(province_name.size() == 0)
            it_country->second->dt.set_data(lines, fld);
        else
        {
            std::map<std::string, std::shared_ptr<place>>::iterator it_province = it_country->second->entry.find(province_name);
            if(it_province == it_country->second->entry.end())
                it_country->second->entry.insert(std::pair<std::string, std::shared_ptr<place>>(province_name, std::shared_ptr<place>(new place(province_name))));
            it_province = it_country->second->entry.find(province_name);
            it_province->second->dt.set_data(lines, fld);
            //            if(it_country->second.dt.confirmed.size() == 0)
            //            {
            //                it_country->second.dt.confirmed.resize(it_province->second.dt.confirmed.size(), 0);
            //                it_country->second.dt.recovered.resize(it_province->second.dt.confirmed.size(), 0);
            //                it_country->second.dt.deadths.resize(it_province->second.dt.confirmed.size(), 0);
            //            }
        }
        it_country->second->latitude = latitude;
        it_country->second->longitude = longitude;
    }

    std::for_each(wrld->entry.begin(), wrld->entry.end(), [&](std::pair<std::string, std::shared_ptr<place>> c){
        switch (fld)
        {
        case data::field_confirmed:
            if(c.second->dt.confirmed.size() == 0)
                c.second->dt.confirmed.resize(dates.size(), 0);
            std::for_each(c.second->entry.begin(), c.second->entry.end(), [&](std::pair<std::string, std::shared_ptr<place>> p)
            {
                auto it_c_dt = c.second->dt.confirmed.begin();
                auto it_p_dt = p.second->dt.confirmed.begin();

                while(it_p_dt != p.second->dt.confirmed.end())
                {
                    *it_c_dt += *it_p_dt;
                    it_c_dt++;
                    it_p_dt++;
                }
            });
            break;
        case data::field_recovered:
            if(c.second->dt.recovered.size() == 0)
                c.second->dt.recovered.resize(dates.size(), 0);
            std::for_each(c.second->entry.begin(), c.second->entry.end(), [&](std::pair<std::string, std::shared_ptr<place>> p)
            {
                auto it_c_dt = c.second->dt.recovered.begin();
                auto it_p_dt = p.second->dt.recovered.begin();

                while(it_p_dt != p.second->dt.recovered.end())
                {
                    *it_c_dt += *it_p_dt;
                    it_c_dt++;
                    it_p_dt++;
                }
            });
            break;
        case data::field_deadth:
            if(c.second->dt.deadths.size() == 0)
                c.second->dt.deadths.resize(dates.size(), 0);
            std::for_each(c.second->entry.begin(), c.second->entry.end(), [&](std::pair<std::string, std::shared_ptr<place>> p)
            {
                auto it_c_dt = c.second->dt.deadths.begin();
                auto it_p_dt = p.second->dt.deadths.begin();

                while(it_p_dt != p.second->dt.deadths.end())
                {
                    *it_c_dt += *it_p_dt;
                    it_c_dt++;
                    it_p_dt++;
                }
            });
            break;
        }
    });

    std::for_each(wrld->entry.begin(), wrld->entry.end(), [&](std::pair<std::string, std::shared_ptr<place>> c){
        switch (fld)
        {
        case data::field_confirmed:
        {
            if(wrld->dt.confirmed.size() == 0)
                wrld->dt.confirmed.resize(dates.size(), 0);
            auto it_c_dt = c.second->dt.confirmed.begin();
            auto it_w_dt = wrld->dt.confirmed.begin();

            while(it_c_dt != c.second->dt.confirmed.end())
            {
                *it_w_dt += *it_c_dt;
                it_c_dt++;
                it_w_dt++;
            }
            break;
        }
        case data::field_recovered:
        {
            if(wrld->dt.recovered.size() == 0)
                wrld->dt.recovered.resize(dates.size(), 0);
            auto it_c_dt = c.second->dt.recovered.begin();
            auto it_w_dt = wrld->dt.recovered.begin();

            while(it_c_dt != c.second->dt.recovered.end())
            {
                *it_w_dt += *it_c_dt;
                it_c_dt++;
                it_w_dt++;
            }
            break;
        }
        case data::field_deadth:
        {
            if(wrld->dt.deadths.size() == 0)
                wrld->dt.deadths.resize(dates.size(), 0);
            auto it_c_dt = c.second->dt.deadths.begin();
            auto it_w_dt = wrld->dt.deadths.begin();

            while(it_c_dt != c.second->dt.deadths.end())
            {
                *it_w_dt += *it_c_dt;
                it_c_dt++;
                it_w_dt++;
            }
            break;
        }
        }
    });
    fs.close();
}

void calculation::print_head(std::ofstream& fs)
{
    std::string s("latitude,longitude,name_type");
    fs.write(s.c_str(), static_cast<long>(s.size()));
    std::for_each(dates.begin(), dates.end(), [&](std::string s)
    {
        s = "," + s;
        fs.write(s.c_str(), static_cast<long>(s.size()));
    });
    fs.write("\n", 1);
}

void calculation::print(std::ofstream& fs, std::string& location, data::field fld)
{
    std::map<std::string, std::shared_ptr<place>>::iterator it_location = wrld->entry.find(location);

    std::shared_ptr<place> p;
    if(it_location != wrld->entry.end())
        p = it_location->second;
    else
        p = wrld;

    std::string  s(std::to_string(p->latitude) + "," + std::to_string(p->longitude) + "," + p->name + "_");
    fs.write(s.c_str(), static_cast<long>(s.size()));
    std::vector<long long> data;

    switch (fld)
    {
    case data::field_confirmed:
        data = p->dt.confirmed;
        fs.write("confirmed", 9);
        break;
    case data::field_recovered:
        data = p->dt.recovered;
        fs.write("recovered", 9);
        break;
    case data::field_deadth:
        data = p->dt.deadths;
        fs.write("deadths", 7);
        break;
    }

    std::for_each(data.begin(), data.end(), [&](long long l)
    {
        s = "," + std::to_string(l);
        fs.write(s.c_str(), static_cast<long>(s.size()));
    });

    fs.write("\n", 1);
}
