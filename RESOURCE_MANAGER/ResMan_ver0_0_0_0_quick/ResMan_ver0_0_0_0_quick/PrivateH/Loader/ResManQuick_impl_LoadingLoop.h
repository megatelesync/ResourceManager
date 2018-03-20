#pragma once

#include "Fwd\ResManQuick_Fwd_Impl.h"
#include "ResManQuick_impl_LoadingLoopCommandQueues.h"
#include "../../PrivateH/ResManQuick_impl_System.h"
#include "Subsystem/MASTER_ResourceManager_Subsystem.h"
#include "../Log/ResManQuick_impl_Log.h"

namespace Dv
{
	namespace ResManQuick
	{
		struct ResManQuick_SubsystemParams;

		namespace Loader
		{
			class LoadingLoop
			{
			public:
				LoadingLoop                                                                                        (LoaderMain *pInOwnerMain, const std::string& InName);

				/**************************************************************************************************************
				* Parameters
				**************************************************************************************************************/
				const std::string&                                GetName                                          () const { return _name; }

				const ResManQuick_SubsystemParams                *GetParams                                        () const { return _pParams;}

				// Is this loading loop performed NOT on the main thread
				virtual bool                                      IsThreaded                                       () const { return true; }

				/**************************************************************************************************************
				* Initialization (must be called from the owning thread)
				**************************************************************************************************************/
				void                                              Initialize                                       (const ResManQuick_SubsystemParams& InParams, Impl::Logger *pInLogger);
				void                                              UpdateRuntimeParams                              (const ResMan::ResourceManagerPublicRuntimeParams& InParams);

				/**************************************************************************************************************
				* This functions to be called from the main thread
				**************************************************************************************************************/
				void                                               Tick                                             ();

				// Pushing and popping commands and results is thread-safe
				LoadingLoopCommandQueues                          *GetCommands                                      ()                     { return &_commands; }
				// Issues command for unlocking the given resource from using by this loop
				virtual void                                       IssueCommand_UnlockResourceForThisLoop           (const LoaderCommandResultRecord& InResultRecord, size_t InNumUnlocks);

				LoaderMain                                        *GetOwnerMain                                     () const                { return _pMain; }
				// Get per-resource data of the loop for the given resource
				virtual const Loader_PerResource_PerLoadingLoop   *GetPerResourceData                               (const Loader_PerResourceData *pInPerResourceData) const;
				virtual Loader_PerResource_PerLoadingLoop         *GetPerResourceData                               (Loader_PerResourceData *pInLoaderData) const;

				/**************************************************************************************************************
				* This functions are handlers to be called from the main thread when the corresponding event takes place
				**************************************************************************************************************/				
				// Notify that the given command result of this loop has been procesed.
				//
				// Must be called before the given processed command result data has been destroyed.
				//
				// Typically unlock resource associated with the given resource record 
				// (must be called after the result has been processsed).
				virtual void                                       Notify_CommandResultProcessed                     (const LoaderCommandResultRecord& InResultRecord);

				// Must be called when the number of unprocessed commands for some other loop drops to zero for the given resource
				// Loading loop on the dedicated thread never unlocks resource when all commands on some other loop are processed.
				virtual void                                       Notify_AllCommandsProcessed_ByOtherLoadingLoop    (LoadingLoop *pInLoop, Loader_PerResourceData *pInResource) {}

				// Must be called when for the given loading loop before the given loading command is enqueued for processing
				virtual void                                       Notify_BeforeCommandEnqueued_ForOtherLoadingLoop(LoadingLoop *pInLoop, const LoaderCommandRecord& InCommand) {};
				virtual void                                       Notify_CommandResultProcessed_ForOtherLoadingLoop(LoadingLoop *pInLoop, const LoaderCommandResultRecord& InResult) {}

			protected:
				/***************************************************************************************************************
				* This functions are to be called from the thread that this loading loop runs on.
				***************************************************************************************************************/
				// Last actions to be performed before returing the result of the command.
				virtual void                                       LockCommandBeforeReturnResult                   (const LoaderCommandRecord& InCommand);

