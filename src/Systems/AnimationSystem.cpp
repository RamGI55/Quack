//
// Created by PC on 16-Jul-26.
//

#include "AnimationSystem.h"

#include "../Component/AnimationComponent.h"
#include "../Component/SpritesComponent.h"
#include "../Core/Coordinator.h"

AnimationSystem::AnimationSystem(SystemManager* manager)
{
}

AnimationSystem::~AnimationSystem()
{
}

void AnimationSystem::Update(float dt, Coordinator& coordinator)
{
    for (auto& entity : Entities)
    {
        auto& anim   = coordinator.GetComponent<AnimationComponent>(entity);
        auto& sprite = coordinator.GetComponent<SpriteComponent>(entity);

        anim.Elapsed += dt;
        while (anim.Elapsed >= anim.FrameDuration)
        {
            anim.Elapsed -= anim.FrameDuration;
            anim.CurrentFrame = (anim.CurrentFrame + 1) % anim.FrameCount;
        }

        // RenderSystem applies this rect to the sprite - keep this system as the only writer of animation data.
        sprite.rect = sf::IntRect({anim.CurrentFrame * anim.FrameSize.x, anim.Row * anim.FrameSize.y},
            anim.FrameSize);
    }
}
