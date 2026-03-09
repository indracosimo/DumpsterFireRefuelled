#include "BehaviorSystem.h"
#include "GameBehavior.h"
#include "GameBehaviorContext.h"

void BehaviorSystem::Clear(GameBehaviorContext& context)
{
    ExitPlay(context);
    behaviors.clear();
}

void BehaviorSystem::EnterPlay(GameBehaviorContext& context)
{
    for (auto& behavior : behaviors) behavior->EnterPlay(context);
}

void BehaviorSystem::Update(GameBehaviorContext& context, float deltaTime)
{
    for (auto& behavior : behaviors)
    {
        behavior->Update(context, deltaTime);
    }
}

void BehaviorSystem::ExitPlay(GameBehaviorContext& context)
{
    for (auto& behavior : behaviors) behavior->ExitPlay(context);
}

