#pragma once

#include "../H/ResManQuick_ManagedResource.h"
#include "ResManQuick_impl_ResourceContainer_Types.h"

#include "CoreUtil/CoreUtil_IdGenerator.h"
#include <string>
#include <map>

/********************************************************************************************************
* TODO:
* 1. To implement the resource container, the ManagedResource to be implemented.
* 1.1. Getting iterator (DONE: 31.12.2016/18:29)
*********************************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			class ResourceContainer
			{
			public:
				/********************************************************************************************************************
				* Typedefs
				********************************************************************************************************************/
				using UrlToResourceMap = std::map<std::string, ManagedResource *>;

				size_t                                                     GenerateID                     () { return _idGenerator.GenerateID(); }

				// Reserve the iterator:
				// WARNING! After reserving the iterator, no other object call can be made except the binding to resource!
				ResourceContainer_Iterator                                 ReserveIterator                ();

				// Bind created to the reserved resource storage location (pointed to by the iterator);
				// When binding, the addressable (i.e. with URL) resources are automatically registered in the by URL map.
				void                                                       BindIteratorToResource         (ResourceContainer_Iterator InReservedIter, boost::optional<std::string> InURL, std::shared_ptr<ManagedResource> pInResource);

				// Get count of all registered resources
				size_t                                                     GetCount                       () const  { return _resources.size(); }
				// Get count of resources with associated URL
				size_t                                                     GetCountAddressable            () const { return _resourcesByURL.size(); }

				const ResourceContainer_Cont&                              GetResources                   () const { return _resources; }
				const UrlToResourceMap&                                    GetUrlToResourceMap            () const { return _resourcesByURL; }

				// Try to find by URL, return nullptr if failed
				ManagedResource                                           *FindByUrl                      (const std::string& InURL) const;

				void                                                       UnregisterResource             (ResourceContainer_Iterator InIter);

			private:
				ResourceContainer_Cont                                     _resources;
				UrlToResourceMap                                           _resourcesByURL;
				bool                                                       _bIteratorReserved = false;
				Util::IdGenerator                                          _idGenerator;
			};
		}
	}
}