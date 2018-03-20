#pragma once

#include "Fwd/ResManQuick_Fwd_Impl.h"
#include "ResManQuick_impl_LoadingThread.h"
#include "ResManQuick_impl_LoadingLoop.h"
#include "../Log/ResManQuick_impl_Log.h"
#include "CoreUtil/CoreUtil_IdGenerator.h"

namespace Dv
{
	namespace ResManQuick
	{
		struct ResManQuick_SubsystemParams;

		namespace Impl
		{
			class Impl_ResourceManagerSubsystem;
			class Impl_ManagedResource;
		}
		namespace Loader
		{		
			/***********************************************************************************
			* Params of the load command
			***********************************************************************************/
			struct LoadCommandParams
			{
				// Parameters of the request itself (resource or group)
				LoadRequestParams                        request;
				// Priority of this particular resource load
				size_t                                   priority;
				bool                                     bPerformRecursive;

				/******************************************************************************
				* Helper getters
				******************************************************************************/
				const LoadRequestParams&                 GetRequest                                       () const                 { return request; }
				size_t                                   GetPriority                                      () const                 { return priority; }
				bool                                     ShouldPerformRecursive                           () const                 { return bPerformRecursive; }

				Impl::Impl_ManagedResource              *GetResource                                      () const                 { return request.GetResource(); }
				Impl::Impl_ManagedGroup                 *GetGroup                                         () const                 { return request.GetGroup(); }
			};

			/**************************************************************************************************
			* TODO:
			* 1. Initialze the loader main (+CODED: 11.01.2017/23:17)
			* 2. Implement function for synchro loading
			* 3. Implement function to wait until all threaded commands to be processed (+CODED: 11.01.2017/23:17).
			*
			* TODO misc:
			* 1. Reogranize main loader to be able to process many command results for the same resource at once.
			* 2. Repair bug about resources (see NotYetImpl)
			*
			** TODO interface:
			* 1. Refactor parameters of loader
			* 2. Rename LoaderCommands to LoadingLoopQueues
			* 3. 
			*
			* Old TODO:
			* 1. Implement Enqueue load command:
			* 1.1. Create Impl_Loader_PerResourceData (+CODED: 08.01.2017/16:24)
			* 1.2. Impl_ManagedResource: Provide BeginLoad, EndLoad, Notify_LoadingStarted/Notify_LoadingEnded
			* (+CODED: 09.01.2017/17:47)
			* 1.3. Create the command base class and implement its concrete classes (for dependencies, for data)
			* (+ CODED: 07.01.2017/19:28)
			* 1.4. Think of priority calculation (+CODED: 09.01.2017/18:16)
			* 1.5. Refactor the resource command record creation (+CODED: 11.01.2017/20:32)
			* 1.6. Gettings parameters of resource loader: is single-threaded?
			* (+CODED: 08.01.2017/23:36)
			* 1.7. Refactor access to LoadCommandParams: resource (+CODED: 11.01.2017/20:32)
			* 1.8. ResourceDependencyList::NotifyLoadBegin() must be called
			* (+CODED: 09.01.2017/18:20)
			* 1.9. Set of subresources to be resetted only if the subresources are loading
			* 1.10. The loader command MUST create new child commands through 
			* MainLoop's BeginLoad()/EndLoop() (+CODED: 11.01.2017/20:32)
			* 2. Implement the end load command
			* 2.1. Implement LoadUpdate class
			* (+CODED: 09.01.2017/18:20)
			* 2.2. Refactor: InCommandResult: access to resource
			* (+CODED: 09.01.2017/18:20)
			* 2.3. How to get the loaded subresource roles set from the command result?
			* (+CODED: 09.91.2017/14:03)
			* 2.4. Which subresources to pass to the dependency list BeginLoad/EndLoad methods?
			* 2.5. Delete objects of the command result record
			* (+CODED: 09.02.2017/18:28)
			* 2.6. Updating the resource object with loaded data - the Notify_LoadEnded argument
			* must take parameter of the new loaded resource
			* (+CODED: 09.02.2017/18:31)
			* 3. Implement Tick() operation
			* 3.1. Recursivity support
			* 3.1.1. Add flag for command to test for recursive execution, test the flag when processing dependencies
			* 3.1.2. Check resource's flag of whether it's always loaded recursively, and no load should occur
			* 3.2. Handle error case: only part of subresources were loaded by loader
			*
			* TODO lower layer:
			* 1. Update queues:
			* 1.1. Queues should not wait for unlock
			***************************************************************************************************/
			class LoaderMain
			{
			public:
				LoaderMain                                               (Impl::Impl_ResourceManagerSubsystem *pInSubsystem);

				void Initialize                                          (const ResManQuick_SubsystemParams& InParams);
				void UpdateRuntimeParams                                 (const ResMan::ResourceManagerPublicRuntimeParams& InParams);

				/**************************************************************************************
				* Getters
				***************************************************************************************/
				Impl::Impl_ResourceManagerSubsystem       *GetSubsystem  () const { return _pSubsystem; }

