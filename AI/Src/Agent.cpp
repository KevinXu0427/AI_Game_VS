#include "Precompiled.h"
#include "Agent.h"
#include "AIWorld.h"

using namespace AI;

AI::Agent::Agent(AIWorld& world) :
	world(world)
{
	world.UnregisterAgent(this);
}

AI::Agent::~Agent()
{
	world.UnregisterAgent(this);
}

X::Math::Matrix3 Agent::LocalToWorld() const
{
	X::Math::Vector2 y = heading;
	X::Math::Vector2 x = { heading.y, -heading.x };
	X::Math::Vector2 p = position;

	return{
		x.x, x.y, 0.0f,
		y.x, y.y, 0.0f,
		p.x, p.y, 1.0f
	};
}