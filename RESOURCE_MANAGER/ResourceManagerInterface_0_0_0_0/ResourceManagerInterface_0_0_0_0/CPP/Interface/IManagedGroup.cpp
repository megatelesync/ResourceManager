#include "../../H/Interface/IManagedGroup.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			size_t GetCount_PrecachedResources(IManagedGroup *pInGroup, ISubresourceRole *pInRole)
			{
				auto pEnumerator = std::move(pInGroup->GetPrecachedResources(pInRole));
				auto count = pEnumerator->GetSize();
				return count;
			}
			size_t GetCount_NotPrecachedResources(IManagedGroup *pInGroup, ISubresourceRole *pInRole)
			{
				auto pEnumerator = std::move(pInGroup->GetNotPrecachedResources(pInRole));
				auto count = pEnumerator->GetSize();
				return count;
			}
			size_t GetCount_PrecachedFailedReosurces(IManagedGroup *pInGroup, ISubresourceRole *pInRole)
			{
				auto pEnumerator = std::move(pInGroup->GetFailedPrecachedResources(pInRole));
				auto count = pEnumerator->GetSize();
				return count;
			}
			size_t GetCount_PrecachedOrFailedPrecached(IManagedGroup *pInGroup, ISubresourceRole *pInRole)
			{
				auto count = GetCount_PrecachedResources(pInGroup, pInRole) + GetCount_PrecachedFailedReosurces(pInGroup, pInRole);
				return count;
			}
		}
	}
}