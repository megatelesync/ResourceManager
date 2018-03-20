#include "../PrivateH/ResManQuick_impl_access_ManagedGroup.h"
#include "../PrivateH/ResManQuick_impl_access_ManagedResource.h"
#include "../PrivateH/ResManQuick_impl_access_ResourceManager.h"

namespace Dv
{
	namespace ResManQuick
	{
		ManagedGroup::ManagedGroup
		(
			Impl::Impl_ResourceManager                         *pInOwnerManager,
			const ResMan::GroupParams                          *pInParams,
			Impl::GroupContElement                             *pInSlot
		) :
			_impl(this, pInOwnerManager, pInParams, pInSlot)
		{
		}

		ResMan::IResourceManager *ManagedGroup::GetOwnerManager() const
		{
			return GetImpl()->GetOwnerManager()->GetInterface();
		}

		const ResMan::GroupPublicParams& ManagedGroup::GetParams() const
		{
			return *(GetImpl()->GetParams()->GetPublic());
		}

		const ResMan::CombinableParams *ManagedGroup::GetCombinableParams() const
		{
			return _impl.GetCombinableParams();
		}

		void ManagedGroup::UpdateCombinableParams(const ResMan::CombinableParams& InNewValue)
		{
			_impl.SetCombinableParams(InNewValue);
		}

		ResMan::SubresourceRoleSet ManagedGroup::GetSubresourceRoles_PrecachedResources() const
		{
			return _impl.GetSubresourceRoles_PrecachedResources();
		}
		ResMan::SubresourceRoleSet ManagedGroup::GetSubresourceRoles_NotPrecachedResources() const
		{
			return _impl.GetSubresourceRoles_NotPrecachedResources();
		}
		ResMan::SubresourceRoleSet ManagedGroup::GetSUbresourceRoles_FailedPrecachedResources() const
		{
			return _impl.GetSUbresourceRoles_FailedPrecachedResources();
		}

		ManagedGroup::ResourceEnumerator ManagedGroup::GetPrecachedResources(const ResMan::ISubresourceRole *pInRole) const
		{
			return GetImpl()->GetPrecachedResources(pInRole);
		}

		ManagedGroup::ResourceEnumerator ManagedGroup::GetFailedPrecachedResources(const ResMan::ISubresourceRole *pInRole) const
		{
			return GetImpl()->GetFailedPrecachedResources(pInRole);
		}

		ManagedGroup::ResourceEnumerator ManagedGroup::GetNotPrecachedResources(const ResMan::ISubresourceRole *pInRole) const
		{
			return GetImpl()->GetNotPrecachedResources(pInRole);
		}

		void ManagedGroup::AttachResource(ResMan::IManagedResource *pInResource)
		{
			_impl.Attach(Impl::CastTyped(pInResource));
		}
		void ManagedGroup::DetachResourceFromGroup(ResMan::IManagedResource *pInResource)
		{
			_impl.DetachResourceFromThis(Impl::CastTyped(pInResource));
		}

		ResMan::ResourcePriority ManagedGroup::GetPrecachePriority() const
		{
			return Util::NotYetImpl<ResMan::ResourcePriority>("ManagedGroup::GetPrecachePriority");
		}

		void ManagedGroup::SetHighestPrecachePriority()
		{
			Util::NotYetImplVoid_Silent("ManagedGroup::SetHighestPrecachePriority()");
		}

		void ManagedGroup::Precache(ResMan::Urgency InUrgency)
		{
			return GetImpl()->Precache(InUrgency);
		}
		const ResMan::GroupCommandState& ManagedGroup::PrecacheSynchro()
		{
			return GetImpl()->PrecacheSynchro();
		}
		const ResMan::GroupCommandState& ManagedGroup::GetCommandState() const
		{
			return GetImpl()->GetCommandState();
		}

		void ManagedGroup::PrecacheDependencyList(ResMan::Urgency InUrgency)
		{
			GetImpl()->PrecacheDependencyList(InUrgency);
		}
		const ResMan::GroupDependencyState& ManagedGroup::GetDependencyListState() const
		{
			return GetImpl()->GetDependencyListState();
		}
	}
}