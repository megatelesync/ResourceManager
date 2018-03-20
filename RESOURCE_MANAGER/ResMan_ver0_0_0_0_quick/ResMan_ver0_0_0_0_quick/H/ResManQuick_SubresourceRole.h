#pragma once

#include "ResManQuick_Fwd.h"
#include "ResManQuick_System.h"
#include "../PrivateH/ResManQuick_impl_SubresourceRole.h"


namespace Dv
{
	namespace ResManQuick
	{


		class SubresourceRole : public ResMan::ISubresourceRole
		{
		public:
			// Concrete type of the resource enumerator
			using TypedResourceEnumerator                         = ContResourceEnumerator<const Impl::ResourceContainer_Cont>;

			SubresourceRole(ResMan::SubresourceRoleIndex InRoleIndex, const ResMan::SubresourceRolePublicParams& InParams);

			virtual	const ResMan::SubresourceRolePublicParams    &GetParams              () const override                 { return GetImpl()->GetParams(); }
			virtual	RoleIndex                                     GetRoleIndex           () const override                 { return GetImpl()->GetRoleIndex(); }
			
			virtual ResourceEnumerator                            GetResources           () override                       { return MakeResourceEnumeratorResult(new TypedResourceEnumerator(&(GetImpl()->GetResources()))); }
			// Search resource with the given URL within resources which main subsresource is of this role
			// (returns nullptr if resource with the given URL is not found)
			virtual ManagedResource                              *GetResourceByURL       (const ResourceURL& InURL) override { return GetImpl()->GetResourceByUrl(InURL); }

			Impl::Impl_SubresourceRole                           *GetImpl                () { return &_impl; }
			const Impl::Impl_SubresourceRole                     *GetImpl                () const { return &_impl; }

		private:
			Impl::Impl_SubresourceRole                      _impl;
		};

		inline SubresourceRole::SubresourceRole(ResMan::SubresourceRoleIndex InRoleIndex, const ResMan::SubresourceRolePublicParams& InParams) :
			_impl(InRoleIndex, InParams)
		{
		}
	}
}