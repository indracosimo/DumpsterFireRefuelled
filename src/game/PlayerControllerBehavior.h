#pragma once
#include "GameBehavior.h"
#include <cstddef>

class PlayerControllerBehavior final : public GameBehavior
{
public:
    explicit PlayerControllerBehavior(size_t cubeIndex);

    void Update(GameBehaviorContext& context, float deltaTime) override;

    float moveSpeed = 7.0f;
    float jumpSpeed = 7.5f;
    float groundEpsilon = 0.05f;
    float coyoteTime = 0.2f;

private:
    size_t m_cubeIndex;
    bool bm_prevJumpDown = false;
    bool bTimeSinceGrounded = 999.0f;
    bool bIsGrounded(const GameBehaviorContext& context, float deltaTime);
};
