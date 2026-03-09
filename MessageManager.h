#ifndef MESSAGEMANAGER_H
#define MESSAGEMANAGER_H

#include <string>
#include <iostream>
#include <glm/glm.hpp>
#include <graphics/renderer.h>

enum class MessageType 
{
	RAYCAST,
	STRING,
	FLOAT,
	MOUSE,
	SPAWNED_CUBES
	//to do
	//error message
	//request message
	//response message
};

class MessageManager
{
public:
	MessageManager(MessageType type, const std::string& message = "");
	virtual ~MessageManager();

	MessageType getType() const;
	std::string getMessage() const;
	virtual void processMessage() const = 0;

protected:
	MessageType type;
	std::string message;
};

//Raycast Message Class
class RaycastMessage : public MessageManager
{
public:
	RaycastMessage(const glm::vec3& startPos = glm::vec3(0.0f),
		const glm::vec3& direction = glm::vec3(0.0f, 0.0f, -1.0f),
		float maxDistance = 100.0f);

	void processMessage() const override;
	glm::vec3 getStartPos() const;
	glm::vec3 getDirection() const;
	float getMaxDistance() const;

private:
	glm::vec3 startPos;
	glm::vec3 direction;
	float maxDistance;
};

//String Message Class
class StringMessage : public MessageManager
{
public:
	StringMessage(const std::string& message);
	void processMessage() const override;
};

//Float Message Class
class FloatMessage : public MessageManager
{
public:
	FloatMessage(float value);
	void processMessage() const override;
	float getValue() const;

private:
	float value;
};

//Mouse Message Class
class MouseMessage : public MessageManager
{
public:
	MouseMessage(int x, int y, bool clicked);
	void processMessage() const override;

private:
	int x, y;
	bool bClicked;
};

class CubeSpawnedMessage : public MessageManager
{
public:
	CubeSpawnedMessage(const CubeTransform& cube);

	void processMessage() const override;
	CubeTransform getCubeData() const;

private:
	CubeTransform cubeData;
};

#endif // MESSAGEMANAGER_H