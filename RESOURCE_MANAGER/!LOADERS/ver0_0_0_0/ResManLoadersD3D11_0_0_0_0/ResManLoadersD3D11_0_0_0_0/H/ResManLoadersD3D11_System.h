#pragma once

/****************************************************************************************
* This file is to be included in all the resource files
****************************************************************************************/
#include "Override/MASTER_ResourceManager_Override.h"
#include "Subsystem/MASTER_ResourceManager_Subsystem.h"

#include "CoreUtil/CoreUtil_Assert.h"

#include <D3D11.h>

#include <string>

namespace Dv
{	
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			namespace ResMan = Dv::ResourceManager::Ver_0_0_0_0;

			/***********************************************************************************
			* ResTypeDescr_Roles_Base
			*
			* Base class for all typed role collections
			***********************************************************************************/
			struct ResTypeDescr_Roles_Base
			{
				// role that represents the created main resource
				// (no applicable for temporary resources)
				ResMan::ISubresourceRole             *pStd;
			};
			
			class D3D11ResTypeDescrBase : public ResMan::IResourceTypeDescr
			{
			public:
				virtual void                                           Delete        (void *pInResource)         override;
				virtual const ResMan::ResourceTypePublicParams&        GetParams     () const                    override;

			protected:
				ResMan::ResourceTypePublicParams        _params;
			};

			/***************************************************************************************
			* Typedef D3D11 resource type descriptor base class
			*
			* TEMPLATE ARGUMENTS:
			*      RoleSetType = Type of the class that describes set of applicable roles
			*      (must be subclass of ResTypeDescr_Roles_Base);
			*      RealResourceType = type of the real resource that the managed resource will grant access to.			
			****************************************************************************************/
			template<class RoleSetTypeArg, class RealResourceTypeArg>
			class TypedD3D11ResTypeDescrBase : public D3D11ResTypeDescrBase
			{
			public:
				using RoleSetType                   = RoleSetTypeArg;
				using RealResourceType              = RealResourceTypeArg;

				explicit                            TypedD3D11ResTypeDescrBase       (const char *pInName, const char *pInSubdirName);

				const RoleSetType&                  GetRoles                         ()                 const { return _roles; }

				/***************************************************************************************************
				* Overload the role registration function:
				* By default, registers the std role only.
				****************************************************************************************************/
				virtual void                        RegisterApplicableRoles  (ResMan::IResourceManagerSubsystem *pInSubsys)    override;

			protected:
				RoleSetType                       *GetRolesPointer      ()                       { return &_roles;  }

				// This function to be called by the all implementing classes that want to register std roles
				void                             _RegisterDefaultRoles  (ResMan::IResourceManagerSubsystem *pInSubsys);

				RoleSetType                        _roles;
			};

			/*************************************************************************************************
			* Base class for any D3D11 loader
			**************************************************************************************************/
			class D3D11ResourceLoaderBase : public ResMan::IResourceLoader
			{
			public:
				/*******************************************************************************************************************
				* Constructors
				*******************************************************************************************************************/
				// constructs non-referenceable loader
				D3D11ResourceLoaderBase                                (ID3D11Device *pInDev, ResMan::ISubresourceRole *pInRole, ResMan::IResourceType *pInResourceType);
				// constructs referenceable loader with the given URL
				D3D11ResourceLoaderBase                                (const std::string& InURL, ID3D11Device *pInDev, ResMan::ISubresourceRole *pInRole, ResMan::IResourceType *pInResourceType);

				/***************************************************************************************
				* Parameters of the loader itself
				***************************************************************************************/
				virtual const ResMan::ResourceLoaderParams&                       GetParams                           () const override;

				virtual const ResMan::LoadableMainSubresourceParams&              GetMainResourceParams               () const override;

				virtual bool                                                      HasNoURL                            () const override;
				virtual const std::string&                                        GetURL                              () const override;

				// By default, whay must be loaded role set is equal to the corresponding role set of the resource type class
				// (i.e. that is returned by IResourceType::GetWhatMustBeLoaded_ToCreateResource())
				virtual ResMan::SubresourceRoleSet                                WhatMustBeLoaded                    (void *pOldPtrOrNull) const override;

