//
// Created by Hyunsoo Park on 12/26/25.
//

#ifndef COLLISIONLAYERS_H
#define COLLISIONLAYERS_H
#include <cstdint>

#define ENABLE_BITMASK_OPERATORS(EnumType) \
    inline EnumType operator|(EnumType a, EnumType b) { \
        return static_cast<EnumType> ( \
            static_cast<uint32_t>(a) | static_cast<uint32_t>(b)); \
        }\
    inline EnumType operator&(EnumType a, EnumType b) { \
        return static_cast<EnumType>(\
            static_cast<uint32_t>(a) & static_cast<uint32_t>(b)); \
        }\
    inline EnumType operator~(EnumType a) {\
        return static_cast<EnumType>(~static_cast<uint32_t>(a));\
        }\
    inline EnumType& operator|=(EnumType& a, EnumType b) {\
        a = a | b; return a;\
        }\
    inline EnumType& operator&=(EnumType& a, EnumType b) {\
        a = a & b; return a;\
        }\
    inline bool HasFlag(EnumType mask, EnumType flag){\
        return static_cast<uint32_t>(mask & flag) != 0;\
        }

#endif //COLLISIONLAYERS_H
