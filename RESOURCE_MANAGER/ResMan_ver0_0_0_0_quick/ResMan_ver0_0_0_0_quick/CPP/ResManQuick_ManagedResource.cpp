#include "../H/ResManQuick_ManagedResource.h"
#include "../PrivateH/ResManQuick_impl_access_ResourceManager.h"

namespace Dv
{
	namespace ResManQuick
	{
		ManagedResource::ManagedResource
		(
			Impl::Impl_ResourceManager  *pInOwnerManager,
			const ResMan::ResourceParams &InParams,
			size_t InID,
			Impl::ResourceContainer_Iterator InIter,
			Impl::ResourceContainer_Iterator InIterRoleResource
		) :
			_impl(this, pInOwnerManager, InParams, InID, InIter, InIterRoleResource)
		{
		}

		void ManagedResource::SetLoader(std::shared_ptr<ResMan::IResourceLoader> pInLoader)
		{
			assert(pInLoader != nullptr);

			// New loader must have the same URL
			assert(pInLoader->HasNoURL() == GetLoader()->HasNoURL());
			if (false == pInLoader->HasNoURL())
			{
				assert(pInLoader->GetURL() == GetLoader()->GetURL());
			}

			_impl.SetLoader(pInLoader);
		}

		ResMan::IResourceManager *ManagedResource::GetOwnerManager() const
		{
			return GetImpl()->GetOwnerManager()->GetInterface(); 
		}

		const ResMan::LoadableMainSubresourceParams *ManagedResource::GetMainSubresourceParams() const
		{
			return GetImpl()->GetMainSubresourceParams();
		}
	    const ResMan::LoadableAddressableSubresourceParams *ManagedResource::GetAddressableSubresource() const
		{
			return GetImpl()->GetAddressableSubresource();
		}

		const ResMan::CombinableParams *ManagedResource::GetOwnCombinableParams() const
		{
			return GetImpl()->GetOwnCombinableParams();
		}
		ResMan::EResourceCombinableParametersMode ManagedResource::GetCombinableParamsMode() const
		{
			return GetImpl()->GetCombinableParamsMode();
		}

		void ManagedResource::SetCombinableParamsMode(ResMan::EResourceCombinableParametersMode InCombinableParamsMode)
		{
			GetImpl()->SetCombinableParamsMode(InCombinableParamsMode);
		}

		void ManagedResource::UpdateCombinableParams(const ResMan::CombinableParams& InParams)
		{
			GetImpl()->UpdateCombinableParams(InParams);
		}

		void ManagedResource::PrecacheSubresources(const ResMan::LoadCommand& InParams)
		{
			GetImpl()->PrecacheSubresources(InParams);
		}
		void ManagedResource::UnloadSubresources(const ResMan::UnloadCommand& InParams)
		{
			GetImpl()->Unload(InParams);
		}
		void ManagedResource::PrecacheSubresourcesSyncho(const ResMan::LoadCommand& InParams)
		{
			GetImpl()->PrecacheSubresourcesSyncho(InParams);
		}

		void ManagedResource::PrecacheDependencyList(const ResMan::PrecacheDependenciesCommand& InParams)
		{
			GetImpl()->PrecacheDependencyList(InParams);
		}
		// Dependency list is always unloaded only for the current resource
		// (no recursive clearing is supported)
		void ManagedResource::UnloadDependencyList()
		{
			GetImpl()->UnloadDependencyLists();
		}
		const ResMan::ResourceDependencyList& ManagedResource::GetDependencyList() const
		{
			return GetLoader()->GetDependencyList();
		}

		void *ManagedResource::AddRef()
		{
			return Util::NotYetImpl<void *>("AddRef");
		}
		void ManagedResource::Release()
		{
			Util::NotYetImplVoid("Release");
		}
		void *ManagedResource::GetResource() const
		{
			return GetImpl()->GetRealResource();
		}
	}
}