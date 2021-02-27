#include "Precompiled.h"
#include "WanderBehaviour.h"

X::Math::Vector2 AI::WanderBehaviour::Calculate(AI::Agent & agent)
{
	mWanderTargetLocal += X::RandomUnitCircle(true) * mWanderJitter;
	mWanderTargetLocal = X::Math::Normalize(mWanderTargetLocal) * mWanderRadius;

	auto wanderTarget = mWanderTargetLocal + X::Math::Vector2{ 0.0f, mWanderDistance };
	auto localToWorld = agent.LocalToWorld();
	agent.destination = X::Math::TransformCoord(wanderTarget, localToWorld);

	X::DrawScreenCircle(
		X::Math::TransformCoord(X::Math::Vector2{ 0.0f, mWanderDistance }, localToWorld),
		mWanderRadius,
		X::Colors::LightBlue);
	X::DrawScreenCircle(agent.destination, 5.0f, X::Colors::GreenYellow);
	X::DrawScreenLine(agent.position, agent.destination, X::Colors::Cyan);

	return SeekBehaviour::Calculate(agent);
}

void AI::WanderBehaviour::SetParam(float jitter, float radius, float distance)
{
	mWanderJitter = jitter;
	mWanderRadius = radius;
	mWanderDistance = distance;
}
