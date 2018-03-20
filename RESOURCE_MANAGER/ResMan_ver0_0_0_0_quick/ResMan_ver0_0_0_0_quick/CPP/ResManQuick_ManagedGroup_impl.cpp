#include "../PrivateH/ResManQuick_impl_ManagedGroup.h"
#include "../PrivateH/ResManQuick_impl_ResourceManager.h"
#include "../PrivateH/ResManQuick_impl_ResourceManagerSubsystem.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			/*********************************************************************************************
			* Typedefs
			*********************************************************************************************/
			using GroupResourceEnumerator = PtrContResourceEnumerator<const RoledGroupResources_ResourceListCont>;

			/*********************************************************************************************
			* Fwd utils
			*********************************************************************************************/
			void Notify_AllResources_That_GroupParamsChanged(ManagedGroup *pInGroup, GroupResources *pResources);
			void Notify_ResourcesOfGivenRole_That_GroupParamsChanged(ManagedGroup *pInGroup, GroupResources_Iterator InResourcesOfRole);
			void Notify_Resources_InResourceList_That_GroupParamsChanged(ManagedGroup *pInGroup, const RoledGroupResources_ResourceListCont *pInCont);

			/*********************************************************************************************
			* ResourceGroupLink implementation
			*********************************************************************************************/
			ResourceGroupLink::ResourceGroupLink
			(
				ManagedGroup                              *pInToGroup,
				GroupResourceRecordPointer                 InGroupResourceRecord
			) :
				_pGroup(pInToGroup),
				_it_group_resource_record(InGroupResourceRecord)
			{
				assert(pInToGroup != nullptr);
			}

			/*********************************************************************************************
			* Impl_ManagedGroup implementation
			*********************************************************************************************/
			Impl_ManagedGroup::Impl_ManagedGroup
			(
				ManagedGroup                                 *pInManagedGroup,
				Impl_ResourceManager                         *pInOwnerManager,
				const ResMan::GroupParams                    *pInParams,
				GroupContElement                             *pInSlot
			) :
				_pManagedGroup(pInManagedGroup),
				_pOwnerManager(pInOwnerManager),
				_pSlot(pInSlot),
				_params(*pInParams)
			{
				assert(pInManagedGroup != nullptr);

				auto pNewCombinableParams = &(pInParams->GetPublic()->GetCombinable());

				size_t numRegisteredRoles = pInOwnerManager->GetSubsystem()->GetSubresourceRoles().size();
				_resources.Initialize(numRegisteredRoles);
				_resources.UpdateReferenceParams(pNewCombinableParams);
			}

			void Impl_ManagedGroup::Attach(ManagedResource *pInResource)
			{
				assert(IsOperable());
				assert(pInResource != nullptr);
				auto it_group_resource_rec = _resources.Attach(pInResource);
				pInResource->GetImpl()->Notify_AttachedToGroup(_pManagedGroup, it_group_resource_rec);
			}

			void Impl_ManagedGroup::DetachResourceFromThis(ManagedResource *pInResource)
			{
				assert(IsOperable());
				assert(pInResource != nullptr);
				auto group = FindGroupLink(pInResource->GetImpl()->GetGroupLinks(), _pManagedGroup);
				assert(group.is_initialized());
				_resources.DetachFrom(group.get());
				pInResource->GetImpl()->Notify_DetachedFromGroup(_pManagedGroup);
			}

			ResMan::SubresourceRoleSet Impl_ManagedGroup::GetSubresourceRoles_PrecachedResources() const
			{
				assert(IsOperable());
				auto subresourceRoleSet = _resources.GetSubresourceRoleSet_ForLoadState(EResourceLoadState::Precached);
				return subresourceRoleSet;
			}
			ResMan::SubresourceRoleSet Impl_ManagedGroup::GetSubresourceRoles_NotPrecachedResources() const
			{
				assert(IsOperable());
				auto subresourceRoleSet = _resources.GetSubresourceRoleSet_ForLoadState(EResourceLoadState::NotPrecached);
				return subresourceRoleSet;
			}
			ResMan::SubresourceRoleSet Impl_ManagedGroup::GetSUbresourceRoles_FailedPrecachedResources() const
			{
				assert(IsOperable());
				auto subresourceRoleSet = _resources.GetSubresourceRoleSet_ForLoadState(EResourceLoadState::PrecacheFailed);
				return subresourceRoleSet;
			}

			ResMan::IManagedGroup::ResourceEnumerator Impl_ManagedGroup::GetPrecachedResources(const ResMan::ISubresourceRole *pInSubresourceRole) const
			{
				assert(IsOperable());
				const auto& resourcesOfRole = _resources.GetRoleResources(pInSubresourceRole);
				return MakeResourceEnumeratorResult(new GroupResourceEnumerator(resourcesOfRole.GetResourceList_Precached()));
			}
			ResMan::IManagedGroup::ResourceEnumerator Impl_ManagedGroup::GetFailedPrecachedResources(const ResMan::ISubresourceRole *pInSubresourceRole) const
			{
				assert(IsOperable());
				const auto& resourcesOfRole = _resources.GetRoleResources(pInSubresourceRole);
				return MakeResourceEnumeratorResult(new GroupResourceEnumerator(resourcesOfRole.GetResourceList_PrecachedFailed()));
			}
			ResMan::IManagedGroup::ResourceEnumerator Impl_ManagedGroup::GetNotPrecachedResources(const ResMan::ISubresourceRole *pInSubresourceRole) const
			{
				assert(IsOperable());
				const auto& resourcesOfRole = _resources.GetRoleResources(pInSubresourceRole);
				return MakeResourceEnumeratorResult(new GroupResourceEnumerator(resourcesOfRole.GetResourceList_NotPrecached()));
			}

			void Impl_ManagedGroup::Clear()
			{
				assert(IsOperable());
				for (auto it_resources_of_role = _resources.begin(); it_resources_of_role != _resources.end(); ++it_resources_of_role)
				{
					for (auto loadStateIndex = 0; loadStateIndex < static_cast<size_t>(EResourceLoadState::NUM_STATES); ++loadStateIndex)
					{
						auto loadState = static_cast<EResourceLoadState>(loadStateIndex);
						auto pResourceList_ForLoadState = it_resources_of_role->GetResourceList_ForState(loadState);

						auto it_resource = pResourceList_ForLoadState->begin();
						RoledGroupResources_ResourceListCont::iterator it_next_resource;
						while (true)
						{
							if (it_resource == pResourceList_ForLoadState->end())
							{
								break;
							}
							it_next_resource = std::next(it_resource);
							DetachResourceFromThis(*it_resource);
							it_resource = it_next_resource;
						}
					}
				}
			}

			void Impl_ManagedGroup::Notify_BeforeUnregisterGroup()
			{
				Clear();
				_MarkUnregistered();
			}

			void Impl_ManagedGroup::SetCombinableParams(const ResMan::CombinableParams& InCombinableParams)
			{				
				assert(IsOperable());
				_resources.UpdateReferenceParams(&InCombinableParams);
				_pOwnerManager->GetGroups()->RecalculateCombinedParams_ForGroup(_pManagedGroup);
				Notify_AllResources_That_GroupParamsChanged(GetManagedGroup(), &_resources);
			}

			GroupResourceRecordPointer Impl_ManagedGroup::FixResourceRecordPosition(GroupResourceRecordPointer InOldRecordPointer)
			{
				return _resources.FixResourceRecordPosition(InOldRecordPointer);
			}

			void Impl_ManagedGroup::PrecacheDependencyList(ResMan::Urgency InUrgency)
			{
				const bool bIsManual = true;
				_pOwnerManager->PrecacheGroupDependencyLists(this, bIsManual, InUrgency);
			}

			const ResMan::GroupDependencyState& Impl_ManagedGroup::GetDependencyListState() const
			{
				assert(IsOperable());
				return _dependencyState;
			}

			void Impl_ManagedGroup::Precache(ResMan::Urgency InUrgency)
			{
				const bool bIsManual = true;
				_pOwnerManager->PrecacheGroup(this, bIsManual, InUrgency);
			}
			const ResMan::GroupCommandState& Impl_ManagedGroup::PrecacheSynchro()
			{
				const bool bIsManual = true;
				_pOwnerManager->PrecacheGroupSynchro(this, bIsManual);
				return _commandState;
			}

			void Impl_ManagedGroup::Notify_LoadBegun(const std::list<Impl::Impl_ManagedResource *> *pInResourcesToLoad)
			{
				_commandState.bDoingGroupCommand = true;

				_commandState.last_result.status_code = ResMan::ECommandStatusCode::OK;
				_commandState.last_result.pFailedResource = nullptr;

				// WARNING: Number of resources to be tried to load cannot be precalculated initially,
				// because recursive resources are also counted!!!
			}

			void Impl_ManagedGroup::Notify_LoadEnded()
			{
				_commandState.bDoingGroupCommand = false;
				// WARNING!!! When group loading ended last_result should not be reset.
			}

			void Impl_ManagedGroup::Notify_GroupResourceLoaded(Impl_ManagedResource *pInInitiallyRequestedResource, Impl_ManagedResource *pInResource, bool bResourceLoadFailed)
			{
				_commandState.counters.Decr_NumResourcesToBeTriedToLoad();

				if (pInInitiallyRequestedResource->GetLoaderData()->GetNumRecursiveCommandResults_ToBePopped() == 0)
				{
					if (nullptr == _commandState.last_result.pFailedResource)
					{
						if (bResourceLoadFailed)
						{
							_commandState.last_result.pFailedResource = pInResource->GetInterface();
							_commandState.last_result.status_code = ResMan::ECommandStatusCode::FileNotFound;
						}
					}
					_pOwnerManager->Notify_GroupResource_TriedToLoad(this, pInResource);
				}
			}

			const ResMan::GroupCommandState& Impl_ManagedGroup::GetCommandState() const
			{
				assert(IsOperable());
				return _commandState;
			}

			bool Impl_ManagedGroup::IsOperable() const
			{
				return _bIsOperable;
			}
			void Impl_ManagedGroup::_MarkUnregistered()
			{
				_bIsOperable = false;
			}

			void Impl_ManagedGroup::Notify_Resource_RecursiveLoadEnded(Impl_ManagedResource *pInResource, const Loader::LoadRequestParams& InInitialRequest, bool bIsFailed)
			{

			}

			void Impl_ManagedGroup::Notify_Resource_LoadEnded(Impl_ManagedResource *pInResource, const Loader::LoadRequestParams& InInitialRequest, const Loader::LoadUpdate& InUpdate)
			{
				
				Util::TODO("Why two different functions to notify resource load ended?");
			}

			void Impl_ManagedGroup::Notify_Resource_LoadBegun(Impl_ManagedResource *pInResource, const Loader::LoadRequestParams& InInitialRequest)
			{
				_commandState.counters.Increment_NumResourcesToBeTriedToLoad();
			}

			/******************************************************************************************
			* utils implementation
			*******************************************************************************************/
			void Notify_AllResources_That_GroupParamsChanged(ManagedGroup *pInGroup, GroupResources *pInResources)
			{				
				for (auto it = pInResources->begin(); it != pInResources->end(); ++it)
				{
					Notify_ResourcesOfGivenRole_That_GroupParamsChanged(pInGroup, it);
				}
			}

			void Notify_ResourcesOfGivenRole_That_GroupParamsChanged(ManagedGroup *pInGroup, GroupResources_Iterator InResourcesOfRole)
			{
				Notify_Resources_InResourceList_That_GroupParamsChanged(pInGroup, InResourcesOfRole->GetResourceList_NotPrecached());
				Notify_Resources_InResourceList_That_GroupParamsChanged(pInGroup, InResourcesOfRole->GetResourceList_Precached());
				Notify_Resources_InResourceList_That_GroupParamsChanged(pInGroup, InResourcesOfRole->GetResourceList_PrecachedFailed());
			}

			void Notify_Resources_InResourceList_That_GroupParamsChanged(ManagedGroup *pInGroup, const RoledGroupResources_ResourceListCont *pInCont)
			{
				auto it_resource = pInCont->begin();
				auto it_next = it_resource;
				while (true)
				{
					if (it_resource == pInCont->end())
					{
						break;
					}
					it_next = std::next(it_resource);

					auto pResource = *it_resource;
					pResource->GetImpl()->Notify_GroupParamsUpdated(pInGroup);

					it_resource = it_next;
				}
			}
		}
	}
}