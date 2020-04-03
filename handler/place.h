#ifndef PLACE_H
#define PLACE_H

#include <map>
#include <memory>
#include <string>

#include "data.h"

class place
{
public:
    std::string name;
    std::map<std::string, std::shared_ptr<place>> entry;
    double latitude = 0;
    double longitude = 0;
    data dt;
    place(std::string name);
};

#endif // PLACE_H
