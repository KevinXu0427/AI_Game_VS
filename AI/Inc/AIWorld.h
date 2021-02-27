#ifndef INCLUDED_AIWORLD_H
#define INCLUDED_AIWORLD_H

#include "Agent.h"

namespace AI {

class AIWorld
{
public:
	using Agents = std::vector<Agent*>;
	using Obstacles = std::vector<X::Math::Circle>;
	using Walls = std::vector<X::Math::LineSegment>;

	Agents GetNeighbourhood(const X::Math::Circle& range);

	void AddObstacles(const X::Math::Circle& obstacle);
	const Obstacles& GetObstacles() const { return mObstacles; }

	void AddWall(const X::Math::LineSegment& wall);
	const Walls& GetWalls() const { return mWalls; }

	void RegisterAgent(Agent* agent);
	void UnregisterAgent(Agent* agent);

private:
	Obstacles mObstacles;
	Walls mWalls;
	Agents mAgents;

};


}

#endif // !INCLUDED_AIWORLD_H