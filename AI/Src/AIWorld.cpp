#include "Precompiled.h"
#include "AIWorld.h"

using namespace AI;

void AIWorld::AddObstacles(const X::Math::Circle& obstacle)
{
	mObstacles.push_back(obstacle);
}

void AIWorld::AddWall(const X::Math::LineSegment& wall)
{
	mWalls.push_back(wall);
}

void AIWorld::RegisterAgent(Agent* agent)
{
	mAgents.push_back(agent);
}

void AIWorld::UnregisterAgent(Agent* agent)
{
	auto iter = std::find(mAgents.begin(), mAgents.end(), agent);
	if (iter != mAgents.end())
	{
		std::iter_swap(iter, mAgents.end() - 1);
		mAgents.pop_back();
	}
}