#pragma once

#include "TestLoader.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/****************************************************************************************
			* TestLoader that loads all what it asked for, without fail codes.
			****************************************************************************************/
			class TestLoader_LoadAll_Failable : public TestLoaderBase
			{
			public:
				TestLoader_LoadAll_Failable
				(
					const ResMan::LoadableMainSubresourceParams                    *pInMainSubresourceParams,
					const ResMan::ResourceLoaderParams                             *pInResourceLoaderParams,
					boost::optional<std::string>                                    InURL,
					ResMan::ResourceDependencyList                                 *pInDepedenciesToLoadFrom = nullptr,
					const ResMan::AddressableSubresourceList                       *pInAddressableSubresourceListToLoadFrom = nullptr
				);

				virtual void                                                        *CreateResourceAndGet                                      (void *pOldPtrOrNull) override;

				/****************************************************************************************************************************
				* IResourceLoader implementation
				*****************************************************************************************************************************/
				virtual bool                                                         LoadTemporaryOwnContent                                   (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                         UnloadTemporaryOwnContent                                 (ResMan::SubresourceRoleSet InRoleSet) override;

				virtual void                                                         LoadDirectDepdendencyListOf                               (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                         UnloadDirectDepedencyListOf                               (ResMan::SubresourceRoleSet InRoleSet) override;

				/****************************************************************************************
				* Error-handling
				****************************************************************************************/
				virtual bool                                                         HasCreationFailed                                          () const override;
				virtual ResMan::SubresourceRoleSet                                   GetSubresourceRoles_FailedLoading                          () const override                                                    { return _rolesThatFailedLoad; }
				virtual ResMan::SubresourceRoleSet                                   GetSubresourceRoles_FailedLoadingDependencies              () const override                                                    { return _rolesThatFailedLoadDependencyLists; }

				void                                                                 SetShouldCreationFail                                      (bool bInNewValue)                                                   { _bCreationFails = true; }
				void                                                                 SetRolesToFailLoad                                         (ResMan::SubresourceRoleSet InRoleToFailLoad)                        { _rolesToFailLoad = InRoleToFailLoad; }
				void                                                                 SetRolesToFailLoadDependencyLists                          (ResMan::SubresourceRoleSet InRoleToFailLoadDependencyLists)         { _rolesToFailLoadDependencyLists = InRoleToFailLoadDependencyLists; }

			private:
				bool                                                 _bCreationFailed = false;
				ResMan::SubresourceRoleSet                          _rolesThatFailedLoad;
				ResMan::SubresourceRoleSet                          _rolesThatFailedLoadDependencyLists;

				bool                                                 _bCreationFails = false;
				ResMan::SubresourceRoleSet                           _rolesToFailLoad;
				ResMan::SubresourceRoleSet                           _rolesToFailLoadDependencyLists;
			};

			// Create default test loader with ALL parameters set to default values
			std::shared_ptr<TestLoader_LoadAll_Failable> CreateDefault_TestLoader_LoadAll_FailsForSubresource
			(
				bool                                                                 bInCreationFails,
				ResMan::SubresourceRoleSet                                           InRolesToFail,
				ResMan::SubresourceRoleSet                                           InDependencyListsToFail,
				ResMan::ISubresourceRole                                            *pInSubresourceRole,
				ResMan::IResourceType                                               *pInResourceType,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList = nullptr,
				const ResMan::AddressableSubresourceList                            *pInAddressableSubresourceListToLoadFrom = nullptr
			);

			std::shared_ptr<TestLoader_LoadAll_Failable> CreateDefault_TestLoader_LoadAll_FailsForSubresource
			(
				bool                                                                 bInCreationFails,
				ResMan::SubresourceRoleSet                                           InRolesToFail,
				ResMan::SubresourceRoleSet                                           InDependencyListsToFail,
				const std::string                                                   &InURL,
				ResMan::ISubresourceRole                                            *pInSubresourceRole,
				ResMan::IResourceType                                               *pInResourceType,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList = nullptr,
				const ResMan::AddressableSubresourceList                            *pInAddressableSubresourceListToLoadFrom = nullptr
			);

			std::shared_ptr<TestLoader_LoadAll_Failable> CreateDefault_TestLoader_LoadAll_FailsForSubresource
			(
				bool                                                                 bInCreationFails,
				ResMan::SubresourceRoleSet                                           InRolesToFail,
				ResMan::SubresourceRoleSet                                           InDependencyListsToFail,
				const ResMan::LoadableMainSubresourceParams                         *pInMainResourceParams,
				const ResMan::ResourceLoaderParams                                  *pInResourceLoaderParams,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList = nullptr,
				const ResMan::AddressableSubresourceList                            *pInAddressableSubresourceListToLoadFrom = nullptr
			);
			std::shared_ptr<TestLoader_LoadAll_Failable> CreateDefault_TestLoader_LoadAll_FailsForSubresource
			(
				bool                                                                 bInCreationFails,
				ResMan::SubresourceRoleSet                                           InRolesToFail,
				ResMan::SubresourceRoleSet                                           InDependencyListsToFail,
				const std::string                                                   &InURL,
				const ResMan::LoadableMainSubresourceParams                         *pInMainResourceParams,
				const ResMan::ResourceLoaderParams                                  *pInResourceLoaderParams,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList = nullptr,
				const ResMan::AddressableSubresourceList                            *pInAddressableSubresourceListToLoadFrom = nullptr
			);
		}
	}
}