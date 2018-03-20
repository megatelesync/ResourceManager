#pragma once

#include "../H/ResManQuick_System.h"
#include "../PrivateH/ResManQuick_impl_ResourceContainer.h"
#include "../PrivateH/ResManQuick_impl_ResourceGroupCombiCont.h"
#include "ResManQuick_impl_ResourceRepository.h"

#include <queue>
#include <memory>

/********************************************************************************************
* TODO:
*
* 1. Implement ResourceContainer (DONE: 01.0.10`7/15:13)
* 2. Implement ResourceGroupCombiCont
*********************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			class Impl_ResourceManagerSubsystem;

			class Impl_ResourceManager
			{
			public:
				~Impl_ResourceManager                                  ();
				Impl_ResourceManager                                   ();

				/*******************************************************************************************
				* Get interface
				*******************************************************************************************/
				ResMan::IResourceManager                               *GetInterface              () const { return _pOwnerManager; }

				/*******************************************************************************************
				* IResourceManager interface
				*******************************************************************************************/
				void                                                    Initialize                (Impl_ResourceManagerSubsystem *pInOwnerSubsystem, ResMan::IResourceManager *pInOwnerManager);
				void                                                    Tick                      ();

				/*******************************************************************************************
				* Enumerating resources
				*******************************************************************************************/
				ResMan::IResourceManager::ResourceEnumerator           GetResourceEnumerator      ();
				ResMan::IResourceManager::ResourceEnumerator           GetResourceWithURL         ();

				/*******************************************************************************************
				* Precaching
				*******************************************************************************************/
				void                                                    PrecacheGroup                          (Impl_ManagedGroup *pInGroup, bool bIsManual, ResMan::Urgency InUrgency);
				void                                                    PrecacheGroupDependencyLists           (Impl_ManagedGroup *pInGroup, bool bIsManual, ResMan::Urgency InUrgency);
				void                                                    PrecacheGroupSynchro                   (Impl_ManagedGroup *pInGroup, bool bIsManual);

				/**********************************************************************************
				* Waiting for events
				**********************************************************************************/
				// Wait until all commands running on all loading loops processed.
				void                        Wait_UntilAllCommandsProcessed                                     ();
				void                        Wait_UntilAllResourceCommandsProcessed                             (ResMan::IManagedResource *pInResource, bool bInRecurisve);
				// Wait until all commands for the given group are processed;
				// If bInRecurisve is true, then child commands are also waited for to complete.
				void                        Wait_UntilAllGroupCommandsProcessed                                (ResMan::IManagedGroup *pInGroup, bool bInRecurisve);

				// Wait until all commands that run NOT on the main thread have been processed.
				// WARNING!!! The non-threaded child commands will not be waited for to complete.
				void                        Wait_UntilAllThreadedCommandsProcessed                             ();
				// Wait until all commands connected with the given resource that run NOT on the main thread have been processed.
				// WARNING!!! The non-threaded child commands will not be waited for to complete.
				void                        Wait_UntilAllThreadedCommandsForResourceProcessed                  (ResMan::IManagedResource *pInResource, bool bInRecursive);
				// Wait until all commands connected with the given group that run NOT on the main thread have been processed.
				// WARNING!!! The non-threaded child commands will not be waited for to complete.
				void                        Wait_UntilAllThreadedCommandsForGroupProcessed                     (ResMan::IManagedGroup *pInGroup, bool bInRecursive);


				/*******************************************************************************************
				* Getters
				*******************************************************************************************/
				const Impl_ResourceManagerSubsystem                    *GetSubsystem              () const { return _pSubsystem; }
				Impl_ResourceManagerSubsystem                          *GetSubsystem              ()                                                      { return _pSubsystem; }

				ResourceRepository                                     *GetResourceRepo           () { return &_resourceRepo; }
				ResourceGroupCombiCont                                 *GetGroups                 () { return &_groups; }

				ResMan::IResourceManager::GroupEnumerator               GetEnumerator_Groups      ();

				/*******************************************************************************************
				* Working with resources
				*******************************************************************************************/
				ManagedResource                                        *FindResourceByURL         (const std::string& InURL);

				ManagedResource                                        *RegisterResource          (const ResMan::PublicResourceParams& InParams, std::shared_ptr<ResMan::IResourceLoader> pInLoader);
				void                                                    UnregisterResource        (ManagedResource *pInResource);

				ManagedResource                                        *PopFailedResource         ();
				// Trying to push failed resource.
				// Return false if resouce has not been added to queue
				// (for example, because resource is already in the queue).
				bool                                                    TryPushFailedResource     (std::shared_ptr<ManagedResource> pInResource);
				
				/*******************************************************************************************
				* Working with groups
				*******************************************************************************************/
				ManagedGroup                                           *RegisterGroup             (const ResMan::GroupParams& InGroupParams);
				void                                                    UnregisterGroup           (ManagedGroup *pInGroup);
				
				ManagedGroup                                           *PopFailedGroup            ();

				void                                                    Notify_GroupResource_TriedToLoad(Impl_ManagedGroup *pInGroup, Impl_ManagedResource *pInResource);

				/********************************************************************************************
				* Working with roles
				********************************************************************************************/
				ResMan::ISubresourceRole                               *GetSubresourceRoleByIndex (ResMan::SubresourceRoleIndex InIndex);

			private:
				// Should error for given combinable params to be processed as fatal;
				// Considers global fatal error params (if there).
				bool                                                    _ShouldProcessErrorAsFatal(const ResMan::CombinableParams *pInCombinableParams) const;

				void                                                    _PrecacheGroupResources(Impl_ManagedGroup *pInGroup, bool bIsManual, ResMan::Urgency InUrgency, bool bPrecacheOnlyDependencies, bool bSynchro);

				ResMan::LoadGroupCommandID                              _PushGroupLoadRequest(ManagedGroup *pInGroup);

				ResMan::IResourceManager                               *_pOwnerManager;
				Impl_ResourceManagerSubsystem                          *_pSubsystem;
				ResourceRepository                                      _resourceRepo;
				std::queue<std::shared_ptr<ManagedResource>>            _failedResources;
				std::priority_queue<std::shared_ptr<ManagedResource>>   _resourceUnloadCandidates;

				ResourceGroupCombiCont                                  _groups;
				std::queue<std::shared_ptr<ManagedGroup>>               _failedGroups;

				Util::IdGenerator                                       _groupCommandIdGenerator;
				std::priority_queue<ManagedGroup *, std::vector<ManagedGroup *>, GroupBaseLoadPriorityComparer>            _loadingGroups;
			};
		} // Impl
	}
} // Dv