				// true if the given resource should not be used by this loop
				virtual bool                                       IsResourceLocked_FromUsingByThisLoop               (Loader_PerResource_ForLoadingLoopThread *pInResource) const;
				// Lock the given resource from using by this particular loading loop.
				virtual void                                       LockResource_FromUsingByThisLoop                   (Loader_PerResource_ForLoadingLoopThread *pInResource);

				Impl::Logger                                      *GetLogger                                         () const { return _pLogger; }

			private:
				// Process all deferred commands for the given resource;
				// Returns count of processed deferred commands.
				size_t                                             _ProcessDeferredCommands_ForResource_IfNotLocked               (Loader_PerResource_ForLoadingLoopThread *pInResource);

				// Process command that is pending for processing in the command queue
				// (returns number of executed commands)
				size_t                                             _ProcessPendingCommand                             (const LoaderCommandRecord& InCommand);
				// Run command really: execute and return result in the result queue
				void                                               _ExecuteCommand                                    (const LoaderCommandRecord& InCommand);

				LoadingLoopCommandQueues                           _commands;
				LoaderMain                                        *_pMain;
				std::string                                        _name;

				Impl::Logger                                      *_pLogger = nullptr;

				// We store only pointer (as params are immutable)
				const ResManQuick_SubsystemParams                  *_pParams = nullptr;
				
			};

			std::string GetScopedLoopName(LoadingLoop *pInLoop);

			// Helper getter for getting loading loop data for the given resource by the given loading loop queues ptr;
			Loader_PerResource_PerLoadingLoop *GetLoadingLoopData(Impl::Impl_ManagedResource *pInResource, LoadingLoopCommandQueues *pInLoadingLoopQueues);

			/***************************************************************************************
			* This is the loading loop version that performs on the main thread
			***************************************************************************************/
			class MainThread_LoadingLoop : public LoadingLoop
			{
			public:
				MainThread_LoadingLoop                                                                                         (LoaderMain *pInOwnerMain, const std::string& InName);

				virtual bool                                      IsThreaded                                                  () const override { return false; }

				virtual Loader_PerResource_PerLoadingLoop         *GetPerResourceData                                          (Loader_PerResourceData *pInPerResourceData) const override;

				// Unlocking of the resource for the main thread loader is performed simply by updating the corresponding locks counter directly
				virtual void                                       IssueCommand_UnlockResourceForThisLoop                      (const LoaderCommandResultRecord& InResultRecord, size_t InNumUnlocks) override;

				// When command result is processed, no unlocking is to be done, because there was no locks
				virtual void                                       Notify_CommandResultProcessed                               (const LoaderCommandResultRecord& InResultRecord) override {}

				virtual void                                       Notify_BeforeCommandEnqueued_ForOtherLoadingLoop             (LoadingLoop *pInLoop, const LoaderCommandRecord& InCommand) override;
				virtual void                                       Notify_CommandResultProcessed_ForOtherLoadingLoop            (LoadingLoop *pInLoop, const LoaderCommandResultRecord& InResult) override;

			protected:
				virtual void                                       LockCommandBeforeReturnResult                                (const LoaderCommandRecord& InCommand) {}

			private:
			};

			/**************************************************************************************************
			* Loading loop to be used for some short-term period,
			* typically as local variable of some function.
			*
			* WARNING!!! All temporary loading loop instances share the same per-resource data,
			* so locking one insntance of the temporary loading loop will lock all temporary loading loops!!!
			**************************************************************************************************/
			class MainThreadTemporary_LoadingLoop : public MainThread_LoadingLoop
			{
			public:
				MainThreadTemporary_LoadingLoop(LoaderMain *pInOwnerMain, const std::string& InName);

				virtual Loader_PerResource_PerLoadingLoop         *GetPerResourceData(Loader_PerResourceData *pInPerResourceData) const override;
			};
		}
	}
}