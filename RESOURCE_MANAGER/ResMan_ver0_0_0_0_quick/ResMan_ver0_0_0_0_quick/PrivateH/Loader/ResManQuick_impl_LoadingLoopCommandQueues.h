#pragma once

/**********************************************************************************************************************
* This header files declares both the command class and queues with supporting structures
**********************************************************************************************************************/
#include "Fwd\ResManQuick_Fwd_Impl.h"
#include "ResManQuick_impl_LoadRequestParams.h"
#include "ResManQuick_impl_LoaderCommandRecord.h"
#include "ResManQuick_impl_LoadUpdate.h"

#include "../Fwd/ResManQuick_Fwd_ResourceManager.h"
#include "Log/Util_ILoggingDevice.h"
//#include "../Log/ResManQuick_impl_Log.h"

#include "boost/optional.hpp"
#include <queue>
#include <mutex>

namespace Dv
{
	namespace ResManQuick
	{
		class ManagedResource;

		namespace Impl
		{
			class ResourceRepository;
		}

		namespace Loader
		{
			/*******************************************************************************************************
			* WARNING: This structure contains data to be accessible ONLY from the MAIN THREAD!!!
			*******************************************************************************************************/
			struct LoaderCommandMainThreadData
			{
				// Parameters of the initial resource command request
				LoadRequestParams                           initial_request_params;
				// Current requested resource
				Impl::Impl_ManagedResource                 *pResource;
			};

			/*****************************************************************************************
			* LoaderCommandQueue
			*
			* Synchronized priority queue for commands
			*****************************************************************************************/
			class LoaderCommandQueue
			{
			public:
				void                                            Enqueue         (const LoaderCommandRecord& InRec);
				boost::optional<LoaderCommandRecord>            Pop             ();

			private:
				std::priority_queue<LoaderCommandRecord>        _queue;
				std::mutex                                      _mutex;
			};

			/******************************************************************************************
			* Loader command result record
			* 
			* WARNING!!! Some data of the result record is owned, and must be deleted 
			* after it has been processed but it's nor managed by the destructor, nor smart ptr;
			******************************************************************************************/
			class LoaderCommandResultRecord
			{
			public:
				/**************************************************************************************
				* Construction
				***************************************************************************************/
				LoaderCommandResultRecord                      (const LoaderCommandRecord& InCommandRecord) : _commandRecord(InCommandRecord) {}

				/**************************************************************************************
				* Getters
				***************************************************************************************/
				const LoaderCommandRecord&                      GetCommandRecord                           () const   { return _commandRecord; }
				LoaderCommandRecord&                            GetCommandRecord                           ()         { return _commandRecord; }				

				/**************************************************************************************
				* Helper getters
				***************************************************************************************/
				size_t                                          GetActualPriority                          () const { return _commandRecord.GetActualPriority(); }
				// command is owned
				LoadCommandBase                                *GetCommand                                 () const { return _commandRecord.GetCommand(); }
				// command is owned
				const LoaderCommandMainThreadData              *GetAttachedMainThreadData                  () const { return static_cast<const LoaderCommandMainThreadData *>(_commandRecord.GetAttachedMainThreadData()); }
				Loader_PerResource_PerLoadingLoop              *GetResource_LoadingLoop                    () const;
				Loader_PerResource_ForLoadingLoopThread        *GetResource_LoadingLoopThreadData          () const;

				Impl::Impl_ManagedResource                     *GetResource                                () const  { return GetAttachedMainThreadData()->pResource;  }

				Impl::Impl_ManagedGroup                        *GetGroup                                   () const { return _commandRecord.GetGroup(); }

			private:
				LoaderCommandRecord                             _commandRecord;
			};

			/************************************************************************************************************
			* Logging
			************************************************************************************************************/
			void Log_CommandResult
			(
			    Util::ILoggingDevice                   *pLog,
				ResMan::IResourceManager               *pInManager,
			    const LoaderCommandResultRecord        *pInResultRecord,
			    bool bIsVerbose
			);

			class LoaderCommandResultQueue
			{
			public:
				void Enqueue(const LoaderCommandResultRecord& InResult);
				boost::optional<LoaderCommandResultRecord> Pop();

			private:
				std::queue<LoaderCommandResultRecord>            _queue;
				std::mutex                                       _mutex;
			};

