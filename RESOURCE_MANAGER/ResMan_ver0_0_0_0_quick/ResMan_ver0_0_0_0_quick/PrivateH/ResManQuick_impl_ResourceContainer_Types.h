#pragma once

#include <memory>
#include <list>

/********************************************************************************************************
* This files includes typedefs of the ResourceContainer class;
* This file is needed to avoid the circular dependency:
* - ManagedResource header uses ResourceContainer_Iterator;
* - ResourceContainer uses ManagedResource (because of std::shared_ptr).
*********************************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		// Fwd declarations of ResManQuick namespace
		class ManagedResource;

		namespace Impl
		{
			using ResourceContainer_Cont     = std::list<std::shared_ptr<ManagedResource>>;
			using ResourceContainer_Iterator = ResourceContainer_Cont::iterator;
		}
	}
}