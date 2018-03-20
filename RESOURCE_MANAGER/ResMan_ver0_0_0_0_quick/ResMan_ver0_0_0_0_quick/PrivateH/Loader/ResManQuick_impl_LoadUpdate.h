#pragma once

#include "../ResManQuick_impl_System.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			/***********************************************************************************
			* Update of the loaded subresources of the given resource
			************************************************************************************/
			struct LoadUpdate
			{
				ResMan::SubresourceRoleSet GetReallyLoaded                                       () const { return reallyLoaded; }
				ResMan::SubresourceRoleSet GetReallyLoadedDependencyLists                        () const { return reallyLoadedDependencyLists; }
				ResMan::SubresourceRoleSet GetReallyUnloaded                                     () const { return reallyUnloaded; }
				ResMan::SubresourceRoleSet GetUnloadCanceled                                     () const { return unloadCanceled; }
				ResMan::SubresourceRoleSet GetLoadedFailed                                       () const { return loadFailed; }
				ResMan::SubresourceRoleSet GetLoadedDependencyListsFailed                        () const { return loadDependencyListFailed; }

				ResMan::SubresourceRoleSet reallyLoaded;
				ResMan::SubresourceRoleSet reallyLoadedDependencyLists;
				ResMan::SubresourceRoleSet reallyUnloaded;
				ResMan::SubresourceRoleSet unloadCanceled;
				ResMan::SubresourceRoleSet loadFailed;
				ResMan::SubresourceRoleSet loadDependencyListFailed;
			};
		}
	}
}
