#pragma once

#include "../H/ResManQuick_SubresourceRole.h"

/********************************************************************************************************
* This file contains access utils for the SubresourceRole class;
*********************************************************************************************************/

/********************************************************************************************************
* TODO:
*********************************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			/***********************************************************************************************
			* Cast the given role to the implementation-specific type
			***********************************************************************************************/
			SubresourceRole                 *CastTyped(ResMan::ISubresourceRole *pInRole);

			inline SubresourceRole *CastTyped(ResMan::ISubresourceRole *pInRole)
			{
				BOOST_ASSERT_MSG(pInRole, "ResManQuick lib: CastTyped to SubresourceRole argument is nullptr");
				const auto pCasted = dynamic_cast<SubresourceRole *>(pInRole);
				BOOST_ASSERT_MSG(pCasted, "ResManQuick lib: CastTyped to SubresourceRole failed");
				return pCasted;
			}
		}
	}
}