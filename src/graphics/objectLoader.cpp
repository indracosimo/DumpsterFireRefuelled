#include "objectLoader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <glm/glm.hpp>

Mesh objectLoader::loadOBJ(const std::string& filepath)
{
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;

    std::vector<float> finalVertices;

    std::ifstream file(filepath);
    if (!file.is_open())
    {
        std::cerr << "Failed to open OBJ: " << filepath << std::endl;
        return Mesh({});
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string type;
        ss >> type;

        if (type == "v")
        {
            glm::vec3 v;
            ss >> v.x >> v.y >> v.z;
            positions.push_back(v);
        }
        else if (type == "vt")
        {
            glm::vec2 t;
            ss >> t.x >> t.y;
            texcoords.push_back(t);
        }
        else if (type == "vn")
        {
            glm::vec3 n;
            ss >> n.x >> n.y >> n.z;
            normals.push_back(glm::normalize(n));
        }
        else if (type == "f")
        {
            int p[3], t[3], n[3];
            for (int i = 0; i < 3; ++i)
            {
                std::string vert;
                ss >> vert;

                sscanf_s(vert.c_str(), "%d/%d/%d", &p[i], &t[i], &n[i]);

                glm::vec3 pos = positions[p[i] - 1];
                glm::vec2 uv = t[i] > 0 ? texcoords[t[i] - 1] : glm::vec2(0.0f);
                glm::vec3 norm = n[i] > 0 ? normals[n[i] - 1] : glm::vec3(0.0f, 1.0f, 0.0f);

                finalVertices.push_back(pos.x);
                finalVertices.push_back(pos.y);
                finalVertices.push_back(pos.z);

                finalVertices.push_back(uv.x);
                finalVertices.push_back(uv.y);

                finalVertices.push_back(norm.x);
                finalVertices.push_back(norm.y);
                finalVertices.push_back(norm.z);
            }
        }
    }

    return Mesh(finalVertices);
}

// Mesh objectLoader::loadOBJ(const std::string& filepath)
// {
//     std::vector<glm::vec3> positions;
//     std::vector<glm::vec2> texcoords;
//     std::vector<glm::vec3> normals;
//
//     std::vector<float> finalVertices;
//
//     std::ifstream file(filepath);
//     if (!file.is_open())
//     {
//         std::cerr << "Failed to open OBJ: " << filepath << std::endl;
//         return Mesh({});
//     }
//
//     std::string line;
//
//     while (std::getline(file, line))
//     {
//         std::stringstream ss(line);
//         std::string type;
//         ss >> type;
//
//         if (type == "v")
//         {
//             glm::vec3 v;
//             ss >> v.x >> v.y >> v.z;
//             positions.push_back(v);
//         }
//         else if (type == "vt")
//         {
//             glm::vec2 t;
//             ss >> t.x >> t.y;
//             texcoords.push_back(t);
//         }
//         else if (type == "vn")
//         {
//             glm::vec3 n;
//             ss >> n.x >> n.y >> n.z;
//             normals.push_back(glm::normalize(n));
//         }
//         else if (type == "f")
//         {
//             int p[3], t[3], n[3];
//
//             for (int i = 0; i < 3; i++)
//             {
//                 std::string vert;
//                 ss >> vert;
//
//                 p[i] = t[i] = n[i] = 0;
//
//                 if (vert.find("//") != std::string::npos)
//                     sscanf_s(vert.c_str(), "%d//%d", &p[i], &n[i]);
//                 else if (vert.find('/') != std::string::npos)
//                     sscanf_s(vert.c_str(), "%d/%d/%d", &p[i], &t[i], &n[i]);
//                 else
//                     p[i] = std::stoi(vert);
//             }
//
//             for (int i = 0; i < 3; i++)
//             {
//                 glm::vec3 pos = positions[p[i] - 1];
//                 glm::vec2 uv  = (t[i] > 0) ? texcoords[t[i] - 1] : glm::vec2(0.0f);
//                 glm::vec3 norm = (n[i] > 0) ? normals[n[i] - 1] : glm::vec3(0.0f, 1.0f, 0.0f);
//
//                 finalVertices.push_back(pos.x);
//                 finalVertices.push_back(pos.y);
//                 finalVertices.push_back(pos.z);
//
//                 finalVertices.push_back(uv.x);
//                 finalVertices.push_back(uv.y);
//
//                 finalVertices.push_back(norm.x);
//                 finalVertices.push_back(norm.y);
//                 finalVertices.push_back(norm.z);
//             }
//         }
//     }
//
//     return Mesh(finalVertices);
// }