				/**********************************************************************************************************************
				* By default, the resource contains no adressable subresources.
				**********************************************************************************************************************/
				virtual size_t                                                    GetNumAddressableSubresources                   (ResMan::ISubresourceRole *pInSubresource) const override;
				virtual size_t                                                    GetNumAddressableSubresources                   (ResMan::SubresourceRoleIndex InSubresourceRoleIndex) const override;
				virtual const ResMan::LoadableAddressableSubresourceParams        &GetAddressableSubresourceParams                (ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex) const override;
				virtual const ResMan::LoadableAddressableSubresourceParams        &GetAddressableSubresourceParams                (ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex) const override;
				virtual void                                                       LinkAdressableSubresourceTo                    (ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex, ResMan::IManagedResource *pInResource) override;
				virtual void                                                       LinkAdressableSubresourceTo                    (ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex, ResMan::IManagedResource *pInResource) override;
				virtual void                                                      *LoadCreateAndGetAddressableSubresource         (ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex, void *pOldPtrOrNull) override;
				virtual void                                                      *LoadCreateAndGetAddressableSubresource         (ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex, void *pOldPtrOrNull) override;
				virtual ResMan::SubresourceRoleSet                          GetAddressableSubresourceRoles                 () const override;

				/**********************************************************************************************************************
				* By default, no content is to be loaded and unloaded,
				* and the loaded temporary content role set is empty.
				**********************************************************************************************************************/
				virtual bool                                                       LoadTemporaryOwnContent                        (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                       UnloadTemporaryOwnContent                      (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual ResMan::SubresourceRoleSet                          WhatOwnTemporaryContentLoaded                  () const override;

				/**********************************************************************************************************************
				* By default, dependency list contains zero elements,
				* Commands for loading, unload, and linking dependencies are ignored.
				**********************************************************************************************************************/
				//  This function to be called before loading dependencies of the given role (typically from the main thread)
				virtual void                                                       BeginLoadDependencyLists                      (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                       EndLoadingDependencyLists                   () override;

				virtual const ResMan::ResourceDependencyList&                      GetDependencyList                             () const override;
				virtual void                                                       LoadDirectDepdendencyListOf                   (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                       UnloadDirectDepedencyListOf                   (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                       LinkDependencyTo                              (ResMan::SubresourceRoleIndex InRoleIndex, DependencyIndex InDependency, ResMan::IManagedResource *pInResource) override;

			protected:
				HRESULT                                                _GetHResult_Create                       () const                  { return _hr_create; }
				void                                                   _SetHResult_Create                       (HRESULT InNewValue, const std::string& InMessage)      { _bHasCreationFailed = FAILED(InNewValue);  _hr_create = InNewValue; }
				bool                                                   _HasNoURL                                () const                  { return _bHasNoURL; }
				void                                                   _SetHResult_Load                         (const ResMan::ISubresourceRole *pInRole, HRESULT InNewValue) { _failedLoading.Include(pInRole); }
				const std::string&                                     _GetURL                                  () const                  { return _url; }
				ID3D11Device                                          *_GetDev                                  () const                  { return _pDev; }
				ResMan::SubresourceRoleSet                             _GetAddrSubresourceRoles                 () const                  { return _adressableSubresourceRoles; }
				const ResMan::LoadableMainSubresourceParams&           _GetParams_MainSubresource               () const                  { return _paramsMainSubresource; }

				/******************************************************************************************************************
				* Working with temp content
				******************************************************************************************************************/
				ResMan::SubresourceRoleSet                      _GetLoadedTempContentRoles               () const                  { return _loadedTemporaryContentRoles;  }
				// IsLoaded - return true, if the given content is loaded
				bool                                                   _IsLoaded                                (ResMan::ISubresourceRole *pInRole) const;
				void                                                   _MarkTempContentRole_Loaded              (ResMan::ISubresourceRole *pInRole);
				void                                                   _MarkTempContentRole_Unloaded            (ResMan::ISubresourceRole *pInRole);
				void                                                   _MakeTempContentRole_LoadFailed          (ResMan::ISubresourceRole  *pInRole);
				// Should temporary content be loaded or unloaded according to the current state of the load flags
				bool                                                   _ShouldLoadTempContent                   (ResMan::SubresourceRoleSet InRolesToLoad, ResMan::ISubresourceRole *pInRole);
				bool                                                   _ShouldTempContentBeUnloaded             (ResMan::SubresourceRoleSet InRolesToUnload, ResMan::ISubresourceRole *pInRole);

				/******************************************************************************************************************
				* Working with dependencies
				******************************************************************************************************************/
				const ResMan::ResourceDependencyList                  *_GetDependencyList                       () const{ return &_dependencies; }
				ResMan::ResourceDependencyList                        *_GetDependencyList                       () { return &_dependencies;  }

				/****************************************************************************************
				* Error-handling
				****************************************************************************************/
				virtual bool                                         HasCreationFailed                             () const override { return _bHasCreationFailed; }
				virtual ResMan::SubresourceRoleSet                   GetSubresourceRoles_FailedLoading             () const override { return _failedLoading; }
				virtual ResMan::SubresourceRoleSet                   GetSubresourceRoles_FailedLoadingDependencies () const override { return _failedLoadingDependencyLists; }

			private:				

				ID3D11Device                                            *_pDev;
				HRESULT                                                  _hr_create = S_OK;

				ResMan::ResourceLoaderParams                            _loaderParams;
				ResMan::SubresourceRoleSet                              _adressableSubresourceRoles;
				ResMan::LoadableMainSubresourceParams                   _paramsMainSubresource;
				ResMan::SubresourceRoleSet                              _loadedTemporaryContentRoles;

				/**************************************************************************************************************************
				* Working with failures
				**************************************************************************************************************************/
				ResMan::SubresourceRoleSet                              _failedLoading;
				ResMan::SubresourceRoleSet                              _failedLoadingDependencyLists;
				bool                                                    _bHasCreationFailed = false;

				bool                                                    _bHasNoURL;
				std::string                                             _url;
				ResMan::ResourceDependencyList                          _dependencies;
			};

			/***********************************************************************************************************
			* TypedD3D11ResourceLoaderBase
			*
			* All members behave like the base class (unless otherwise stated) 
			*
			* TEMPLATE ARGUMENTS:
			*		ResourceTypeArg is TYPED resource type 
			*       (must be subclass of ResMan::TypedResourceType)
			***********************************************************************************************************/
			template<class ResourceTypeArg>
			class TypedD3D11ResourceLoaderBase : public D3D11ResourceLoaderBase
			{
			public:
				/*******************************************************************************************************
				* TYPEDEFS
				*******************************************************************************************************/
				using ResourceType                 = ResourceTypeArg;
				using TypedResourceTypeDescr       = typename ResourceType::ResourceTypeDescr;
				using TypedRoleSet                 = typename TypedResourceTypeDescr::RoleSetType;
				// Real resource that this loader creates
				using RealResourceType             = typename TypedResourceTypeDescr::RealResourceType;

				/*******************************************************************************************************
				* CONSTRUCTION
				*******************************************************************************************************/
				TypedD3D11ResourceLoaderBase
				(
					ID3D11Device                          *pInDev, 
					ResMan::ISubresourceRole              *pInRole, 
					ResourceType                          *pInTypedResourceType
				);

				TypedD3D11ResourceLoaderBase
				(
					const std::string                      &InURL,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				);

				/*******************************************************************************************************
				* Typed functions
				* Overloading functions to take pointer to REAL Typed resource.
				*******************************************************************************************************/
				virtual ResMan::SubresourceRoleSet        WhatMustBeLoaded                     (void *pOldPtrOrNull) const override final;
				virtual void                              *CreateResourceAndGet                (void *pOldPtrOrNull) override final;

				virtual RealResourceType                 *Typed_CreateResourceAndGet           (RealResourceType *pOldPtrOrNull) =0;
				// By default, whay must be loaded role set is equal to the corresponding role set of the resource type class
				// (i.e. that is returned by IResourceType::GetWhatMustBeLoaded_ToCreateResource())
				virtual ResMan::SubresourceRoleSet        Typed_WhatMustBeLoaded               (RealResourceType *pOldPtrOrNull) const;

				/******************************************************************************************************************
				* Working with types
				******************************************************************************************************************/
				ResourceType                              *GetTyped_ResourceType               ()             const { return _pTypedResourceType;  }
				const TypedResourceTypeDescr              *GetTyped_ResourceTypeDescr          ()             const { return _pTypedResourceType->GetDescr();  }

				/******************************************************************************************************************
				* Working with roles
				******************************************************************************************************************/
				const TypedRoleSet                        &GetRoles                            ()             const { return GetTyped_ResourceTypeDescr()->GetRoles();  }

			protected:
				ResourceType                              *_pTypedResourceType;
			};

			/**********************************************************************************************************
			* TypedD3D11ResourceLoaderBase implementation
			**********************************************************************************************************/
			template<class ResourceTypeArg>
			TypedD3D11ResourceLoaderBase<ResourceTypeArg>::TypedD3D11ResourceLoaderBase
			(
				ID3D11Device                          *pInDev,
				ResMan::ISubresourceRole              *pInRole,
				ResourceType                          *pInTypedResourceType
			) : 
				D3D11ResourceLoaderBase(pInDev, pInRole, pInTypedResourceType->GetResourceType()),
				_pTypedResourceType(pInTypedResourceType)
			{
				//static_assert(std::is_base_of<ResMan::TypedResourceType, ResourceTypeArg>::value, "ResourceTypeArg has wrong base type");
			}

			template<class ResourceTypeArg>
			TypedD3D11ResourceLoaderBase<ResourceTypeArg>::TypedD3D11ResourceLoaderBase
			(
				const std::string                      &InURL,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				ResourceType                           *pInTypedResourceType
			) : D3D11ResourceLoaderBase(InURL, pInDev, pInRole, pInTypedResourceType->GetResourceType()),
				_pTypedResourceType(pInTypedResourceType)
			{
			}

			template<class ResourceTypeArg>
			ResMan::SubresourceRoleSet TypedD3D11ResourceLoaderBase<ResourceTypeArg>::WhatMustBeLoaded(void *pOldPtrOrNull) const
			{
				auto pTypedResource = static_cast<RealResourceType *>(pOldPtrOrNull);
				return Typed_WhatMustBeLoaded(pTypedResource);
			}

			template<class ResourceTypeArg>
			void *TypedD3D11ResourceLoaderBase<ResourceTypeArg>::CreateResourceAndGet(void *pOldPtrOrNull)
			{
				auto pTypedResource = static_cast<RealResourceType *>(pOldPtrOrNull);
				return Typed_CreateResourceAndGet(pTypedResource);
			}

			template<class ResourceTypeArg>
			ResMan::SubresourceRoleSet TypedD3D11ResourceLoaderBase<ResourceTypeArg>::Typed_WhatMustBeLoaded(RealResourceType *pOldPtrOrNull) const
			{
				return ResMan::GetSubresourceRoles_ThatMustBeLoadedToCreateResource(_GetParams_MainSubresource().GetResourceType());
			}

			/**********************************************************************************************************
			* TypedD3D11ResTypeDescrBase implementation
			**********************************************************************************************************/
			template<class RoleSetTypeArg, class RealResourceTypeArg>
			TypedD3D11ResTypeDescrBase<RoleSetTypeArg, RealResourceTypeArg>::TypedD3D11ResTypeDescrBase(const char *pInName, const char *pInSubdirName)
			{
				static_assert(std::is_base_of<ResTypeDescr_Roles_Base, RoleSetTypeArg>::value, "RoleSetTypeArg has wrong base type");
				assert(pInName != nullptr);
				_params.SetName(pInName);
				_params.SetSubdirectoryName(pInSubdirName);
			}

			template<class RoleSetTypeArg, class RealResourceTypeArg>
			void TypedD3D11ResTypeDescrBase<RoleSetTypeArg, RealResourceTypeArg>::RegisterApplicableRoles(ResMan::IResourceManagerSubsystem *pInSubsys)
			{
				_RegisterDefaultRoles(pInSubsys);
			}

			template<class RoleSetTypeArg, class RealResourceTypeArg>
			void TypedD3D11ResTypeDescrBase<RoleSetTypeArg, RealResourceTypeArg>::_RegisterDefaultRoles(ResMan::IResourceManagerSubsystem *pInSubsys)
			{
				_roles.pStd = ResMan::RegisterRealResourceSubresourceRole(pInSubsys, _params.GetSubresourceRolesPtr(), "std");
			}
		}
	}
} // Dv