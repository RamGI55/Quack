#include <iostream>
#include <SFML/Graphics.hpp>

#include "Game.h"
#include "Component/InputComponent.h"
#include "Component/SpritesComponent.h"
#include "Component/TransformComponent.h"
#include "Core/Coordinator.h"

int main()
{
    Game game;
    game.Run();
    return 0;
}