				/***************************************************************************************
				* Parameters of the subsystem that this loader linked with
				****************************************************************************************/
				const ResManQuick_SubsystemParams&      GetParams     () const;

				/***************************************************************************************
				* Getting loader loops
				*
				* WARNING!!! This interface is for the LoaderMain implementation only!
				***************************************************************************************/
				MainThread_LoadingLoop                *GetLoadingLoop_MainThread           ()           { return &_mainThreadLoop; }
				LoadingThread                         *GetLoadingThread                    ()           { return &_loadingThreadLoop; }

				/***************************************************************************************
				* Loading resources
				* WARNING!!!: The resource must be loadable (the loader must be linked with the resource)
				***************************************************************************************/
				// Enqueue load command to do it asynchronously
				// (priority is not calculated, but used as it provided)
				void EnqueueLoadCommand                                  (const LoadCommandParams& InParams);
				// Perform load synchronously
				void LoadSynchro                                         (const LoadCommandParams& InParams);

				/****************************************************************************************
				* Loading
				*
				* This methods do consider parameters of the resource, and can omit the load
				* based on some flags
				****************************************************************************************/
				// Enqueue the given command for resource with the given actual (as-is) priority
				void EnqueueCommandForResource
				(
					LoadingLoopCommandQueues                            *pInLoadingLoopQueues,
					const LoadRequestParams&                             InRequestParams,
					size_t                                               InActualPriority,
					LoadCommandBase                                     *pInCommand,
					Impl::Impl_ManagedResource                          *pInResource,
					Loader_PerResourceData                              *pOutPerResourceData,
					Loader_PerResourceData                              *pInInitiallyRequestedResource_LoaderData
				);
				void EndLoad
				(
					LoadingLoopCommandQueues                            *pInLoadingLoopQueues,
					const Loader::LoaderCommandResultRecord             &InResultRecord, 
					Loader_PerResourceData                              *pOutResourceLoaderData,
					Loader_PerResourceData                              *pOutInitiallyRequestedResourceLoaderData,
					bool                                                 bInRecursiveCommandsSucceeded
				);

				/****************************************************************************************
				* Waiting for events
				****************************************************************************************/
				// Wait until all commands running on all loading loops processed.
				void Wait_UntilAllCommandsProcessed                                       ();
				void Wait_UntilAllResourceCommandsProcessed                               (ResMan::IManagedResource *pInResource, bool bInRecurisve);
				// Wait until all commands for the given group are processed;
				// If bInRecurisve is true, then child commands are also waited for to complete.
				void Wait_UntilAllGroupCommandsProcessed                                  (ResMan::IManagedGroup *pInGroup, bool bInRecurisve);

				// Wait until all commands that run NOT on the main thread have been processed.
				// WARNING!!! The non-threaded child commands will not be waited for to complete.
				void Wait_UntilAllThreadedCommandsProcessed                                ();
				// Wait until all commands connected with the given resource that run NOT on the main thread have been processed.
				// WARNING!!! The non-threaded child commands will not be waited for to complete.
				void Wait_UntilAllThreadedCommandsForResourceProcessed                     (ResMan::IManagedResource *pInResource, bool bInRecursive);

				/****************************************************************************************
				* Tick
				****************************************************************************************/
				void Tick();

			private:
				// Process all commands and results of the given loading loop
				void _Process_AllCommandsAndResults_OfLoadingLoop                (LoadingLoop *pInLoop);
				// Process resource loading commands and results of the given loading loop
				void _Process_AllResourceCommandsAndResults_OfLoadingLoop        (LoadingLoop *pInLoop, ResMan::IManagedResource *pInResource, bool bInRecursive);
				void _Process_CommandAndResult                                   (LoadingLoopCommandQueues *pInQueues);

				void _EnqueueLoadCommand_InQueues                                (const LoadCommandParams& InParams, LoadingLoopCommandQueues *pInQueues, bool bInShouldKeepTheSameQueue);

				void _ProcessLoadingThreadLoop_ResultQueue();

				void _ProcessMainCommand                                  (const Loader::LoaderCommandResultRecord& InCommandResult, LoadingLoopCommandQueues *pOutCommands, bool bInRecursiveCommandsSucceeded);
				// Returns false recursive command is failed
				bool _ProcessRecursiveCommands                            (const Loader::LoaderCommandResultRecord& InCommandResult, LoadingLoopCommandQueues *pInLoadingLoopQueues);
				void _ProcessCommandResultQueue                           (LoadingLoopCommandQueues *pInLoadingLoopQueues);

				// this logger is only for the threaded logger, so it's not public
				Impl::Logger                                  _threadedLogger;

				Impl::Impl_ResourceManagerSubsystem          *_pSubsystem;
				LoadingThread                                 _loadingThreadLoop;
				MainThread_LoadingLoop                        _mainThreadLoop;

				Util::IdGenerator                             _commandIdGenerator;
			};
		}
	} // ResManQuick
} // Dv