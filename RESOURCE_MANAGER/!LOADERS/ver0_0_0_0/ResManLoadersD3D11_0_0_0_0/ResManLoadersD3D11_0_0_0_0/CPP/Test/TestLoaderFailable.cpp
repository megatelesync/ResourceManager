#include "../../H/Test/TestLoaderFailable.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			TestLoader_LoadAll_Failable::TestLoader_LoadAll_Failable
			(
				const ResMan::LoadableMainSubresourceParams                    *pInMainSubresourceParams,
				const ResMan::ResourceLoaderParams                             *pInResourceLoaderParams,
				boost::optional<std::string>                                    InURL,
				ResMan::ResourceDependencyList                                 *pInDepedenciesToLoadFrom,
				const ResMan::AddressableSubresourceList                       *pInAddressableSubresourceListToLoadFrom
			) : TestLoaderBase(pInMainSubresourceParams, pInResourceLoaderParams, InURL, pInDepedenciesToLoadFrom, pInAddressableSubresourceListToLoadFrom)
			{
			}

			bool TestLoader_LoadAll_Failable::HasCreationFailed() const
			{
				return _bCreationFailed;
			}

			void *TestLoader_LoadAll_Failable::CreateResourceAndGet(void *pOldPtrOrNull)
			{
				_bCreationFailed = _bCreationFails;
				if(false == _bCreationFailed)
				{
					return new TestRealType(_realType);
				}
				else
				{
					return nullptr;
				}
			}

			bool TestLoader_LoadAll_Failable::LoadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				_rolesThatFailedLoad = ResMan::SubresourceRoleSet{};
				auto rolesToMarkFailed = InRoleSet;
				rolesToMarkFailed.IntersectWith(_rolesToFailLoad);
				if (false == rolesToMarkFailed.IsEmpty())
				{
					_rolesThatFailedLoad.MergeWith(rolesToMarkFailed);
					return false;
				}
				else
				{
					return true;
				}
			}

			void TestLoader_LoadAll_Failable::UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				_rolesThatFailedLoad.Subtract(InRoleSet);
			}

			void TestLoader_LoadAll_Failable::LoadDirectDepdendencyListOf(ResMan::SubresourceRoleSet InRoleSet)
			{
				_rolesThatFailedLoadDependencyLists = ResMan::SubresourceRoleSet{};
				auto rolesToMarkFailedDependencyLists = InRoleSet;
				rolesToMarkFailedDependencyLists.IntersectWith(_rolesToFailLoadDependencyLists);
				if (false == rolesToMarkFailedDependencyLists.IsEmpty())
				{
					_rolesThatFailedLoadDependencyLists.MergeWith(rolesToMarkFailedDependencyLists);
				}
			}
			void TestLoader_LoadAll_Failable::UnloadDirectDepedencyListOf(ResMan::SubresourceRoleSet InRoleSet)
			{
				_rolesThatFailedLoadDependencyLists.Subtract(InRoleSet);
			}

			/***********************************************************************************************************************************
			* Loaders creation functions
			***********************************************************************************************************************************/
			std::shared_ptr<TestLoader_LoadAll_Failable> CreateDefault_TestLoader_LoadAll_FailsForSubresource
			(
				bool                                                                 bInCreationFails,
				ResMan::SubresourceRoleSet                                           InRolesToFail,
				ResMan::SubresourceRoleSet                                           InDependencyListsToFail,
				ResMan::ISubresourceRole                                            *pInSubresourceRole,
				ResMan::IResourceType                                               *pInResourceType,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList,
				const ResMan::AddressableSubresourceList                            *pInAddressableSubresourceListToLoadFrom
			)
			{
				auto mainSubresourceParams = GetDefault_LoadableMainSubresourceParams(pInSubresourceRole, pInResourceType);
				auto resourceLoaderParams = GetDefault_ResourceLoaderParams();
				auto pResultLoader = CreateDefault_TestLoader_LoadAll_FailsForSubresource(bInCreationFails, InRolesToFail, InDependencyListsToFail, &mainSubresourceParams, &resourceLoaderParams, pInSourceDependencyList, pInAddressableSubresourceListToLoadFrom);
				return pResultLoader;
			}
			std::shared_ptr<TestLoader_LoadAll_Failable> CreateDefault_TestLoader_LoadAll_FailsForSubresource
			(
				bool                                                                 bInCreationFails,
				ResMan::SubresourceRoleSet                                           InRolesToFail,
				ResMan::SubresourceRoleSet                                           InDependencyListsToFail,
				const std::string                                                   &InURL,
				ResMan::ISubresourceRole                                            *pInSubresourceRole,
				ResMan::IResourceType                                               *pInResourceType,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList,
				const ResMan::AddressableSubresourceList                            *pInAddressableSubresourceListToLoadFrom
			)
			{
				auto mainSubresourceParams = GetDefault_LoadableMainSubresourceParams(pInSubresourceRole, pInResourceType);
				auto resourceLoaderParams = GetDefault_ResourceLoaderParams();
				auto pResultLoader = CreateDefault_TestLoader_LoadAll_FailsForSubresource(bInCreationFails, InRolesToFail, InDependencyListsToFail, InURL, &mainSubresourceParams, &resourceLoaderParams, pInSourceDependencyList, pInAddressableSubresourceListToLoadFrom);
				return pResultLoader;
			}
			std::shared_ptr<TestLoader_LoadAll_Failable> CreateDefault_TestLoader_LoadAll_FailsForSubresource
			(
				bool                                                                 bInCreationFails,
				ResMan::SubresourceRoleSet                                           InRolesToFail,
				ResMan::SubresourceRoleSet                                           InDependencyListsToFail,
				const ResMan::LoadableMainSubresourceParams                         *pInMainResourceParams,
				const ResMan::ResourceLoaderParams                                  *pInResourceLoaderParams,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList,
				const ResMan::AddressableSubresourceList                            *pInAddressableSubresourceListToLoadFrom
			)
			{
				auto pLoad = std::make_shared<TestLoader_LoadAll_Failable>(pInMainResourceParams, pInResourceLoaderParams, boost::optional<std::string>(), pInSourceDependencyList, pInAddressableSubresourceListToLoadFrom);
				pLoad->SetShouldCreationFail(bInCreationFails);
				pLoad->SetRolesToFailLoad(InRolesToFail);
				pLoad->SetRolesToFailLoadDependencyLists(InDependencyListsToFail);
				return pLoad;
			}
			std::shared_ptr<TestLoader_LoadAll_Failable> CreateDefault_TestLoader_LoadAll_FailsForSubresource
			(
				bool                                                                 bInCreationFails,
				ResMan::SubresourceRoleSet                                           InRolesToFail,
				ResMan::SubresourceRoleSet                                           InDependencyListsToFail,
				const std::string                                                   &InURL,
				const ResMan::LoadableMainSubresourceParams                         *pInMainResourceParams,
				const ResMan::ResourceLoaderParams                                  *pInResourceLoaderParams,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList,
				const ResMan::AddressableSubresourceList                            *pInAddressableSubresourceListToLoadFrom
			)
			{
				auto pLoad = std::make_shared<TestLoader_LoadAll_Failable>(pInMainResourceParams, pInResourceLoaderParams, boost::optional<std::string>(InURL), pInSourceDependencyList, pInAddressableSubresourceListToLoadFrom);
				pLoad->SetShouldCreationFail(bInCreationFails);
				pLoad->SetRolesToFailLoad(InRolesToFail);
				pLoad->SetRolesToFailLoadDependencyLists(InDependencyListsToFail);
				return pLoad;
			}
		}
	}
}