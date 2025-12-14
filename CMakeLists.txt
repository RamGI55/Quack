cmake_minimum_required(VERSION 3.28)
project(DUCKDUCKROAD LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.2
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(main src/main.cpp
        src/Windows/GameScreen.cpp
        src/Windows/GameScreen.h
        src/Game.cpp
        src/Game.h
        src/Core/EntityManager.cpp
        src/Core/EntityManager.h
        src/Core/Types.h
        src/Core/Types.h
        src/Core/IComponentArray.h
        src/Core/EntityManager.h
        src/Core/ComponentManager.h
        src/Component/TransformComponent.h
        src/Component/InputComponent.h
        src/Core/System.h
        src/Core/SystemManager.h
        src/Core/Coordinator.h
        src/Component/SpritesComponent.h
        src/Component/SoundComponent.h
        src/Component/PlayerComponent.h
        src/Component/CollisionComponent.h
)
target_compile_features(main PRIVATE cxx_std_17)
target_link_libraries(main PRIVATE SFML::Graphics)
