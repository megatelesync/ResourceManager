#include "../../H/Interface/IManagedResource.h"
#include "../../H/Override/IResourceLoader.h"
#include "../../H/Interface/IManagedGroup.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			/******************************************************************************************
			* IManagedResource utils implementation
			******************************************************************************************/
			const LoadableMainSubresourceParams *GetMainResourceParameters(IManagedResource *pInResource)
			{
				assert(pInResource != nullptr);
				return pInResource->GetMainSubresourceParams();
			}

			size_t GetActiveGroupsCount(IManagedResource *pInResource)
			{
				auto pEnumerator = std::move(pInResource->GetActiveGroups());
				return pEnumerator->GetSize();
			}
			size_t IsNotAttachedToAnyActiveGroup(IManagedResource *pInResource)
			{
				return 0 == GetActiveGroupsCount(pInResource);
			}
			bool IsAttachedToActiveGroup(IManagedResource *pInResource, IManagedGroup *pInGroup)
			{
				auto pEnumerator = std::move(pInResource->GetActiveGroups());
				while (true)
				{
					auto pCurrentGroup = pEnumerator->GetCurrent();
					if (nullptr == pCurrentGroup)
					{
						break;
					}
					if (pCurrentGroup == pInGroup)
					{
						return true;
					}
					pEnumerator->MoveNext();
				}
				return false;
			}

			void UnloadAllSubresources(IManagedResource *pInResource, Urgency InUrgency)
			{
				assert(pInResource != nullptr);
				UnloadCommand command;
				command.args.subresources = GetApplicableSubresourceRoles(pInResource);
				command.args.urgency = InUrgency;
				pInResource->UnloadSubresources(command);
			}

			void PrecacheAll_DependencyLists_And_Subresources_Synchro(IManagedResource *pInResource, Urgency InUrgency)
			{
				PrecachedAllDependencyLists(pInResource, InUrgency);
				PrecacheAllSubresources(pInResource, InUrgency);
			}

			void PrecacheAll_DependencyLists_And_Subresources(IManagedResource *pInResource, Urgency InUrgency)
			{
				// We do NOT use synchro loading of dependency lists,
				// however, next command will wait until all dependency lists are loaded;
				PrecachedAllDependencyLists(pInResource, InUrgency);
				PrecacheAllSubresourcesSynchro(pInResource, InUrgency);
			}

			void PrecacheAllSubresources(IManagedResource *pInResource, Urgency InUrgency)
			{
				assert(pInResource != nullptr);
				LoadCommand command;
				command.args.subresources = GetApplicableSubresourceRoles(pInResource);
				command.args.urgency = InUrgency;
				pInResource->PrecacheSubresources(command);
			}

			void PrecacheAllSubresourcesSynchro(IManagedResource *pInResource, Urgency InUrgency)
			{
				assert(pInResource != nullptr);
				LoadCommand command;
				command.args.subresources = GetApplicableSubresourceRoles(pInResource);
				command.args.urgency = InUrgency;
				pInResource->PrecacheSubresourcesSyncho(command);
			}


			void PrecachedAllDependencyLists(IManagedResource *pInResource, Urgency InUrgency)
			{
				auto applicableRoles = GetApplicableSubresourceRoles(pInResource);
				PrecachedDependencyLists(pInResource, applicableRoles, InUrgency);
			}

			void PrecacheSubresources(IManagedResource *pInResource, SubresourceRoleSet InSubresourcesToLoad, Urgency InUrgency)
			{
				assert(pInResource != nullptr);
				LoadCommand command;
				command.args.subresources = InSubresourcesToLoad;
				command.args.urgency = InUrgency;
				pInResource->PrecacheSubresources(command);
			}
			
			void PrecacheSubresourcesSynchro(IManagedResource *pInResource, SubresourceRoleSet InSubresourcesToLoad, Urgency InUrgency)
			{
				assert(pInResource != nullptr);
				LoadCommand command;
				command.args.subresources = InSubresourcesToLoad;
				command.args.urgency = InUrgency;
				pInResource->PrecacheSubresourcesSyncho(command);
			}

			void PrecachedDependencyLists(IManagedResource *pInResource, SubresourceRoleSet InDependencyListsToLoad, Urgency InUrgency)
			{
				assert(pInResource != nullptr);
				PrecacheDependenciesCommand command;
				command.args.subresources = InDependencyListsToLoad;
				command.args.urgency = InUrgency;
				pInResource->PrecacheDependencyList(command);
			}

			SubresourceRoleSet GetLoadingDependencyLists(IManagedResource *pInResource)
			{
				auto roles = pInResource->GetDependencyList().GetLoadingSubresourceRoles();
				return roles;
			}

			bool AreDependencyListsLoading(IManagedResource *pInResource)
			{
				return false == GetLoadingDependencyLists(pInResource).IsEmpty();
			}

			SubresourceRoleSet GetDependencyLists_InLoadingQueue(IManagedResource *pInResource)
			{
				auto roles = pInResource->GetLoadingState().GetRecursiveLoadingDependencyListsRoles();
				return roles;
			}

			bool AreDependencyLists_InLoadingQueue(IManagedResource *pInResource)
			{
				return false == GetDependencyLists_InLoadingQueue(pInResource).IsEmpty();
			}

			SubresourceRoleSet GetSubresources_InLoadingQueue(IManagedResource *pInResource)
			{
				auto roles = pInResource->GetLoadingState().GetRecursiveLoadingSubresourceRoles();
				return roles;
			}

			bool AreSubresources_InLoadingQueue(IManagedResource *pInResource)
			{
				return false == GetSubresources_InLoadingQueue(pInResource).IsEmpty();
			}

			SubresourceRoleSet GetLoaded_DependencyLists(IManagedResource *pInResource)
			{
				auto roles = pInResource->GetDependencyList().GetLoadedSubresourcesDependencies();
				return roles;
			}

			bool AreDependencyLists_Loaded(IManagedResource *pInResource)
			{
				return false == GetLoaded_DependencyLists(pInResource).IsEmpty();
			}

			SubresourceRoleSet GetLoaded_Subresources(IManagedResource *pInResource)
			{
				auto roles = pInResource->GetLoadedSubresources();
				return roles;
			}

			bool AreSubresources_Loaded(IManagedResource *pInResource)
			{
				return false == GetLoaded_Subresources(pInResource).IsEmpty();
			}

			SubresourceRoleSet GetLoadingFailed_Subresources(IManagedResource *pInResource)
			{
				return pInResource->GetLoadingFailedSubresources();
			}
			bool AreSubresources_Failed_Loading(IManagedResource *pInResource)
			{
				return false == GetLoadingFailed_Subresources(pInResource).IsEmpty();
			}
			SubresourceRoleSet GetLoadingFailed_DependencyLists(IManagedResource *pInResource)
			{
				return pInResource->GetLoadingFailedDependencyLists();
			}
			bool AreDependencyLists_Failed_Loading(IManagedResource *pInResource)
			{
				return false == GetLoadingFailed_DependencyLists(pInResource).IsEmpty();
			}

			IResourceType *GetResourceType(IManagedResource *pInResource)
			{
				const LoadableMainSubresourceParams *pMainResourceParams = GetMainResourceParameters(pInResource);
				if (nullptr == pMainResourceParams)
				{
					return pInResource->GetAddressableSubresource()->GetResourceType();
				}
				return pMainResourceParams->GetResourceType();
			}
		}
	}
}