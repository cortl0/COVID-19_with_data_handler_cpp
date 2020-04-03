#include "data.h"

data::data()
{

}

void data::set_data(std::list<std::string> v_data, field fld)
{
    unsigned int count = 0;
    switch (fld)
    {
    case data::field_confirmed:
        confirmed.resize(v_data.size());
        std::for_each(v_data.begin(), v_data.end(), [&](std::string s)
        {
            confirmed[count++] = std::stoll(s);
        });
        break;
    case data::field_recovered:
        recovered.resize(v_data.size());
        std::for_each(v_data.begin(), v_data.end(), [&](std::string s)
        {
            recovered[count++] = std::stoll(s);
        });
        break;
    case data::field_deadth:
        deadths.resize(v_data.size());
        std::for_each(v_data.begin(), v_data.end(), [&](std::string s)
        {
            deadths[count++] = std::stoll(s);
        });
        break;
    }
}
