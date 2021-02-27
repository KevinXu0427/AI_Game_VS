#ifndef INCLUDED_AI_STEERINGMODULE_H
#define INCLUDED_AI_STEERINGMODULE_H

#include "SteeringBehavior.h"

namespace AI {

class SteeringModule
{
public:
	SteeringModule(Agent& agent);
	
	template <class BehaviorType>
	BehaviorType* AddBehavior(std::string name);

	template <class BehaviorType>
	BehaviorType* GetBehavior(std::string name);

	X::Math::Vector2 Calculate();

private:
	using BehaviorMap = std::unordered_map<std::string, std::unique_ptr<SteeringBehavior>>;

	Agent& mAgent;
	BehaviorMap mBehaviors;
};

template <class BehaviorType>
BehaviorType* SteeringModule::AddBehavior(std::string name)
{
	// [iterater, bool] structure bonding.
	auto [iter, success] = mBehaviors.try_emplace(std::move(name), std::make_unique<BehaviorType>());
	return success ? static_cast<BehaviorType*>(iter->second.get()) : nullptr;
}

template <class BehaviorType>
BehaviorType* SteeringModule::GetBehavior(std::string name)
{
	auto iter = mBehaviors.find(name);
	return iter != mBehaviors.end() ? iter->second.get() : nullptr;
}

}
#endif // !INCLUDED_AI_STEERINGMODULE_H