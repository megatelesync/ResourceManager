#pragma once

/**********************************************************************************************************************
* This header files declares utility functions to work with command queues.
* This utils can be used both on the loader thread as on the main thread.
**********************************************************************************************************************/
#include "ResManQuick_impl_LoadingLoopCommandQueues.h"
#include "Core/ResourceManagerConstants.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			// Urgency to be used for urgent special commands
			// (should never be used initially in resource loading requests)
			const ResMan::Urgency Urgency_UrgencySpecial = 9E6;

			/**********************************************************************
			* Actual priorities
			**********************************************************************/
			// Compute priority to be used for the command actually
			size_t ComputeActualCommandPriority(ResMan::Urgency InUrgency, ResMan::ResourcePriority InResourcePriority);

			// Maximal possible actual priority
			const size_t ActualPriority_Maximal = ComputeActualCommandPriority(Urgency_UrgencySpecial, ResMan::ResourcePriority_Maximal);

			const size_t ActualPriority_Unlock = ActualPriority_Maximal;

			/******************************************************************************************************
			* implementation
			******************************************************************************************************/
			inline size_t ComputeActualCommandPriority(ResMan::Urgency InUrgency, ResMan::ResourcePriority InResourcePriority)
			{
				return InUrgency + InResourcePriority;
			}
		}
	}
}