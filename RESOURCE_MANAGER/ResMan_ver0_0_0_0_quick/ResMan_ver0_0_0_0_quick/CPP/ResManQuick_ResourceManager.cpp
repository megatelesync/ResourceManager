#include "../H/ResManQuick_ResourceManager.h"
#include "../PrivateH/ResManQuick_impl_access_ResourceManagerSubsystem.h"
#include "../PrivateH/ResManQuick_impl_access_ManagedResource.h"
#include "../PrivateH/ResManQuick_impl_access_ManagedGroup.h"


namespace Dv
{
	namespace ResManQuick
	{
		/***************************************************************************************************************
		* Typedefs
		***************************************************************************************************************/
		using AllResourceTypeEnumerator = ContResourceTypeEnumerator<const Impl::Impl_ResourceTypeContainer::ContType>;
		using AllSubresourceRoleEnumerator = ContSubresourceRoleEnumerator<const Impl::Impl_SubresourceRoleContainer::ContType>;

		/***************************************************************************************************************
		* ResourceManager implmentation
		***************************************************************************************************************/
		ResourceManager::~ResourceManager()
		{
		}

		ResourceManager::ResourceManager(Impl::Impl_ResourceManager *pInImpl)
		{
			assert(pInImpl != nullptr);
			_pImpl = pInImpl;
		}

		ResMan::IResourceManagerSubsystem *ResourceManager::GetSubsystem() const
		{ 
			return _pImpl->GetSubsystem()->GetSubsystem(); 
		}
		
		ResourceManager::ResourceTypeEnumerator ResourceManager::GetResourceTypes()
		{ 
			const auto& resourceTypesCont = _pImpl->GetSubsystem()->GetResourceTypes();
			return MakeEnumeratorResult_ResourceType(new AllResourceTypeEnumerator(&resourceTypesCont));
		}

		ResourceManager::RoleEnumerator ResourceManager::GetSubresourceRoles()
		{ 
			const auto& subresourceRolesCont = _pImpl->GetSubsystem()->GetSubresourceRoles();
			return MakeEnumeratorResult_SubresourceRole(new AllSubresourceRoleEnumerator(&subresourceRolesCont));
		}

		ResMan::ISubresourceRole *ResourceManager::GetSubresourceRoleByIndex(ResMan::SubresourceRoleIndex InIndex)
		{
			return _pImpl->GetSubresourceRoleByIndex(InIndex);
		}

		ResMan::IManagedGroup *ResourceManager::RegisterGroup(const ResMan::GroupParams& InParams)
		{
			return GetImpl()->RegisterGroup(InParams);
		}
		void ResourceManager::UnregisterGroup(ResMan::IManagedGroup *pInGroup)
		{
			GetImpl()->UnregisterGroup(Impl::CastTyped(pInGroup));
		}
		ResourceManager::ResourceEnumerator ResourceManager::GetResourceEnumerator()
		{
			return GetImpl()->GetResourceEnumerator();
		}
		ResourceManager::ResourceEnumerator ResourceManager::GetResourceWithURL()
		{
			return GetImpl()->GetResourceWithURL();
		}
		ResMan::IManagedResource *ResourceManager::FindResourceByURL(const std::string& InURL)
		{
			return GetImpl()->FindResourceByURL(InURL);
		}
		ResMan::IManagedResource *ResourceManager::RegisterResource(const ResMan::PublicResourceParams& InParams, std::shared_ptr<ResMan::IResourceLoader>  pInLoader)
		{
			assert(&InParams != nullptr);			
			return GetImpl()->RegisterResource(InParams, pInLoader);
		}
		void ResourceManager::UnregisterResource(ResMan::IManagedResource *pInResource)
		{
			assert(pInResource != nullptr);
			GetImpl()->UnregisterResource(Impl::CastTyped(pInResource));
		}
		ResMan::IManagedResource *ResourceManager::PopFailedResource()
		{
			return GetImpl()->PopFailedResource();
		}
		ResMan::IManagedGroup *ResourceManager::PopFailedGroup()
		{
			return GetImpl()->PopFailedGroup();
		}

		void ResourceManager::Wait_UntilAllCommandsProcessed()
		{
			GetImpl()->Wait_UntilAllCommandsProcessed();
		}
		void ResourceManager::Wait_UntilAllResourceCommandsProcessed(ResMan::IManagedResource *pInResource, bool bInRecurisve)
		{
			GetImpl()->Wait_UntilAllResourceCommandsProcessed(pInResource, bInRecurisve);
		}
		void ResourceManager::Wait_UntilAllGroupCommandsProcessed(ResMan::IManagedGroup *pInGroup, bool bInRecurisve)
		{
			GetImpl()->Wait_UntilAllGroupCommandsProcessed(pInGroup, bInRecurisve);
		}

		void ResourceManager::Wait_UntilAllThreadedCommandsProcessed()
		{
			GetImpl()->Wait_UntilAllThreadedCommandsProcessed();
		}

		void ResourceManager::Wait_UntilAllThreadedCommandsForResourceProcessed(ResMan::IManagedResource *pInResource, bool bInRecursive)
		{
			GetImpl()->Wait_UntilAllThreadedCommandsForResourceProcessed(pInResource, bInRecursive);
		}
	}
}