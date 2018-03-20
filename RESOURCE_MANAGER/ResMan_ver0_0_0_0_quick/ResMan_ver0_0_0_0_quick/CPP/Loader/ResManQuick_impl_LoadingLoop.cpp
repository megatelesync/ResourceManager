#include "../../PrivateH/Loader/ResManQuick_impl_LoadingLoop.h"
#include "../../PrivateH/Loader/ResManQuick_impl_Loader_PerResourceData.h"
#include "../../PrivateH/Loader/ResManQuick_impl_utils_LoadingLoopCommandQueues.h"
#include "../../H/ResManQuick_SubsystemParams.h"
#include "../../PrivateH/ResManQuick_impl_access_ManagedResource.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			class LoaderMain;

			/*************************************************************************************************************************
			* Constants
			*************************************************************************************************************************/
			// This resource ID is to be used for all special commands (for example, unlocks)
			const ResMan::LoadResourceCommandID LOAD_RESOURCE_COMMAND_ID_SPECIAL = ResMan::LoadResourceCommandID(0);

			/**************************************************************************************************************************
			* Utils
			**************************************************************************************************************************/
			Loader_PerResource_ForLoadingLoopThread *GetPerResourceData_ForLoadingLoopThread(const LoaderCommandRecord& InRecord);

			/**************************************************************************************************************************
			* Log utils
			**************************************************************************************************************************/
			void Log_LoadingLoop_Initialized_IfNecessary                                                    (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const ResManQuick_SubsystemParams *pInParams);
			void Log_LoadingLoop_Initialized                                                                (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const ResManQuick_SubsystemParams *pInParams);
			void Log_LoadingLoop_UpdateRuntimeParams_IfNecessary                                            (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const ResMan::ResourceManagerPublicRuntimeParams *pInParams);
			void Log_LoadingLoop_UpdateRuntimeParams                                                        (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const ResMan::ResourceManagerPublicRuntimeParams *pInParams);

			void Log_LoadingLoop_BeginProcessingCommand_IfNecessary                                         (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand);
			void Log_LoadingLoop_BeginProcessingCommand                                                     (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand);
			void Log_LoadingLoop_EndProcessingCommand_IfNecessary                                           (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand, size_t InNumCommandsProcessedThisTick);
			void Log_LoadingLoop_EndProcessingCommand                                                       (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand, size_t InNumCommandsProcessedThisTick);

			void Log_LoadingLoop_BeforeExecutingCommand_IfNecessary                                         (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand);
			void Log_LoadingLoop_BeforeExecutingCommand                                                     (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand);
			void Log_LoadingLoop_BeforeReturnCommandResult_IfNecessary                                      (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand);
			void Log_LoadingLoop_BeforeReturnCommandResult                                                  (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand);

			void Log_LoadingLoop_BeforeLockingCommandFromUsingByThisLoop_IfNecessary                        (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData, size_t InNumLocksFromUsingByThisLoop_BeforeLock);
			void Log_LoadingLoop_BeforeLockingCommandFromUsingByThisLoop                                    (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData, size_t InNumLocksFromUsingByThisLoop_BeforeLock);

			void Log_LoadingLoop_BeforeExecutingDeferredCommand_IfNecessary                                 (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData, const LoaderCommandRecord *pInCommand, size_t InNumExecutedCommands_BeforeExecutingThis);
			void Log_LoadingLoop_BeforeExecutingDeferredCommand                                             (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData, const LoaderCommandRecord *pInCommand, size_t InNumExecutedCommands_BeforeExecutingThis);
			void Log_LoadingLoop_ResourceIsLocked_IfNecessary                                               (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData);
			void Log_LoadingLoop_ResourceIsLocked                                                           (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData);

			void Log_LoadingLoop_Notified_CommandResultProcessed_ForOtherLoadingLoop_IfNecessary            (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, LoadingLoop *pInOtherLoop, const LoaderCommandResultRecord *pInResult);
			void Log_LoadingLoop_Notified_CommandResultProcessed_ForOtherLoadingLoop                        (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, LoadingLoop *pInOtherLoop, const LoaderCommandResultRecord *pInResult);
			void Log_LoadingLoop_Notified_BeforeCommandEnqueued_ForOtherLoadingLoop_IfNecessary             (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, LoadingLoop *pInOtherLoop, const LoaderCommandRecord *pInCommand);
			void Log_LoadingLoop_Notified_BeforeCommandEnqueued_ForOtherLoadingLoop                         (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, LoadingLoop *pInOtherLoop, const LoaderCommandRecord *pInCommand);

			void Log_LoadingLoop_BeginUnlockingResourceForLoop_IfNecessary                                  (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandResultRecord *pInCommandResult, size_t InNumUnlocksToPerform);
			void Log_LoadingLoop_BeginUnlockingResourceForLoop                                              (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandResultRecord *pInCommandResult, size_t InNumUnlocksToPerform);
			void Log_LoadingLoop_EndUnlockingResourceForLoop_IfNecessary                                    (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand, size_t InNumUnlocksToPerform);
			void Log_LoadingLoop_EndUnlockingResourceForLoop                                                (Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand, size_t InNumUnlocksToPerform);

			std::string GetLoadingLoopRecord_Title                                                          (LoadingLoop *pInThisLoop, const std::string& InRecordName);
			std::string GetLoadingLoopRecord_Title                                                          (LoadingLoop *pInThisLoop, LoadingLoop *pInOtherLoop, const std::string& InRecordName);
			

			/**************************************************************************************************************************
			* Loading loop implementation
			**************************************************************************************************************************/
			LoadingLoop::LoadingLoop(LoaderMain *pInOwnerMain, const std::string& InName) :
				_pMain(pInOwnerMain),
				_commands(this),
				_name(InName)
			{
			}

			void LoadingLoop::Initialize(const ResManQuick_SubsystemParams& InParams, Impl::Logger *pInLogger)
			{
				assert(pInLogger != nullptr);

				_pParams = &InParams;
				_pLogger = pInLogger;

				Log_LoadingLoop_Initialized_IfNecessary(pInLogger, this, _pParams);
			}

			void LoadingLoop::UpdateRuntimeParams(const ResMan::ResourceManagerPublicRuntimeParams& InParams)
			{
				// no code is ok
				Log_LoadingLoop_UpdateRuntimeParams_IfNecessary(_pLogger, this, &InParams);
			}

			void LoadingLoop::Tick()
			{
				size_t numCommands_executedThisTick = 0;

				auto pendingCommand = _commands.PopCommand();
				if (pendingCommand.is_initialized())
				{
					numCommands_executedThisTick += _ProcessPendingCommand(pendingCommand.get());
				}
			}

			void Begin_ProcessingCommand(Impl::Logger *pInLogger, LoadingLoop *pInSenderLoop, const LoaderCommandRecord *pInCommand)
			{
				Log_LoadingLoop_BeginProcessingCommand_IfNecessary(pInLogger, pInSenderLoop, pInCommand);
				pInCommand->GetCommand()->BeginProcessing_OnLoadingLoopThread(GetPerResourceData_ForLoadingLoopThread(*pInCommand));
			}

			void End_ProcessingCommand(Impl::Logger *pInLogger, LoadingLoop *pInSenderLoop, const LoaderCommandRecord *pInCommand, size_t InNumCommandsProcessedThisTick)
			{
				pInCommand->GetCommand()->EndProcessing_OnLoadingLoopThread();
				Log_LoadingLoop_EndProcessingCommand_IfNecessary(pInLogger, pInSenderLoop, pInCommand, InNumCommandsProcessedThisTick);
			}

			class CommandProcessingScoper
			{
			public:
				CommandProcessingScoper(Impl::Logger *pInLogger, LoadingLoop *pInOwnerLoop, const LoaderCommandRecord *pInCommand, size_t InNumCommandsProcessedThisTIck) :
					_pLogger(pInLogger),
					_pCommand(pInCommand),
					_pOwnerLoop(pInOwnerLoop),
					_numCommands_executedThisTick(InNumCommandsProcessedThisTIck)
				{
					Begin_ProcessingCommand(_pLogger, _pOwnerLoop, _pCommand);
				}
				~CommandProcessingScoper()
				{
					End_ProcessingCommand(_pLogger, _pOwnerLoop, _pCommand, _numCommands_executedThisTick);
				}
				void IncrNumExecutedCommands(size_t InValue)
				{
					_numCommands_executedThisTick += InValue;
				}
				size_t GetNumCommands_ExecutedThisTick() const
				{
					return _numCommands_executedThisTick;
				}

			private:
				Impl::Logger                       *_pLogger;
				LoadingLoop                        *_pOwnerLoop;
				const LoaderCommandRecord          *_pCommand;
				size_t                              _numCommands_executedThisTick;
			};

			size_t LoadingLoop::_ProcessPendingCommand(const LoaderCommandRecord& InCommand)
			{
				CommandProcessingScoper commandProcessingScoper(_pLogger, this, &InCommand, 0);

				if (IsResourceLocked_FromUsingByThisLoop(GetPerResourceData_ForLoadingLoopThread(InCommand)))
				{
					GetPerResourceData_ForLoadingLoopThread(InCommand)->EnqueueDefferedCommand(InCommand);
				}
				else
				{
					_ExecuteCommand(InCommand);
					commandProcessingScoper.IncrNumExecutedCommands(1);
				}

				// If after processing current command the resource is no longer locked from using by this thread,
				// all deferred commands for the corresponding resource should be processed
				size_t numExecutedDeferredCommands = _ProcessDeferredCommands_ForResource_IfNotLocked(GetPerResourceData_ForLoadingLoopThread(InCommand));
				commandProcessingScoper.IncrNumExecutedCommands(numExecutedDeferredCommands);

				return commandProcessingScoper.GetNumCommands_ExecutedThisTick();
			}

			size_t LoadingLoop::_ProcessDeferredCommands_ForResource_IfNotLocked(Loader_PerResource_ForLoadingLoopThread *pInResource)
			{
				size_t numExecutedDeferredCommands = 0;

				// If after processing current command the resource is no longer locked from using by this thread,
				// all deferred commands for the corresponding resource should be processed
				if (false == IsResourceLocked_FromUsingByThisLoop(pInResource))
				{
					while (true)
					{
						auto pendingCommand = pInResource->PopDefferedCommand();
						if (!pendingCommand.is_initialized())
						{
							return numExecutedDeferredCommands;
						}
						Log_LoadingLoop_BeforeExecutingDeferredCommand_IfNecessary(_pLogger, this, pInResource, pendingCommand.get_ptr(), numExecutedDeferredCommands);
						_ExecuteCommand(pendingCommand.get());
						numExecutedDeferredCommands++;
					}
				}
				else
				{
					Log_LoadingLoop_ResourceIsLocked_IfNecessary(_pLogger, this, pInResource);
				}

				return numExecutedDeferredCommands;
			}

			void LoadingLoop::_ExecuteCommand(const LoaderCommandRecord& InCommand)
			{
				Log_LoadingLoop_BeforeExecutingCommand_IfNecessary(_pLogger, this, &InCommand);
				auto pCommand = InCommand.GetCommand();
				pCommand->Execute_OnLoadingLoopThread();
				if (pCommand->ShouldReturnResult())
				{
					LockCommandBeforeReturnResult(InCommand);

					Log_LoadingLoop_BeforeReturnCommandResult_IfNecessary(_pLogger, this, &InCommand);
					LoaderCommandResultRecord resultRecord (InCommand);
					_commands.EnqueueResult(resultRecord);
				}
			}

			void LoadingLoop::Notify_CommandResultProcessed(const LoaderCommandResultRecord& InResultRecord)
			{
				// We unlock because we locked the resource from using by this loading loop
				// until the result is processed.
				size_t numUnlocks = 1;
				IssueCommand_UnlockResourceForThisLoop(InResultRecord, numUnlocks);
			}

			void LoadingLoop::LockCommandBeforeReturnResult(const LoaderCommandRecord& InCommand)
			{
				// On the loading loop on the dedicated thread we must lock
				// the resource from using by this loop until notified the result has been processed.
				if (!InCommand.GetCommand()->IsCustomResourceLockingControl())
				{
					// If locking control is not custom, the loading loop must lock the resource by itself
					LockResource_FromUsingByThisLoop(GetPerResourceData_ForLoadingLoopThread(InCommand));
				}
			}

			void LoadingLoop::IssueCommand_UnlockResourceForThisLoop(const LoaderCommandResultRecord& InResultRecord, size_t InNumUnlocks)
			{
				Log_LoadingLoop_BeginUnlockingResourceForLoop_IfNecessary(GetLogger(), this, &InResultRecord, InNumUnlocks);

				auto pCommand = new Loader::UnlockResourceCommand(InNumUnlocks);
				LoaderCommandRecord commandRecord
				(
					LOAD_RESOURCE_COMMAND_ID_SPECIAL,
					InResultRecord,
					ActualPriority_Unlock,
					pCommand
				);
				_commands.Enqueue(commandRecord);

				Log_LoadingLoop_EndUnlockingResourceForLoop_IfNecessary(GetLogger(), this, &commandRecord, InNumUnlocks);
			}

			bool LoadingLoop::IsResourceLocked_FromUsingByThisLoop(Loader_PerResource_ForLoadingLoopThread *pInResource) const
			{
				bool bLocked = pInResource->GetNumLocks_FromUsingByThisLoop() > 0;
				return bLocked;
			}
			void LoadingLoop::LockResource_FromUsingByThisLoop(Loader_PerResource_ForLoadingLoopThread *pInResource)
			{
				Log_LoadingLoop_BeforeLockingCommandFromUsingByThisLoop_IfNecessary(_pLogger, this, pInResource, pInResource->GetNumLocks_FromUsingByThisLoop());

				const size_t numLocks = 1;
				pInResource->AddNumLocks_FromUsingByThisLoop(numLocks);
			}

			const Loader_PerResource_PerLoadingLoop *LoadingLoop::GetPerResourceData(const Loader_PerResourceData *pInPerResourceData) const
			{
				return pInPerResourceData->GetLoaderLoopData_ForLoaderOnDedicatedThread();
			}

			Loader_PerResource_PerLoadingLoop *LoadingLoop::GetPerResourceData(Loader_PerResourceData *pInPerResourceData) const
			{
				return pInPerResourceData->GetLoaderLoopData_ForLoaderOnDedicatedThread();
			}

			/**************************************************************************************************************************
			* MainThread_LoadingLoop implementation
			**************************************************************************************************************************/
			MainThread_LoadingLoop::MainThread_LoadingLoop(LoaderMain *pInOwnerMain, const std::string& InName) :
				LoadingLoop(pInOwnerMain, InName)
			{
			}

			void MainThread_LoadingLoop::IssueCommand_UnlockResourceForThisLoop(const LoaderCommandResultRecord& InResultRecord, size_t InNumUnlocks)
			{
				InResultRecord.GetResource_LoadingLoopThreadData()->SubtractNumLocks_FromUsingByThisLoop(InNumUnlocks);
			}

			void MainThread_LoadingLoop::Notify_BeforeCommandEnqueued_ForOtherLoadingLoop(LoadingLoop *pInLoop, const LoaderCommandRecord& InCommand)
			{
				Log_LoadingLoop_Notified_BeforeCommandEnqueued_ForOtherLoadingLoop_IfNecessary(GetLogger(), this, pInLoop, &InCommand);
				// We must lock only if the resource is used by some threaded loading loop;
				// if the resource is used by loop that is processing on the main thread entirely, no conflicts possible,
				// so no locks are necessary.
				if (pInLoop != this && pInLoop->IsThreaded())
				{
					// We need to lock the resource associated with the given command
					// from using by the current loading loop when command is enqueued for some other loop.
					// WARNING!!! Refactor access to resource - it's wrong to access it through the attached main thread data
					Loader_PerResource_PerLoadingLoop *pThisLoadingLoop_PerResourceData = InCommand.GetAttachedMainThreadData()->pResource->GetLoaderLoopData(this);
					auto pThisLoadingLoop_PerResourceData_ForLoadingThread = pThisLoadingLoop_PerResourceData->GetPerResourceData_ForLoadingLoopThread();
					LockResource_FromUsingByThisLoop(pThisLoadingLoop_PerResourceData_ForLoadingThread);
				}
			}

			void MainThread_LoadingLoop::Notify_CommandResultProcessed_ForOtherLoadingLoop(LoadingLoop *pInLoop, const LoaderCommandResultRecord& InResult)
			{
				Log_LoadingLoop_Notified_CommandResultProcessed_ForOtherLoadingLoop_IfNecessary(GetLogger(), this, pInLoop, &InResult);
				// We should unlock also when the sender thread is threaded only (because we locking only if the send loop is threaded)!!!
				if (pInLoop != this && pInLoop->IsThreaded())
				{
					size_t numUnlocks = 1;
					IssueCommand_UnlockResourceForThisLoop(InResult, numUnlocks);
				}
			}

			Loader_PerResource_PerLoadingLoop *MainThread_LoadingLoop::GetPerResourceData(Loader_PerResourceData *pInPerResourceData) const
			{
				return pInPerResourceData->GetLoaderLoopData_ForMainThreadLoader();
			}

			/**************************************************************************************************************************
			* MainThreadTemporary_LoadingLoop implementation
			**************************************************************************************************************************/
			MainThreadTemporary_LoadingLoop::MainThreadTemporary_LoadingLoop(LoaderMain *pInOwnerMain, const std::string& InName) :
				MainThread_LoadingLoop(pInOwnerMain, InName)
			{

			}

			Loader_PerResource_PerLoadingLoop *MainThreadTemporary_LoadingLoop::GetPerResourceData(Loader_PerResourceData *pInPerResourceData) const
			{
				return pInPerResourceData->GetLoaderLoopData_ForTemporaryLoader();
			}

			/**************************************************************************************************************************
			* Utils implementation
			**************************************************************************************************************************/
			Loader_PerResource_ForLoadingLoopThread *GetPerResourceData_ForLoadingLoopThread(const LoaderCommandRecord& InRecord)
			{
				return const_cast<Loader_PerResource_ForLoadingLoopThread *>(InRecord.GetResource_LoadingLoopThreadData());
			}

			Loader_PerResource_PerLoadingLoop *GetLoadingLoopData(Impl::Impl_ManagedResource *pInResource, LoadingLoopCommandQueues *pInLoadingLoopQueues)
			{
				return pInResource->GetLoaderLoopData(pInLoadingLoopQueues->GetOwnerLoadingLoop());
			}

			/**************************************************************************************************************************
			* Logging utils implementation
			**************************************************************************************************************************/
			std::string GetScopedLoopName(LoadingLoop *pInLoop)
			{
				return std::string("\"") + pInLoop->GetName() + std::string("\"");
			}
			std::string GetLoadingLoopRecord_Title(LoadingLoop *pInThisLoop, const std::string& InRecordName)
			{
				std::string logRecordTitle = std::string("LoadingLoop=") + GetScopedLoopName(pInThisLoop) + std::string(" : ") + InRecordName;
				return logRecordTitle;
			}
			std::string GetLoadingLoopRecord_Title(LoadingLoop *pInThisLoop, LoadingLoop *pInOtherLoop, const std::string& InRecordName)
			{
				std::string logRecordTitle = std::string("LoadingLoop=") + GetScopedLoopName(pInThisLoop) + "; OtherLoadingLoop=" + GetScopedLoopName(pInOtherLoop) + std::string(" : ") + InRecordName;
				return logRecordTitle;
			}
			void Log_LoadingLoop_Initialized_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const ResManQuick_SubsystemParams *pInParams)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_Initialized(pInLogger, pInThisLoop, pInParams);
				}
			}
			void Log_LoadingLoop_Initialized(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const ResManQuick_SubsystemParams *pInParams)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "Initialized");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_UpdateRuntimeParams_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const ResMan::ResourceManagerPublicRuntimeParams *pInParams)
			{
				bool bShouldLog = true;
				if(bShouldLog)
				{
					Log_LoadingLoop_UpdateRuntimeParams(pInLogger, pInThisLoop, pInParams);
				}
			}
			void Log_LoadingLoop_UpdateRuntimeParams(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const ResMan::ResourceManagerPublicRuntimeParams *pInParams)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "Update runtime params");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_BeginProcessingCommand_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_BeginProcessingCommand(pInLogger, pInThisLoop, pInCommand);
				}
			}

			void Log_LoadingLoop_BeginProcessingCommand(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "Begin Processing Command");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_EndProcessingCommand_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand, size_t InNumCommandsProcessedThisTick)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_EndProcessingCommand(pInLogger, pInThisLoop, pInCommand, InNumCommandsProcessedThisTick);
				}
			}

			void Log_LoadingLoop_EndProcessingCommand(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand, size_t InNumCommandsProcessedThisTick)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "End Processing Command");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_BeforeExecutingCommand_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_BeforeExecutingCommand(pInLogger, pInThisLoop, pInCommand);
				}
			}

			void Log_LoadingLoop_BeforeExecutingCommand(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "Before executing command");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_BeforeReturnCommandResult_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_BeforeReturnCommandResult(pInLogger, pInThisLoop, pInCommand);
				}
			}

			void Log_LoadingLoop_BeforeReturnCommandResult(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "Before return command result");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_BeforeLockingCommandFromUsingByThisLoop_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData, size_t InNumLocksFromUsingByThisLoop_BeforeLock)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_BeforeLockingCommandFromUsingByThisLoop(pInLogger, pInThisLoop, pPerResourceData, InNumLocksFromUsingByThisLoop_BeforeLock);
				}
			}

			void Log_LoadingLoop_BeforeLockingCommandFromUsingByThisLoop(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData, size_t InNumLocksFromUsingByThisLoop_BeforeLock)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "Before locking command from using by this loop");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_BeforeExecutingDeferredCommand_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData, const LoaderCommandRecord *pInCommand, size_t InNumExecutedCommands_BeforeExecutingThis)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_BeforeExecutingDeferredCommand(pInLogger, pInThisLoop, pPerResourceData, pInCommand, InNumExecutedCommands_BeforeExecutingThis);
				}
			}

			void Log_LoadingLoop_BeforeExecutingDeferredCommand(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData, const LoaderCommandRecord *pInCommand, size_t InNumExecutedCommands_BeforeExecutingThis)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "Before executing deferred command");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_ResourceIsLocked_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_ResourceIsLocked(pInLogger, pInThisLoop, pPerResourceData);
				}
			}

			void Log_LoadingLoop_ResourceIsLocked(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const Loader_PerResource_ForLoadingLoopThread *pPerResourceData)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "Resource is locked");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_Notified_CommandResultProcessed_ForOtherLoadingLoop_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, LoadingLoop *pInOtherLoop, const LoaderCommandResultRecord *pInResult)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_Notified_CommandResultProcessed_ForOtherLoadingLoop(pInLogger, pInThisLoop, pInOtherLoop, pInResult);
				}
			}

			void Log_LoadingLoop_Notified_CommandResultProcessed_ForOtherLoadingLoop(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, LoadingLoop *pInOtherLoop, const LoaderCommandResultRecord *pInResult)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, pInOtherLoop, "Notified: Other loading loop processed command");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_Notified_BeforeCommandEnqueued_ForOtherLoadingLoop_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, LoadingLoop *pInOtherLoop, const LoaderCommandRecord *pInCommand)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_Notified_BeforeCommandEnqueued_ForOtherLoadingLoop(pInLogger, pInThisLoop, pInOtherLoop, pInCommand);
				}
			}

			void Log_LoadingLoop_Notified_BeforeCommandEnqueued_ForOtherLoadingLoop(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, LoadingLoop *pInOtherLoop, const LoaderCommandRecord *pInCommand)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, pInOtherLoop, "Notified: Before command enqueued in other loading loop");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_BeginUnlockingResourceForLoop_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandResultRecord *pInCommandResult, size_t InNumUnlocksToPerform)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_BeginUnlockingResourceForLoop(pInLogger, pInThisLoop, pInCommandResult, InNumUnlocksToPerform);
				}
			}

			void Log_LoadingLoop_BeginUnlockingResourceForLoop(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandResultRecord *pInCommandResult, size_t InNumUnlocksToPerform)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "Begin unlocking resource for this loop");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}

			void Log_LoadingLoop_EndUnlockingResourceForLoop_IfNecessary(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand, size_t InNumUnlocksToPerform)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoadingLoop_EndUnlockingResourceForLoop(pInLogger, pInThisLoop, pInCommand, InNumUnlocksToPerform);
				}
			}

			void Log_LoadingLoop_EndUnlockingResourceForLoop(Impl::Logger *pInLogger, LoadingLoop *pInThisLoop, const LoaderCommandRecord *pInCommand, size_t InNumUnlocksToPerform)
			{
				std::string logRecordTitle = GetLoadingLoopRecord_Title(pInThisLoop, "End unlocking resource for this loop");
				Util::LogRecordScoper recordScoper(pInLogger, logRecordTitle, Util::EMessageType::Status);
			}
		}
	}
}