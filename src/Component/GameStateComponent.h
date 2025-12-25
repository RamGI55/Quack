//
// Created by Hyunsoo Park on 12/25/25.
//

#ifndef GAMESTATECOMPONENT_H
#define GAMESTATECOMPONENT_H
#include <cstdint>

enum class GameState {
    PreGame,
    InGame,
    Paused,
    GameOver
};

enum class Difficulties {
    Easy,
    Medium,
    Hard,
    Hardcore
};

struct GameStateComponent {

    Difficulties difficulty = Difficulties::Medium;
    GameState state         = GameState::Paused;
    float CurrentTime       = 0.0f;
    uint32_t CurrentLevel   = 0;
    uint32_t FilledDucks    = 0;

};
#endif //GAMESTATECOMPONENT_H
