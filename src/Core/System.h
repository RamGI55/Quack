//
// Created by PC on 13-Dec-25.
//

#ifndef DUCKDUCKROAD_SYSTEM_H
#define DUCKDUCKROAD_SYSTEM_H
#include <set>
#include "Types.h"


class System
{
public:
    std::set<Entity> Entities; // why std::set is faster?
    virtual ~System() = default;
};


#endif //DUCKDUCKROAD_SYSTEM_H