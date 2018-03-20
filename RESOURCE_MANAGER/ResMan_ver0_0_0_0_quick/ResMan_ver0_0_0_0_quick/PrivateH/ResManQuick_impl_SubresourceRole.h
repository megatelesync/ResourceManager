#pragma once

#include "../H/ResManQuick_System.h"
#include "../PrivateH/ResManQuick_impl_ResourceContainer.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			class Impl_SubresourceRole
			{
			public:
				Impl_SubresourceRole(ResMan::SubresourceRoleIndex InRoleIndex, const ResMan::SubresourceRolePublicParams& InParams);

				const ResMan::SubresourceRolePublicParams& GetParams                     () const { return _params;  }
				ResMan::SubresourceRoleIndex               GetRoleIndex                  () const { return _roleIndex;  }
				const ResourceContainer_Cont&              GetResources                  () const { return _resources.GetResources(); }
				ManagedResource                           *GetResourceByUrl              (const std::string& InURL) const { return _resources.FindByUrl(InURL); }
				ResourceContainer_Iterator                 ReserveResourceIterator       ();
				void                                       BindIteratorToResource        (ResourceContainer_Iterator InIterResource, boost::optional<std::string> InURL, std::shared_ptr<ManagedResource> pInResource);
				void                                       UnregisterResource            (Impl_ManagedResource *pInResource);

			private:
				ResourceContainer                                    _resources;
				const ResMan::SubresourceRolePublicParams            _params;
				ResMan::SubresourceRoleIndex                         _roleIndex;
			};

			inline Impl_SubresourceRole::Impl_SubresourceRole(ResMan::SubresourceRoleIndex InRoleIndex, const ResMan::SubresourceRolePublicParams& InParams) :
				_roleIndex(InRoleIndex),
				_params(InParams)
			{
			}

			inline ResourceContainer_Iterator Impl_SubresourceRole::ReserveResourceIterator()
			{
				return _resources.ReserveIterator();
			}
			inline void Impl_SubresourceRole::BindIteratorToResource(ResourceContainer_Iterator InIterResource, boost::optional<std::string> InURL, std::shared_ptr<ManagedResource> pInResource)
			{
				_resources.BindIteratorToResource(InIterResource, InURL, pInResource);
			}

			inline void Impl_SubresourceRole::UnregisterResource(Impl_ManagedResource *pInResource)
			{
				_resources.UnregisterResource(pInResource->GetIter_RoleResource());
			}
		}
	}
}