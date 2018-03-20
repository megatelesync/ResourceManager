#pragma once

#include "ResourceManagerTypes.h"

/****************************************************************************************
* This header file contains basic constants
****************************************************************************************/

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			/*********************************************************************************
			* ResourcePriority
			*********************************************************************************/			
			const ResourcePriority ResourcePriority_Maximal = 7E5;
			const ResourcePriority ResourcePriority_Normal = 4E5;
			const ResourcePriority ResourcePriority_Min = 2E5;
			const ResourcePriority ResourcePriority_Lowest = 0;

			/*********************************************************************************
			* Urgency
			*********************************************************************************/
			const Urgency Urgency_Urgent = 7E6;
			const Urgency Urgency_Normal = 4E6;
			const Urgency Urgency_Min = 2E6;
			const Urgency Urgency_Lowest = 0;
		}
	}
}