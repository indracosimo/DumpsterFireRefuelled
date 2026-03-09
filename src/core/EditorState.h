#pragma once
#include <vector>
#include "glm/vec3.hpp"

struct EditorState
{
    enum class State { PLAY, EDIT };

    State currentState = State::EDIT;
    bool bPaused = false;
    float playModeSpeed = 1.0f;

    std::vector<glm::vec3> originalPositions;
    std::vector<glm::vec3> originalRotations;
    std::vector<glm::vec3> originalScales;
};
