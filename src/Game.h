    //
    // Created by PC on 13-Dec-25.
    //

    #ifndef DUCKDUCKROAD_GAME_H
    #define DUCKDUCKROAD_GAME_H
    #include "Core/Coordinator.h"
    #include "SFML/Graphics/RectangleShape.hpp"
    #include "SFML/Graphics/RenderWindow.hpp"
    #include "Systems/GridMovementSystem.h"


    class Game
    {
    public:
        Game();
        void Run();
        void Init();
        void ProcessEvents();
        void Update(float dt);
        void Render();

    private:

        unsigned int mWindowWidth = 1920u;
        unsigned int mWindowHeight = 1080u;
        uint32_t mMaxFramerate = 144;

        sf::RenderWindow mWindow;

        Coordinator mCoordinator;
        std::shared_ptr<GridMovementSystem> mGridMovementSystem;
        Entity mPlayer;
        sf::Clock mClock;

        // Temp - must be deleted.
        sf::RectangleShape mTempRectangle; // TEMP!

    };


    #endif //DUCKDUCKROAD_GAME_H