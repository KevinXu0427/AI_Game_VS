#ifndef INCLUDED_AI_SEEDBEHAVIOR_H
#define INCLUDED_AI_SEEDBEHAVIOR_H

#include "SteeringBehavior.h"
#include "Agent.h"

namespace AI {

class SeekBehaviour : public AI::SteeringBehavior
{
public:
	X::Math::Vector2 Calculate(AI::Agent& agent) override
	{
		auto desiredVelocity = X::Math::Normalize(agent.destination - agent.position) * agent.maxSpeed;
		return desiredVelocity - agent.velocity;
	}
};

}



#endif // !INCLUDED_AI_SEEDBEHAVIOR_H