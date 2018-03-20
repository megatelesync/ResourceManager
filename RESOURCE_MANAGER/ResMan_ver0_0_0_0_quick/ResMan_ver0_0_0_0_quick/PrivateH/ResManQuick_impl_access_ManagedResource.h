#pragma once

#include "../H/ResManQuick_ManagedResource.h"
#include "../H/ResManQuick_ResourceType.h"

/********************************************************************************************************
* This file contains access utils for the ManagedResource class;
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
			* Cast to the implementation-specific type
			***********************************************************************************************/
			ManagedResource                *CastTyped                          (ResMan::IManagedResource *pInResource);

			/***********************************************************************************************
			* Access utils
			***********************************************************************************************/
			std::shared_ptr<ResMan::IResourceLoader>   GetLoader                          (ResMan::IManagedResource *pInResource);

			ResMan::IResourceType          *GetResourceType                    (Impl::Impl_ManagedResource *pInResource);

			ResMan::IResourceTypeDescr     *GetResourceTypeDescr               (Impl::Impl_ManagedResource *pInResource);
			ResMan::IResourceTypeDescr     *GetResourceTypeDescr               (ResMan::IManagedResource *pInResource);


			/***********************************************************************************************
			* IMPLEMENTATION
			***********************************************************************************************/
			inline std::shared_ptr<ResMan::IResourceLoader> GetLoader(ResMan::IManagedResource *pInResource)
			{
				return CastTyped(pInResource)->GetImpl()->GetLoader();
			}

			inline ManagedResource *CastTyped(ResMan::IManagedResource *pInResource)
			{
				BOOST_ASSERT_MSG(pInResource, "ResManQuick lib: CastTyped to ManagedResource argument is nullptr");
				const auto pCasted = dynamic_cast<ManagedResource *>(pInResource);
				BOOST_ASSERT_MSG(pCasted, "ResManQuick lib: CastTyped to ManagedResource failed");
				return pCasted;
			}

			inline ResMan::IResourceType *GetResourceType(Impl::Impl_ManagedResource *pInResource)
			{
				return pInResource->GetLoader()->GetMainResourceParams().GetResourceType();
			}

			inline ResMan::IResourceTypeDescr *GetResourceTypeDescr(Impl::Impl_ManagedResource *pInResource)
			{
				return GetResourceType(pInResource)->GetDescr();
			}

			inline ResMan::IResourceTypeDescr *GetResourceTypeDescr(ResMan::IManagedResource *pInResource)
			{
				return GetResourceType(CastTyped(pInResource)->GetImpl())->GetDescr();
			}
		}
	}
}