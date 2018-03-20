#include "../PrivateH/ResManQuick_impl_GroupResources.h"
#include "../H/ResManQuick_ManagedResource.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			/************************************************************************************************
			* Utils implementation
			************************************************************************************************/
			void Compute_LoadState_SubresourceRole_Masks              (SubresoureRolesForLoadState_Cont *pSubresourceRoles_ForLoadStates, GroupResources_Cont *pInRoledResources, ResMan::SubresourceRoleIndex);

			/************************************************************************************************
			* GroupResources implementation
			************************************************************************************************/
			void GroupResources::Initialize(size_t InNumRegisteredRoles)
			{
				assert(!IsInitialized());
				_loadState_SubresourceRole_Masks.resize(static_cast<size_t>(EResourceLoadState::NUM_STATES));
				_roledResources.resize(InNumRegisteredRoles);
			}

			GroupResourceRecordPointer GroupResources::Attach(ManagedResource *pInResource)
			{
				assert(pInResource != nullptr);
				auto pRole                             = pInResource->GetParams().GetRole();
				auto& cont_resources_of_role           = _roledResources[pRole->GetRoleIndex()];
				EResourceLoadState loadState           = ComputeAppopriateResourceLoadState(&_referenceParams, pInResource->GetImpl());
				Compute_LoadState_SubresourceRole_Masks(&_loadState_SubresourceRole_Masks, &_roledResources, pRole->GetRoleIndex());

				auto it_resource = cont_resources_of_role.Register(loadState, pInResource);
				return it_resource;
			}

			void GroupResources::DetachFrom(GroupResourceRecordPointer pInGroupResourceRecord)
			{
				auto pRole = pInGroupResourceRecord->GetParams().GetRole();
				auto& cont_resources_of_role = _roledResources[pRole->GetRoleIndex()];
				cont_resources_of_role.Erase(pInGroupResourceRecord);
				Compute_LoadState_SubresourceRole_Masks(&_loadState_SubresourceRole_Masks, &_roledResources, pRole->GetRoleIndex());
			}

			GroupResourceRecordPointer GroupResources::FixResourceRecordPosition(GroupResourceRecordPointer pInGroupResourceRecord)
			{
				auto pRole = pInGroupResourceRecord->GetParams().GetRole();
				auto& cont_resources_of_role = _roledResources[pRole->GetRoleIndex()];
				auto oldLoadState = pInGroupResourceRecord.GetLoadState();
				EResourceLoadState newLoadState = ComputeAppopriateResourceLoadState(&_referenceParams, (*(pInGroupResourceRecord.GetIterator()))->GetImpl());
				// We need to prevent erasing and adding resource to the same container (which the MoveState function does),
				// to avoid iterate the same resource record twice (before move and after move) when iterating resource record container.
				if(newLoadState != oldLoadState)
				{
					auto newRecordPointer = cont_resources_of_role.MoveState(pInGroupResourceRecord, newLoadState);
					Compute_LoadState_SubresourceRole_Masks(&_loadState_SubresourceRole_Masks, &_roledResources, pRole->GetRoleIndex());
					return newRecordPointer;
				}
				else
				{
					return pInGroupResourceRecord;
				}
			}

			const RoledGroupResources& GroupResources::GetRoleResources(const ResMan::ISubresourceRole *pInRole) const
			{
				assert(pInRole != nullptr);
				return _roledResources[pInRole->GetRoleIndex()];
			}

			/************************************************************************************************
			* Utils implementation
			************************************************************************************************/
			void Compute_LoadState_SubresourceRole_Masks(SubresoureRolesForLoadState_Cont *pSubresourceRoles_ForLoadStates, GroupResources_Cont *pInRoledResources, ResMan::SubresourceRoleIndex InSubresourceRoleIndex)
			{
				assert(pSubresourceRoles_ForLoadStates != nullptr);
				assert(pInRoledResources != nullptr);

				const auto &roledResources                 = *pInRoledResources;
				const auto &resourcesOfRole                = roledResources[InSubresourceRoleIndex];

				auto & resourceRoles_ForLoadState          = *pSubresourceRoles_ForLoadStates;

				for (auto loadStateIndex = 0; loadStateIndex < static_cast<size_t>(EResourceLoadState::NUM_STATES); loadStateIndex++)
				{
					auto loadState = static_cast<EResourceLoadState>(loadStateIndex);
					auto pLoadStateResources = resourcesOfRole.GetResourceList_ForState(loadState);
					auto pLoadState_LoadMaskToCompute = &(resourceRoles_ForLoadState[loadStateIndex]);

					if (pLoadStateResources->size() > 0)
					{
						pLoadState_LoadMaskToCompute->Include(InSubresourceRoleIndex);
					}
					else
					{
						pLoadState_LoadMaskToCompute->Exclude(InSubresourceRoleIndex);
					}
				}
			}

			EResourceLoadState ComputeAppopriateLoadState_DependencyLists(const ResMan::CombinableParams *pInReferenceParams, Impl::Impl_ManagedResource *pInManagedResource)
			{
				auto pLoaderData = pInManagedResource->GetLoaderData();

				const auto dependencyListsThatMustBeLoaded = pInReferenceParams->GetSubresourceRolesToKeepDependencies();
				
				if(false == dependencyListsThatMustBeLoaded.IsEmpty())
				{
					auto dependencyListsThatLoadedButFailed = pLoaderData->GetLoadedFailedDependencyLists();
					auto dependencyListsThatFailed_AndMustBeLoaded = dependencyListsThatMustBeLoaded;
					dependencyListsThatFailed_AndMustBeLoaded.IntersectWith(dependencyListsThatLoadedButFailed);
					if (false == dependencyListsThatFailed_AndMustBeLoaded.IsEmpty())
					{
						return EResourceLoadState::PrecacheFailed;
					}

					auto dependencyListsThatLoaded = pLoaderData->GetLoadedDependencyLists();
					auto dependencyListsThatLoaded_AndMustBeLoaded = dependencyListsThatMustBeLoaded;
					dependencyListsThatLoaded_AndMustBeLoaded.IntersectWith(dependencyListsThatLoaded);
					if (dependencyListsThatLoaded_AndMustBeLoaded.IsEmpty())
					{
						return EResourceLoadState::NotPrecached;
					}
				}


				return EResourceLoadState::Precached;
			}

			EResourceLoadState ComputeAppopriateLoadState_Subresources(const ResMan::CombinableParams *pInReferenceParams, Impl::Impl_ManagedResource *pInManagedResource)
			{
				auto pLoaderData = pInManagedResource->GetLoaderData();

				auto resourcesThatMustBeLoaded                            = pInReferenceParams->GetSubresourceRolesToLoad();

				auto resourcesThatLoadedButFailed                         = pLoaderData->GetLoadedFailedSubresources();
				auto resourcesThatFailed_AndMustBeLoaded = resourcesThatMustBeLoaded;
				resourcesThatFailed_AndMustBeLoaded.IntersectWith(resourcesThatLoadedButFailed);
				if (false == resourcesThatFailed_AndMustBeLoaded.IsEmpty())
				{
					return EResourceLoadState::PrecacheFailed;
				}

				auto resourcesThatLoaded = pLoaderData->GetLoadedSubresources();
				if (false == resourcesThatLoaded.Contains(resourcesThatMustBeLoaded))
				{
					return EResourceLoadState::NotPrecached;
				}

				return EResourceLoadState::Precached;
			}
			
			EResourceLoadState ComputeAppopriateResourceLoadState(const ResMan::CombinableParams *pInReferenceParams, Impl::Impl_ManagedResource *pInManagedResource)
			{
				EResourceLoadState appropriateLoadState_dependencyLists   = ComputeAppopriateLoadState_DependencyLists(pInReferenceParams, pInManagedResource);
				EResourceLoadState appropriateLoadState_subresources      = ComputeAppopriateLoadState_Subresources(pInReferenceParams, pInManagedResource);
				if (appropriateLoadState_dependencyLists == EResourceLoadState::NotPrecached || appropriateLoadState_subresources == EResourceLoadState::NotPrecached)
				{
					return EResourceLoadState::NotPrecached;
				}

				if (appropriateLoadState_dependencyLists == EResourceLoadState::PrecacheFailed || appropriateLoadState_subresources == EResourceLoadState::PrecacheFailed)
				{
					return EResourceLoadState::PrecacheFailed;
				}

				return EResourceLoadState::Precached;
			}
		}
	}
}