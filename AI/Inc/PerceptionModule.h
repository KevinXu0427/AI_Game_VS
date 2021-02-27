#ifndef INCLUDED_AI_PERCEPTIONMODULE_H
#define INCLUDED_AI_PERCEPTIONMODULE_H

#include "MemoryRecord.h"
#include "Sensor.h"

namespace AI {

class Agent;

using ImportanceCalculator = std::function<void(MemoryRecord&)>;

class PerceptionModule
{
public:
	PerceptionModule(Agent& agent, ImportanceCalculator Calculator);
	~PerceptionModule() {}

	template <class SensorType>
	SensorType* AddSensor(std::string name);

	template <class SensorType>
	SensorType* GetSensor(std::string name);

	void Update(float deltalTime);

	const MemoryRecords& GetMemoryRecords() const { return mMemory; }


private:
	using SensorMap = std::unordered_map<std::string, std::unique_ptr<Sensor>>;

	Agent& mAgent;
	SensorMap mSensors;
	ImportanceCalculator mCalculator;
	MemoryRecords mMemory;

	float mMemorySpan; // how long agent can remember things
};


template<class SensorType>
SensorType* PerceptionModule::AddSensor(std::string name)
{
	auto[iter, success] = mSensors.try_emplace(std::move(name), std::make_unique<SensorType>());
	return success ? static_cast<SensorType*>(iter->second.get()) : nullptr;
}

template<class SensorType>
SensorType* PerceptionModule::GetSensor(std::string name)
{
	auto iter = mSensors.find(name);
	return iter != mSensors.end() ? iter->second.get() : nullptr;
}

}


#endif // !INCLUDED_AI_PERCEPTIONMODULE_H