			/************************************************************************************************************
			* Loading loop queues
			*
			* This class provides access to command queues and their results
			************************************************************************************************************/
			class LoadingLoopCommandQueues
			{
			public:
				LoadingLoopCommandQueues(LoadingLoop *pInOwnerLoadingLoop);

				/***************************************************************************************************************
				* Interface for users of the loading loop
				****************************************************************************************************************/
				void                                              Enqueue                                              (const LoaderCommandRecord& InCommandRecord);
				boost::optional<LoaderCommandRecord>              PopCommand                                           ();

				/***************************************************************************************************************
				* Interface for loading loop itself
				****************************************************************************************************************/
				boost::optional<LoaderCommandResultRecord>        PopResult                                            ();
				void                                              EnqueueResult                                        (const LoaderCommandResultRecord& InCommandResultRecord);

				/***************************************************************************************************************
				* Getters
				****************************************************************************************************************/
				LoadingLoop                                      *GetOwnerLoadingLoop                                  () const { return _pOwnerLoadingLoop; }

				/***************************************************************************************************************
				* Counting commands and results
				*
				* This functions must be called from the main thread only.
				****************************************************************************************************************/
				// Get total count of commands entered into the command queued
				size_t                                            GetTotalEnteredCommands                              () const { return _numEnteredCommands; }
				// Get total count of command results popped
				size_t                                            GetTotalPoppedCommandResults                         () const { return _numPoppedCommands; }
				// Get num commands entered into the command queued, but that were not popped
				// (this results can be not ever outputted yet)
				size_t                                            GetNumResultsToBePopped                              () const { return GetTotalEnteredCommands() - GetTotalPoppedCommandResults(); }

			private:
				size_t                                            _numEnteredCommands = 0;
				size_t                                            _numPoppedCommands = 0;

				LoaderCommandQueue                                _commands;
				LoaderCommandResultQueue                          _results;
				LoadingLoop                                      *_pOwnerLoadingLoop;
			};
			LoaderMain                                           *GetOwnerLoaderMain                                    (LoadingLoopCommandQueues *pInLoadingLoopQueues);

			/***********************************************************************************
			* TODO:
			***********************************************************************************/
			class LoadCommandBase
			{
			public:
				/**********************************************************************************************************************************
				* Construction
				* Loader pointer can be nullptr if command is not load command (for example, unblock command)
				**********************************************************************************************************************************/
				LoadCommandBase
				(
					std::shared_ptr<ResMan::IResourceLoader>            pInLoader,
					ResMan::SubresourceRoleSet                          InSubresourcesToLoad,
					bool                                                bInRecursive,
					bool                                                bInShouldKeepLoadingLoop
				);

				/*******************************************************************************************************
				* The loader getter:
				* 
				* WARNING!!! Pointer to te resource loader can be getted from any thread 
				* but it's NOT thread-safe to access the loader without synchronization;
				* See the main loader class how to access the loader.
				*******************************************************************************************************/
				std::shared_ptr<ResMan::IResourceLoader>               GetLoader                                        () const                     { return _pLoader; }

				/*******************************************************************************************************
				* Immutable getters: This getters can be called from any thread.
				*******************************************************************************************************/
				const ResMan::ResourceLoaderParams&                     GetLoaderParams                                  () const;
				virtual bool                                            ShouldReturnResult                               () const { return true; }
				bool                                                    IsRecursive                                      () const { return _bRecursive; }
				// True if recursive commands must be enqueued to the command queue corredponding to the result queue that
				// this command is contained in.
				bool                                                    ShouldKeepLoadingLoop                            () const { return _bKeepLoadingLoop; }

				// Get set of subresources to load (empty if only dependency lists to be loaded)
				virtual ResMan::SubresourceRoleSet                     GetSubresourcesToLoad                            () const { return _subresourcesToLoad; }
				ResMan::SubresourceRoleSet                             GetSubresourceDependencyListsToLoad              () const { return _subresourcesToLoad; }

