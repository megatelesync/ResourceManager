#pragma once

#include "../H/ResManQuick_Fwd.h"
#include "../PrivateH/ResManQuick_impl_GroupResources.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			/******************************************************************************************************
			* Link from the given resource to the given group
			******************************************************************************************************/
			class ResourceGroupLink
			{
			public:
				/*************************************************************************************************
				* Create link to the given group and iterator to the resource record within
				* the container of the group resources (GroupResources)
				*************************************************************************************************/
				ResourceGroupLink
				(
					ManagedGroup                              *pInToGroup,
					GroupResourceRecordPointer                 InGroupResourceRecord
				);

				ManagedGroup                                  *GetGroup                                 () const                                     { return _pGroup; }
				GroupResourceRecordPointer                     GetIterToResourceRecord                  () const                                     { return _it_group_resource_record; }
				void                                           SetFixedPointerToResourceRecord          (GroupResourceRecordPointer InNewValue)      { _it_group_resource_record = InNewValue;  }

			private:
				ManagedGroup                                   *_pGroup;
				GroupResourceRecordPointer                      _it_group_resource_record;

			};
			using ResourceGroupLink_Cont = std::list<ResourceGroupLink>;
		}
	}
}