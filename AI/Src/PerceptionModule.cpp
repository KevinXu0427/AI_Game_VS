#include "Precompiled.h"
#include "PerceptionModule.h"

using namespace AI;

PerceptionModule::PerceptionModule(Agent& agent, ImportanceCalculator calculator)
	: mAgent(agent)
	, mCalculator(std::move(calculator))
{}

void AI::PerceptionModule::Update(float deltalTime)
{
	// Update sensors and refresh/add memory records
	for (auto&[name, sensor] : mSensors)
	{
		sensor->Update(mAgent, mMemory, deltalTime);
	}

	//Remove any records older than memory span
	for (auto iter = mMemory.begin(); iter != mMemory.end();)
	{
		if (iter->lastRecordedTime + mMemorySpan < X::GetTime())
		{
			iter = mMemory.erase(iter);
		}
		else
		{
			++iter;
		}
	}

	// calculator impratance on new records
	for (auto m : mMemory)
	{
		mCalculator(m);
	}

	// sort records by importance
	mMemory.sort([](MemoryRecord& a, MemoryRecord& b)
	{
		return a.importance > b.importance;
	});
}
