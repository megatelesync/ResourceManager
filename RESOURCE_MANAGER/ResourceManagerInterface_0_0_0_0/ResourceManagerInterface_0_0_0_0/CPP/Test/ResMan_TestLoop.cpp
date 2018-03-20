#include "../../H/Test/ResMan_TestLoop.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			void TestLoop(size_t InMaxIterations, IResourceManagerSubsystem *pInSubsystem, int Seconds_SleepPerTick, double InSimulationTimeElapsedPerTick)
			{
				for (size_t i = 0; i < InMaxIterations; i++)
				{
					if (Seconds_SleepPerTick > 0)
					{
						std::this_thread::sleep_for(std::chrono::seconds(Seconds_SleepPerTick));
					}
					pInSubsystem->Tick(InSimulationTimeElapsedPerTick);
				}
			}
		}
	}
}