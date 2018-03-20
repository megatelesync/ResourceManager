#include "../../H/ResManLoadersD3D11_System.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/**********************************************************************************************************
			* D3D11ResTypeDescrBase implementation
			**********************************************************************************************************/
			void D3D11ResTypeDescrBase::Delete(void *pInResource)
			{
				assert(pInResource != nullptr);
				auto pRes = static_cast<IUnknown *>(pInResource);
				pRes->Release();
			}

			const ResMan::ResourceTypePublicParams& D3D11ResTypeDescrBase::GetParams() const
			{
				return _params;
			}

			/**********************************************************************************************************
			* D3D11ResourceLoaderBase implementation
			**********************************************************************************************************/
			D3D11ResourceLoaderBase::D3D11ResourceLoaderBase(ID3D11Device *pInDev, ResMan::ISubresourceRole *pInRole, ResMan::IResourceType *pInResourceType) :
				_pDev(pInDev)
			{
				assert(pInDev != nullptr);
				assert(pInRole != nullptr);

				_bHasNoURL = true;

				_paramsMainSubresource.SetRole(pInRole);
				_paramsMainSubresource.SetResourceType(pInResourceType);
			}

			D3D11ResourceLoaderBase::D3D11ResourceLoaderBase(const std::string& InURL, ID3D11Device *pInDev, ResMan::ISubresourceRole *pInRole, ResMan::IResourceType *pInResourceType) :
				D3D11ResourceLoaderBase(pInDev, pInRole, pInResourceType)
			{
				_bHasNoURL = false;
				_url = InURL;
			}

			const ResMan::ResourceLoaderParams& D3D11ResourceLoaderBase::GetParams() const
			{
				return _loaderParams;
			}

			const ResMan::LoadableMainSubresourceParams& D3D11ResourceLoaderBase::GetMainResourceParams() const
			{
				return _paramsMainSubresource;
			}

			bool D3D11ResourceLoaderBase::HasNoURL() const
			{
				return true;
			}

			const std::string& D3D11ResourceLoaderBase::GetURL() const
			{
				return _url;
			}

			ResMan::SubresourceRoleSet D3D11ResourceLoaderBase::WhatMustBeLoaded(void *pOldPtrOrNull) const
			{
				return ResMan::GetSubresourceRoles_ThatMustBeLoadedToCreateResource(_GetParams_MainSubresource().GetResourceType());
			}

			size_t D3D11ResourceLoaderBase::GetNumAddressableSubresources(ResMan::ISubresourceRole *pInSubresource) const
			{
				assert(pInSubresource != nullptr);
				return 0;
			}
			size_t D3D11ResourceLoaderBase::GetNumAddressableSubresources(ResMan::SubresourceRoleIndex InIndex) const
			{
				return 0;
			}
			const ResMan::LoadableAddressableSubresourceParams &D3D11ResourceLoaderBase::GetAddressableSubresourceParams(ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex) const
			{
				return Util::InvalidCall<ResMan::LoadableAddressableSubresourceParams&>("Should not call: D3D11ResourceLoaderBase::GetAddressableSubresourceParams: ZERO adressable subresources");
			}
			const ResMan::LoadableAddressableSubresourceParams &D3D11ResourceLoaderBase::GetAddressableSubresourceParams(ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex) const
			{
				return Util::InvalidCall<ResMan::LoadableAddressableSubresourceParams&>("Should not call: D3D11ResourceLoaderBase::GetAddressableSubresourceParams: ZERO adressable subresources");
			}
			void D3D11ResourceLoaderBase::LinkAdressableSubresourceTo(ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex, ResMan::IManagedResource *pInResource)
			{
				assert(pInSubresource != nullptr);
				return Util::InvalidCallVoid("Should not call: D3D11ResourceLoaderBase::LinkAdressableSubresourceTo: ZERO adressable subresources");
			}
			void D3D11ResourceLoaderBase::LinkAdressableSubresourceTo(ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex, ResMan::IManagedResource *pInResource)
			{
				return Util::InvalidCallVoid("Should not call: D3D11ResourceLoaderBase::LinkAdressableSubresourceTo: ZERO adressable subresources");
			}
			void *D3D11ResourceLoaderBase::LoadCreateAndGetAddressableSubresource(ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex, void *pOldPtrOrNull)
			{
				return Util::InvalidCall<void *>("Should not call: D3D11ResourceLoaderBase::LoadCreateAndGetAddressableSubresource: ZERO adressable subresources");
			}
			void *D3D11ResourceLoaderBase::LoadCreateAndGetAddressableSubresource(ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex, void *pOldPtrOrNull)
			{
				return Util::InvalidCall<void *>("Should not call: D3D11ResourceLoaderBase::LoadCreateAndGetAddressableSubresource: ZERO adressable subresources");
			}
			ResMan::SubresourceRoleSet D3D11ResourceLoaderBase::GetAddressableSubresourceRoles() const
			{
				return _adressableSubresourceRoles;
			}
			bool D3D11ResourceLoaderBase::LoadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				// no content is loaded
				return true;
			}
			void D3D11ResourceLoaderBase::UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				// no content is unloaded
			}

			ResMan::SubresourceRoleSet D3D11ResourceLoaderBase::WhatOwnTemporaryContentLoaded() const
			{
				return _loadedTemporaryContentRoles;
			}

			void D3D11ResourceLoaderBase::LoadDirectDepdendencyListOf(ResMan::SubresourceRoleSet InRoleSet)
			{
				// ignore the message
			}

			void D3D11ResourceLoaderBase::UnloadDirectDepedencyListOf(ResMan::SubresourceRoleSet InRoleSet)
			{
				// ignore the message
			}

			void D3D11ResourceLoaderBase::LinkDependencyTo(ResMan::SubresourceRoleIndex InRoleIndex, DependencyIndex InDependency, ResMan::IManagedResource *pInResource)
			{
				// ignore the message
			}
			const ResMan::ResourceDependencyList& D3D11ResourceLoaderBase::GetDependencyList() const
			{
				return _dependencies;
			}

			void D3D11ResourceLoaderBase::BeginLoadDependencyLists(ResMan::SubresourceRoleSet InRoleSet)
			{
				ResMan::BeginLoadExtraDependencyLists(&_dependencies, InRoleSet);
			}
			void D3D11ResourceLoaderBase::EndLoadingDependencyLists()
			{
				ResMan::EndLoadingDependencyLists(&_dependencies);
			}

			bool D3D11ResourceLoaderBase::_IsLoaded(ResMan::ISubresourceRole *pInRole) const
			{
				return _loadedTemporaryContentRoles.IsSet(pInRole);
			}
			void D3D11ResourceLoaderBase::_MarkTempContentRole_Loaded(ResMan::ISubresourceRole *pInRole)
			{
				assert(pInRole != nullptr);
				_loadedTemporaryContentRoles.Include(pInRole);
			}
			void D3D11ResourceLoaderBase::_MarkTempContentRole_Unloaded(ResMan::ISubresourceRole *pInRole)
			{
				assert(pInRole != nullptr);
				_loadedTemporaryContentRoles.Exclude(pInRole);
			}
			void D3D11ResourceLoaderBase::_MakeTempContentRole_LoadFailed(ResMan::ISubresourceRole  *pInRole)
			{
				_failedLoading.Include(pInRole);
			}

			bool D3D11ResourceLoaderBase::_ShouldLoadTempContent(ResMan::SubresourceRoleSet InRolesToLoad, ResMan::ISubresourceRole *pInRole)
			{
				assert(&InRolesToLoad != nullptr);
				assert(pInRole != nullptr);
				bool bResult = InRolesToLoad.IsSet(pInRole) && !_GetLoadedTempContentRoles().IsSet(pInRole);
				return bResult;
			}
			bool D3D11ResourceLoaderBase::_ShouldTempContentBeUnloaded(ResMan::SubresourceRoleSet InRolesToUnload, ResMan::ISubresourceRole *pInRole)
			{
				assert(&InRolesToUnload != nullptr);
				assert(pInRole != nullptr);
				bool bResult = InRolesToUnload.IsSet(pInRole) && _GetLoadedTempContentRoles().IsSet(pInRole);
				return bResult;
			}
		}
	}
} // Dv