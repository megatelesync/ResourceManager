#pragma once

#include "LoadCommand.h"

/*****************************************************************************************
* TODO:
* 1. Maybe we need to rename this header to loading state?
******************************************************************************************/

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class IManagedResource;

			enum ECommandStatusCode
			{
				OK,
				FileNotFound
			};

			struct GroupCommand_Counters
			{
			public:
				void Reset_NumResourcesToBeTriedToLoad                          (size_t InNewCounter)     { numResourcesToBeTriedToLoad = InNewCounter; }

				size_t GetNumResourcesToBeTriedToLoad                           () const                  { return numResourcesToBeTriedToLoad; }

				void Increment_NumResourcesToBeTriedToLoad                      ()                        { numResourcesToBeTriedToLoad++; }
				void Decr_NumResourcesToBeTriedToLoad                           ()                        { assert(numResourcesToBeTriedToLoad > 0);  numResourcesToBeTriedToLoad--; }


			private:
				// Current number of resources that are to be tried to load
				size_t                                numResourcesToBeTriedToLoad = 0;
			};

			struct GroupCommandResult
			{
				GroupCommandResult() : 
					status_code(ECommandStatusCode::OK),
					pFailedResource(nullptr) {}

				ECommandStatusCode                     status_code;
				IManagedResource *                     pFailedResource;
			};

			struct GroupCommandState
			{
				GroupCommandState() :
					bDoingGroupCommand(false)
				{
				}

				bool                    bDoingGroupCommand;
				GroupCommandResult      last_result;
				GroupCommand_Counters   counters;
			};

			class ResourceLoadingState
			{
			public:
				// Get subresources which roles are REQUESTED to be loaded
				// (dependent resources are also included)
				// Only the LoadSubresources() modify this set (not UnloadSubresources)
				SubresourceRoleSet                    GetRecursiveLoadingSubresourceRoles                  () const               { return _subresourcesInLoadingQueue; }
				SubresourceRoleSet                    GetRecursiveLoadingDependencyListsRoles              () const               { return _dependencyListsInLoadingQueue; }

				// Separate  set of subresources that are REQUESTED to be unloaded 
				// (separate set is needed because importance of load and unload operations
				// can be different, and the unload operation can be canceled out by the load operation)
				SubresourceRoleSet                    GetRecursiveUnloadingSubresourceRoles                () const               { return _recursiveUnloadingSubresourceRoles; }

				void                                  SubresourcesInLoadingQueue_Add                       (SubresourceRoleSet InRoles) { _subresourcesInLoadingQueue.MergeWith(InRoles); }
				void                                  SubresourcesInLoadingQueue_Remove                    (SubresourceRoleSet InRoles) { _subresourcesInLoadingQueue.Subtract(InRoles); }
				void                                  SubresourcesInLoadingQueue_Clear                     ()                           { _subresourcesInLoadingQueue.Clear(); }

				void                                  DependencyListsInLoadingQueue_Add                    (SubresourceRoleSet InRoles) { _dependencyListsInLoadingQueue.MergeWith(InRoles); }
				void                                  DependencyListsInLoadingQueue_Remove                 (SubresourceRoleSet InRoles) { _dependencyListsInLoadingQueue.Subtract(InRoles); }
				void                                  DependencyListsInLoadingQueue_Clear                  ()                           { _dependencyListsInLoadingQueue.Clear(); }

				void                                  AddResubresourcesToUnload                            (SubresourceRoleSet InRoles) { _recursiveUnloadingSubresourceRoles.MergeWith(InRoles); }
				void                                  RemoveResubresourcesToUnload                         (SubresourceRoleSet InRoles) { _recursiveUnloadingSubresourceRoles.Subtract(InRoles); }

			private:
				SubresourceRoleSet                    _subresourcesInLoadingQueue;
				SubresourceRoleSet                    _dependencyListsInLoadingQueue;			

				SubresourceRoleSet                    _recursiveUnloadingSubresourceRoles;
			};
		}
	}
}

