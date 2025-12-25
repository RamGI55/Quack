//
// Created by Hyunsoo Park on 12/25/25.
//

#ifndef GAMERULECOMPONENT_H
#define GAMERULECOMPONENT_H
#include <cstdint>



struct GameRuleComponent {

    float InitTime      = 0.0f;

};

struct FrogRuleComponent {

    uint32_t InitLives  = 3;
    float TotalPlayTime = 300.f;
    float BonusPlayTime = 30.f;
    uint32_t DuckSlots  = 5;

    // Point Related
    uint32_t InitPoints         = 0;
    uint32_t GoalEventPoints    = 1000;
    uint32_t BonusEventPoints   = 1000;
    uint32_t SuccessEventPoints = 500;



};

#endif //GAMERULECOMPONENT_H
