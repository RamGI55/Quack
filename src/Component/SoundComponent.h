//
// Created by PC on 14-Dec-25.
//

#ifndef DUCKDUCKROAD_SOUNDCOMPONENT_H
#define DUCKDUCKROAD_SOUNDCOMPONENT_H
#include <optional>
#include <string>

#include "SFML/Audio/Sound.hpp"

struct SoundComponent
{
    std::optional<sf::Sound> Sound;
    std::shared_ptr<sf::SoundBuffer> SoundBuffer;
    std::shared_ptr<sf::SoundBuffer> SecondSoundBuffer;

};
#endif //DUCKDUCKROAD_SOUNDCOMPONENT_H