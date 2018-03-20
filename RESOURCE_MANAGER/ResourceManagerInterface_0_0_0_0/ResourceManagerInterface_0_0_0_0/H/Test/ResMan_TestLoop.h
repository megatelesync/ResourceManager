#pragma once

#include "../Subsystem/MASTER_ResourceManager_Subsystem.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			// Call the tick specific number of times
			void TestLoop(size_t InMaxIterations, IResourceManagerSubsystem *pInSubsystem, int Seconds_SleepPerTick = 0, double InSimulationTimeElapsedPerTick = 0.0);
		}
	}
}