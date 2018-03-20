#include "../../PrivateH/Loader/ResManQuick_impl_LoaderMain.h"
#include "../../PrivateH/ResManQuick_impl_System.h"
#include "../../PrivateH/Loader/ResManQuick_impl_utils_LoadingLoopCommandQueues.h"
#include "../../PrivateH/ResManQuick_impl_access_ResourceManagerSubsystem.h"
#include "../../PrivateH/ResManQuick_impl_access_ResourceManager.h"
#include "../../PrivateH/ResManQuick_impl_access_ManagedResource.h"
#include "../../PrivateH/ResManQuick_impl_access_ManagedGroup.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			// Utility functions
			LoadCommandBase *CreateLoadCommandFromParameters
			(
				const LoadCommandParams&           InParams,
				Impl::Impl_ManagedResource        *pInResource,
				bool                               bInShouldKeepLoadingLoop
			);
			LoaderCommandRecord CreateRecord
			(
				ResMan::LoadResourceCommandID                            InResourceCommandID,
				ResMan::LoadResourceCommandID                            InInitialResourceCommandID,
				const LoadRequestParams&                                 InInitialRequestParams,
				size_t                                                   InActualPriority,
				LoadCommandBase                                         *pInCommand,
				Impl::Impl_ManagedResource                              *pInResource,
				Loader_PerResource_PerLoadingLoop                       *pInLoaderLoopData
			);
			void DeleteCommandResultObjects(const Loader::LoaderCommandResultRecord& InResultRecord);
			void EnqueueCommandForResource
			(
				const Loader::LoaderCommandResultRecord&                   InResultRecord,
				Impl::Impl_ManagedResource                                *pInResource,
				size_t                                                     InActualPriority,
				Loader::LoadCommandBase                                   *pInCommand,
				LoadingLoopCommandQueues                                  *pInLoadingLoopQueues
			);		

			// Computes priority of the recursive command for loading the given resource to be forked out from the given record
			bool IsChildCommandForRecursive(const LoadRequestParams *pInParams, Impl::Impl_ManagedResource *pInResource);
			size_t ComputeRecursiveCommandPriorityForResource(const Loader::LoaderCommandResultRecord& InResultRecord, Impl::Impl_ManagedResource *pInResource);

			// Returns false if no more recursive commands
			bool ProcessNextRecursiveCommand
			(
				const Loader::LoaderCommandResultRecord&                   InResultRecord,
				LoadingLoopCommandQueues                                  *pInLoadingLoopQueues,
				bool                                                      *pbOutIsFailed
			);

			// Choose queues to process the command according to parameters of the resource and parameters of the loader itself.
			LoadingLoopCommandQueues *ChooseCommandQueues(LoaderMain *pInLoaderMain, Impl::Impl_ManagedResource *pInResource);

			// Choose command queues used to launch the recursive command for the given command result record
			LoadingLoopCommandQueues *ChooseRecursiveCommandQueues(LoadingLoopCommandQueues *pInParentCommandQueues, const Loader::LoaderCommandResultRecord& InCommandResult);

			// Fwd logging
			void Log_LoaderMain_EnqueueCommandForResource_IfShould
			(
				LoaderMain                             *pInLoaderMain,
				size_t                                  InActualPriority,
				const Loader::LoadRequestParams        *pInRequestParams,
				Loader::LoadCommandBase                *pInCommandBase,
				Impl::Impl_ManagedResource             *pInResource
			);
			void Log_LoaderMain_EnqueueCommandForResource
			(
				LoaderMain                             *pInLoaderMain,
				size_t                                  InActualPriority,
				const Loader::LoadRequestParams        *pInRequestParams,
				Loader::LoadCommandBase                *pInCommandBase,
				Impl::Impl_ManagedResource             *pInResource
			);
			void Log_CommandResultPopped_IfNecessary
			(
				LoaderMain                             *pInLoaderMain,
				LoadingLoopCommandQueues               *pInLoadingQueue,
				const LoaderCommandResultRecord        *pInResultRecord
			);
			void Log_CommandResultPopped
			(
				LoaderMain                             *pInLoaderMain,
				LoadingLoopCommandQueues               *pInLoadingQueue,
				const LoaderCommandResultRecord        *pInResultRecord
			);

			/*****************************************************************************************
			* LoaderMain implementation
			*****************************************************************************************/
			LoaderMain::LoaderMain(Impl::Impl_ResourceManagerSubsystem *pInSubsystem) :
				_pSubsystem(pInSubsystem),
				_loadingThreadLoop(this, "loadingThreadLoop"),
				_mainThreadLoop(this, "mainThreadLoop")
				
			{
				assert(pInSubsystem != nullptr);
			}

			void LoaderMain::Initialize(const ResManQuick_SubsystemParams& InParams)
			{				
				assert(&InParams != nullptr);

				// Skip id zero, because id zero is used for special commands:
				_commandIdGenerator.GenerateID();

				_threadedLogger.Initialize(&InParams, InParams.GetLogSettings_Threaded());

				_mainThreadLoop.Initialize(InParams, GetSubsystem()->GetLogger_MainThread());

				if(false ==InParams.IsSingleThreaded())
				{
					_loadingThreadLoop.InitializeAndStart(InParams, &_threadedLogger);
				}
			}

			void LoaderMain::UpdateRuntimeParams(const ResMan::ResourceManagerPublicRuntimeParams& InParams)
			{
				assert(&InParams != nullptr);
				_mainThreadLoop.UpdateRuntimeParams(InParams);
				_loadingThreadLoop.UpdateRuntimeParams(InParams);
			}

			void LoaderMain::EnqueueLoadCommand
			(
				const LoadCommandParams& InParams
			)
			{
				assert(&InParams != nullptr);
				auto pResource = InParams.GetResource();
				assert(pResource->GetLoader() != nullptr);

				bool bShouldPerformSynchro = GetParams().ShouldDoAllCommandsSynchro() || pResource->GetCombinableParams().IsLoadingSynchro();
				if(bShouldPerformSynchro)
				{
					LoadSynchro(InParams);
				}
				else
				{
					const bool bShouldKeepLoadingLoop = false;
					LoadingLoopCommandQueues *pLoadingLoopQueues = ChooseCommandQueues(this, pResource);
					_EnqueueLoadCommand_InQueues(InParams, pLoadingLoopQueues, bShouldKeepLoadingLoop);
				}
			}

			void LoaderMain::LoadSynchro(const LoadCommandParams& InParams)
			{
				// WARNING!!! All threaded commands must be processed to avoid resource access conflicts
				// It must be done before loading the resource synchronously!
				Wait_UntilAllThreadedCommandsProcessed();

				MainThreadTemporary_LoadingLoop loop(this, "mainThreadTemporary_LoadingLoop");
				loop.Initialize(GetSubsystem()->GetParams(), GetSubsystem()->GetLogger_MainThread());

				const bool bShouldKeepLoadingLoop = true;
				_EnqueueLoadCommand_InQueues(InParams, loop.GetCommands(), bShouldKeepLoadingLoop);

				// Process all commands to get the result
				_Process_AllCommandsAndResults_OfLoadingLoop(&loop);
			}

			void LoaderMain::_Process_AllCommandsAndResults_OfLoadingLoop(LoadingLoop *pInLoop)
			{
				auto pQueues = pInLoop->GetCommands();
				while (true)
				{
					if (pQueues->GetNumResultsToBePopped() == 0)
					{
						break;
					}
					_Process_CommandAndResult(pQueues);
				}
			}

			void LoaderMain::_Process_AllResourceCommandsAndResults_OfLoadingLoop(LoadingLoop *pInLoop, ResMan::IManagedResource *pInResource, bool bInRecursive)
			{
				Util::TODO("bInRecursive flag is not checked here");
				auto pQueues = pInLoop->GetCommands();
				auto pResourceImpl = Impl::CastTyped(pInResource)->GetImpl();
				auto pLoaderLoopResourceData = pResourceImpl->GetLoaderLoopData(pInLoop);
				while (true)
				{
					auto numUnprocessedCommands = pLoaderLoopResourceData->GetNumUnprocessedCommands();
					if (numUnprocessedCommands == 0)
					{
						break;
					}					
					_Process_CommandAndResult(pQueues);
				}
			}

			void LoaderMain::_Process_CommandAndResult(LoadingLoopCommandQueues *pInQueues)
			{
				pInQueues->GetOwnerLoadingLoop()->Tick();
				_ProcessCommandResultQueue(pInQueues);
			}

			void LoaderMain::_EnqueueLoadCommand_InQueues(const LoadCommandParams& InParams, LoadingLoopCommandQueues *pInQueues, bool bInShouldKeepTheSameQueue)
			{
				auto pResource = InParams.GetResource();
				auto pCommand = CreateLoadCommandFromParameters(InParams, pResource, bInShouldKeepTheSameQueue);
				pResource->EnqueueCommandForResource(InParams.request, InParams.priority, pCommand, pInQueues);
			}

			bool ShouldPerform_EnqueueCommandForResource
			(
				LoaderMain                                          *pInLoaderMain,
				const LoadRequestParams&                             InParams,
				size_t                                               InActualPriority,
				LoadCommandBase                                     *pInCommand,
				Impl::Impl_ManagedResource                          *pInResource,
				Loader_PerResourceData                              *pInResourceLoaderData
			)
			{
				auto combinableParams = pInResource->GetCombinableParams();
				if(combinableParams.IsAutoLoadFreezed() && false == InParams.IsManual())
				{
					return false;
				}
				if (combinableParams.IsManualLoadFreezed() && true == InParams.IsManual())
				{
					return false;
				}

				auto loadedDependencyLists = pInResource->GetLoaderData()->GetLoadedDependencyLists();
				bool bLoadingKeptDependencyLists = loadedDependencyLists.Contains(InParams.GetSubresourceRolesToLoad());
				auto loadedSubresources = pInResource->GetLoadedSubresources();
				bool bLoadingKeptSubresources = loadedSubresources.Contains(InParams.GetSubresourceRolesToLoad());
				if (bLoadingKeptDependencyLists && bLoadingKeptSubresources)
				{
					return false;
				}
				return true;
			}

			void Log_LoaderMain_EnqueueCommandForResource_IfShould
			(
				LoaderMain                             *pInLoaderMain,
				size_t                                  InActualPriority,
				const Loader::LoadRequestParams        *pInRequestParams,
				Loader::LoadCommandBase                *pInCommandBase,
				Impl::Impl_ManagedResource             *pInResource
			)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_LoaderMain_EnqueueCommandForResource(pInLoaderMain, InActualPriority, pInRequestParams, pInCommandBase, pInResource);
				}
			}
			void Log_LoaderMain_EnqueueCommandForResource
			(
				LoaderMain                             *pInLoaderMain,
				size_t                                  InActualPriority,
				const Loader::LoadRequestParams        *pInRequestParams,
				Loader::LoadCommandBase                *pInCommandBase,
				Impl::Impl_ManagedResource             *pInResource
			)
			{
				const bool bInRecordIsResult = false;
				const bool bIsVerbose = false;

				auto pManager = pInLoaderMain->GetSubsystem()->GetManager();
				auto pManagerInterface = pManager->GetInterface();
				auto pLogger = pInLoaderMain->GetSubsystem()->GetLogger_MainThread();
				Util::LogRecordScoper record_scoper(pLogger, "LoaderMain: EnqueueCommandForResource", Util::EMessageType::Status);

				Impl::Log_RequestParams(pLogger, pManagerInterface, pInRequestParams, Util::EMessageType::Status);
				Impl::Log_LoadCommand(pLogger, pManagerInterface, pInCommandBase, bInRecordIsResult, bIsVerbose);
				*pLogger << "Resource: ";
				Impl::Log_Resource_LeanAndMean(pLogger, pInResource, Util::EMessageType::Status);
				*pLogger << Util::endl;
			}
			void Log_CommandResultPopped_IfNecessary
			(
				LoaderMain                             *pInLoaderMain,
				LoadingLoopCommandQueues               *pInLoadingQueue,
				const LoaderCommandResultRecord        *pInResultRecord
			)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_CommandResultPopped(pInLoaderMain, pInLoadingQueue, pInResultRecord);
				}
			}
			void Log_CommandResultPopped
			(
				LoaderMain                             *pInLoaderMain,
				LoadingLoopCommandQueues               *pInLoadingQueue,
				const LoaderCommandResultRecord        *pInResultRecord
			)
			{
				assert(pInLoaderMain != nullptr);
				assert(pInLoadingQueue != nullptr);
				assert(pInResultRecord != nullptr);

				const bool bVerbose              = true;

				auto pSubsystem                  = pInLoaderMain->GetSubsystem();
				auto pLog                        = pSubsystem->GetLogger_MainThread();
				auto pManager                    = pInLoaderMain->GetSubsystem()->GetManager()->GetInterface();
				auto pLoop                       = pInLoadingQueue->GetOwnerLoadingLoop();

				*pLog << "Command result popped from loop: " << GetScopedLoopName(pLoop) << Util::endl;

				Log_CommandResult(pLog, pManager, pInResultRecord, bVerbose);
			}

			bool IsChildCommandForRecursive(const LoadRequestParams *pInParams, Impl::Impl_ManagedResource *pInResource)
			{
				return pInParams->GetResource() != pInResource;
			}

			// All counters that should be updated by loader main when enqueueing a new command
			// are to be updated within this function
			void EnqueueCommand_UpdateCounters
			(
				LoadingLoopCommandQueues                            *pInLoadingLoopQueues,
				const LoadRequestParams&                             InParams,
				size_t                                               InActualPriority,
				LoadCommandBase                                     *pInCommand,
				Impl::Impl_ManagedResource                          *pInResource,
				Loader_PerResourceData                              *pOutResourceLoaderData,
				Loader_PerResourceData                              *pInInitiallyRequestedResource_LoaderData
			)
			{			
				// Increment the enqueued commands for recursive commands
				if (IsChildCommandForRecursive(&InParams, pInResource))
				{
					pInInitiallyRequestedResource_LoaderData->Incr_NumRecursiveCommands_Enqueued();
				}

				// Increment counters of commands enqueued for this resource
				pOutResourceLoaderData->Incr_NumEnqueuedCommands(pInLoadingLoopQueues);
			}

			void BeginLoad_DependencyLists(Loader_PerResourceData *pOutResourceLoaderData, std::shared_ptr<ResMan::IResourceLoader>pInLoader, ResMan::SubresourceRoleSet InSubresources)
			{
				pInLoader->BeginLoadDependencyLists(InSubresources);
				pOutResourceLoaderData->IncludeDependencyListsInLoadingQueue(InSubresources);
			}
			void BeginLoad_Subresources(Loader_PerResourceData *pOutResourceLoaderData, std::shared_ptr<ResMan::IResourceLoader>pInLoader, ResMan::SubresourceRoleSet InSubresources)
			{
				pInLoader->BeginLoadDependencyLists(InSubresources);
				pOutResourceLoaderData->IncludeSubresourcesInLoadingQueue(InSubresources);
			}

			void LoaderMain::EnqueueCommandForResource
			(
				LoadingLoopCommandQueues                            *pInLoadingLoopQueues,
				const LoadRequestParams&                             InParams,
				size_t                                               InActualPriority,
				LoadCommandBase                                     *pInCommand,
				Impl::Impl_ManagedResource                          *pInResource,
				Loader_PerResourceData                              *pOutResourceLoaderData,
				Loader_PerResourceData                              *pInInitiallyRequestedResource_LoaderData
			)
			{
				if (false == ShouldPerform_EnqueueCommandForResource(this, InParams, InActualPriority, pInCommand, pInResource, pOutResourceLoaderData))
				{
					return;
				}

				auto pGroup = InParams.GetGroup();
				if(pGroup)
				{
					pGroup->Notify_Resource_LoadBegun(pInResource, InParams);
				}

				EnqueueCommand_UpdateCounters(pInLoadingLoopQueues, InParams, InActualPriority, pInCommand, pInResource, pOutResourceLoaderData, pInInitiallyRequestedResource_LoaderData);

				auto pResourceLoadingLoopData = GetLoadingLoopData(pInResource, pInLoadingLoopQueues);

				pInCommand->BeginEnqueue(pResourceLoadingLoopData->GetMainThreadData());

				// Both loading subresources and loading dependency lists to be marked loading
				BeginLoad_DependencyLists(pOutResourceLoaderData, pInResource->GetLoader(), pInCommand->GetSubresourceDependencyListsToLoad());
				BeginLoad_Subresources(pOutResourceLoaderData, pInResource->GetLoader(), pInCommand->GetSubresourcesToLoad());

				auto resourceCommandId = ResMan::LoadResourceCommandID(_commandIdGenerator.GenerateID());
				auto command = CreateRecord
				(
					resourceCommandId,
					resourceCommandId,
					InParams,
					InActualPriority,
					pInCommand,
					pInResource,
					pResourceLoadingLoopData
				);

				_mainThreadLoop.Notify_BeforeCommandEnqueued_ForOtherLoadingLoop(pInLoadingLoopQueues->GetOwnerLoadingLoop(), command);

				pInLoadingLoopQueues->Enqueue(command);

				pInCommand->EndEnqueue(InParams, pInResource);

				Log_LoaderMain_EnqueueCommandForResource_IfShould
				(
					this,
					InActualPriority,
					&InParams,
					pInCommand,
					pInResource
				);
			}

			void LoaderMain::Tick()
			{
				_mainThreadLoop.Tick();
				_ProcessCommandResultQueue(_mainThreadLoop.GetCommands());
				if(false == GetParams().IsSingleThreaded())
				{
					_ProcessLoadingThreadLoop_ResultQueue();
				}
			}

			void LoaderMain::_ProcessLoadingThreadLoop_ResultQueue()
			{
				_ProcessCommandResultQueue(_loadingThreadLoop.GetCommands());
			}

			void LoaderMain::_ProcessCommandResultQueue(LoadingLoopCommandQueues *pInLoadingLoopQueues)
			{
				auto pendingResult = pInLoadingLoopQueues->PopResult();
				if (false == pendingResult.is_initialized())
				{
					return;
				}

				Log_CommandResultPopped_IfNecessary(this, pInLoadingLoopQueues, pendingResult.get_ptr());

				auto savedLoadingDependencyLists = pendingResult->GetResource()->GetLoader()->GetDependencyList().GetLoadingSubresourceRoles();
				// The last command could be the the loading dependency lists command,
				// so the dependency lists could be loaded. So let's update the loaded dependency lists to have actual values here.
				// The reason is to update loaded dependencies with actual values.
				pendingResult->GetResource()->GetLoader()->EndLoadingDependencyLists();

				pendingResult->GetCommand()->BeginProcessDependencyLists();
				// We need to start loading dependency lists again
				pendingResult->GetResource()->GetLoader()->BeginLoadDependencyLists(savedLoadingDependencyLists);
				// Issue recursive commands as fast as we can, so let them load on the loader thread
				// while we processing the rest on the main thread
				bool bRegisteringRecursiveCommands_Succeedeed = true;
				if(pendingResult->GetCommand()->IsRecursive())
				{
					bRegisteringRecursiveCommands_Succeedeed = _ProcessRecursiveCommands(pendingResult.get(), pInLoadingLoopQueues);
				}
				pendingResult->GetCommand()->EndProcessDependencyLists();

				_ProcessMainCommand(pendingResult.get(), pInLoadingLoopQueues, bRegisteringRecursiveCommands_Succeedeed);

				// Addressable subresources to be processed after the main command has been processed:
				// because the LoadOwnContent() of the loader may update list of addressable subresources
				pendingResult->GetCommand()->ProcessAllAdressableSubresources(GetSubsystem()->GetManager()->GetResourceRepo());

				// Notify that the command  result has been processed
				pInLoadingLoopQueues->GetOwnerLoadingLoop()->Notify_CommandResultProcessed(pendingResult.get());
				// Delete result command
				DeleteCommandResultObjects(pendingResult.get());
			}

			bool IsRecursiveChildCommand(const Loader::LoaderCommandResultRecord& InResultRecord)
			{
				auto pMainThreadData = InResultRecord.GetCommandRecord().GetAttachedMainThreadData();
				bool bRecursiveChildCommand = pMainThreadData->initial_request_params.GetResource() != InResultRecord.GetResource();
				return bRecursiveChildCommand;
			}

			void CountPoppedRecursiveChildCommands_ForResource(const Loader::LoaderCommandResultRecord& InResultRecord, Loader_PerResourceData *pOutInitiallyRequestedResourceLoaderData, bool bInFailedResults)
			{
				if (IsRecursiveChildCommand(InResultRecord))
				{
					pOutInitiallyRequestedResourceLoaderData->Incr_NumRecursiveCommandResults_Popped();
					if (bInFailedResults)
					{
						pOutInitiallyRequestedResourceLoaderData->Incr_NumRecursiveCommandResults_PoppedButFailed();
					}
				}
			}

			void Notify_RecursiveEndLoad_IfNecessary
			(
				const LoadRequestParams& InInitialRequestParams,
				Impl::Impl_ManagedResource *pInManagedResource, const Loader_PerResourceData *pOutInitiallyRequestedResourceLoaderData,
				bool bInIsFailed
			)
			{
				if (pOutInitiallyRequestedResourceLoaderData->GetNumRecursiveCommandResults_ToBePopped() == 0)
				{
					pInManagedResource->Notify_RecursiveLoadEnded(InInitialRequestParams, bInIsFailed);
				}
			}


			size_t ResourceEndLoad_UpdateCounter_NumUnprocessedCommand_ForLoop(LoadingLoopCommandQueues *pInLoadingLoopQueues, Loader_PerResourceData *pOutResourceLoaderData)
			{
				auto pLoaderLoopResourceData = pOutResourceLoaderData->GetLoaderLoopData(pInLoadingLoopQueues->GetOwnerLoadingLoop());
				auto numUnprocessedCommands = pLoaderLoopResourceData->DecrementUnprocessedCommands();
				return numUnprocessedCommands;
			}

			size_t ResourceEndLoad_UpdateCommandsToBePoppedFromAllQueues_AndNotifyResourceIfZeroed(Impl::Impl_ManagedResource *pInResource, LoadingLoopCommandQueues *pInLoadingLoopQueues, Loader_PerResourceData *pOutResourceLoaderData)
			{
				pOutResourceLoaderData->Incr_NumPoppedCommands(pInLoadingLoopQueues);
				auto totalCommands_ForResource_toBePoppedFromAllQueues = pOutResourceLoaderData->GetTotalCommands_ToBePopped_FromAllLoops();
				if (0 == totalCommands_ForResource_toBePoppedFromAllQueues)
				{
					// Notify here
					pInResource->Notify_TotalCommandsToBePoppedZeroed();
				}
				return totalCommands_ForResource_toBePoppedFromAllQueues;
			}

			void ResourceEndLoad_UpdateCommandCounters_AndNotifyResourceIfTotalCommandsToBePoppedZeroed(Impl::Impl_ManagedResource *pInResource, LoadingLoopCommandQueues *pInLoadingLoopQueues, Loader_PerResourceData *pOutResourceLoaderData)
			{
				ResourceEndLoad_UpdateCounter_NumUnprocessedCommand_ForLoop(pInLoadingLoopQueues, pOutResourceLoaderData);
				ResourceEndLoad_UpdateCommandsToBePoppedFromAllQueues_AndNotifyResourceIfZeroed(pInResource, pInLoadingLoopQueues, pOutResourceLoaderData);
			}

			void Process_EndLoad_GroupResource(const Loader::LoaderCommandResultRecord *pInResultRecord, bool bResourceLoadFailed)
			{
				auto pGroup = pInResultRecord->GetGroup();
				assert(pGroup != nullptr);
				auto pInitiallyRequestedResource = pInResultRecord->GetAttachedMainThreadData()->initial_request_params.GetResource();
				pGroup->Notify_GroupResourceLoaded(pInitiallyRequestedResource, pInResultRecord->GetResource(), bResourceLoadFailed);
				// WARNING!!! The group must update count of loaded resource itself, groups are NOT managed by the loader main.
			}


			void ClearLoading_SubresourcesAndDependencyLists(std::shared_ptr<ResMan::IResourceLoader>pInLoader, Loader_PerResourceData *pOutResourceLoaderData)
			{
				pInLoader->EndLoadingDependencyLists();
				pOutResourceLoaderData->ClearLoadingSubresources();
			}

			void LoaderMain::EndLoad
			(
				LoadingLoopCommandQueues                                *pInLoadingLoopQueues,
				const Loader::LoaderCommandResultRecord&                 InResultRecord, 
				Loader_PerResourceData                                  *pOutResourceLoaderData,
				Loader_PerResourceData                                  *pOutInitiallyRequestedResourceLoaderData,
				bool                                                     bInRecursiveCommandsSucceeded
			)
			{
				const auto& loadUpdate = InResultRecord.GetCommand()->GetLoadUpdate_AfterExecutedOnThreadOfLoadingLoop();

				auto pLoaderLoopResourceData = pOutResourceLoaderData->GetLoaderLoopData(pInLoadingLoopQueues->GetOwnerLoadingLoop());

				ResourceEndLoad_UpdateCommandCounters_AndNotifyResourceIfTotalCommandsToBePoppedZeroed(InResultRecord.GetResource(), pInLoadingLoopQueues, pOutResourceLoaderData);

				size_t totalCommands_ForResource_ToBePoppedFromAllQueues = pOutResourceLoaderData->GetTotalCommands_ToBePopped_FromAllLoops();
				if (totalCommands_ForResource_ToBePoppedFromAllQueues == 0)
				{
					ClearLoading_SubresourcesAndDependencyLists(InResultRecord.GetResource()->GetLoader(), pOutResourceLoaderData);
				}

				// If loading failed, then we already failed
				bool bFailed                                                    = InResultRecord.GetCommand()->HasLoadFailed();

				// We should try to create object only if the object must be created according to resource type params
				auto pResourceType                                              = ResMan::GetResourceType(InResultRecord.GetResource()->GetInterface());
				auto pRealResourceObjectRole                                    = pResourceType->GetParams().GetSubresourceRoles().GetRealResourceSubresourceRole();
				
				bool bShouldCreateRealResource = InResultRecord.GetCommand()->GetSubresourcesToLoad().IsSet(pRealResourceObjectRole);

				// By default, new resource object is old resource object 
				void *pNewResourceObject = InResultRecord.GetResource()->GetRealResource();
				if (bShouldCreateRealResource)
				{
					if (false == bFailed)
					{
						// Create resource
						pNewResourceObject = InResultRecord.GetCommand()->CreateNewResource(InResultRecord.GetResource()->GetRealResource());
						if (InResultRecord.GetCommand()->HasCreationFailed())
						{
							bFailed = true;
							pOutResourceLoaderData->IncludeFailedLoadedSubresources(ResMan::SubresourceRoleSet{ pRealResourceObjectRole->GetRoleIndex() });
						}
					}
				}
			
				if (bFailed)
				{
					pOutInitiallyRequestedResourceLoaderData->SetSomeCommandInQueueFailedRecursively(true);
				}

				// Count child commands
				CountPoppedRecursiveChildCommands_ForResource(InResultRecord, pOutInitiallyRequestedResourceLoaderData, bFailed);

				// update loaded subresources
				pOutResourceLoaderData->IncludeLoadedSubresources(loadUpdate.reallyLoaded);
				pOutResourceLoaderData->IncludeLoadedDependencyLists(loadUpdate.reallyLoadedDependencyLists);
				pOutResourceLoaderData->IncludeFailedLoadedSubresources(loadUpdate.GetLoadedFailed());
				pOutResourceLoaderData->IncludeFailedLoadedDependencyLists(loadUpdate.GetLoadedDependencyListsFailed());

				// Notify the main thread loader loop that command has been processed by the other loading loop
				_mainThreadLoop.Notify_CommandResultProcessed_ForOtherLoadingLoop(pInLoadingLoopQueues->GetOwnerLoadingLoop(), InResultRecord);
				size_t numCommandsToBePoppedForThisLoop = pLoaderLoopResourceData->GetNumUnprocessedCommands();
				if (numCommandsToBePoppedForThisLoop == 0)
				{
					// The loader on the main thread
					// must be notified when the loader thread has no more unprocessed commands linked with the given resource
					_mainThreadLoop.Notify_AllCommandsProcessed_ByOtherLoadingLoop(pInLoadingLoopQueues->GetOwnerLoadingLoop(), pOutResourceLoaderData);
				}

				auto bFailedRecursive = bFailed || pOutInitiallyRequestedResourceLoaderData->HasSomeCommandInQueueFailedRecursively();
				// Notify about end load
				auto initiallyRequestedParams = InResultRecord.GetAttachedMainThreadData()->initial_request_params;
				InResultRecord.GetResource()->Notify_LoadEnded(initiallyRequestedParams, loadUpdate, pNewResourceObject);
				Notify_RecursiveEndLoad_IfNecessary(initiallyRequestedParams, initiallyRequestedParams.GetResource(), pOutInitiallyRequestedResourceLoaderData, bFailedRecursive);

				// Last of all - perform actions on group
				if(InResultRecord.GetGroup() != nullptr)
				{
					auto pResource = InResultRecord.GetResource();
					auto pInitiallyRequestedParams = &initiallyRequestedParams;
					bool bArgFailed = bFailed;
					if (pOutInitiallyRequestedResourceLoaderData->GetTotalCommands_ToBePopped_FromAllLoops() == 0)
					{
						bArgFailed = bFailedRecursive;
					}
					InResultRecord.GetGroup()->Notify_GroupResourceLoaded(pInitiallyRequestedParams->GetResource(), pResource, bArgFailed);
				}
			}

			void LoaderMain::Wait_UntilAllCommandsProcessed()
			{
				Wait_UntilAllThreadedCommandsProcessed();
				_Process_AllCommandsAndResults_OfLoadingLoop(&_mainThreadLoop);
			}

			void LoaderMain::Wait_UntilAllResourceCommandsProcessed(ResMan::IManagedResource *pInResource, bool bInRecursive)
			{
				Wait_UntilAllThreadedCommandsForResourceProcessed(pInResource, bInRecursive);
				_Process_AllResourceCommandsAndResults_OfLoadingLoop(&_mainThreadLoop, pInResource, bInRecursive);
			}

			void LoaderMain::Wait_UntilAllGroupCommandsProcessed(ResMan::IManagedGroup *pInGroup, bool bInRecursive)
			{
				auto pGroupImpl = Impl::CastTyped(pInGroup)->GetImpl();
				while(true)
				{
					auto numResourcesToBeTriedToLoad = pGroupImpl->GetCommandState().counters.GetNumResourcesToBeTriedToLoad();
					if (0 == numResourcesToBeTriedToLoad)
					{
						break;
					}
					if(false == GetParams().IsSingleThreaded())
					{
						_Process_CommandAndResult(_loadingThreadLoop.GetCommands());
					}
					_Process_CommandAndResult(_mainThreadLoop.GetCommands());
				}
			}

			void LoaderMain::Wait_UntilAllThreadedCommandsProcessed()
			{
				if (false == GetParams().IsSingleThreaded())
				{
					auto pCommands = _loadingThreadLoop.GetCommands();
					while (true)
					{
						if (pCommands->GetNumResultsToBePopped() == 0)
						{
							break;
						}
						_ProcessLoadingThreadLoop_ResultQueue();
					}
				}
			}

			void LoaderMain::Wait_UntilAllThreadedCommandsForResourceProcessed(ResMan::IManagedResource *pInResource, bool bInRecursive)
			{
				_Process_AllResourceCommandsAndResults_OfLoadingLoop(_loadingThreadLoop.GetLoop(), pInResource, bInRecursive);
			}

			void LoaderMain::_ProcessMainCommand(const Loader::LoaderCommandResultRecord& InCommandResult, LoadingLoopCommandQueues *pOutCommands, bool bInRecursiveCommandsSucceeded)
			{
				InCommandResult.GetResource()->EndLoad(InCommandResult, pOutCommands, bInRecursiveCommandsSucceeded);
			}

			bool LoaderMain::_ProcessRecursiveCommands
			(
				const Loader::LoaderCommandResultRecord&                            InCommandResult,
				LoadingLoopCommandQueues                                           *pInLoadingLoopQueues
			)
			{
				bool bSomeRecursiveCommandFailed = false;
				while (true)
				{
					bool bNotEnd = ProcessNextRecursiveCommand(InCommandResult, pInLoadingLoopQueues, &bSomeRecursiveCommandFailed);
					if (!bNotEnd)
					{
						break;
					}
				}

				return false == bSomeRecursiveCommandFailed;
			}

			const ResManQuick_SubsystemParams& LoaderMain::GetParams() const
			{
				return _pSubsystem->GetParams();
			}

			LoadingLoopCommandQueues *ChooseRecursiveCommandQueues(LoadingLoopCommandQueues *pInParentCommandQueues, const Loader::LoaderCommandResultRecord& InCommandResult)
			{
				if (InCommandResult.GetCommand()->ShouldKeepLoadingLoop())
				{
					return pInParentCommandQueues;
				}
				else
				{
					return ChooseCommandQueues(GetOwnerLoaderMain(pInParentCommandQueues), InCommandResult.GetResource());
				}
			}

			LoadingLoopCommandQueues *ChooseCommandQueues(LoaderMain *pInLoaderMain, Impl::Impl_ManagedResource *pInResource)
			{
				if (pInLoaderMain->GetParams().IsSingleThreaded() || ! ResMan::CanBeThreaded(pInResource->GetLoader().get()))
				{
					return pInLoaderMain->GetLoadingLoop_MainThread()->GetCommands(); 
				}
				else
				{
					return pInLoaderMain->GetLoadingThread()->GetCommands();
				}
			}

			/*****************************************************************************************
			* Utility functions implementation
			*****************************************************************************************/
			LoadCommandBase *CreateLoadCommandFromParameters
			(
				const LoadCommandParams&           InParams,
				Impl::Impl_ManagedResource        *pInResource,
				bool                               bInShouldKeepLoadingLoop
			)
			{
				if (InParams.request.ShouldLoadOnlyDepenendencyLists())
				{
					auto pLoadCommand = new LoadDependencyListsCommand(pInResource->GetLoader(), InParams.request.GetSubresourceRolesToLoad(), InParams.bPerformRecursive, bInShouldKeepLoadingLoop);
					return pLoadCommand;
				}
				else
				{
					auto pLoadCommand = new LoadSubresourcesCommand(pInResource->GetLoader(), InParams.request.GetSubresourceRolesToLoad(), InParams.bPerformRecursive, bInShouldKeepLoadingLoop);
					return pLoadCommand;
				};

			}

			LoaderCommandRecord CreateRecord
			(
				ResMan::LoadResourceCommandID                      InResourceCommandID,
				ResMan::LoadResourceCommandID                      InInitialResourceCommandID,
				const LoadRequestParams&                           InInitialRequestParams,
				size_t                                             InActualPriority,
				LoadCommandBase                                   *pInCommand,
				Impl::Impl_ManagedResource                        *pInResource,
				Loader_PerResource_PerLoadingLoop                 *pInLoaderLoopData
			)
			{
				auto pCommandMainThreadData = new LoaderCommandMainThreadData();
				pCommandMainThreadData->initial_request_params = InInitialRequestParams;
				pCommandMainThreadData->pResource = pInResource;

				LoaderCommandRecord load_command (InResourceCommandID, InInitialResourceCommandID, InActualPriority, pInCommand, pCommandMainThreadData, pInLoaderLoopData);
				return load_command;
			}

			void DeleteCommandResultObjects(const Loader::LoaderCommandResultRecord& InResultRecord)
			{
				delete InResultRecord.GetCommand();
				delete InResultRecord.GetAttachedMainThreadData();
			}

			void EnqueueRecursiveCommand(const Loader::LoaderCommandResultRecord& InResultRecord, LoadingLoopCommandQueues *pInLoadingLoopQueues, ManagedResource *pRecursiveResource)
			{
				auto pRecursiveLoadCommand          = InResultRecord.GetCommand()->CreateNewChildCommand(InResultRecord);
				auto actualPriority                 = ComputeRecursiveCommandPriorityForResource(InResultRecord, pRecursiveResource->GetImpl());
				auto pRecursiveCommandQueues        = ChooseRecursiveCommandQueues(pInLoadingLoopQueues, InResultRecord);

				EnqueueCommandForResource(InResultRecord, pRecursiveResource->GetImpl(), actualPriority, pRecursiveLoadCommand, pRecursiveCommandQueues);
			}

			bool ProcessNextRecursiveCommand
			(
				const Loader::LoaderCommandResultRecord&                   InResultRecord,
				LoadingLoopCommandQueues                                  *pInLoadingLoopQueues,
				bool                                                      *pbOutIsFailed
			)
			{
				*pbOutIsFailed = false;

				auto pLoaderMain = GetOwnerLoaderMain(pInLoadingLoopQueues);
				auto pRecursiveResource = InResultRecord.GetCommand()->ProcessNextRelevantDependency(Impl::GetResourceRepo(pLoaderMain->GetSubsystem()), pbOutIsFailed);
				if (pRecursiveResource == nullptr)
				{
					return false;
				}
				EnqueueRecursiveCommand(InResultRecord, pInLoadingLoopQueues, pRecursiveResource);
				return true;
			}
			size_t ComputeRecursiveCommandPriorityForResource(const Loader::LoaderCommandResultRecord& InResultRecord, Impl::Impl_ManagedResource *pInResource)
			{
				return InResultRecord.GetActualPriority();
			}

			void EnqueueCommandForResource
			(
				const Loader::LoaderCommandResultRecord&         InResultRecord,
				Impl::Impl_ManagedResource                      *pInResource,
				size_t                                           InActualPriority,
				Loader::LoadCommandBase                         *pInCommand,
				LoadingLoopCommandQueues                                  *pInLoadingLoopQueues
			)
			{
				pInResource->EnqueueCommandForResource
				(
					InResultRecord.GetAttachedMainThreadData()->initial_request_params,
					InActualPriority,
					pInCommand,
					pInLoadingLoopQueues
				);
			}

			/*************************************************************************************************
			* LoadingLoopQueues access functions impl
			*************************************************************************************************/
			LoaderMain *GetOwnerLoaderMain(LoadingLoopCommandQueues *pInLoadingLoopQueues)
			{
				return pInLoadingLoopQueues->GetOwnerLoadingLoop()->GetOwnerMain();
			}
		}
	} // ResManQuick
} // Dv