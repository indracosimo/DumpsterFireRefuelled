#include "level_IO.h"
#include "jsonAdapters.h"
#include <filesystem>
#include <fstream>

namespace dataFrame::serialize
{
    bool SaveLevelToFile(const LevelData& level, const std::string& path)
    {
        namespace filesys = std::filesystem;
        filesys::path filepath(path);
        filesys::create_directories(filepath.parent_path());
        nlohmann::json j = level;

        std::ofstream out(path, std::ios::binary);
        if (!out) return false;

        out << j.dump(2);
        return true;
    }

    bool LoadLevelFromFile(LevelData& outLevel, const std::string& path)
    {
        std::ifstream in(path, std::ios::binary);
        if (!in) return false;

        try
        {
            nlohmann::json j;
            in >> j;
            outLevel = j.get<LevelData>();
            return true;
        }
        catch (const nlohmann::json::exception&)
        {
            return false;
        }
    }
    std::string GenerateLevelFilename(const std::string& levelName)
    {
        namespace fs = std::filesystem;
    
        std::string dir = "assets/levels/";
        int counter = 0;
        std::string filename;
    
        do {
            std::stringstream ss;
            ss << dir << levelName << "_" << std::setfill('0') << std::setw(3) << counter << ".json";
            filename = ss.str();
            counter++;
        } while (fs::exists(filename));
    
        return filename;
    }
}