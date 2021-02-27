#ifndef INCLUDED_AI_MEMORYRECORD_H
#define INCLUDED_AI_MEMORYRECORD_H

#include <XEngine.h>

namespace AI {

using Property = std::variant<int, float, X::Math::Vector2>;

struct MemoryRecord
{
	int entityID = 0;

	std::unordered_map<std::string, Property> properties;
	float lastRecordedTime = 0.0f;
	float importance = 0.0f;
};

using MemoryRecords = std::list<MemoryRecord>;

}// namespace AI

#endif // !INCLUDED_AI_MEMORYRECORD_H