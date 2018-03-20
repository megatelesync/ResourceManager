#pragma once

#include "../Utils/ISubresourceRole.h"
#include "../Utils/ResourceDependencyList.h"
#include "../Interface/IManagedResource.h"
#include "Log/Util_ILoggingDevice.h"

#include <string>

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class IResourceType;
			class ISubresourceRole;
			class IResourceManager;

			/********************************************************************************************
			* Parameters of the loadable adressable subresource
			********************************************************************************************/
			class LoadableAddressableSubresourceParams
			{
			public:
				/***************************************************************************************
				* GETTERS
				****************************************************************************************/
				const PublicResourceParams         *GetPublicResourceParams       () const                                      { return &_resourceParams; }

				ISubresourceRole                   *GetRole                       () const                                      { return _resourceParams.GetRole(); }
				IResourceType                      *GetResourceType               () const;
				const std::string&                  GetURL                        () const;

				/***************************************************************************************
				* SETTERS
				****************************************************************************************/
				PublicResourceParams               *GetPublicResourceParams       ()                                            { return &_resourceParams; }

				void                                SetRole                       (ISubresourceRole *pInValue)                  { _resourceParams.SetRole(pInValue); }
				void                                SetResourceType               (IResourceType *pInResourceType);
				void                                SetURL                        (const std::string& InURL);

			private:
				PublicResourceParams                _resourceParams;
				IResourceType                      *_pResourceType;
				std::string                         _url;
			};
			bool Check_URL_Correct(const std::string& InString);
			void AssertAddressableSubresourceParamsCorrect(const LoadableAddressableSubresourceParams *pInParams);

			/********************************************************************************************
			* Parameters of the resource loader itself
			********************************************************************************************/
			class ResourceLoaderParams
			{
			public:
				ResourceLoaderParams() :
					_bCanBeThreaded(true),
					_bProcedural(false)
				{
				}

				// true if this loader can be executed on thread other than main
				bool                                CanBeThreaded                          () const                   { return _bCanBeThreaded; }
				void                                TurnCanBeThreadedOff                   ()                         { _bCanBeThreaded = false; }
				void                                SetEnableState_SearchURLsInDataDir     (bool bInValue)            { _bSearchURLsInDataDir = bInValue; }

				// true if this loader generates resource procedurally
				bool                                IsProcedural                           () const                   { return _bProcedural; }
				void                                SetIsProceduralOn                      ()                         { _bProcedural = true; }
				bool                                ShouldSearchURLsInDataDir              () const                   { return _bSearchURLsInDataDir; }


			private:
				bool                                _bCanBeThreaded;
				bool                                _bProcedural;
				bool                                _bSearchURLsInDataDir = true;
			};

			/********************************************************************************************
			* Resource loader
			*
			* Determines the rules of how the real resource to be created.
			* It may load the resource from some persistent storage or generate it.
			* Contains all parameters to load the real resource.
			* The loader never deletes the created real resource;
			* The loader mantains its state between loads.
			*
			* The loader loads and unloads the data if and only if it received the corresponding command,
			* no decision making is performed to determine whether to load dependencies.
			* However, if some dependent subresource role  must be lodaded to be able to load subresource of the given role,
			* and the needed resources are not loaded, the loader may load the dependent subresource.
			* For example, if source code must be loaded to create the blob, and the blob code is not loaded,
			* then the source code is to be loaded when loading the blob.
			*
			* THREADING
			* - Typically UnloadTemporaryOwnContent() should be called on the main thread
			*   because resource deleting is typically performed on the main thread.
			*
			* - Typically CreateResourceAndGet() should be called on the main thread - this allows
			* to implement the resource unloading fully on the main thread by calling the
			* UnloadTemporaryOwnContent() and then calling CreateResourceAndGet().
			*
			* TODO questions:
			* 1. May be we need function to store state of the loading - which subresources are loading now?
			* 1.1. If we have this function then it will be NOT thread safe to access it!
			********************************************************************************************/
			class IResourceLoader
			{
			public:
				/***************************************************************************************
				* TYPEDEFS
				***************************************************************************************/
				using DependencyIndex = ResourceDependencyList::DependencyIndex;

				/***************************************************************************************
				* CONSTRUCTION
				***************************************************************************************/
				IResourceLoader() = default;

				/***************************************************************************************
				* SPECIAL FUNCTIONS
				***************************************************************************************/
				virtual                                      ~IResourceLoader() = default;

				// Copying is forbidden since it's polymorphic type
				IResourceLoader&                              operator=               (const IResourceLoader&) = delete;
				IResourceLoader(const IResourceLoader&) = delete;

				// The resource loader is not designed to be moved:
				// it should always be passed by pointer
				IResourceLoader&                              operator=               (IResourceLoader&&) = delete;
				IResourceLoader(IResourceLoader&&) = delete;

				/***************************************************************************************
				* Parameters of the loader itself
				***************************************************************************************/
				virtual const ResourceLoaderParams&           GetParams() const = 0;

				/***************************************************************************************
				* Parameters of the main resource to be created
				***************************************************************************************/
				virtual const LoadableMainSubresourceParams&  GetMainResourceParams() const = 0;

				/***************************************************************************************
				* Source identification
				*
				* Resource can be unically identifiable by the URL or may be not identifiable.
				***************************************************************************************/
				// true if the resource to be returned has NO URL
				virtual bool                                  HasNoURL() const = 0;
				// unical URL of the resource to be returned
				virtual const std::string&                    GetURL() const = 0;

				/****************************************************************************************
				* Create NEW INSTANCE of the resource
				* without deleting old instance and without removing the temporary load data.
				* (dependencies).
				*
				* If you need to recreate the real resource, you may pass ptr to the old instance.
				* The loader may use the old loaded data from the old instance (for example, created vertex buffers).
				* to avoid reloading them.
				*
				* Otherwise always pass nullptr.
				****************************************************************************************/
				virtual void                                 *CreateResourceAndGet(void *pOldPtrOrNull) = 0;
				// Returns set of subresources that must be loaded to make it possible to create the resource
				// (for example, mesh can be created without loading animations, 
				// but cannot be created if the geometry data was not loaded)
				virtual SubresourceRoleSet                    WhatMustBeLoaded(void *pOldPtrOrNull) const = 0;

				/****************************************************************************************
				* Error-handling
				****************************************************************************************/
				virtual bool                                               HasCreationFailed                                  () const =0;
				virtual SubresourceRoleSet                                 GetSubresourceRoles_FailedLoading                  () const =0;
				virtual SubresourceRoleSet                                 GetSubresourceRoles_FailedLoadingDependencies      () const =0;

				/****************************************************************************************
				* Working with adressable subresources
				*
				* Addressable subresources are subresources that have URL.
				* So, we have no need to load them if they're already loaded.
				* The difference of addressable subresource from the depedency
				* is that the addressable subresource is to be loaded by this loader itself,
				* not by another loader instance.
				*
				* Addressable subresource list for given type is loaded,
				* if the corresponding LoadeTemporaryOwnContent is called.
				****************************************************************************************/
				// Returns number of elements in the list of addressable subresources of the given subresource role
				virtual size_t                                            GetNumAddressableSubresources                                 (ISubresourceRole *pInSubresource) const = 0;
				virtual size_t                                            GetNumAddressableSubresources                                 (SubresourceRoleIndex InSubresourceRoleIndex) const = 0;
				virtual const LoadableAddressableSubresourceParams        &GetAddressableSubresourceParams                              (ISubresourceRole *pInSubresource, AddressableSubresourceIndex InIndex) const = 0;
				virtual const LoadableAddressableSubresourceParams        &GetAddressableSubresourceParams                              (SubresourceRoleIndex InSubresourceRoleIndex, AddressableSubresourceIndex InIndex) const = 0;

				// Link the addressable subresource to the managed subresource
				virtual void                                               LinkAdressableSubresourceTo                                  (ISubresourceRole *pInSubresource, AddressableSubresourceIndex InIndex, IManagedResource *pInResource) = 0;
				virtual void                                               LinkAdressableSubresourceTo                                  (SubresourceRoleIndex InSubresourceRoleIndex, AddressableSubresourceIndex InIndex, IManagedResource *pInResource) = 0;
				// Load, create and get addressable subresource of the given type
				// (Typically we do it when the resource with the given URL is not registered in the manager);
				// The function takes pointer to the old MAIN RESOURCE instance (pOldPtrOrNull),
				// that is to be nullptr if have no such resource loaded.
				virtual void                                              *LoadCreateAndGetAddressableSubresource                       (ISubresourceRole *pInSubresource, AddressableSubresourceIndex InIndex, void *pOldPtrOrNull) = 0;
				virtual void                                              *LoadCreateAndGetAddressableSubresource                       (SubresourceRoleIndex InSubresourceRoleIndex, AddressableSubresourceIndex InIndex, void *pOldPtrOrNull) = 0;
				// Get set of subresource roles that have address in the given resource
				// (Some resources of the given role may be addressable while
				// others are not; if there's at least one addressable resource
				// of the given role then the corresponding role is set in the adressable subresource roles)
				virtual SubresourceRoleSet                                 GetAddressableSubresourceRoles                               () const = 0;

				/****************************************************************************************
				* Loading and unloading temporary data
				*
				* The temporary content is the content from which the resource to be built.
				****************************************************************************************/
				// Load own temporary content subresources only of the given set of roles
				// If some subresource of the given role is addressable, this function will NOT load it,
				// but will enroll the URL into the list of addressable subresource URLs of the given role.
				//
				// LoadTemporaryOwnContent returns true if the given subresources are loaded (fully);
				// If any (ever one) of prompted subresources it not loaded, returns LoadTemporaryOwnContent false.
				// The unload command may not fail (always unloads), so return type is void.
				virtual bool                              LoadTemporaryOwnContent                                                       (SubresourceRoleSet InRoleSet) = 0;
				virtual void                              UnloadTemporaryOwnContent                                                     (SubresourceRoleSet InRoleSet) = 0;
				// Returns roles of the own temporary content that already loaded
				virtual SubresourceRoleSet                WhatOwnTemporaryContentLoaded                                                 () const = 0;

				/****************************************************************************************
				* Working with dependencies
				****************************************************************************************/
				//  This function to be called before loading dependencies of the given role (typically from the main thread)
				virtual void                              BeginLoadDependencyLists                                                      (SubresourceRoleSet InRoleSet) =0;
				virtual void                              EndLoadingDependencyLists                                                     ()                             =0;

				// Access list of the loaded dependencies
				virtual const ResourceDependencyList&    GetDependencyList                                                              () const = 0;
				// Load list of direct dependencies for the given set of subresource roles
				// (for example, for mesh, you may note to load dependencies of materials only)
				virtual void                             LoadDirectDepdendencyListOf                                                    (SubresourceRoleSet InRoleSet) = 0;
				// Unload list of direct dependencies for the given set of subresource roles
				virtual void                             UnloadDirectDepedencyListOf                                                    (SubresourceRoleSet InRoleSet) = 0;
				// Link direct dependency to the managed resource
				virtual void                             LinkDependencyTo                                                               (SubresourceRoleIndex InRoleIndex, DependencyIndex InDependency, IManagedResource *pInResource) = 0;
			};
			void                                        Log_ResourceLoader                                 (Util::ILoggingDevice *pInLog, const IResourceLoader *pInLoader, IResourceManager *pInResourceManager);
			
			bool                                        HasURL                                             (IResourceLoader *pInLoader, std::string *pOutURL, SubresourceRoleIndex InRoleIndex, AddressableSubresourceIndex InIndex);
			bool                                        HasMainSubresourceURL                              (IResourceLoader *pInLoader, std::string *pOutURL);
			bool                                        CanBeThreaded                                      (IResourceLoader *pLoader);

			/*******************************************************************************************************
			* Addressable subresource enumerator
			*
			* This enumerator enumerates all dependencies within the given resource loader
			* that belong to the given set of subresource roles
			*
			* Right after construction the enumerator state points to first adressable subresource
			* (if possible, otherwise IsEnd() returns true).
			*******************************************************************************************************/
			class AdressableSubresourceEnumerator
			{
			public:
				/******************************************************************************************************
				* Construction
				******************************************************************************************************/
				AdressableSubresourceEnumerator                                                                          (IResourceLoader *pInLoader, SubresourceRoleEnumerator InCurrentProcessingRole);

				/******************************************************************************************************
				* Special operations
				******************************************************************************************************/
				AdressableSubresourceEnumerator                                                                           (const AdressableSubresourceEnumerator&) = default;
				AdressableSubresourceEnumerator&                 operator=                                                (const AdressableSubresourceEnumerator&) = default;

				/******************************************************************************************************
				* Getters
				******************************************************************************************************/
				// Get number of dependencies of the given role
				size_t                                           GetNumAddressableSubresources_OfSubresourceRole           () const { return _pLoader->GetNumAddressableSubresources(GetSubresourceRoleIndex()); }
				SubresourceRoleIndex                             GetSubresourceRoleIndex                                   () const { return enumerator_currentProcessingRole.GetRoleIndex(); }
				AddressableSubresourceIndex                      GetAddressableSubresourceIndex                            () const { return _addressableSubresourceIndex; }
				// Getting dependency: will return nullptr, if addressable subresource is out of range
				const LoadableAddressableSubresourceParams      *GetCurrentSubresourceParams                               () const { return &(_pLoader->GetAddressableSubresourceParams(GetSubresourceRoleIndex(), GetAddressableSubresourceIndex())); }

				/******************************************************************************************************
				* Is no more dependencies
				******************************************************************************************************/
				bool                                  IsEnd() const
				{
					return enumerator_currentProcessingRole.IsEnd();
				}

				/******************************************************************************************************
				* Moving
				******************************************************************************************************/
				// true if some move operaion was called
				bool                                  HasBeenMoved() const { return _bMoved; }
				// Move to next dependency (always moves to correct dependency, if possible)
				bool                                  MoveNext();

				// Move to next subresource role (whic may contain depedencies, or may not)
				bool                                  MoveNextSubresourceRole();
				// Move to next subresource role for which (!) at least one dependency is registered
				bool                                  MoveNextSubresourceRole_WithDependencies();

			private:
				void                                               _Initialize_CurrentSubresourceRole();
				bool                                               _IsAdressableSubresourceIndex_OutOfRange() const;

				IResourceLoader                                    *_pLoader;
				AddressableSubresourceIndex                         _addressableSubresourceIndex;
				SubresourceRoleEnumerator                           enumerator_currentProcessingRole;
				bool                                                _bMoved;
			};


			/************************************************************************************************************
			* Helper accessor implementation
			************************************************************************************************************/
			inline bool CanBeThreaded(IResourceLoader *pLoader)
			{
				return pLoader->GetParams().CanBeThreaded();
			}
			
			/************************************************************************************************************
			* LoadableAddressableSubresourceParams implementation
			************************************************************************************************************/
			inline IResourceType *LoadableAddressableSubresourceParams::GetResourceType() const
			{
				return _pResourceType;
			}

			inline const std::string& LoadableAddressableSubresourceParams::GetURL() const
			{
				return _url;
			}
			
			inline void LoadableAddressableSubresourceParams::SetResourceType(IResourceType *pInResourceType)
			{
				_pResourceType = pInResourceType;
			}

			inline void LoadableAddressableSubresourceParams::SetURL(const std::string& InURL)
			{
				assert(&InURL != nullptr);
				_url = InURL;
			}
		}
	}
}
