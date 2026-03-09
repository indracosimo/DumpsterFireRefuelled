#include "MessageManager.h"

MessageManager::MessageManager(MessageType type, const std::string& message) : type(type), message(message) {}

MessageManager::~MessageManager() {}

MessageType MessageManager::getType() const
{
    return type;
}

std::string MessageManager::getMessage() const
{
    return message;
}

//RaycastMessage 
RaycastMessage::RaycastMessage(const glm::vec3& startPos, const glm::vec3& direction, float maxDistance)
    : MessageManager(MessageType::RAYCAST), startPos(startPos), direction(direction), maxDistance(maxDistance) {}


void RaycastMessage::processMessage() const
{
    std::cout << "Raycast Message: Start Position ("
        << startPos.x << ", " << startPos.y << ", " << startPos.z
        << "), Direction (" << direction.x << ", " << direction.y << ", " << direction.z
        << "), Max Distance: " << maxDistance << std::endl;
}

glm::vec3 RaycastMessage::getStartPos() const
{
    return startPos;
}

glm::vec3 RaycastMessage::getDirection() const
{
    return direction;
}

float RaycastMessage::getMaxDistance() const
{
    return maxDistance;
}

//StringMessage 
StringMessage::StringMessage(const std::string& message) : MessageManager(MessageType::STRING, message) {}

void StringMessage::processMessage() const
{
    std::cout << "String Message: " << message << std::endl;
}

//FloatMessage 
FloatMessage::FloatMessage(float value) : MessageManager(MessageType::FLOAT, std::to_string(value)), value(value) {}

void FloatMessage::processMessage() const
{
    std::cout << "Float Message: " << message << std::endl;
}

float FloatMessage::getValue() const
{
    return value;
}

//MouseMessage 
MouseMessage::MouseMessage(int x, int y, bool clicked) : MessageManager(MessageType::MOUSE), x(x), y(y), bClicked(clicked) {}

void MouseMessage::processMessage() const
{
    std::cout << "Mouse message: Position (" << x << ", " << y << "), Clicked: " << (bClicked ? "Yes" : "No") << std::endl;
}

//CubeSoawnedMessage
CubeSpawnedMessage::CubeSpawnedMessage(const CubeTransform& cube) : MessageManager(MessageType::SPAWNED_CUBES), cubeData(cube) {}

void CubeSpawnedMessage::processMessage() const
{
	std::cout << "Cube Spawned: "
	<< "Name: " << cubeData.name << ", "
	<< "Position: (" << cubeData.position.x << ", "
	<< cubeData.position.y << ", " << cubeData.position.z << ")"
    << "Rotation: (" << cubeData.rotation.x << ", "
	<< cubeData.rotation.y << ", " << cubeData.rotation.z << ")"
	<< "Scale: (" << cubeData.scale.x << ", " << cubeData.scale.y << ", " << cubeData.scale.z << ")"
	<< std::endl;
}

CubeTransform CubeSpawnedMessage::getCubeData() const
{
    return CubeTransform();
}
