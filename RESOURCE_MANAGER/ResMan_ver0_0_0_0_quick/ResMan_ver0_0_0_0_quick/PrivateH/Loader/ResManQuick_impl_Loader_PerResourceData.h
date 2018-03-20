#pragma once
#include "Fwd\ResManQuick_Fwd_Impl.h"

#include "ResManQuick_impl_Loader_PerResource_PerLoadingLoop.h"
#include "../ResManQuick_impl_System.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			/*****************************************************************************************
			* Data to be connected with each of the managed resource objects.
			*
			* Data is read-only for the managed resoure object itself,
			* non-const operations can be accessed by the loader only.
			*****************************************************************************************/
			class Loader_PerResourceData
			{
			public:
				Loader_PerResourceData();

				void                                            SetSomeCommandInQueueFailedRecursively                   (bool bInValue)                                                      { _bSomeCommandInQueueFailedRecursively = bInValue; }
				bool                                            HasSomeCommandInQueueFailedRecursively                   () const                                                             { return _bSomeCommandInQueueFailedRecursively; }

				/***********************************************************************************************
				* Counter of total number of commands in all queues
				*
				* This values are to be accessed only from the MAIN THREAD
				***********************************************************************************************/
				size_t                                           GetTotalCommands_EnqueuedForAllLoops                    () const                                                              { return _totalCommands_enqueuedForAllLoops; }
				size_t                                           GetTotalCommands_PoppedFromAllLoops                     () const                                                              { return _totalCommands_poppedFromAllLoops; }
				size_t                                           GetTotalCommands_ToBePopped_FromAllLoops                () const                                                              { assert(GetTotalCommands_EnqueuedForAllLoops() >= GetTotalCommands_PoppedFromAllLoops());  return GetTotalCommands_EnqueuedForAllLoops() - GetTotalCommands_PoppedFromAllLoops(); }

				void                                             Incr_NumEnqueuedCommands                                (LoadingLoopCommandQueues *pInLoadingLoopQueues)                      { _totalCommands_enqueuedForAllLoops++; }
				void                                             Incr_NumPoppedCommands                                  (LoadingLoopCommandQueues *pInLoadingLoopQueues)                      { assert(_totalCommands_poppedFromAllLoops <= _totalCommands_enqueuedForAllLoops);  _totalCommands_poppedFromAllLoops++; }

				/**********************************************************************************************
				* Get loader loop data related to the given loop
				**********************************************************************************************/
				const Loader_PerResource_PerLoadingLoop          *GetLoaderLoopData                                      (Loader::LoadingLoop *pLoadingLoop) const;
				Loader_PerResource_PerLoadingLoop                *GetLoaderLoopData                                      (Loader::LoadingLoop *pLoadingLoop);

				/*********************************************************************************************
				* Working with dedicated loader thread
				**********************************************************************************************/
				const Loader_PerResource_PerLoadingLoop          *GetLoaderLoopData_ForLoaderOnDedicatedThread          () const                  { return &_loaderLoopData_loadingThread; }
				Loader_PerResource_PerLoadingLoop                *GetLoaderLoopData_ForLoaderOnDedicatedThread          ()                        { return &_loaderLoopData_loadingThread; }

				/*********************************************************************************************
				* Working with loader on the main thread
				**********************************************************************************************/
				const Loader_PerResource_PerLoadingLoop          *GetLoaderLoopData_ForMainThreadLoader                 () const                  { return &_loaderLoopData_mainThread; }
				Loader_PerResource_PerLoadingLoop                *GetLoaderLoopData_ForMainThreadLoader                 ()                        { return &_loaderLoopData_mainThread; }

				/*********************************************************************************************
				* Working with temporary loader
				**********************************************************************************************/
				const Loader_PerResource_PerLoadingLoop          *GetLoaderLoopData_ForTemporaryLoader                  () const                  { return &_loaderLoopData_temporaryLoader; }
				Loader_PerResource_PerLoadingLoop                *GetLoaderLoopData_ForTemporaryLoader                  ()                        { return &_loaderLoopData_temporaryLoader; }

				/*********************************************************************************************
				* Working with dedicated loader thread num unprocessed commands
				**********************************************************************************************/
				size_t                          GetNumLoaderThreadUnprocessedCommands() const { return GetLoaderLoopData_ForLoaderOnDedicatedThread()->GetNumUnprocessedCommands(); }

				/**********************************************************************************************
				* Working with loading resources
				**********************************************************************************************/
				// Clear both loading subresources and dependency lists
				void                            ClearLoadingSubresources() { _subresourcesInLoadingQueue.Clear(); _dependencyListsInLoadingQueue.Clear(); }
				
				void                            IncludeSubresourcesInLoadingQueue(ResMan::SubresourceRoleSet InRoles) { _subresourcesInLoadingQueue.MergeWith(InRoles); }
				void                            IncludeDependencyListsInLoadingQueue(ResMan::SubresourceRoleSet InRoles) { _dependencyListsInLoadingQueue.MergeWith(InRoles); }

				ResMan::SubresourceRoleSet      GetSubresourcesInLoadingQueue() const { return _subresourcesInLoadingQueue; }
				ResMan::SubresourceRoleSet      GetDependencyListsInLoadingQueue() const { return _dependencyListsInLoadingQueue; }

				/**********************************************************************************************
				* Working with loaded subresources
				*
				* Loaded subresources contain both subresources that are loaded with success result
				* as subresources that are loaded with failure result.
				**********************************************************************************************/
				void                            IncludeLoadedSubresources(ResMan::SubresourceRoleSet InRoles)       { _loadedSubresources.MergeWith(InRoles); }
				void                            IncludeLoadedDependencyLists(ResMan::SubresourceRoleSet InRoles)    { _loadedDependencyLists.MergeWith(InRoles); }

				void                            IncludeFailedLoadedSubresources(ResMan::SubresourceRoleSet InRoles) { _loadedFailedSubresources.MergeWith(InRoles); }
				void                            IncludeFailedLoadedDependencyLists(ResMan::SubresourceRoleSet InRoles) { _loadedFailedDependencyLists.MergeWith(InRoles); }

				void                            SubtractFailedLoadedSubresources(ResMan::SubresourceRoleSet InRoles) { _loadedFailedDependencyLists.Subtract(InRoles); }
				void                            SubtractFailedLoadedDependencyLists(ResMan::SubresourceRoleSet InRoles) { _loadedFailedSubresources.Subtract(InRoles); }

				void                            SubtractLoadedDependencyLists(ResMan::SubresourceRoleSet InRoles) { _loadedDependencyLists.Subtract(InRoles); _loadedFailedDependencyLists.Subtract(InRoles); }
				void                            SubtractLoadedSubresources(ResMan::SubresourceRoleSet InRoles) { _loadedSubresources.Subtract(InRoles); _loadedFailedSubresources.Subtract(InRoles); }

				ResMan::SubresourceRoleSet      GetLoadedSubresources() const { return _loadedSubresources; };
				ResMan::SubresourceRoleSet      GetLoadedDependencyLists() const { return _loadedDependencyLists; }

				ResMan::SubresourceRoleSet      GetLoadedFailedSubresources() const { return _loadedFailedSubresources; }
				ResMan::SubresourceRoleSet      GetLoadedFailedDependencyLists() const { return _loadedFailedDependencyLists; }


				/**********************************************************************************************
				* Working with recursive commands counts
				**********************************************************************************************/
				// Number of recursive commands that enqueued for the given resource;
				size_t                          GetNumRecursiveCommands_Enqueued                                     () const                           { return _numRecursiveCommands_Enqueued; }
				// Number of recursive command results for which the initial loading resource is the given resource
				// (result may have any code: sucess or failure)
				size_t                          GetNumRecursiveCommandResults_Popped                                 () const                           { return _numRecursiveCommands_Popped; }
				// Number of recursive command results that popped for which the initial loading resource is the given resource
				// and the result is failed result.
				size_t                          GetNumRecursiveCommandResults_PoppedButFailed                        () const                           { return _numRecursiveCommands_PoppedButFailed; }

				// Number of recusive commands that are to be popped
				size_t                          GetNumRecursiveCommandResults_ToBePopped                             () const                           { return _numRecursiveCommands_Enqueued - _numRecursiveCommands_Popped; }
				size_t                          GetNumRecursiveCommandsResults_PoppedSucceeded                       () const                           { assert(GetNumRecursiveCommandResults_Popped() >= GetNumRecursiveCommandResults_PoppedButFailed());  return GetNumRecursiveCommandResults_Popped() - GetNumRecursiveCommandResults_PoppedButFailed(); }

				void                            Incr_NumRecursiveCommands_Enqueued                                   () { _numRecursiveCommands_Enqueued++; }
				void                            Incr_NumRecursiveCommandResults_Popped                               () { _numRecursiveCommands_Popped++; }
				void                            Incr_NumRecursiveCommandResults_PoppedButFailed                      () { _numRecursiveCommands_PoppedButFailed++; }
				

			private:
				bool                           _bSomeCommandInQueueFailedRecursively = false;

				ResMan::SubresourceRoleSet     _loadedSubresources;
				ResMan::SubresourceRoleSet     _loadedDependencyLists;
				ResMan::SubresourceRoleSet     _subresourcesInLoadingQueue;
				ResMan::SubresourceRoleSet     _dependencyListsInLoadingQueue;
				ResMan::SubresourceRoleSet     _loadedFailedSubresources;
				ResMan::SubresourceRoleSet     _loadedFailedDependencyLists;

				Loader_PerResource_PerLoadingLoop         _loaderLoopData_loadingThread;
				Loader_PerResource_PerLoadingLoop         _loaderLoopData_mainThread;
				Loader_PerResource_PerLoadingLoop         _loaderLoopData_temporaryLoader;

				size_t                                    _numRecursiveCommands_Enqueued = 0;
				size_t                                    _numRecursiveCommands_Popped = 0;
				size_t                                    _numRecursiveCommands_PoppedButFailed = 0;

				size_t                                    _totalCommands_enqueuedForAllLoops = 0;
				size_t                                    _totalCommands_poppedFromAllLoops = 0;
			};
		}
	}
}