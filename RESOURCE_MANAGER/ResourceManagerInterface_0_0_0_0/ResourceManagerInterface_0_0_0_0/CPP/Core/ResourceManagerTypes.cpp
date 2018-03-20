#include "../../H/Core/ResourceManagerTypes.h"

#include <cassert>

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			bool AddressableSubresourceIndex_IsNull(NullableAddressableSubresourceIndex InIndex)
			{
				return InIndex < 0;
			}

			AddressableSubresourceIndex AddressableSubresourceIndex_ToNonNull(NullableAddressableSubresourceIndex InIndex)
			{
				assert(!AddressableSubresourceIndex_IsNull(InIndex));
				return static_cast<AddressableSubresourceIndex>(InIndex);
			}

			NullableAddressableSubresourceIndex AddressableSubresourceIndex_GetNull()
			{
				return -1;
			}
		}
	}
}