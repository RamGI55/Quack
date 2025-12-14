#include <iostream>
#include <SFML/Graphics.hpp>

#include "Component/InputComponent.h"
#include "Component/SpritesComponent.h"
#include "Component/TransformComponent.h"
#include "Core/Coordinator.h"

Coordinator gCoordinator;
int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "CMake SFML Project");
    window.setFramerateLimit(144);

    gCoordinator.Init();
    gCoordinator.RegisterComponent<SpriteComponent>();
    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}
