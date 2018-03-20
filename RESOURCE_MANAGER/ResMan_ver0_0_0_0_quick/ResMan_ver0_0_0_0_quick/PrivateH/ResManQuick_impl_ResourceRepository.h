#pragma once

#include "../H/ResManQuick_System.h"
#include "../PrivateH/ResManQuick_impl_ResourceContainer.h"
/********************************************************************************************************
* TODO:
*********************************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			class Impl_ResourceManagerSubsystem;
			class Impl_ResourceManager;

			/*******************************************************************************************************************************
			* ResourceRepository
			*
			* Stores and registers resources, performs logging.
			********************************************************************************************************************************/
			class ResourceRepository
			{
			public:
				/***************************************************************************************************************************
				* Construction
				***************************************************************************************************************************/
				~ResourceRepository();
				ResourceRepository();

				/***************************************************************************************************************************
				* Getters
				***************************************************************************************************************************/
				Impl_ResourceManagerSubsystem                          *GetSubsystem                                     () const;

				/***************************************************************************************************************************
				* Initialization
				***************************************************************************************************************************/
				void Initialize                                                                                          (Impl_ResourceManager *pInOwnerManager);

				/***************************************************************************************************************************
				* Getting resources
				***************************************************************************************************************************/
				const ResourceContainer_Cont                           *GetResources                                     () const { return &(_resources.GetResources()); }
				const ResourceContainer::UrlToResourceMap              *GetUrlToResourceMap                              () const { return &(_resources.GetUrlToResourceMap()); }

				/***************************************************************************************************************************
				* Searching
				***************************************************************************************************************************/
				ManagedResource                                        *FindByURL                                        (const std::string& InURL);
				
				/***************************************************************************************************************************
				* Registration
				***************************************************************************************************************************/
				ManagedResource                                        *RegisterResource                                 (const ResMan::PublicResourceParams& InParams, std::shared_ptr<ResMan::IResourceLoader> pInLoader, ResMan::IResourceType *pInResourceType, ResMan::NullableAddressableSubresourceIndex InAdressableSubresourceIndex);
				// Register resource that is addressable
				ManagedResource                                        *RegisterAddressableResource                      (std::shared_ptr<ResMan::IResourceLoader> pInLoader, const ResMan::LoadableAddressableSubresourceParams *pInAddressableParams, ResMan::NullableAddressableSubresourceIndex InAdressableSubresourceIndex);
				void                                                    UnregisterResource                               (Impl_ManagedResource *pInResource);

			private:
				ManagedResource                                        *_RegisterNewResource                             (const ResMan::PublicResourceParams& InParams, std::shared_ptr<ResMan::IResourceLoader> pInLoader, boost::optional<std::string> InURL, ResMan::NullableAddressableSubresourceIndex InAdressableSubresourceIndex);

				Impl_ResourceManager                                   *_pOwnerManager;
				ResourceContainer                                       _resources;
			};
		}
	}
}