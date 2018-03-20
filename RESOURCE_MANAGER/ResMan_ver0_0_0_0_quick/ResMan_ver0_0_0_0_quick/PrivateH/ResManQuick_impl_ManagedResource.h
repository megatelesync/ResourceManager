#pragma once

// We need to include implementation-specific fwd include file 
// because the managed resoure class interacts with implementation of the Loader
#include "../PrivateH/Loader/Fwd/ResManQuick_Fwd_Impl.h"
#include "../PrivateH/Fwd/ResManQuick_Fwd_ManagedResource.h"

// We cannot include the ResourceContainer main class, because it depende on 
// ManagedResource, which header includes the Impl_ManagedResource header.
#include "ResManQuick_impl_ResourceContainer_Types.h"
#include "../H/ResManQuick_System.h"
#include "ResManQuick_impl_ResourceGroupLink.h"
#include "Loader/ResManQuick_impl_Loader_PerResourceData.h"
#include "Loader\Fwd\ResManQuick_Fwd_LoadUpdate.h"
#include "Loader\Fwd\ResManQuick_Fwd_LoaderMain.h"
#include "ResManQuick_impl_ResourceGroupCombiCont.h"
#include "../H/ResManQuick_Fwd.h"

#include "Utils/ResourceCombinableParamsCalculator.h"

