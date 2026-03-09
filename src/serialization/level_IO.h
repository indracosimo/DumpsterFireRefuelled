#pragma once
#include <string>
#include "LevelTypes.h"

namespace dataFrame::serialize
{
    bool SaveLevelToFile(const LevelData& level, const std::string& path);
    bool LoadLevelFromFile(LevelData& outLevel, const std::string& path);
    std::string GenerateLevelFilename(const std::string& levelName);
}