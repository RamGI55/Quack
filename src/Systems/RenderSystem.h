//
// Created by PC on 15-Dec-25.
//

#ifndef DUCKDUCKROAD_RENDERSYSTEM_H
#define DUCKDUCKROAD_RENDERSYSTEM_H
#include "../Core/System.h"
#include "SFML/Graphics/Texture.hpp"

class Coordinator;

class RenderSystem : public System
{
public:
    void init(std::shared_ptr<sf::Texture>, Coordinator& coordinator);
    void update(float dt, Coordinator& coordinator);

private:

};


#endif //DUCKDUCKROAD_RENDERSYSTEM_H