//
// Created by Hyunsoo Park on 12/26/25.
//

#ifndef QKCOLLISIONLAYER_H
#define QKCOLLISIONLAYER_H
#include "../../Core/CollisionLayers.h"


enum class QKCollisionType: uint32_t
{
    Default     = 0,
    Player      = 1 << 0,
    Enemy       = 1 << 1,
    Terrain     = 1 << 2,
    Blocked     = 1 << 3,
    Goal        = 1 << 4,
};
ENABLE_BITMASK_OPERATORS(QKCollisionType)

#endif //QKCOLLISIONLAYER_H
