#ifndef INCLUDED_WANDERBEHAVIOUR_H
#define INCLUDED_WANDERBEHAVIOUR_H

#include "Agent.h"
#include "SeekBehavior.h"

namespace AI {

class WanderBehaviour : public SeekBehaviour
{
public:
	X::Math::Vector2 Calculate(AI::Agent& agent);
	void SetParam(float jitter, float radius, float distance);

private:
	X::Math::Vector2 mWanderTargetLocal;
	float mWanderJitter;
	float mWanderRadius;
	float mWanderDistance;
};

} // namespace AI


#endif // !INCLUDED_WANDERBEHAVIOUR_H