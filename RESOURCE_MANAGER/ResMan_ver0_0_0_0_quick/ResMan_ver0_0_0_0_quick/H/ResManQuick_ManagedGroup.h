#pragma once

#include "interface/IManagedGroup.h"
#include "../PrivateH/ResManQuick_impl_ManagedGroup.h"

namespace Dv
{
	namespace ResManQuick
	{
		/***********************************************************************************************************************
		* TODO:
		* 1. Interface:
		***********************************************************************************************************************/
		class ManagedGroup : public ResMan::IManagedGroup
		{
		public:
			/*********************************************************************************************
			* CONSTRUCTION
			*********************************************************************************************/
			ManagedGroup
			(				
				Impl::Impl_ResourceManager                         *pInOwnerManager,
				const ResMan::GroupParams                          *pInParams,
				Impl::GroupContElement                             *pInSlot
			);

			/*********************************************************************************************
			* GETTERS
			*********************************************************************************************/
			virtual ResMan::IResourceManager                       *GetOwnerManager() const override;

			virtual const ResMan::GroupPublicParams&                GetParams() const override;

			/*********************************************************************************************
			* UPDATING PARAMS
			*********************************************************************************************/
			virtual const ResMan::CombinableParams                 *GetCombinableParams             () const override;
			virtual void                                            UpdateCombinableParams          (const ResMan::CombinableParams&) override;

			/*********************************************************************************************
			* RESOURCE ENUMERATION
			*********************************************************************************************/
			// Get subresource roles for which resources are loaded
			virtual ResMan::SubresourceRoleSet                      GetSubresourceRoles_PrecachedResources                () const override;
			virtual ResMan::SubresourceRoleSet                      GetSubresourceRoles_NotPrecachedResources             () const override;
			virtual ResMan::SubresourceRoleSet                      GetSUbresourceRoles_FailedPrecachedResources          () const override;

			// enumerate only resources that are cached according to the group's precache settings
			// and only those of them that play the given role (the role is the role that is associated with the resource)
			virtual ResourceEnumerator                              GetPrecachedResources            (const ResMan::ISubresourceRole *pInRole) const override;

			virtual ResourceEnumerator                              GetFailedPrecachedResources      (const ResMan::ISubresourceRole *pInRole) const override;

			// enumerate only resources that are NOT precached appropriately
			// and only those of them that play the given role (the role is the role that is associated with the resource)
			virtual ResourceEnumerator                              GetNotPrecachedResources         (const ResMan::ISubresourceRole *pInRole) const override;

			/*********************************************************************************************
			* RESOURCE REGISTRATION
			*********************************************************************************************/
			// Attaching resource: if the resource is already included in the group, will do nothing.
			virtual void                                            AttachResource                 (ResMan::IManagedResource *pInResource) override;
			virtual void                                            DetachResourceFromGroup        (ResMan::IManagedResource *pInResource) override;

			/********************************************************************************************
			* PRIORITY MANAGEMENT
			*********************************************************************************************/
			virtual ResMan::ResourcePriority                        GetPrecachePriority            () const override;
			virtual void                                            SetHighestPrecachePriority     () override;

			/********************************************************************************************
			* PRECACHING
			********************************************************************************************/
			virtual void                                            Precache                       (ResMan::Urgency InUrgency) override;
			virtual const ResMan::GroupCommandState&                PrecacheSynchro                () override;
			virtual const ResMan::GroupCommandState&                GetCommandState                () const override;

			/********************************************************************************************
			* DEPENDENCY LIST
			********************************************************************************************/
			virtual void                                           PrecacheDependencyList              (ResMan::Urgency InUrgency) override;
			virtual const ResMan::GroupDependencyState&            GetDependencyListState              () const override;

			/*********************************************************************************************
			* Wokring with implementation
			*********************************************************************************************/
			const Impl::Impl_ManagedGroup                          *GetImpl                        () const { return &_impl; }
			Impl::Impl_ManagedGroup                                *GetImpl                        () { return &_impl; }

		private:
			Impl::Impl_ManagedGroup                  _impl;
		};

		class GroupBaseLoadPriorityComparer
		{
		public:
			bool                                     operator() (ManagedGroup *pA, ManagedGroup *pB)
			{
				return pA->GetParams().GetBaseLoadPriority() < pB->GetParams().GetBaseLoadPriority();
			}
		};
	}
}