				/********************************************************************************************************
				* Working with commands queues
				********************************************************************************************************/
				// This method to be called before enqueueing this command into the command queue on the main thread;
				// Implementation must update the corresponding counters of commands by itself using the given resource loader data.
				// Default implementation just increases number of commands in the queue.
				// WARNING!!! This method must be called on the main thread.
				virtual void                                            BeginEnqueue                         (Loader_PerResource_ForMainThread *pOutMainThreadData);
				// This method to be called after the given command has been enqueued into the command queue on the main thread;
				// Implementation must notify the given managed resource about the corresponding command event.
				// WARNING!!! This method must be called on the main thread.
				virtual void                                            EndEnqueue                           (const LoadRequestParams& InParams, Impl::Impl_ManagedResource *pInResource);

				/********************************************************************************************************
				* Functional scopes: 
				* BeginProcessing_OnLoaderLoopThread(...) must be called before calling ANY other function on the thread of the loading loop.
				********************************************************************************************************/
				void                                                    BeginProcessing_OnLoadingLoopThread        (Loader_PerResource_ForLoadingLoopThread *pInPerfResource_ForLoadingLoopThread);
				void                                                    EndProcessing_OnLoadingLoopThread          ();
				bool                                                    IsLoadingLoopThreadContext                 () const { return _pPerResource_ForLoadingLoopThread != nullptr; }

				/*******************************************************************************************************
				* Getters to be used from the loading loop thread only
				*******************************************************************************************************/
				Loader_PerResource_ForLoadingLoopThread                *GetPerResourceData_ForLoadingLoopThread() const { return _pPerResource_ForLoadingLoopThread; }

				/********************************************************************************************************
				* Execution on the thread of the loading loop.
				* See the "Functional scopes" section for more info.
				********************************************************************************************************/
				// Call this function to do what the command must do on the thread of the loading loop.
				virtual void                                            Execute_OnLoadingLoopThread                      () = 0;
				// Get state of subresources update since the last call to ExecuteOnLoaderThread:
				// WARNING: Execute_OnLoadingLoopThread must recalculate the update by itself
				const LoadUpdate&                                       GetLoadUpdate_AfterExecutedOnThreadOfLoadingLoop        () const { return _updateSinceLastCall; }

				// True if loading is failed
				virtual bool                                            HasLoadFailed                                           () const { return false; }
				virtual bool                                            HasCreationFailed                                       () const { return false; }

				/*******************************************************************************************************
				* Working with locks
				*
				* WARNING!!! Lock functions always must be called on the thread of the loading loop.
				*
				* Ordinary commands should never access (for reading or writing) the locking data.
				* Commands that work with locks should follow the following steps:
				* 1. override the IsCustomResourceLockingControl() to return true;
				* 2. Perform actions on the lock data inside the Execute_OnLoadingLoopThread().				
				*******************************************************************************************************/
				virtual bool                                            IsCustomResourceLockingControl() const { return false; }

				/********************************************************************************************************
				* Creating final resources
				* Resource are always to be created on the main thread.
				********************************************************************************************************/
				void                                                   *CreateNewResource(void *pOldResource);

				/********************************************************************************************************
				* Working with addressable subresources.
				********************************************************************************************************/
				// Process all adressable subresources: create, link, register
				void                                                   ProcessAllAdressableSubresources    (Impl::ResourceRepository *pInRepo);
				

				/********************************************************************************************************
				* Working with dependencies.
				*
				* WARNING!!! This functions always must be called from the main thread.
				********************************************************************************************************/
				// BeginProcessDependendcyLists/EndProcessDependencyLists() are operator scopes for ProcessNextRelevantDependecy()/CreateNewChildCommand()
				void                                                    BeginProcessDependencyLists          ();
				void                                                    EndProcessDependencyLists();

				ManagedResource                                        *ProcessNextRelevantDependency        (Impl::ResourceRepository *pInRepo, bool *pbOutRegistrationFailed);
				// Create new child command UNBOUND to the load thread data
				// (return nullptr if should not child command corresponding to the given dependency)
				virtual LoadCommandBase                                *CreateNewChildCommand                (const LoaderCommandResultRecord& InResultRecord) =0;

			protected:
				bool                                                   IsFirstDependency                     () const { return _bFirstDependency; }
				void                                                   SetFirstDependencyVisited             () { _bFirstDependency = false; }

				// Dependency index of the current processing dependency
				const ResMan::ResourceDependencyList                   *_GetDependencyList                   () const;

