//
// Created by PC on 13-Dec-25.
//

#ifndef DUCKDUCKROAD_GAME_H
#define DUCKDUCKROAD_GAME_H
#include "SFML/Graphics/RenderWindow.hpp"


class Game
{
public:
    Game();
    void Run();


private:
    sf::RenderWindow Window_;
    uint32_t mWindowWidth;
    uint32_t mWindowHeight;
    uint32_t mMaxFramerate;

};


#endif //DUCKDUCKROAD_GAME_H