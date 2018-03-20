#include "../PrivateH/ResManQuick_impl_access_ManagedResource.h"
#include "../PrivateH/Loader/ResManQuick_impl_LoaderMain.h"
#include "../PrivateH/ResManQuick_impl_ResourceGroupLink.h"
#include "../PrivateH/ResManQuick_impl_ResourceGroupCombiCont.h"
#include "../PrivateH/ResManQuick_impl_access_ManagedGroup.h"
#include "../PrivateH/ResManQuick_impl_access_ResourceManagerSubsystem.h"
#include "../PrivateH/ResManQuick_impl_access_ResourceManager.h"
#include "../PrivateH/Loader/ResManQuick_impl_LoadUpdate.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			// Group enumerator for enumerating active groups
			using TypedActiveGroupEnumerator = PtrContGroupEnumerator<const ResourceGroupCombi_SequentialGroupCont>;

			/************************************************************************************
			* utils fwd declarations
			************************************************************************************/
			// Get enumerator of active groups for group combi that can be nullptr.
			ResMan::IManagedResource::GroupEnumerator GetActiveGroupEnumerator(ResourceGroupCombi *pInNullOrGroupCombi);

			bool ShouldReloadSubresources_BasedOnParams(Impl_ManagedResource *pInResource, const ResMan::CombinableParams& InReferenceParams);
			bool ShouldReloadDependencyLists_BasedOnParams(Impl_ManagedResource *pInResource, const ResMan::CombinableParams& InReferenceParams);

			/************************************************************************************
			* Impl_ManagedResource implementation
			************************************************************************************/
			Impl_ManagedResource::~Impl_ManagedResource()
			{
				// WARNING!!! We can NOT access the resource manager here - 
				// because it resource manager is destroyed earlier than resources when destroying container of resources.
				DestroyRealResource();
			}

			Impl_ManagedResource::Impl_ManagedResource
			(
				ManagedResource                                             *pInResource,
				Impl_ResourceManager                                        *pInOwnerManager,
				const ResMan::ResourceParams&                                InParams, 
				size_t                                                       InID,
				ResourceContainer_Iterator                                   InIter, 
				ResourceContainer_Iterator                                   InIterRoleResource
			) :
				_pInterface(pInResource),
				_pOwnerManager(pInOwnerManager),
				_params(InParams),
				_id(InID),
				_it(InIter),
				_it_role_resource(InIterRoleResource),
				_linkCount(0),
				_loadingState(),// default loading state is ok
				_groupMask(0)
			{
				_combinableParamsCalculator.SetOwnParams(InParams.pub.GetCombinableParams());
				// Here we must only rebind to combi, without precaching
				// (we may not precache here, because iterator is not yet binded to this resource)
				_RebindToCombi();
			}

			void Impl_ManagedResource::SetLoader(std::shared_ptr<ResMan::IResourceLoader> pInLoader)
			{
				assert(pInLoader.get() != nullptr);
				_params.pLoader = pInLoader;
				if (GetRealResource())
				{
					DestroyRealResource();

					const bool bForceSynchro = true;
					const bool bForceReload_DependencyLists = true;
					const bool bForceReload_Subresources = true;
					_ReloadSubresources_BasedOnParams(bForceSynchro, bForceReload_DependencyLists, bForceReload_Subresources);
				}
			}

			void Impl_ManagedResource::Notify_AfterRegisteredInResourceRepo()
			{
				Notify_CombinableParamsChanged();
			}

			const ResMan::CombinableParams &Impl_ManagedResource::GetCombinableParams() const
			{
				return *(_combinableParamsCalculator.GetResultAggregatedParams());
			}

			const ResMan::LoadableMainSubresourceParams *Impl_ManagedResource::GetMainSubresourceParams() const
			{
				if (IsAddressableSubresource())
				{
					return nullptr;
				}
				else
				{
					return &(GetLoader()->GetMainResourceParams());
				}
			}
			const ResMan::LoadableAddressableSubresourceParams *Impl_ManagedResource::GetAddressableSubresource() const
			{
				if(IsAddressableSubresource())
				{
					auto pAddressableSubresource = &(GetLoader()->GetAddressableSubresourceParams(_params.pub.GetRole(), _params.addressable_subresource_index));
					return pAddressableSubresource;
				}
				else
				{
					return nullptr;
				}
			}
			bool Impl_ManagedResource::IsAddressableSubresource() const
			{
				return false == ResMan::AddressableSubresourceIndex_IsNull(_params.addressable_subresource_index);
			}

			const ResMan::CombinableParams *Impl_ManagedResource::GetOwnCombinableParams() const
			{
				return _combinableParamsCalculator.GetOwnParams();
			}
			ResMan::EResourceCombinableParametersMode Impl_ManagedResource::GetCombinableParamsMode() const
			{
				return _combinableParamsCalculator.GetMode();
			}

			void Impl_ManagedResource::SetCombinableParamsMode(ResMan::EResourceCombinableParametersMode InCombinableParamsMode)
			{
				_combinableParamsCalculator.SetMode(InCombinableParamsMode);
				Notify_CombinableParamsChanged();
			}
			void Impl_ManagedResource::UpdateCombinableParams(const ResMan::CombinableParams& InNewValue)
			{
				_combinableParamsCalculator.SetOwnParams(&InNewValue);
				Notify_CombinableParamsChanged();
			}

			ResMan::IManagedResource::GroupEnumerator Impl_ManagedResource::GetActiveGroups() const
			{
				return GetActiveGroupEnumerator(_pGroupCombi);
			}

			ResMan::SubresourceRoleSet Impl_ManagedResource::GetLoadedSubresources() const
			{
				return GetLoaderData()->GetLoadedSubresources();
			}

			ResMan::SubresourceRoleSet Impl_ManagedResource::GetLoadingFailedSubresources() const
			{ 
				return GetLoaderData()->GetLoadedFailedSubresources();
			}
			ResMan::SubresourceRoleSet Impl_ManagedResource::GetLoadingFailedDependencyLists() const
			{
				return GetLoaderData()->GetLoadedFailedDependencyLists();
			}


			void Impl_ManagedResource::PrecacheSubresources(const ResMan::LoadCommand& InParams)
			{
				const bool bIsManual = true;
				GetImpl_Subsystem(GetOwnerManager())->PrecacheResource(this, InParams, bIsManual);
			}
			void Impl_ManagedResource::PrecacheSubresourcesSyncho(const ResMan::LoadCommand& InParams)
			{
				const bool bIsManual = true;
				GetImpl_Subsystem(GetOwnerManager())->PrecacheResourceSynchro(this, InParams, bIsManual);
			}

			void Impl_ManagedResource::PrecacheDependencyList(const ResMan::PrecacheDependenciesCommand& InParams)
			{
				const bool bIsManual = true;
				GetImpl_Subsystem(GetOwnerManager())->PrecacheResourceDependencyLists(this, InParams, bIsManual);
			}

			void Impl_ManagedResource::Notify_FailHasBeenProcessed()
			{
				assert(_numFailsToBePopped > 0);
				_numFailsToBePopped--;
			}

			void Impl_ManagedResource::Notify_AttachedToGroup(ManagedGroup *pInGroup, GroupResourceRecordPointer InIterGroupResource)
			{
				_groups.push_back(ResourceGroupLink(pInGroup, InIterGroupResource));
				_groupMask.set(GetID(pInGroup));
				_RebindToCombi_AndRecalculateCombinableParams();
			}

			void Impl_ManagedResource::Notify_DetachedFromGroup(ManagedGroup *pInGroup)
			{
				_groupMask.reset(GetID(pInGroup));
				_EraseLinks_ToGroup(pInGroup);
				_RebindToCombi_AndRecalculateCombinableParams();
			}

			void Impl_ManagedResource::Notify_GroupCombiParamsUpdated()
			{
				if (_pGroupCombi != nullptr)
				{
					_combinableParamsCalculator.Notify_GroupParamsUpdated(&(_pGroupCombi->GetCombinedParams()));
				}
				else
				{
					_combinableParamsCalculator.Notify_GroupParamsUpdated(nullptr);
				}
				Notify_CombinableParamsChanged();
			}

			void Impl_ManagedResource::Notify_GroupParamsUpdated(ManagedGroup *pInGroup)
			{
				Notify_GroupCombiParamsUpdated();
			}

			void Impl_ManagedResource::Notify_CombinableParamsChanged()
			{
				const bool bForceSynchro = false;
				const bool bForceReload_DependencyLists = false;
				const bool bForceReload_Subresources = false;
				_ReloadSubresources_BasedOnParams(bForceSynchro, bForceReload_DependencyLists, bForceReload_Subresources);
				_FixAll_ResourceRecordPositions_OfAllGroups();
			}

			void Impl_ManagedResource::EnqueueCommandForResource
			(
				const Loader::LoadRequestParams&                          InRequestParams,
				size_t                                                    InActualPriority,
				Loader::LoadCommandBase                                  *pInCommand,
				Loader::LoadingLoopCommandQueues                         *pInLoadingLoopQueues
			)
			{
				auto pInitiallyRequestedResource_LoaderData = &(InRequestParams.GetResource()->_loaderData);
				pInLoadingLoopQueues->GetOwnerLoadingLoop()->GetOwnerMain()->EnqueueCommandForResource(pInLoadingLoopQueues, InRequestParams, InActualPriority, pInCommand, this, &_loaderData, pInitiallyRequestedResource_LoaderData);
			}

			void Impl_ManagedResource::EndLoad(const Loader::LoaderCommandResultRecord& InResultRecord, Loader::LoadingLoopCommandQueues *pInLoadingLoopCommandQueues, bool bInRecursiveCommandsSucceeded)
			{
				auto pInitiallyRequestedResource_LoaderData = &(InResultRecord.GetAttachedMainThreadData()->initial_request_params.GetResource()->_loaderData);
				if (false == bInRecursiveCommandsSucceeded)
				{
					pInitiallyRequestedResource_LoaderData->SetSomeCommandInQueueFailedRecursively(true);
				}
				Loader::GetOwnerLoaderMain(pInLoadingLoopCommandQueues)->EndLoad(pInLoadingLoopCommandQueues, InResultRecord, &_loaderData, pInitiallyRequestedResource_LoaderData, bInRecursiveCommandsSucceeded);
			}

			void Impl_ManagedResource::Notify_LoadBegun(const Loader::LoadRequestParams& InInitialRequest)
			{
				// Only the current recursive loaded resource subresources must be upadet,
				// because the parent resource's recursive loading state is already updated
				_loadingState.SubresourcesInLoadingQueue_Add(_loaderData.GetSubresourcesInLoadingQueue());
				_loadingState.DependencyListsInLoadingQueue_Add(_loaderData.GetDependencyListsInLoadingQueue());
			}


			void Notify_ActiveGroups_LoadEnded(Impl_ManagedResource *pInResource, ResourceGroupCombi *pInNullOrGroupGombi, const Loader::LoadRequestParams& InInitialRequest, const Loader::LoadUpdate& InUpdate)
			{
				if (pInNullOrGroupGombi != nullptr)
				{
					for (const auto& pGroup : pInNullOrGroupGombi->GetActiveGroups())
					{
						pGroup->GetImpl()->Notify_Resource_LoadEnded(pInResource, InInitialRequest, InUpdate);
					}
				}
			}

			void Notify_ActiveGroups_RecursiveLoadEnded(Impl_ManagedResource *pInResource, ResourceGroupCombi *pInNullOrGroupGombi, const Loader::LoadRequestParams& InInitialRequest, bool bIsFailed)
			{
				if (pInNullOrGroupGombi != nullptr)
				{
					for (const auto& pGroup : pInNullOrGroupGombi->GetActiveGroups())
					{
						pGroup->GetImpl()->Notify_Resource_RecursiveLoadEnded(pInResource, InInitialRequest, bIsFailed);
					}
				}
			}

			void Impl_ManagedResource::Notify_LoadEnded(const Loader::LoadRequestParams& InInitialRequest, const Loader::LoadUpdate& InUpdate, void *pInNewResourceObject)
			{
				_pNullOrRealResource = pInNewResourceObject;
				Notify_ActiveGroups_LoadEnded(this, _pGroupCombi, InInitialRequest, InUpdate);
				// State of resource in any of the groups may be changed
				_FixAll_ResourceRecordPositions_OfAllGroups();
			}

			void Impl_ManagedResource::Notify_RecursiveLoadEnded(const Loader::LoadRequestParams& InInitialRequest, bool bIsFailed)
			{
				if(bIsFailed)
				{
					if (GetOwnerManager()->TryPushFailedResource(*_it))
					{
						_numFailsToBePopped++;
					}
				}
				Notify_ActiveGroups_RecursiveLoadEnded(this, _pGroupCombi, InInitialRequest, bIsFailed);
				// At the end the recursive failed load flag should be resetted;
				_loaderData.SetSomeCommandInQueueFailedRecursively(false);
			}

			void Impl_ManagedResource::Notify_TotalCommandsToBePoppedZeroed()
			{
				_loadingState.SubresourcesInLoadingQueue_Clear();
				_loadingState.DependencyListsInLoadingQueue_Clear();
			}

			void Impl_ManagedResource::Notify_LoadedAsAdressableSubresource(void *pInNewResource)
			{
				ResMan::SubresourceRoleSet loadedSubresourceRoles { _params.pub.GetRole()->GetRoleIndex() };
				_loaderData.IncludeLoadedSubresources(loadedSubresourceRoles);
				_pNullOrRealResource = pInNewResource;
				// Here we should not update groups, because addressable subresource is to be loaded only  if it's has not been created yet.
			}

			void Impl_ManagedResource::DetachResource_FromAllGroups()
			{
				if (_pGroupCombi != nullptr)
				{
					auto groupMask = _pGroupCombi->GetGroupMask();
					for (auto groupIndex = 0; groupIndex < groupMask.size(); ++groupIndex)
					{
						if(groupMask[groupIndex])
						{
							auto pGroup = GetOwnerManager()->GetGroups()->GetGroupByID(groupIndex);
							pGroup->DetachResourceFromGroup(GetInterface());
						}
					}
				}
			}


			bool ShouldUnload(Impl_ManagedResource *pInManagedResource, const ResMan::UnloadCommand& InUnloadCommand)
			{
				auto pSubsystem = pInManagedResource->GetOwnerManager()->GetSubsystem();
				auto pRuntimeParams = &(pSubsystem->GetRuntimeParams());
				if (pRuntimeParams->IsFreezed())
				{
					return false;
				}

				auto pCombinableParams = &(pInManagedResource->GetCombinableParams());
				if (pCombinableParams->IsManualUnloadFreezed())
				{
					return false;
				}

				if (InUnloadCommand.args.subresources.IsEmpty())
				{
					return false;
				}

				return true;
			}

			bool ShouldUnloadDependencyLists(Impl_ManagedResource *pInManagedResource)
			{
				auto pSubsystem = pInManagedResource->GetOwnerManager()->GetSubsystem();
				auto pRuntimeParams = &(pSubsystem->GetRuntimeParams());
				if (pRuntimeParams->IsFreezed())
				{
					return false;
				}

				auto pCombinableParams = &(pInManagedResource->GetCombinableParams());
				if (pCombinableParams->IsManualUnloadFreezed())
				{
					return false;
				}

				return true;
			}

			void Impl_ManagedResource::UnloadDependencyLists()
			{
				bool bShouldUnload = ShouldUnloadDependencyLists(this);
				if(bShouldUnload)
				{ 
					auto subresourceRolesToUnload = ResMan::GetApplicableSubresourceRoles(GetInterface());
					GetOwnerManager()->Wait_UntilAllCommandsProcessed();
					_params.pLoader->UnloadDirectDepedencyListOf(subresourceRolesToUnload);
					_loaderData.SubtractLoadedDependencyLists(subresourceRolesToUnload);
					_FixAll_ResourceRecordPositions_OfAllGroups();
				}
			}

			void Impl_ManagedResource::Unload(const ResMan::UnloadCommand& InUnloadCommand)
			{
				auto bShouldUnload = ShouldUnload(this, InUnloadCommand);
				if(bShouldUnload)
				{
					GetOwnerManager()->Wait_UntilAllCommandsProcessed();
					_params.pLoader->UnloadTemporaryOwnContent(InUnloadCommand.args.subresources);
					_loaderData.SubtractLoadedSubresources(InUnloadCommand.args.subresources);
					_FixAll_ResourceRecordPositions_OfAllGroups();

					auto resourceThatMustBeLoadedWhileRealResourceIsLoaded_SpecifiedInUnloadCommand = InUnloadCommand.args.subresources;
					auto pResourceType = ResMan::GetResourceType(GetInterface());
					resourceThatMustBeLoadedWhileRealResourceIsLoaded_SpecifiedInUnloadCommand.IntersectWith(pResourceType->GetParams().GetSubresourceRoles().GetRolesThatMustBeLoadedWhileRealResourceIsLoaded());
					if(false == resourceThatMustBeLoadedWhileRealResourceIsLoaded_SpecifiedInUnloadCommand.IsEmpty())
					{
						DestroyRealResource();
					}
				}
			}

			const Loader::Loader_PerResource_PerLoadingLoop *Impl_ManagedResource::GetLoaderLoopData(Loader::LoadingLoop *pLoadingLoop) const
			{
				return _loaderData.GetLoaderLoopData(pLoadingLoop);
			}

			Loader::Loader_PerResource_PerLoadingLoop *Impl_ManagedResource::GetLoaderLoopData(Loader::LoadingLoop *pLoadingLoop)
			{
				return _loaderData.GetLoaderLoopData(pLoadingLoop);
			}

			void Impl_ManagedResource::DestroyRealResource()
			{
				auto pDescr = GetResourceTypeDescr(this);
				pDescr->Delete(_pNullOrRealResource);
				_pNullOrRealResource = nullptr;
			}			

			void Impl_ManagedResource::_RebindToCombi_AndRecalculateCombinableParams()
			{
				_RebindToCombi();
				Notify_GroupCombiParamsUpdated();
			}

			void Impl_ManagedResource::_RebindToCombi()
			{
				if (_groupMask.none())
				{
					_pGroupCombi = nullptr;
				}
				else
				{
					_pGroupCombi = _pOwnerManager->GetGroups()->PrecalculateCombi(_groupMask);
				}
			}

			void Impl_ManagedResource::_ReloadSubresources_BasedOnParams(bool bInForceSynchro, bool bInForceReload_DependencyLists, bool bInForceReload_Subresources)
			{
				// Reloading subresources of resource based on its parameters is never manual, it's automatic
				const bool bIsManual = false;
				const auto& referenceCombinableParams = GetCombinableParams();

				if (bInForceReload_DependencyLists || ShouldReloadDependencyLists_BasedOnParams(this, referenceCombinableParams))
				{
					ResMan::PrecacheDependenciesCommand precacheDependencyListsCommand;
					precacheDependencyListsCommand.args.subresources = referenceCombinableParams.GetSubresourceRolesToKeepDependencies();
					precacheDependencyListsCommand.args.urgency = ResMan::Urgency_Normal;

					if (bInForceSynchro)
					{
						GetImpl_Subsystem(GetOwnerManager())->PrecacheResourceDependencyListsSynchro(this, precacheDependencyListsCommand, bIsManual);
					}
					else
					{
						GetImpl_Subsystem(GetOwnerManager())->PrecacheResourceDependencyLists(this, precacheDependencyListsCommand, bIsManual);
					}
				}

				if (bInForceReload_Subresources || ShouldReloadSubresources_BasedOnParams(this, referenceCombinableParams))
				{
					ResMan::LoadCommand loadCommand;
					loadCommand.args.subresources = referenceCombinableParams.GetSubresourceRolesToLoad();
					loadCommand.args.urgency = ResMan::Urgency_Normal;

					if(bInForceSynchro)
					{
						GetImpl_Subsystem(GetOwnerManager())->PrecacheResourceSynchro(this, loadCommand, bIsManual);
					}
					else
					{
						GetImpl_Subsystem(GetOwnerManager())->PrecacheResource(this, loadCommand, bIsManual);
					}
				}
			}

			void Impl_ManagedResource::_FixAll_ResourceRecordPositions_OfAllGroups()
			{
				auto it = _groups.begin();
				while (it != _groups.end())
				{
					auto oldPointerToResourceRecord = it->GetIterToResourceRecord();
					auto newPointerToResourceRecord = it->GetGroup()->GetImpl()->FixResourceRecordPosition(oldPointerToResourceRecord);
					it->SetFixedPointerToResourceRecord(newPointerToResourceRecord);
					++it;
				}
			}

			void Impl_ManagedResource::_EraseLinks_ToGroup(ManagedGroup *pInGroup)
			{
				assert(pInGroup != nullptr);
				auto it = _groups.begin();
				while (it != _groups.end())
				{
					if (it->GetGroup() == pInGroup)
					{
						it = _groups.erase(it);
					}
					else
					{
						++it;
					}
				}
			}

			/************************************************************************************
			* utils functions implementation
			************************************************************************************/
			boost::optional<GroupResourceRecordPointer> FindGroupLink(const ResourceGroupLink_Cont *pGroupLinks, ManagedGroup *pInGroup)
			{
				for (auto it = pGroupLinks->cbegin(); it != pGroupLinks->cend(); ++it)
				{
					if (it->GetGroup() == pInGroup)
					{
						return it->GetIterToResourceRecord();
					}
				}
				return boost::optional<GroupResourceRecordPointer>();
			}

			// Get enumerator of active groups for group combi that can be nullptr.
			ResMan::IManagedResource::GroupEnumerator GetActiveGroupEnumerator(ResourceGroupCombi *pInNullOrGroupCombi)
			{
				if (pInNullOrGroupCombi == nullptr)
				{
					return Util::MakeEnumeratorResult_Empty<ResMan::IManagedGroup>();
				}
				else
				{
					return MakeGroupEnumeratorResult(new TypedActiveGroupEnumerator(&(pInNullOrGroupCombi->GetActiveGroups())));
				}
			}

			bool ShouldReloadSubresources_BasedOnParams(Impl_ManagedResource *pInResource, const ResMan::CombinableParams& InReferenceParams)
			{
				assert(pInResource != nullptr);

				// WARNING: We should not skip loading based on parameters here - this decisions are to be made by the subsystem class.
				// but we must check based on the appropriate load state

				EResourceLoadState appropriateLoadState = ComputeAppopriateLoadState_Subresources(&InReferenceParams, pInResource);
				return appropriateLoadState == EResourceLoadState::NotPrecached;
			}

			bool ShouldReloadDependencyLists_BasedOnParams(Impl_ManagedResource *pInResource, const ResMan::CombinableParams& InReferenceParams)
			{
				assert(pInResource != nullptr);
				assert(pInResource != nullptr);

				// WARNING: We should not skip loading based on parameters here - this decisions are to be made by the subsystem class.
				// but we must check based on the appropriate load state

				EResourceLoadState appropriateLoadState = ComputeAppopriateLoadState_DependencyLists(&InReferenceParams, pInResource);
				return appropriateLoadState == EResourceLoadState::NotPrecached;
			}
		}
	}
}