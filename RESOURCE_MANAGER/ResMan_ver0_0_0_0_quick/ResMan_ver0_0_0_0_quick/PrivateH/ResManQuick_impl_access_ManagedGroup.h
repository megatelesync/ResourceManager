#pragma once

#include "../H/ResManQuick_ManagedGroup.h"
#include "../PrivateH/ResManQuick_impl_ManagedGroup.h"
#include "../PrivateH/ResManQuick_impl_GroupCont.h"

/********************************************************************************************************
* This file contains access utils for the ManagedGroup class;
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
			ManagedGroup                            *CastTyped                (ResMan::IManagedGroup *pInGroup);
			GroupContElement                        *GetSlot                  (ResMan::IManagedGroup *pInGroup);
			size_t                                   GetID                    (ResMan::IManagedGroup *pInGroup);
			bool                                     IsActive                 (ResMan::IManagedGroup *pInGroup);
			
			const ResMan::GroupParams               *GetParams                (ResMan::IManagedGroup *pInGroup);
			const ResMan::GroupPublicParams         *GetPublicParams          (ResMan::IManagedGroup *pInGroup);
			
			const std::string                        GetName                  (const Impl::Impl_ManagedGroup *pInGroup);
			size_t                                   GetID                    (const Impl::Impl_ManagedGroup *pInGroup);
			bool                                     IsActive                 (const Impl::Impl_ManagedGroup *pInGroup);


			/***************************************************************************************
			* implementation
			***************************************************************************************/
			inline ManagedGroup *CastTyped(ResMan::IManagedGroup *pInGroup)
			{
				return static_cast<ManagedGroup *>(pInGroup);
			}
			inline GroupContElement *GetSlot(ResMan::IManagedGroup *pInGroup)
			{
				assert(pInGroup != nullptr);
				return CastTyped(pInGroup)->GetImpl()->GetSlot();
			}

			inline size_t GetID(ResMan::IManagedGroup *pInGroup)
			{
				assert(pInGroup != nullptr);
				return CastTyped(pInGroup)->GetImpl()->GetSlot()->GetID();
			}

			inline bool IsActive(ResMan::IManagedGroup *pInGroup)
			{
				assert(pInGroup != nullptr);
				return CastTyped(pInGroup)->GetParams().GetCombinable().IsActive();
			}

			inline const std::string GetName(const Impl::Impl_ManagedGroup *pInGroup)
			{
				return pInGroup->GetParams()->GetName();
			}
			inline size_t GetID(const Impl::Impl_ManagedGroup *pInGroup)
			{
				return pInGroup->GetSlot()->GetID();
			}
			inline bool IsActive(const Impl::Impl_ManagedGroup *pInGroup)
			{
				return pInGroup->GetParams()->GetPublic()->GetCombinable().IsActive();
			}

			inline const ResMan::GroupParams *GetParams(ResMan::IManagedGroup *pInGroup)
			{
				return CastTyped(pInGroup)->GetImpl()->GetParams();
			}

			inline const ResMan::GroupPublicParams *GetPublicParams(ResMan::IManagedGroup *pInGroup)
			{
				return GetParams(pInGroup)->GetPublic();
			}
		}
	}
}