//
// Created by PC on 13-Dec-25.
//
#pragma once

#include <bitset>
#include <cstdint>


using Entity = std::uint32_t;
constexpr Entity MAX_ENTITIES = 10000;
using ComponentType = std::uint8_t;
constexpr ComponentType MAX_COMPONENTS = 32;
using Signature = std::bitset<MAX_COMPONENTS>;