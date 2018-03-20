#include "../../PrivateH/Loader/ResManQuick_impl_utils_LoadingLoopCommandQueues.h"
#include "../../PrivateH/ResManQuick_impl_ResourceRepository.h"
#include "../../PrivateH/ResManQuick_impl_access_ManagedResource.h"
#include "../../PrivateH/Loader/ResManQuick_impl_Loader_PerResourceData.h"
#include "../../PrivateH/Log/ResManQuick_impl_Log.h"

#include <memory>

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			/*********************************************************************************************
			* LoadingLoopCommandQueues implementation
			*********************************************************************************************/
			LoadingLoopCommandQueues::LoadingLoopCommandQueues(LoadingLoop *pInOwnerLoadingLoop) :
				_pOwnerLoadingLoop(pInOwnerLoadingLoop)
			{
			}

			void LoadingLoopCommandQueues::Enqueue(const LoaderCommandRecord& InCommandRecord)
			{
				_numEnteredCommands++;
				_commands.Enqueue(InCommandRecord);
			}

			boost::optional<LoaderCommandRecord> LoadingLoopCommandQueues::PopCommand()
			{
				return _commands.Pop();
			}

			boost::optional<LoaderCommandResultRecord> LoadingLoopCommandQueues::PopResult()
			{
				++_numPoppedCommands;
				return _results.Pop();
			}

			void LoadingLoopCommandQueues::EnqueueResult(const LoaderCommandResultRecord& InResultRecord)
			{
				return _results.Enqueue(InResultRecord);
			}

			/*********************************************************************************************
			* LoaderCommandResultRecord implementation
			*********************************************************************************************/
			Loader_PerResource_PerLoadingLoop *LoaderCommandResultRecord::GetResource_LoadingLoop() const
			{ 
				return const_cast<Loader_PerResource_PerLoadingLoop *>(static_cast<const Loader_PerResource_PerLoadingLoop *>(_commandRecord.GetResource_LoadingLoop())); 
			}
			Loader_PerResource_ForLoadingLoopThread *LoaderCommandResultRecord::GetResource_LoadingLoopThreadData() const
			{ 
				return static_cast<Loader_PerResource_ForLoadingLoopThread *>(GetResource_LoadingLoop()->GetPerResourceData_ForLoadingLoopThread());
			}

			/*********************************************************************************************
			* LoaderCommandRecord implementation
			*********************************************************************************************/
			LoaderCommandRecord::LoaderCommandRecord
			(
				ResMan::LoadResourceCommandID                     InCommandID,
				ResMan::LoadResourceCommandID                     InInitialRequestCommandID,
				size_t                                            InActualPriority,
				LoadCommandBase                                  *pInCommand,
				LoaderCommandMainThreadData                      *pInMainThreadData,
				Loader_PerResource_PerLoadingLoop                 *pInLoopData
			)
			{
				_resourceCommandId                               = InCommandID;
				_initialResourceCommandId                        = InInitialRequestCommandID;

				_actualPriority                                  = InActualPriority;
				_pAttachedMainThreadData                         = pInMainThreadData;
				_pLoaderLoopData                                 = pInLoopData;
				_pCommand                                        = pInCommand;
			}

			LoaderCommandRecord::LoaderCommandRecord
			(
				ResMan::LoadResourceCommandID                     InCommandID,
				const LoaderCommandResultRecord&                  InResultRecord,
				size_t                                            InActualPriority,
				LoadCommandBase                                  *pInCommand
			) : LoaderCommandRecord
				(
					InCommandID,
					InResultRecord.GetCommandRecord().GetResourceCommandID(),
					InActualPriority, 
					pInCommand,
					const_cast<LoaderCommandMainThreadData *>(InResultRecord.GetAttachedMainThreadData()),
					InResultRecord.GetResource_LoadingLoop()
				)
			{
			}

			const Loader_PerResource_ForLoadingLoopThread *LoaderCommandRecord::GetResource_LoadingLoopThreadData() const
			{ 
				return static_cast<const Loader_PerResource_ForLoadingLoopThread *>(GetResource_LoadingLoop()->GetPerResourceData_ForLoadingLoopThread());
			}

			Impl::Impl_ManagedGroup *LoaderCommandRecord::GetGroup() const
			{ 
				return _pAttachedMainThreadData->initial_request_params.GetGroup(); 
			}

			/*********************************************************************************************
			* UnlockResourceCommand implementation
			*********************************************************************************************/
			UnlockResourceCommand::UnlockResourceCommand
			(
				size_t                                                InNumUnlocks
			) :
				LoadCommandBase(nullptr, ResMan::SubresourceRoleSet(), false, true),
				_numUnlocks(InNumUnlocks)
			{
			}

			void UnlockResourceCommand::Execute_OnLoadingLoopThread()
			{
				PerformUnlock();
			}

			void UnlockResourceCommand::PerformUnlock()
			{
				assert(IsLoadingLoopThreadContext());
				GetPerResourceData_ForLoadingLoopThread()->SubtractNumLocks_FromUsingByThisLoop(_numUnlocks);
			}

			/*********************************************************************************************
			* LoadSubresourcesCommand implementation
			*********************************************************************************************/
			LoadSubresourcesCommand::LoadSubresourcesCommand
			(
				std::shared_ptr<ResMan::IResourceLoader>            pInLoader,
				ResMan::SubresourceRoleSet                          InSubresourcesToLoad,
				bool                                                bInRecursive,
				bool                                                bInShouldKeepLoadingLoop
			) : LoadCommandBase(pInLoader, InSubresourcesToLoad, bInRecursive, bInShouldKeepLoadingLoop)
			{
			}

			void LoadSubresourcesCommand::Execute_OnLoadingLoopThread()
			{
				assert(IsLoadingLoopThreadContext());
				_SaveStateBeforeUpdate();
				GetLoader()->LoadTemporaryOwnContent(GetSubresourcesToLoad());
				_RecalculateUpdateSinceLastCall();
			}
			bool LoadSubresourcesCommand::HasLoadFailed() const
			{
				auto failedSubresourceRoles = GetLoader()->GetSubresourceRoles_FailedLoading();
				auto subresourceRolesToBeLoaded_ButFailed = failedSubresourceRoles;
				subresourceRolesToBeLoaded_ButFailed.IntersectWith(GetSubresourcesToLoad());
				bool bFailed = false == subresourceRolesToBeLoaded_ButFailed.IsEmpty();
				return bFailed;
			}

			bool LoadSubresourcesCommand::HasCreationFailed() const
			{
				return GetLoader()->HasCreationFailed();
			}

			LoadCommandBase *LoadSubresourcesCommand::CreateNewChildCommand(const LoaderCommandResultRecord& InResultRecord)
			{
				if (_GetDependencyLoader() == nullptr)
				{
					return nullptr;
				}

				auto pCommand = new LoadSubresourcesCommand(_GetDependencyLoader(), GetSubresourcesToLoad(), IsRecursive(), ShouldKeepLoadingLoop());
				return pCommand;
			}

			/*********************************************************************************************
			* LoadDependencyListsCommand implementation
			*********************************************************************************************/
			LoadDependencyListsCommand::LoadDependencyListsCommand
			(
				std::shared_ptr<ResMan::IResourceLoader>     pInLoader,
				ResMan::SubresourceRoleSet                   InSubresourcesToLoad,
				bool                                         bInRecursive,
				bool                                         bInShouldKeepLoadingLoop
			) : LoadCommandBase(pInLoader, InSubresourcesToLoad, bInRecursive, bInShouldKeepLoadingLoop)
			{
			}

			bool LoadDependencyListsCommand::HasLoadFailed() const
			{
				auto failedDependencyLists = GetLoader()->GetSubresourceRoles_FailedLoadingDependencies();
				auto subresourceRolesToBeLoaded_ButFailed = failedDependencyLists;
				subresourceRolesToBeLoaded_ButFailed.IntersectWith(GetSubresourcesToLoad());
				bool bFailed = false == subresourceRolesToBeLoaded_ButFailed.IsEmpty();
				return bFailed;
			}

			void LoadDependencyListsCommand::Execute_OnLoadingLoopThread()
			{
				assert(IsLoadingLoopThreadContext());
				_SaveStateBeforeUpdate();
				GetLoader()->LoadDirectDepdendencyListOf(GetSubresourceDependencyListsToLoad());
				_RecalculateUpdateSinceLastCall();
			}

			LoadCommandBase *LoadDependencyListsCommand::CreateNewChildCommand(const LoaderCommandResultRecord& InResultRecord)
			{
				if (_GetDependency() == nullptr)
				{
					return nullptr;
				}

				auto pCommand = new LoadDependencyListsCommand(_GetDependencyLoader(), GetSubresourceDependencyListsToLoad(), IsRecursive(), ShouldKeepLoadingLoop());
				return pCommand;
			}

			/*********************************************************************************************
			* LoadCommandBase implementation
			*********************************************************************************************/
			LoadCommandBase::LoadCommandBase(std::shared_ptr<ResMan::IResourceLoader> pInLoader, ResMan::SubresourceRoleSet InSubresourcesToLoad, bool bInRecursive, bool bInShouldKeepLoadingLoop) :
				_pLoader(pInLoader),
				_pPerResource_ForLoadingLoopThread(nullptr),
				_subresourcesToLoad(InSubresourcesToLoad),
				_bFirstDependency(true),
				// WARNING!!! DependencyEnumerator should not be initialized here, because dependency lisy may be not preloaded yet,
				// It's to be initialized before starting to process command's result
				// (it's initialized here only to make it compilable)
				_dependencyEnumerator(_GetDependencyList(), InSubresourcesToLoad.GetEnumerator()),
				_bRecursive(bInRecursive),
				_bKeepLoadingLoop(bInShouldKeepLoadingLoop)
			{
			}

			void LoadCommandBase::BeginProcessDependencyLists()
			{
				_dependencyEnumerator = ResMan::DependencyEnumerator(_GetDependencyList(), _subresourcesToLoad.GetEnumerator());
			}
			void LoadCommandBase::EndProcessDependencyLists()
			{
				Util::EmptyBlockVoid("Empty");
			}

			const ResMan::ResourceLoaderParams& LoadCommandBase::GetLoaderParams() const
			{
				return _pLoader->GetParams();
			}

			void *LoadCommandBase::CreateNewResource(void *pOldResource)
			{
				return _pLoader->CreateResourceAndGet(pOldResource);
			}

			void LoadCommandBase::ProcessAllAdressableSubresources(Impl::ResourceRepository *pInRepo)
			{
				auto pLoader = GetLoader();
				ResMan::AdressableSubresourceEnumerator enumerator(GetLoader().get(), GetLoader()->GetAddressableSubresourceRoles().GetEnumerator());
				while (true)
				{
					if (enumerator.IsEnd())
					{
						break;
					}

					auto subresourceRoleIndex                                = enumerator.GetSubresourceRoleIndex();
					auto pSubresourceParams                                 = enumerator.GetCurrentSubresourceParams();
					auto adressableSubresourceIndex                         = enumerator.GetAddressableSubresourceIndex();

					auto pRegisteredAddressableSubresource = pInRepo->RegisterAddressableResource(pLoader, pSubresourceParams, adressableSubresourceIndex);
					GetLoader()->LinkAdressableSubresourceTo(subresourceRoleIndex, adressableSubresourceIndex, pRegisteredAddressableSubresource);

					if(nullptr == pRegisteredAddressableSubresource->GetResource())
					{
						const auto pOldAdressableSubresource = nullptr;
						auto pNewRealResource = GetLoader()->LoadCreateAndGetAddressableSubresource(subresourceRoleIndex, adressableSubresourceIndex, pOldAdressableSubresource);
						pRegisteredAddressableSubresource->GetImpl()->Notify_LoadedAsAdressableSubresource(pNewRealResource);
					}

					enumerator.MoveNext();
				}
			}

			void LoadCommandBase::BeginEnqueue(Loader_PerResource_ForMainThread *pOutMainThreadData)
			{
				pOutMainThreadData->IncrUnprocessedCommands();
			}
			
			void LoadCommandBase::EndEnqueue(const LoadRequestParams& InParams, Impl::Impl_ManagedResource *pInResource)
			{
				pInResource->Notify_LoadBegun(InParams);
			}

			void LoadCommandBase::BeginProcessing_OnLoadingLoopThread(Loader_PerResource_ForLoadingLoopThread *pInPerfResource_ForLoadingLoopThread)
			{
				assert(pInPerfResource_ForLoadingLoopThread != nullptr);
				_pPerResource_ForLoadingLoopThread = pInPerfResource_ForLoadingLoopThread;
			}

			void LoadCommandBase::EndProcessing_OnLoadingLoopThread()
			{
				_pPerResource_ForLoadingLoopThread = nullptr;
			}
			
			void LoadCommandBase::_SaveStateBeforeUpdate()
			{
				_loadedRoles_before_Execute_OnLoadingLoopThread                          = GetLoader()->WhatOwnTemporaryContentLoaded();
			}
			
			void LoadCommandBase::_RecalculateUpdateSinceLastCall()
			{
				auto failedLoadedDependencyLists = GetLoader()->GetSubresourceRoles_FailedLoadingDependencies();
				auto failedLoadedSubresources = GetLoader()->GetSubresourceRoles_FailedLoading();

				// WARNING!!! Loading dependency lists are not yet copied to loaded dependency lists;
				// So we cannot compute update as "what loaded before subtract what loaded after",
				// and should return result directly
				auto loadedDependencyLists = GetSubresourceDependencyListsToLoad();
				loadedDependencyLists.Subtract(failedLoadedDependencyLists);

				auto loadedSubresources                                         = GetLoader()->WhatOwnTemporaryContentLoaded();

				loadedSubresources.Subtract(_loadedRoles_before_Execute_OnLoadingLoopThread);

				_updateSinceLastCall.reallyLoaded                               = loadedSubresources;
				_updateSinceLastCall.reallyLoadedDependencyLists                = loadedDependencyLists;
				_updateSinceLastCall.loadFailed                                 = failedLoadedSubresources;
				_updateSinceLastCall.loadDependencyListFailed                   = failedLoadedDependencyLists;
			}

			const ResMan::ResourceDependencyList *LoadCommandBase::_GetDependencyList() const
			{
				return GetLoader() != nullptr ? &(GetLoader()->GetDependencyList()) : nullptr;
			}

			ManagedResource *LoadCommandBase::ProcessNextRelevantDependency(Impl::ResourceRepository *pInRepo, bool *pbOutRegistrationFailed)
			{
				*pbOutRegistrationFailed = false;

				if(!IsFirstDependency())
				{
					_dependencyEnumerator.MoveNext();
				}
				else
				{
					SetFirstDependencyVisited();
				}

				if (_dependencyEnumerator.IsEnd())
				{
					return nullptr;
				}

				auto pDependency                                   = _dependencyEnumerator.GetDependency();
				auto pDesiredDependencyLoader                      = pDependency->GetDesiredLoader();
				auto pDependencyResourceType                       = pDesiredDependencyLoader->GetMainResourceParams().GetResourceType();

				if(pDependency->GetManagedResource() == nullptr)
				{
					auto resourceParams = pDesiredDependencyLoader->GetMainResourceParams().GetPublicResourceParams();
					auto pRegisteredDependencyResource = pInRepo->RegisterResource(resourceParams, pDesiredDependencyLoader, pDependencyResourceType, ResMan::AddressableSubresourceIndex_GetNull());

					if (nullptr == pRegisteredDependencyResource)
					{
						*pbOutRegistrationFailed = true;
					}
					else
					{
						GetLoader()->LinkDependencyTo(_dependencyEnumerator.GetSubresourceRoleIndex(), _dependencyEnumerator.GetDependencyIndex(), pRegisteredDependencyResource);
					}

					return pRegisteredDependencyResource;
				}
				else
				{
					return Impl::CastTyped(pDependency->GetManagedResource());
				}
			}

			std::shared_ptr<ResMan::IResourceLoader>LoadCommandBase::_GetDependencyLoader() const 
			{ 
				// The loader to be used for the current dependency can be different from the desired loader
				return Impl::GetLoader(_GetDependency()->GetManagedResource());
			}

			/*********************************************************************************************
			* LoaderCommandQueue implementation
			*********************************************************************************************/
			void LoaderCommandQueue::Enqueue(const LoaderCommandRecord& InRec)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.push(InRec);
			}
			boost::optional<LoaderCommandRecord> LoaderCommandQueue::Pop()
			{
				std::lock_guard<std::mutex> lock(_mutex);
				if (_queue.size() > 0)
				{
					auto top = _queue.top();
					_queue.pop();
					return top;
				}
				else
				{
					return boost::optional<LoaderCommandRecord>();
				}
			}

			/*********************************************************************************************
			* LoaderCommandResultQueue implementation
			*********************************************************************************************/
			void LoaderCommandResultQueue::Enqueue(const LoaderCommandResultRecord& InRec)
			{
				std::lock_guard<std::mutex> lock(_mutex);
				_queue.push(InRec);
			}
			boost::optional<LoaderCommandResultRecord> LoaderCommandResultQueue::Pop()
			{
				std::lock_guard<std::mutex> lock(_mutex);
				if (_queue.size() > 0)
				{
					auto top = _queue.front();
					_queue.pop();
					return top;
				}
				else
				{
					return boost::optional<LoaderCommandResultRecord>();
				}
			}

			/*********************************************************************************************
			* LoaderCommandRecord implementation
			*********************************************************************************************/
			bool operator<(const LoaderCommandRecord& lhs, const LoaderCommandRecord& rhs)
			{
				return lhs.GetActualPriority() < rhs.GetActualPriority();
			}

			/***************************************************************************************************************
			* logging implementation
			***************************************************************************************************************/
			void Log_CommandResult
			(
				Util::ILoggingDevice                   *pLog,
				ResMan::IResourceManager               *pInManager,
				const LoaderCommandResultRecord        *pInResultRecord,
				bool bIsVerbose
			)
			{
				const bool bIsResult = true;

				const auto initialRequestParams = pInResultRecord->GetAttachedMainThreadData()->initial_request_params;

				*pLog << "Initial request: " << Util::endl;
				Impl::Log_RequestParams(pLog, pInManager, &initialRequestParams, Util::EMessageType::Info);
				*pLog << "ActualPriority: " << pInResultRecord->GetActualPriority() << Util::endl;
				Impl::Log_LoadCommand(pLog, pInManager, pInResultRecord->GetCommand(), bIsResult, bIsVerbose);
				*pLog << "Current resource: ";
				Impl::Log_Resource_LeanAndMean(pLog, pInResultRecord->GetResource(), Util::EMessageType::Info);
				*pLog << Util::endl;
			}
		} // Loader
	} // ResManQuick
} // Dv