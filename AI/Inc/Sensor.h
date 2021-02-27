#ifndef INCLUDED_AI_SENSOR_H
#define INCLUDED_AI_SENSOR_H

//#include "MemoryRecord.h"
class MemoryRecord;

namespace AI {

class Agent;

class Sensor
{
public:
	virtual ~Sensor() = default;

	virtual void Update(Agent& agent, MemoryRecords& memory, float deltalTime) = 0;

};


}// Namespace AI



#endif // !INCLUDED_AI_SENSOR_H