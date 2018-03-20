#pragma once

#include <cstdint>

/****************************************************************************************
* This header file contains basic typedefs of the resource manager
****************************************************************************************/

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			using ResourcePriority = uint32_t;

			// Load or unload urgency
			using Urgency = uint32_t;

			// Adressable subresource index
			// (cannot be negative)
			using AddressableSubresourceIndex          = uint32_t;

			// Nullable addressable subresource index
			// (it's tendered for the cases when some subresource index can be provided or not)
			using NullableAddressableSubresourceIndex  = int32_t;

			bool                                                AddressableSubresourceIndex_IsNull               (NullableAddressableSubresourceIndex InIndex);
			AddressableSubresourceIndex                         AddressableSubresourceIndex_ToNonNull            (NullableAddressableSubresourceIndex InIndex);
			NullableAddressableSubresourceIndex                 AddressableSubresourceIndex_GetNull              ();
		} // Ver_0_0_0_0
	} // ResourceManager
} // Dv
#pragma once
