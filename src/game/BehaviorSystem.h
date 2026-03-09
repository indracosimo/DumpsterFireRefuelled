#pragma once
#include <memory>
#include <vector>

class GameBehavior;
struct GameBehaviorContext;

class BehaviorSystem
{
public:
    void Clear(GameBehaviorContext& context);

    template <class T, class... Args>
    T* Add(Args&&... args)
    {
        auto pointer = std::make_unique<T>(std::forward<Args>(args)...);
        T* raw = pointer.get();
        behaviors.push_back(std::move(pointer));
        return raw;
    }

    void EnterPlay(GameBehaviorContext& context);
    void Update(GameBehaviorContext& context, float deltaTime);
    void ExitPlay(GameBehaviorContext& context);

private:
    std::vector<std::unique_ptr<GameBehavior>> behaviors;
        
};
