#ifndef INCLUDED_AI_STEERINGBEHAVIOR_H
#define INCLUDED_AI_STEERINGBEHAVIOR_H

#include "Common.h"

namespace AI {

class Agent;

class SteeringBehavior
{
public:
	SteeringBehavior() = default;
	virtual ~SteeringBehavior() = default;

	virtual X::Math::Vector2 Calculate(Agent& agent) = 0;

	void SetActive(bool active) { mActive = active; }
	bool IsActive() const { return mActive; }

private:
	bool mActive = false;
};

} // namespace AI

#endif // !INCLUDED_AI_STEERINGBEHAVIOUR_H