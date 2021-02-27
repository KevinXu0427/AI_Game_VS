#include "Precompiled.h"
#include "SteeringModule.h"

using namespace AI;

SteeringModule::SteeringModule(Agent& agent) :
	mAgent(agent)
{}

X::Math::Vector2 SteeringModule::Calculate()
{
	X::Math::Vector2 total;
	for (auto& [name, behavior] : mBehaviors)
	{
		if (behavior->IsActive())
		{
			total += behavior->Calculate(mAgent);
		}
	}

	return total;
}