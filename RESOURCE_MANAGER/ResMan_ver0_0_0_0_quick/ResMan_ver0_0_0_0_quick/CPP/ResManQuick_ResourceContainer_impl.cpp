#pragma once

#include "../PrivateH/ResManQuick_impl_ResourceContainer.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			ResourceContainer_Iterator ResourceContainer::ReserveIterator()
			{
				assert(!_bIteratorReserved);
				auto it_new = _resources.insert(_resources.end(), nullptr);
				_bIteratorReserved = true;
				return it_new;
			}

			void ResourceContainer::BindIteratorToResource(ResourceContainer_Iterator InReservedIter, boost::optional<std::string> InURL, std::shared_ptr<ManagedResource> pInResource)
			{
				assert(_bIteratorReserved);
				assert(pInResource != nullptr);
				*InReservedIter = pInResource;
				auto pLoader = pInResource->GetLoader();
				if (pLoader != nullptr)
				{
					if (InURL.is_initialized())
					{
						_resourcesByURL.insert(std::pair<std::string, ManagedResource *>(InURL.get(), pInResource.get()));
					}
				}
				_bIteratorReserved = false;
			}

			ManagedResource *ResourceContainer::FindByUrl(const std::string& InURL) const
			{
				assert(&InURL != nullptr);
				assert(!_bIteratorReserved);
				auto it_resource = _resourcesByURL.find(InURL);
				if (it_resource == _resourcesByURL.end())
				{
					return nullptr;
				}
				else
				{
					return it_resource->second;
				}
			}

			void ResourceContainer::UnregisterResource(ResourceContainer_Iterator InIter)
			{
				assert(!_bIteratorReserved);
				auto pResource = InIter->get();
				if(false == pResource->HasNoURL())
				{
					_resourcesByURL.erase(pResource->GetURL());
				}
				_resources.erase(InIter);
			}
		}
	}
}