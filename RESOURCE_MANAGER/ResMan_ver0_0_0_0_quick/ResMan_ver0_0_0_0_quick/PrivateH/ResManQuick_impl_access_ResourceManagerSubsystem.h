#pragma once

#include "../H/ResManQuick_ResourceManagerSubsystem.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			/***********************************************************************************************
			* Cast the given subsystem to the implementation-specific type
			***********************************************************************************************/
			const ResourceManagerSubsystem          *CastTyped(const ResMan::IResourceManagerSubsystem *pInSubsystem);
			ResourceManagerSubsystem                *CastTyped(ResMan::IResourceManagerSubsystem *pInSubsystem);

			const Impl::Impl_ResourceManager        *GetImpl_ResourceManager(const ResMan::IResourceManagerSubsystem *pInSubsystem);
			Impl::Impl_ResourceManager              *GetImpl_ResourceManager(ResMan::IResourceManagerSubsystem *pInSubsystem);

			ResourceRepository                      *GetResourceRepo(Impl::Impl_ResourceManagerSubsystem *pInSubsystem);
			ResourceRepository                      *GetResourceRepo(ResMan::IResourceManagerSubsystem *pInSubsystem);


			/***********************************************************************************************
			* implementation
			***********************************************************************************************/
			inline const ResourceManagerSubsystem *CastTyped(const ResMan::IResourceManagerSubsystem *pInSubsystem)
			{
				BOOST_ASSERT_MSG(pInSubsystem, "ResManQuick lib: CastTyped to const ResourceManagerSubsystem argument is nullptr");
				const auto pCasted = dynamic_cast<const ResourceManagerSubsystem *>(pInSubsystem);
				BOOST_ASSERT_MSG(pCasted, "ResManQuick lib: const CastTyped to const ResourceManagerSubsystem failed");
				return pCasted;
			}
			inline ResourceManagerSubsystem *CastTyped(ResMan::IResourceManagerSubsystem *pInSubsystem)
			{
				BOOST_ASSERT_MSG(pInSubsystem, "ResManQuick lib: CastTyped to ResourceManagerSubsystem argument is nullptr");
				const auto pCasted = dynamic_cast<ResourceManagerSubsystem *>(pInSubsystem);
				BOOST_ASSERT_MSG(pCasted, "ResManQuick lib: const CastTyped to ResourceManagerSubsystem failed");
				return pCasted;
			}
			inline const Impl::Impl_ResourceManager *GetImpl_ResourceManager(const ResMan::IResourceManagerSubsystem *pInSubsystem)
			{
				return CastTyped(pInSubsystem)->GetImpl()->GetManager();
			}
			inline Impl::Impl_ResourceManager *GetImpl_ResourceManager(ResMan::IResourceManagerSubsystem *pInSubsystem)
			{
				return CastTyped(pInSubsystem)->GetImpl()->GetManager();
			}
			inline ResourceRepository *GetResourceRepo(ResMan::IResourceManagerSubsystem *pInSubsystem)
			{
				return CastTyped(pInSubsystem)->GetImpl()->GetManager()->GetResourceRepo();
			}
			inline ResourceRepository *GetResourceRepo(Impl::Impl_ResourceManagerSubsystem *pInSubsystem)
			{
				return pInSubsystem->GetManager()->GetResourceRepo();
			}
		}
	}
}