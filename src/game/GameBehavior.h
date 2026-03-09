#pragma once

struct GameBehaviorContext;

class GameBehavior
{
public:
    virtual ~GameBehavior() = default;
    virtual void EnterPlay(GameBehaviorContext& context){}
    virtual void Update(GameBehaviorContext& context, float deltaTime) = 0;
    virtual void ExitPlay(GameBehaviorContext& context){}

};