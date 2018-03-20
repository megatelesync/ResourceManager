#pragma once

#include <string>
#include <memory>
#include "../Utils/ResourceDependencyList.h"
#include "CommandState.h"
#include "LoadCommand.h"
#include "CombinableParams.h"
#include "../Core/ResourceManagerConstants.h"
#include "IResourceType.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class LoadableAddressableSubresourceParams;
			class ResourceLoaderParams;
			class LoadableMainSubresourceParams;
			class IManagedGroup;
			class IResourceLoader;
			class ISubresourceRole;

			// Resource parameters that are accessible from the managable resource interface
			class PublicResourceParams
			{
			public:	
				/**************************************************************************************
				* GETTERS
				**************************************************************************************/
				ISubresourceRole                     *GetRole                         () const;
				const CombinableParams               *GetCombinableParams             () const { return &_combinableParams; }
				const std::string&                    GetName                         () const { return _name; }

				/**************************************************************************************
				* SETTERS (FOR INITIALIZATION)
				**************************************************************************************/
				void                                  SetRole                         (ISubresourceRole *pInValue);
				CombinableParams                     *GetCombinableParams             () { return &_combinableParams; }
				void                                  SetName                         (const std::string& InName) { _name = InName; }

			private:
				ISubresourceRole                    *_pRole;
				CombinableParams                     _combinableParams;
				std::string                          _name = "{UNNAMED}";
			};

			struct ResourceParams
			{		
				/*************************************************************************************************************
				* Parameters to be exposed
				**************************************************************************************************************/
				PublicResourceParams                 pub;

				/**************************************************************************************************************
				* The loader:
				* Irrespective of whether the resource was loaded as an addressable subresource or not,
				* this loader pointer must point to the loader that created this resource.
				*
				* Loader will be owned by the managed resource
				* Must be created using the C++ new operator.
				**************************************************************************************************************/
				std::shared_ptr<IResourceLoader>     pLoader;

				/*************************************************************************************************************
				* Addressable subresource
				**************************************************************************************************************/
				// Index of the subresource role
				// (negative if the resource is not addressable subresource of the given resource)
				NullableAddressableSubresourceIndex   addressable_subresource_index;

				/*************************************************************************************************************
				* Getters
				**************************************************************************************************************/
				std::shared_ptr<IResourceLoader>     GetLoader                                    () { return pLoader; }
			};

			/********************************************************************************************
			* Parameters of the main subresource, created by the resource loader
			********************************************************************************************/
			class LoadableMainSubresourceParams
			{
			public:
				/***************************************************************************************
				* GETTERS
				****************************************************************************************/
				ISubresourceRole                   *GetRole()           const;
				IResourceType                      *GetResourceType()           const;
				const PublicResourceParams&         GetPublicResourceParams()           const { return _params; }

				/***************************************************************************************
				* SETTERS (FOR INITIALIZATION ONLY)
				****************************************************************************************/
				void                                SetRole(ISubresourceRole *pInRole);
				void                                SetResourceType(IResourceType *pInResourceType);
				void                                SetPublicResourceParams(const PublicResourceParams& InParams);

			private:
				IResourceType                      *_pResourceType;
				PublicResourceParams                _params;
			};

			/******************************************************************************************
			* Managed resource
			******************************************************************************************/
			class IManagedResource
			{
			public:
				/*************************************************************************************
				* TYPEDEFS
				**************************************************************************************/
				using GroupEnumerator = std::unique_ptr<Util::IEnumerator<IManagedGroup>>;

				/**********************************************************************************
				* CONSTRUCTION
				**********************************************************************************/
				IManagedResource                                                          () = default;

				/*************************************************************************************
				* SPECIAL FUNCTIONS
				**************************************************************************************/
				virtual                                      ~IManagedResource            ()                                       = default;

				IManagedResource                                                          (const IManagedResource&)                = delete;
				IManagedResource&                            operator=                    (const IManagedResource&)                = delete;

				/*************************************************************************************
				* GETTERS
				**************************************************************************************/
				virtual IResourceManager                     *GetOwnerManager             () const                                 =0;

				// true if the resource to be returned has NO URL
				virtual bool                                  HasNoURL                    () const                                 =0;
				// unical URL of the resource to be returned
				virtual const std::string&                    GetURL                      () const                                 =0;
				virtual const ResourceLoaderParams           *GetLoaderParams             () const                                 =0;
				// Get main subresource params (will return nullptr if resource is addressable subresource)
				virtual const LoadableMainSubresourceParams  *GetMainSubresourceParams    () const                                 =0;
				// Get parameters of the addressable subresources (return nullptr if this resource is NOT addressable subresource)
				virtual const LoadableAddressableSubresourceParams *GetAddressableSubresource() const = 0;
				// Public parameters of the resource
				virtual const PublicResourceParams&           GetParams                   () const                                 =0;
				// Loader is available but for const access only
				// WARNING!!! WARNING!!! WARNING!!! 
				// It was a design error to expose the loader and this getter is deprecated!!!
				// Do NOT access it for thread-safety reasons.
				// To access immutable properties of the loader use corresponding getters of the IManagedResource interface itself.
				virtual std::shared_ptr<IResourceLoader>                GetLoader                   () const                                 =0;
				// Group that this resource belongs to this moment
				// (or nullptr if no owner group linked with the resource;
				// if group is nullptr it does not mean that this resource is not member of any group;
				// it only means that the resource is not OWNED by any group)
				virtual IManagedGroup                        *GetGroup                    () const                                 =0;
				// Enumerate active groups this resource belongs to
				virtual GroupEnumerator                       GetActiveGroups             () const                                 =0;

				/*********************************************************************************************
				* UPDATING PARAMS
				*********************************************************************************************/
				virtual void                                  SetLoader                   (std::shared_ptr<IResourceLoader> pInLoader)                          =0;

				/*********************************************************************************************
				* COMBINABLE PARAMS
				*********************************************************************************************/
				// Get actual combinable params to be used for this resource 
				// (this params are calculated based both on own combinable params and combinable params of the group)
				virtual const CombinableParams&               GetCombinableParams                    () const                                                   =0;
				virtual const CombinableParams               *GetOwnCombinableParams                 () const                                                   =0;
				virtual EResourceCombinableParametersMode     GetCombinableParamsMode                () const                                                   =0;
				virtual void                                  SetCombinableParamsMode                (EResourceCombinableParametersMode InCombinableParamsMode) =0;
				virtual void                                  UpdateCombinableParams                 (const CombinableParams&)                                  =0;

				/*************************************************************************************
				* PRECACHING/UNLOADING
				**************************************************************************************/
				// Precache the given set or subresources asynchronously.
				// Precaching and unloading are always done recursively, so a dependency is always to be precached 
				// if the corresponding subsresource is set in the command params;
				// The dependencies are loaded if there were not yet.
				virtual void PrecacheSubresources                                         (const LoadCommand& InParams)            =0;
				virtual void UnloadSubresources                                           (const UnloadCommand& InParams)          =0;
				// Precache subsresource and block the execution until they're loaded (or failed to load)
				virtual void PrecacheSubresourcesSyncho                                   (const LoadCommand&) =0;

				// Current state of loading.
				virtual const ResourceLoadingState&           GetLoadingState             () const                                 =0;

				// Get set of loaded subresources (for loaded dependency address the dependency list)
				virtual SubresourceRoleSet                    GetLoadedSubresources       () const                                 =0;
				virtual SubresourceRoleSet                    GetLoadingFailedSubresources() const                                 =0;
				virtual SubresourceRoleSet                    GetLoadingFailedDependencyLists() const                              =0;

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
				virtual void PrecacheDependencyList                                       (const PrecacheDependenciesCommand& InParams)                                       =0;
				// Dependency list is always unloaded only for the current resource
				// (no recursive clearing is supported)
				virtual void UnloadDependencyList                                         ()                                       =0;
				// Get list of resource dependencies
				virtual const ResourceDependencyList&         GetDependencyList           () const                                 =0;

				/*************************************************************************************
				* REFERENCE COUNTING
				*************************************************************************************/
				// Add reference and get pointer to the resource
				virtual void                                  *AddRef                     ()                                       =0;
				virtual void                                   Release                    ()                                       =0;

				/**************************************************************************************
				* REAL RESOURCE
				**************************************************************************************/
				// Get real resource (returns nullptr if not loaded)
				virtual void                                  *GetResource                () const                                 =0;
			};

			const LoadableMainSubresourceParams               *GetMainResourceParameters(IManagedResource *pInResource);
			IResourceType                                     *GetResourceType(IManagedResource *pInResource);
			SubresourceRoleSet                                 GetApplicableSubresourceRoles(IManagedResource *pInResource);

			/*********************************************************************************************************************************************************
			* Worrking with groups
			**********************************************************************************************************************************************************/
			// Getting count of active groups that this resource belong to
			size_t                                            GetActiveGroupsCount(IManagedResource *pInResource);
			// Zero or groups that this resource is attached to are active
			size_t                                            IsNotAttachedToAnyActiveGroup(IManagedResource *pInResource);
			// Is this resource attached to the given ACTIVE group
			bool                                              IsAttachedToActiveGroup(IManagedResource *pInResource, IManagedGroup *pInGroup);

			/*********************************************************************************************************************************************************
            * Precaching 
			**********************************************************************************************************************************************************/
			void                                               PrecacheAll_DependencyLists_And_Subresources_Synchro(IManagedResource *pInResource, Urgency InUrgency = Urgency_Normal);
			void                                               PrecacheAll_DependencyLists_And_Subresources(IManagedResource *pInResource);

			// Precache all subresources of the given resource
			void                                               PrecacheAllSubresources(IManagedResource *pInResource, Urgency InUrgency = Urgency_Normal);
			void                                               PrecacheAllSubresourcesSynchro(IManagedResource *pInResource, Urgency InUrgency = Urgency_Normal);
			void                                               PrecachedAllDependencyLists(IManagedResource *pInResource, Urgency InUrgency = Urgency_Normal);
			// Precache the given set of subresources
			void                                               PrecacheSubresources(IManagedResource *pInResource, SubresourceRoleSet InSubresourcesToLoad, Urgency InUrgency = Urgency_Normal);
			void                                               PrecacheSubresourcesSynchro(IManagedResource *pInResource, SubresourceRoleSet InSubresourcesToLoad, Urgency InUrgency = Urgency_Normal);
			// Precache the given set of dependency lists
			void                                               PrecachedDependencyLists(IManagedResource *pInResource, SubresourceRoleSet InDependencyListsToLoad, Urgency InUrgency = Urgency_Normal);

			/*********************************************************************************************************************************************************
			* Unloading
			**********************************************************************************************************************************************************/
			void                                               UnloadAllSubresources(IManagedResource *pInResource, Urgency InUrgency = Urgency_Normal);

			/*********************************************************************************************************************************************************
			* Getting loading state of the resource
			**********************************************************************************************************************************************************/
			// Get set of loading dependency lists for current resource only (does not include dependency lists of loading dependencies of this resource)
			SubresourceRoleSet                                 GetLoadingDependencyLists                        (IManagedResource *pInResource);
			bool                                               AreDependencyListsLoading                        (IManagedResource *pInResource);
			// Get set of dependency lists queued for loading (including recursive)
			SubresourceRoleSet                                 GetDependencyLists_InLoadingQueue                (IManagedResource *pInResource);
			bool                                               AreDependencyLists_InLoadingQueue                (IManagedResource *pInResource);
			// Get set of subresources queued in loading queue (including recursive)
			SubresourceRoleSet                                 GetSubresources_InLoadingQueue                   (IManagedResource *pInResource);
			bool                                               AreSubresources_InLoadingQueue                   (IManagedResource *pInResource);
			// Non-recursive loaded dependency lists
			SubresourceRoleSet                                 GetLoaded_DependencyLists                        (IManagedResource *pInResource);
			bool                                               AreDependencyLists_Loaded                        (IManagedResource *pInResource);
			// Get recursively-loaded dependency lists
			//SubresourceRoleSet                                 GetLoaded_DependencyLists_Recursive(IManagedResource *pInResource);
			// Non-recursive loaded subresources
			SubresourceRoleSet                                 GetLoaded_Subresources(IManagedResource *pInResource);
			bool                                               AreSubresources_Loaded(IManagedResource *pInResource);
			SubresourceRoleSet                                 GetLoadingFailed_Subresources(IManagedResource *pInResource);
			bool                                               AreSubresources_Failed_Loading(IManagedResource *pInResource);
			SubresourceRoleSet                                 GetLoadingFailed_DependencyLists(IManagedResource *pInResource);
			bool                                               AreDependencyLists_Failed_Loading(IManagedResource *pInResource);



			/**************************************************************************************************
			* Typed managed resource.
			*
			* This resource is like a pointer, objects of this class are always to be passed by value;
			* To access corresponding member of the IManagedResource, operator-> is provided.
			*
			* Const-ness: never const
			**************************************************************************************************/
			template<class RealResourceType>
			class TypedManagedResource
			{
			public:
				/**************************************************************************************************************
				* TYPEDEFS
				**************************************************************************************************************/
				using ThisType = TypedManagedResource<RealResourceType>;

				/**************************************************************************************************************
				* CONSTRUCTION
				**************************************************************************************************************/
				TypedManagedResource                                                       () : _pManagedResource(nullptr) {}
				TypedManagedResource                                                       (IManagedResource *pInManagedResource) : _pManagedResource(pInManagedResource) {}

				/**************************************************************************************************************
				* SPECIAL FUNCTIONS
				**************************************************************************************************************/
				TypedManagedResource                                                       (const ThisType&) = default;
				ThisType&                                     operator=                    (const ThisType&) = default;

				TypedManagedResource                                                       (ThisType&&) = default;
				ThisType&                                     operator=                    (ThisType&&) = default;

				/***************************************************************************************************************
				* RESOURCE ACCESS
				***************************************************************************************************************/
				IManagedResource                              *operator->                  () const { return _pManagedResource; }

				/***************************************************************************************************************
				* GETTERS
				***************************************************************************************************************/
				RealResourceType                              *GetTypedRealResource        () const                                { return  static_cast<RealResourceType *>(_pManagedResource->GetResource()); }
				IManagedResource                              *GetManagedResource          () const                                { return _pManagedResource; }

			private:
				IManagedResource                              *_pManagedResource;
			};

			/******************************************************************************************
			* IManagedResource utils implementation
			******************************************************************************************/
			
			inline SubresourceRoleSet GetApplicableSubresourceRoles(IManagedResource *pInResource)
			{
				return GetResourceType(pInResource)->GetParams().GetSubresourceRoles().GetApplicableSubresourceRoles();
			}

			/******************************************************************************************
			* PublicResourceParams implementation
			******************************************************************************************/
			inline ISubresourceRole *PublicResourceParams::GetRole() const
			{
				return _pRole;
			}
			inline void PublicResourceParams::SetRole(ISubresourceRole *pInValue)
			{
				_pRole = pInValue;
			}


			/************************************************************************************************************
			* LoadableMainSubresourceParams implementation
			************************************************************************************************************/
			inline ISubresourceRole *LoadableMainSubresourceParams::GetRole() const
			{
				return _params.GetRole();
			}

			inline IResourceType *LoadableMainSubresourceParams::GetResourceType() const
			{
				return _pResourceType;
			}

			inline void LoadableMainSubresourceParams::SetPublicResourceParams(const PublicResourceParams& InParams)
			{
				_params = InParams;
			}

			inline void LoadableMainSubresourceParams::SetRole(ISubresourceRole *pInRole)
			{
				_params.SetRole(pInRole);
			}

			inline void LoadableMainSubresourceParams::SetResourceType(IResourceType *pInResourceType)
			{
				_pResourceType = pInResourceType;
			}
		}
	}
}