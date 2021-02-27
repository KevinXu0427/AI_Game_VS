#ifndef INCLUDED_AI_AGENT_H
#define INCLUDED_AI_AGENT_H

#include "Common.h"

namespace AI {

class AIWorld;

class Agent
{
public:
	AIWorld& world;
	Agent(AIWorld& world);
	~Agent();

	X::Math::Matrix3 LocalToWorld() const;

public:
	X::Math::Vector2 position;
	X::Math::Vector2 velocity;
	X::Math::Vector2 destination;
	X::Math::Vector2 heading;
	float radius = 1.0f;
	float maxSpeed = 1.0f;
	float mass = 1.0f;

};

} // namespace AI

#endif // !INCLUDED_AI_AGENT_H