/********************************************************************************************************
* TODO:
*********************************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			class ResourceGroupCombi;
			class Impl_ResourceManager;

			/******************************************************************************************************
			* TODO:
			* 
			* 1. 
			******************************************************************************************************/
			class Impl_ManagedResource
			{
			public:
				// We need custom destructor to destroy the loader and the resource
				~Impl_ManagedResource();
				// Construction
				Impl_ManagedResource
				(
					ManagedResource                                             *pInResource,
					Impl_ResourceManager                                        *pInOwnerManager,
					const ResMan::ResourceParams                                &InParams, 
					size_t                                                       InID,
					ResourceContainer_Iterator                                   InIter, 
					ResourceContainer_Iterator                                   InIterRoleResource
				);
				// This function is to be called after the resource has been registered in the resource repository
				void                                                             Notify_AfterRegisteredInResourceRepo();

				/*********************************************************************************************************************************
				* GETTERS
				*********************************************************************************************************************************/
				ManagedResource                                  *GetInterface                    () const         { return _pInterface; }

				Impl_ResourceManager                             *GetOwnerManager                 () const         { return _pOwnerManager; }
				ResourceContainer_Iterator                        GetIter                         () const         { return _it; }
				ResourceContainer_Iterator                        GetIter_RoleResource            () const         { return _it_role_resource; }
				const ResMan::ResourceParams                     &GetParams                       () const         { return _params; }
				const ResMan::ResourceLoadingState               &GetLoadingState                 () const         { return _loadingState;  }
				int                                               GetLinkCount                    () const         { return _linkCount;  }
				std::shared_ptr<ResMan::IResourceLoader>          GetLoader                       () const         { return _params.pLoader; }

				const Loader::Loader_PerResourceData             *GetLoaderData                   () const         { return &_loaderData; }

				size_t                                            GetResourceID                   () const         { return _id; }

				bool                                                                IsAddressableSubresource              () const;
				const ResMan::LoadableMainSubresourceParams                        *GetMainSubresourceParams              () const;
				const ResMan::LoadableAddressableSubresourceParams                 *GetAddressableSubresource             () const;

				/*********************************************************************************************
				* UPDATING PARAMS
				*********************************************************************************************/
				void                                                                SetLoader                             (std::shared_ptr<ResMan::IResourceLoader> pInLoader);

				/**********************************************************************************************
				* WORKING WITH REAL RESOURCE
				**********************************************************************************************/
				// Real loaded resource: can be nullptr
				void                                             *GetRealResource                 () const         { return _pNullOrRealResource; }
				// Call this function to destroy the real resource (provided for the unloader)
				void                                              DestroyRealResource             ();

				/*********************************************************************************************
				* UPDATING PARAMS
				*********************************************************************************************/
				// Get calculated combinable params
				const ResMan::CombinableParams                   &GetCombinableParams             () const;
				const ResMan::CombinableParams                    *GetOwnCombinableParams         () const;
				ResMan::EResourceCombinableParametersMode         GetCombinableParamsMode         () const;
				void                                              SetCombinableParamsMode         (ResMan::EResourceCombinableParametersMode InCombinableParamsMode);
				void                                              UpdateCombinableParams          (const ResMan::CombinableParams&);

				/*******************************************************************************************************
				* Get ptr to container of links to group resource record
				*******************************************************************************************************/
				const ResourceGroupLink_Cont                     *GetGroupLinks                   () const         { return &_groups; }

				/*******************************************************************************************************
				* IManagedResource interface implementing functions
				********************************************************************************************************/
				ResMan::IManagedResource::GroupEnumerator         GetActiveGroups                 () const;

				void                                              PrecacheSubresources           (const ResMan::LoadCommand& InParams);
				void                                              PrecacheSubresourcesSyncho     (const ResMan::LoadCommand& InParams);

				ResMan::SubresourceRoleSet                        GetLoadedSubresources          () const;
				ResMan::SubresourceRoleSet                        GetLoadingFailedSubresources   () const;
				ResMan::SubresourceRoleSet                        GetLoadingFailedDependencyLists() const;

				void                                              PrecacheDependencyList         (const ResMan::PrecacheDependenciesCommand& InParams);

				/*******************************************************************************************************
				* Event handlers
				*******************************************************************************************************/
				// This method is to be called when the failure has been popped from the queue
				void                                              Notify_FailHasBeenProcessed    ();
				void                                              Notify_AttachedToGroup         (ManagedGroup *pInGroup, GroupResourceRecordPointer InIterGroupResource);
				void                                              Notify_DetachedFromGroup       (ManagedGroup *pInGroup);
				// This function is called when combinable parameters of the given group updated
				void                                              Notify_GroupParamsUpdated      (ManagedGroup *pInGroup);
				// This function is called when combinable parameters of group combination updated
				void                                              Notify_GroupCombiParamsUpdated ();
				// This function is called when agreggated (both group and resource) combinable params to be changed
				void                                              Notify_CombinableParamsChanged ();

				/*******************************************************************************************************
				* This functions to be used by the loader main class exclusively
				*******************************************************************************************************/
				// The main loader calls EnqueueCommandForResource(...) before enqueueing any command into the command queue;
				// It's necessary to let the resource object pass to the loader the loader-related data connected with this resource.
				void                                              EnqueueCommandForResource
				(
					const Loader::LoadRequestParams&                InRequestParams,
					size_t                                          InActualPriority,
					Loader::LoadCommandBase                        *pInCommand,
					Loader::LoadingLoopCommandQueues               *pInLoadingLoopQueues
				);
				// The main loader calls EndLoad() right after dequeueing the load command result
				void                                              EndLoad                        (const Loader::LoaderCommandResultRecord& InResultRecord, Loader::LoadingLoopCommandQueues *pInLoadingLoopCommandQueues, bool bInRecursiveCommandsSucceeded);
				// The main loader calls Notify_LoadBegun() right after the loading command enqueued
				void                                              Notify_LoadBegun               (const Loader::LoadRequestParams& InInitialRequest);
				// The main loader calls Notify_LoadEnded() right after the loading command result is processed
				void                                              Notify_LoadEnded               (const Loader::LoadRequestParams& InInitialRequest, const Loader::LoadUpdate& InUpdate, void *pInNewResourceObject);
				// The main loader calls Notify_RecursiveLoadEnded() right after the loading command result is processed
				void                                              Notify_RecursiveLoadEnded      (const Loader::LoadRequestParams& InInitialRequest, bool bIsFailed);
				void                                              Notify_TotalCommandsToBePoppedZeroed ();

				// To be called when resource loaded as a subresource
				void                                              Notify_LoadedAsAdressableSubresource(void *pInNewResource);

				// Get loader loop data related to the given loop
				const Loader::Loader_PerResource_PerLoadingLoop  *GetLoaderLoopData              (Loader::LoadingLoop *pLoadingLoop) const;
				Loader::Loader_PerResource_PerLoadingLoop        *GetLoaderLoopData              (Loader::LoadingLoop *pLoadingLoop);

				void                                              DetachResource_FromAllGroups   ();

				// Get number of failed commands enqueued for processing, but has not been yet processed
				size_t                                            GetNumFailsToBePopped          () const { return _numFailsToBePopped; }

				/************************************************************************************************************************
				* Unloading
				*************************************************************************************************************************/
				void                                              UnloadDependencyLists          ();
				void                                              Unload                         (const ResMan::UnloadCommand& InUnloadCommand);

			private:
				ManagedResource                                  *_pInterface;

				void                                              _EraseLinks_ToGroup            (ManagedGroup *pInGroup);

				// Reloads the subresources and dependency lists according to params
				void                                              _ReloadSubresources_BasedOnParams              (bool bInForceSynchro, bool bInForceReload_DependencyLists, bool bInForceReload_Subresources);
				void                                              _FixAll_ResourceRecordPositions_OfAllGroups    ();

				void                                              _RebindToCombi_AndRecalculateCombinableParams                                  ();
				void                                              _RebindToCombi                                                                 ();

				Impl_ResourceManager                             *_pOwnerManager;

				// iterator to this element in the ResourceCont of the ResourceManager object
				ResourceContainer_Iterator                        _it;
				// iterator to record about this resource in the ResourceCont of the role that this resource belongs to
				ResourceContainer_Iterator                        _it_role_resource;
				ResMan::ResourceParams                            _params;
				ResMan::ResourceLoadingState                      _loadingState;
				int                                               _linkCount;

				ResMan::ResourceCombinableParamsCalculator        _combinableParamsCalculator;

				GroupMask                                         _groupMask;
				ResourceGroupLink_Cont                            _groups;
				ResourceGroupCombi                               *_pGroupCombi;

				size_t                                            _numFailsToBePopped = 0;
				size_t                                            _id;

				void                                             *_pNullOrRealResource = nullptr;
				Loader::Loader_PerResourceData                    _loaderData;
			};
			boost::optional<GroupResourceRecordPointer> FindGroupLink(const ResourceGroupLink_Cont *pGroupLinks, ManagedGroup *pInGroup);
		}
	}
}