				// Current used dependency
				const ResMan::Dependency                               *_GetDependency() const { return _dependencyEnumerator.GetDependency(); }
				// loader to be used for the current dependency
				// (or nullptr if the current dependency should not be loaded)
				std::shared_ptr<ResMan::IResourceLoader>                _GetDependencyLoader() const;

				// Typically to be called to save the state before update:
				// typically should be called at the start of Execute_OnLoadingLoopThread()
				void                                                    _SaveStateBeforeUpdate();
				// Recalculate update from the associated loader: 
				// typically should be called at the end of Execute_OnLoadingLoopThread()
				void                                                    _RecalculateUpdateSinceLastCall();

			private:

				std::shared_ptr<ResMan::IResourceLoader>                _pLoader;
				Loader_PerResource_ForLoadingLoopThread                *_pPerResource_ForLoadingLoopThread;
				ResMan::SubresourceRoleSet                              _subresourcesToLoad;

				bool                                                    _bFirstDependency;
				ResMan::DependencyEnumerator                            _dependencyEnumerator;
				bool                                                    _bRecursive = true;
				bool                                                    _bKeepLoadingLoop = false;

				/****************************************************************************************
				* Update-related stuff
				****************************************************************************************/
				LoadUpdate                                              _updateSinceLastCall;
				ResMan::SubresourceRoleSet                              _loadedRoles_before_Execute_OnLoadingLoopThread;
				ResMan::SubresourceRoleSet                              _loadedDependencyListRoles_before_Execute_OnLoadingLoopThread;
			};


			class LoadSubresourcesCommand : public LoadCommandBase
			{
			public:
				LoadSubresourcesCommand
				(
					std::shared_ptr<ResMan::IResourceLoader>            pInLoader,
					ResMan::SubresourceRoleSet                          InSubresourcesToLoad,
					bool                                                bInRecursive,
					bool                                                bInShouldKeepLoadingLoop
				);

				virtual bool                                            HasLoadFailed                              () const override;
				virtual bool                                            HasCreationFailed                          () const override;
				virtual void                                            Execute_OnLoadingLoopThread                () override;

				virtual LoadCommandBase                                *CreateNewChildCommand(const LoaderCommandResultRecord& InResultRecord) override;
			};
			
			class LoadDependencyListsCommand : public LoadCommandBase
			{
			public:
				LoadDependencyListsCommand
				(
					std::shared_ptr<ResMan::IResourceLoader>            pInLoader,
					ResMan::SubresourceRoleSet                          InSubresourcesToLoad,
					bool                                                bInRecursive,
					bool                                                bInShouldKeepLoadingLoop
				);

				virtual bool                                            HasLoadFailed                               () const override;
				virtual void                                            Execute_OnLoadingLoopThread                 () override;

				// no subresources are loaded if only dependency lists to be loaded
				virtual ResMan::SubresourceRoleSet                      GetSubresourcesToLoad                       () const override { return ResMan::SubresourceRoleSet(); }

				virtual LoadCommandBase                                *CreateNewChildCommand                        (const LoaderCommandResultRecord& InResultRecord) override;

			private:
			};

			class UnlockResourceCommand : public LoadCommandBase
			{
			public:
				UnlockResourceCommand
				(
					size_t                                                InNumUnlocks
				);

				// We should not count unlocks as unprocessed commands,
				// and we should not notify the resource object about unlocks,
				// so we override the default functions.
				virtual void                                            BeginEnqueue                         (Loader_PerResourceData *pOutResourceLoaderData) {};
				virtual void                                            EndEnqueue                           (const LoadRequestParams& InParams, Impl::Impl_ManagedResource *pInResource) {}

				// Unlocks commands controls resource locks itself
				virtual bool                                            IsCustomResourceLockingControl       () const override { return true; }
				virtual void                                            Execute_OnLoadingLoopThread                () override;

				// Unlock command never returns result
				virtual bool                                            ShouldReturnResult                  () const override { return false; }
				
				// No new child creation makes sense for the unlock command, so override with empty stub function
				virtual LoadCommandBase                                *CreateNewChildCommand               (const LoaderCommandResultRecord& InResultRecord) override { return Util::InvalidCall<LoadCommandBase *>("Unlock command should not create child commands!"); }

			private:
				void                                                    PerformUnlock();

				size_t                                                  _numUnlocks;
			};
		} // Loader
	} // ResManQuick
} // Dv