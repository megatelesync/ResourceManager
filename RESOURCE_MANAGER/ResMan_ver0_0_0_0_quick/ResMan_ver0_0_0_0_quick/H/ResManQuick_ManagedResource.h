#pragma once

#include "ResManQuick_Fwd.h"
#include "ResManQuick_System.h"
#include "interface/IManagedResource.h"
#include "../PrivateH/ResManQuick_impl_ManagedResource.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			// Fwd declarations of the Impl namespace
			class Impl_ResourceManager;
		}
		/***********************************************************************************************************************
		* TODO:
		* 1. Interface:
		* 1.1. Where to get the public resources params?
		* 1.1.1. Reference to some global params?
		***********************************************************************************************************************/
		class ManagedResource : public ResMan::IManagedResource
		{
		public:
			// Construction
			ManagedResource
			(
				Impl::Impl_ResourceManager                                  *pInOwnerManagerImpl,
				const ResMan::ResourceParams                                &InParams,
				size_t                                                       InID,
				Impl::ResourceContainer_Iterator                             InIter,
				Impl::ResourceContainer_Iterator                             InIterRoleResource
			);

			virtual ResMan::IResourceManager                     *GetOwnerManager() const override;
			
			// true if the resource to be returned has NO URL
			virtual bool                                          HasNoURL                 () const override            { return GetLoader()->HasNoURL(); }
			// unical URL of the resource to be returned
			virtual const std::string&                            GetURL                   () const override            { return GetLoader()->GetURL(); }
			virtual const ResMan::ResourceLoaderParams           *GetLoaderParams          () const override            { return &(GetLoader()->GetParams()); }
			// Public parameters of the resource
			virtual const ResMan::LoadableMainSubresourceParams  *GetMainSubresourceParams() const override;
			virtual const ResMan::LoadableAddressableSubresourceParams   *GetAddressableSubresource() const override;
			virtual const ResMan::PublicResourceParams&           GetParams                () const override            { return GetImpl()->GetParams().pub; }
			// Get parameters that may be influenced by the set of groups that this resource belongs to
			virtual const ResMan::CombinableParams&               GetCombinableParams      () const override            { return GetImpl()->GetCombinableParams(); }
			// Loader is available but for const access only
			virtual std::shared_ptr<ResMan::IResourceLoader>      GetLoader                () const override            { return GetImpl()->GetLoader();  }
			// Group that this resource belongs to this moment
			// (or nullptr if no owner group linked with the resource;
			// if group is nullptr it does not mean that this resource is not member of any group;
			// it only means that the resource is not OWNED by any group)
			virtual ResMan::IManagedGroup                        *GetGroup                 () const override            { return Util::NotYetImpl<ResMan::IManagedGroup *>("Where to get managed group?"); }
			// Enumerate active groups this resource belongs to
			virtual GroupEnumerator                               GetActiveGroups          () const override            { return GetImpl()->GetActiveGroups(); }

			void                                                  SetLoader                (std::shared_ptr<ResMan::IResourceLoader> pInLoader);

			/*********************************************************************************************
			* UPDATING PARAMS
			*********************************************************************************************/
			virtual const ResMan::CombinableParams               *GetOwnCombinableParams             () const override;
			virtual ResMan::EResourceCombinableParametersMode     GetCombinableParamsMode            () const override;
			virtual void                                          SetCombinableParamsMode            (ResMan::EResourceCombinableParametersMode InCombinableParamsMode) override;
			virtual void                                          UpdateCombinableParams             (const ResMan::CombinableParams&) override;

			/*************************************************************************************
			* PRECACHING/UNLOADING
			**************************************************************************************/
			// Precache the given set or subresources asynchronously.
			// Precaching and unloading are always done recursively, so a dependency is always to be precached 
			// if the corresponding subsresource is set in the command params;
			// The dependencies are loaded if there were not yet.
			virtual void                                          PrecacheSubresources                   (const ResMan::LoadCommand& InParams) override;
			virtual void                                          UnloadSubresources                     (const ResMan::UnloadCommand& InParams) override;
			// Precache subsresource and block the execution until they're loaded (or failed to load)
			virtual void                                          PrecacheSubresourcesSyncho             (const ResMan::LoadCommand& InParams) override;

			// State of the last precache or unload command
			virtual const ResMan::ResourceLoadingState&           GetLoadingState                         () const override { return GetImpl()->GetLoadingState(); }

			virtual ResMan::SubresourceRoleSet                    GetLoadedSubresources                   () const override { return GetImpl()->GetLoadedSubresources(); }
			virtual ResMan::SubresourceRoleSet                    GetLoadingFailedSubresources            () const override { return GetImpl()->GetLoadingFailedSubresources(); }
			virtual ResMan::SubresourceRoleSet                    GetLoadingFailedDependencyLists         () const override { return GetImpl()->GetLoadingFailedDependencyLists(); };

			/*************************************************************************************
			* DEPENDENCY LIST
			**************************************************************************************/
			// When issuing a dependency list precache command the subresources
			// FOR WHICH DIRECT dependencies to be load 
			// For example, you want to cache indirect and direct dependencies of the mesh;
			// If you specify only the mesh subresource, then only direct dependencies of the mesh will be loaded
			// (i.e. materials, animations etc.); 
			// To load indirect dependencies (e.g. specular, diffuse, normal textures),
			// You must specify the corresponding groups also (in addition to specifying the mesh subresource).
			virtual void                                          PrecacheDependencyList                  (const ResMan::PrecacheDependenciesCommand& InParams) override;
			// Dependency list is always unloaded only for the current resource
			// (no recursive clearing is supported)
			virtual void                                           UnloadDependencyList                   () override;
			// Get list of resource dependencies
			virtual const ResMan::ResourceDependencyList&          GetDependencyList                      () const override;

			/*************************************************************************************
			* REFERENCE COUNTING
			*************************************************************************************/
			// Add reference and get pointer to the resource
			virtual void                                          *AddRef                                 () override;
			virtual void                                           Release                                () override;

			/**************************************************************************************
			* REAL RESOURCE
			**************************************************************************************/
			// Get real resource (returns nullptr if not loaded)
			virtual void                                          *GetResource                            () const override;

			/************************************************************************************************************
			* Acessing implementation
			************************************************************************************************************/
			const Impl::Impl_ManagedResource                     *GetImpl                                 () const          { return &_impl; }
			Impl::Impl_ManagedResource                           *GetImpl                                 ()                { return &_impl; }

		private:
			Impl::Impl_ManagedResource                            _impl;
		};
	}
}