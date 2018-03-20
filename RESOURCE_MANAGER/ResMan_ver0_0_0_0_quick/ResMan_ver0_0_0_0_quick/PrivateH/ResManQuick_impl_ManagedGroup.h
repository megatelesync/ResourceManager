#pragma once

#include "../PrivateH/ResManQuick_impl_System.h"
#include "ResManQuick_impl_GroupResources.h"
#include "ResManQuick_impl_ResourceGroupLink.h"
#include "Fwd\ResManQuick_Fwd_ManagedResource.h"
#include "../H/ResManQuick_Fwd.h"
#include "../PrivateH/Loader/Fwd/ResManQuick_Fwd_LoaderMain.h"

/********************************************************************************************************
* TODO:
*********************************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			class GroupContElement;
			class Impl_ResourceManager;

			class Impl_ManagedGroup
			{
			public:
				Impl_ManagedGroup
				(
					ManagedGroup                                 *pInManagedGroup,
					Impl_ResourceManager                         *pInOwnerManager,
					const ResMan::GroupParams                    *pInParams,
					GroupContElement                             *pInSlot
				);

				/*******************************************************************************************
				* Is this group still registered and can be used
				*******************************************************************************************/
				bool                                              IsOperable         () const;

				/*******************************************************************************************
				* Getters
				*******************************************************************************************/
				Impl_ResourceManager                             *GetOwnerManager    () const                        { return _pOwnerManager; }

				ManagedGroup                                     *GetManagedGroup    () const                        { return _pManagedGroup; }

				GroupContElement                                 *GetSlot            () const                        { return _pSlot;  }

				const ResMan::GroupParams                        *GetParams          () const                        { return &_params; }

				const ResMan::CombinableParams                   *GetCombinableParams() const { return _resources.GetReferenceParams(); }

				/*******************************************************************************************
				* Working with resources
				*******************************************************************************************/
				void                                              Attach                           (ManagedResource *pInResource);
				void                                              DetachResourceFromThis           (ManagedResource *pInResource);
				void                                              Clear                            ();

				ResMan::SubresourceRoleSet                       GetSubresourceRoles_PrecachedResources                    () const;
				ResMan::SubresourceRoleSet                       GetSubresourceRoles_NotPrecachedResources                 () const;
				ResMan::SubresourceRoleSet                       GetSUbresourceRoles_FailedPrecachedResources              () const;

				ResMan::IManagedGroup::ResourceEnumerator         GetPrecachedResources            (const ResMan::ISubresourceRole *pInSubresourceRole) const;
				ResMan::IManagedGroup::ResourceEnumerator         GetFailedPrecachedResources      (const ResMan::ISubresourceRole *pInSubresourceRole) const;
				ResMan::IManagedGroup::ResourceEnumerator         GetNotPrecachedResources         (const ResMan::ISubresourceRole *pInSubresourceRole) const;

				/*******************************************************************************************
				* Cancel group registration
				*******************************************************************************************/
				void                                              Notify_BeforeUnregisterGroup     ();

				/*******************************************************************************************
				* Working with resource parameters
				*******************************************************************************************/
				void                                              SetCombinableParams              (const ResMan::CombinableParams& InCombinableParams);

				GroupResourceRecordPointer                        FixResourceRecordPosition        (GroupResourceRecordPointer InOldRecordPointer);

				/*******************************************************************************************
				* Working with dependency lists
				*******************************************************************************************/
				void                                              PrecacheDependencyList           (ResMan::Urgency InUrgency);
				const ResMan::GroupDependencyState&               GetDependencyListState           () const;

				/*******************************************************************************************
				* Precaching
				*******************************************************************************************/
				void                                              Precache                         (ResMan::Urgency InUrgency);
				const ResMan::GroupCommandState&                  PrecacheSynchro                  ();
				const ResMan::GroupCommandState&                  GetCommandState                  () const;

				/*******************************************************************************************
				* Precache events
				*******************************************************************************************/
				// Called by the manager when the group began to load.
				// Here the command state to be changed;
				// WARNING!!! Ptr to list of loading resources cannot be saved!
				void                                              Notify_LoadBegun                 (const std::list<Impl::Impl_ManagedResource *> *pInResourcesToLoad);
				void                                              Notify_LoadEnded                 ();
				void                                              Notify_GroupResourceLoaded       (Impl_ManagedResource *pInInitiallyRequestedResource, Impl_ManagedResource *pInResource, bool bResourceLoadFailed);

				/*******************************************************************************************
				* Resource load events
				*******************************************************************************************/
				void Notify_Resource_RecursiveLoadEnded       (Impl_ManagedResource *pInResource, const Loader::LoadRequestParams& InInitialRequest, bool bIsFailed);
				void Notify_Resource_LoadEnded                (Impl_ManagedResource *pInResource, const Loader::LoadRequestParams& InInitialRequest, const Loader::LoadUpdate& InUpdate);
				void Notify_Resource_LoadBegun                (Impl_ManagedResource *pInResource, const Loader::LoadRequestParams& InInitialRequest);

			private:
				void                                              _MarkUnregistered                 ();

				ManagedGroup                                      *_pManagedGroup;
				Impl_ResourceManager                              *_pOwnerManager;
				GroupContElement                                  *_pSlot;
				GroupResources                                     _resources;
				ResMan::GroupDependencyState                       _dependencyState;
				ResMan::GroupCommandState                          _commandState;
				ResMan::GroupParams                                _params;
				bool                                               _bIsOperable = true;
			};
		}
	}
}