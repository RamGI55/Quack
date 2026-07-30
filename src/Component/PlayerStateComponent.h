//
// Created by Hyunsoo Park on 12/25/25.
//

#ifndef PLAYESTATECOMPONENT_H
#define PLAYESTATECOMPONENT_H
#include <string>

struct PlayerStateComponent
{
    int PlayerScore         = 0;
    int LivesRemaining      = 0;
    float PlayTime          = 0.0f;
    std::string PlayerName;
};
#endif //PLAYESTATECOMPONENT_H
