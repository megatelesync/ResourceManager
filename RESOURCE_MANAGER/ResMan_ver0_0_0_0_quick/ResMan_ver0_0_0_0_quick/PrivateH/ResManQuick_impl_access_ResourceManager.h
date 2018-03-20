#pragma once

#include "../H/ResManQuick_ResourceManager.h"

/********************************************************************************************************
* This file contains access utils for the ResourceManager class;
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
			* Cast the given manager to the implementation-specific type
			***********************************************************************************************/
			ResourceManager                *CastTyped              (ResMan::IResourceManager *pInManager);
			ResourceRepository             *GetResourceRepo        (ResMan::IResourceManager *pInManager);

			Impl_ResourceManagerSubsystem  *GetImpl_Subsystem      (Impl::Impl_ResourceManager *pInManager);
			Impl_ResourceManagerSubsystem  *GetImpl_Subsystem      (ResMan::IResourceManager *pInManager);


			/***********************************************************************************************
			* implementation
			***********************************************************************************************/
			inline ResourceManager *CastTyped(ResMan::IResourceManager *pInManager)
			{
				BOOST_ASSERT_MSG(pInManager, "ResManQuick lib: CastTyped to ResourceManager argument is nullptr");
				const auto pCasted = dynamic_cast<ResourceManager *>(pInManager);
				BOOST_ASSERT_MSG(pCasted, "ResManQuick lib: CastTyped to ResourceManager failed");
				return pCasted;
			}

			inline Impl_ResourceManagerSubsystem  *GetImpl_Subsystem(Impl::Impl_ResourceManager *pInManager)
			{
				return pInManager->GetSubsystem();
			}

			inline Impl_ResourceManagerSubsystem  *GetImpl_Subsystem(ResMan::IResourceManager *pInManager)
			{
				return CastTyped(pInManager)->GetImpl()->GetSubsystem();
			}

			inline ResourceRepository *GetResourceRepo(ResMan::IResourceManager *pInManager)
			{
				return CastTyped(pInManager)->GetImpl()->GetResourceRepo();
			}
		}
	}
}