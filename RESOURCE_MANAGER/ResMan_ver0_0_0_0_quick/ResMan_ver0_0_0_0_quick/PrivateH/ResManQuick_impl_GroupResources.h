#pragma once

#include "ResManQuick_impl_RoledGroupResources.h"
#include "../H/ResManQuick_System.h"
#include "Fwd\ResManQuick_Fwd_ManagedResource.h"

#include <vector>

/********************************************************************************************************
* TODO:
* 1. Think about decision making where to attach to?
*********************************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			using GroupResources_Cont                       = std::vector<RoledGroupResources>;
			using GroupResources_Iterator                   = GroupResources_Cont::iterator;
			using SubresoureRolesForLoadState_Cont          = std::vector<ResMan::SubresourceRoleSet>;

			/************************************************************************************************
			* utils fwd declarations
			************************************************************************************************/
			// Get appropriate load state of the given resource
			EResourceLoadState ComputeAppopriateResourceLoadState                             (const ResMan::CombinableParams *pInReferenceParams, Impl::Impl_ManagedResource *pInManagedResource);
			EResourceLoadState ComputeAppopriateLoadState_DependencyLists                     (const ResMan::CombinableParams *pInReferenceParams, Impl::Impl_ManagedResource *pInManagedResource);
			EResourceLoadState ComputeAppopriateLoadState_Subresources                        (const ResMan::CombinableParams *pInReferenceParams, Impl::Impl_ManagedResource *pInManagedResource);

			/********************************************************************************************
			* TODO:
			* 1. Attach must return iterator
			********************************************************************************************/
			class GroupResources
			{
			public:
				void                                   Initialize                      (size_t InNumRegisteredRoles);
				bool                                   IsInitialized                   () const                               { return !_roledResources.empty(); }

				// Parameters to check for whether the given resource is properly cached
				const ResMan::CombinableParams        *GetReferenceParams              () const                               { return &_referenceParams; }

				// Only setups the value of the reference parameters, does NOT automatically recalculates the resource record positions
				// (after calling this method, are need to call the FixResourceRecordPosition() for each of the stored resource links)
				void                                   UpdateReferenceParams           (const ResMan::CombinableParams *pInCombinableParams) { _referenceParams = *pInCombinableParams; }

				// Register resources in the group
				GroupResourceRecordPointer             Attach                          (ManagedResource *pInResource);
				// Detach resources from group resources
				void                                   DetachFrom                      (GroupResourceRecordPointer pInGroupResourceRecord);
				GroupResourceRecordPointer             FixResourceRecordPosition       (GroupResourceRecordPointer pInGroupResourceRecord);

				const RoledGroupResources&             GetRoleResources                (const ResMan::ISubresourceRole *pInRole) const;

				GroupResources_Iterator                begin                           () { return _roledResources.begin(); }
				GroupResources_Iterator                end                             () { return _roledResources.end(); }

				// Get subresource roles that are registered for the current load state
				ResMan::SubresourceRoleSet             GetSubresourceRoleSet_ForLoadState(EResourceLoadState InLoadState) const { return _loadState_SubresourceRole_Masks[static_cast<size_t>(InLoadState)]; }

			private:
				ResMan::CombinableParams                                                _referenceParams;
				GroupResources_Cont                                                     _roledResources;
				SubresoureRolesForLoadState_Cont                                        _loadState_SubresourceRole_Masks;
			};
		}
	}
}