#include "../../H/Test/TestLoader.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/****************************************************************************************************************
			* TestLoader_LoadAll_NoFail implementation
			****************************************************************************************************************/
			TestLoader_LoadAll_NoFail::TestLoader_LoadAll_NoFail
			(
				const ResMan::LoadableMainSubresourceParams                    *pInMainSubresourceParams,
				const ResMan::ResourceLoaderParams                             *pInResourceLoaderParams,
				boost::optional<std::string>                                    InURL,
				ResMan::ResourceDependencyList                                 *pInDepedenciesToLoadFrom,
				const ResMan::AddressableSubresourceList                       *pInAddressableSubresourceListToLoadFrom
			) : TestLoaderBase(pInMainSubresourceParams, pInResourceLoaderParams, InURL, pInDepedenciesToLoadFrom, pInAddressableSubresourceListToLoadFrom)
			{
			}

			bool TestLoader_LoadAll_NoFail::LoadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{				
				bool bLastResult = TestLoaderBase::LoadTemporaryOwnContent(InRoleSet);
				_loadedTemporaryOwnContent.MergeWith(InRoleSet);
				ResMan::CopyNotLoadedAddressableSubresources(&_addressableSubresources, &_addressableSubresourcesToLoadFrom, InRoleSet);

				return true;
			}
			void TestLoader_LoadAll_NoFail::UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				TestLoaderBase::UnloadTemporaryOwnContent(InRoleSet);
				_loadedTemporaryOwnContent.Subtract(InRoleSet);
			}

			void TestLoader_LoadAll_NoFail::LoadDirectDepdendencyListOf(ResMan::SubresourceRoleSet InRoleSet)
			{
				TestLoaderBase::LoadDirectDepdendencyListOf(InRoleSet);

				if(nullptr != _pDependenciesToLoadFrom)
				{
					// We should NOT call begin()/end(), because begin and end are already called
					const bool bCallBeginEnd = false;
					ResMan::CopySubresourceDependencies(InRoleSet, _pDependenciesToLoadFrom, &_dependencyList, bCallBeginEnd);
				}
			}
			void TestLoader_LoadAll_NoFail::UnloadDirectDepedencyListOf(ResMan::SubresourceRoleSet InRoleSet)
			{
				TestLoaderBase::UnloadDirectDepedencyListOf(InRoleSet);
				_dependencyList.ClearSubresourceDependencies(InRoleSet);
			}

			/****************************************************************************************************************
			* TestLoader_LoadAll_NoFail utils implementation
			****************************************************************************************************************/
			ResMan::ResourceLoaderParams GetDefault_ResourceLoaderParams()
			{
				ResMan::ResourceLoaderParams defaultParams;
				defaultParams.TurnCanBeThreadedOff();
				defaultParams.SetIsProceduralOn();
				return defaultParams;
			}
			ResMan::PublicResourceParams GetDefault_PublicResourceParams
			(
				ResMan::ISubresourceRole                                            *pInSubresourceRole
			)
			{
				ResMan::PublicResourceParams defaultParams;
				defaultParams.SetRole(pInSubresourceRole);
				return defaultParams;
			}
			ResMan::LoadableMainSubresourceParams GetDefault_LoadableMainSubresourceParams
			(
				ResMan::ISubresourceRole                                            *pInSubresourceRole,
				ResMan::IResourceType                                               *pInResourceType
			)
			{
				ResMan::LoadableMainSubresourceParams defaultParams;
				defaultParams.SetPublicResourceParams(GetDefault_PublicResourceParams(pInSubresourceRole));
				defaultParams.SetResourceType(pInResourceType);
				return defaultParams;
			}

			std::shared_ptr<TestLoader_LoadAll_NoFail> CreateDefault_TestLoader_LoadAll_NoFail(ResMan::ISubresourceRole *pInSubresourceRole, ResMan::IResourceType *pInResourceType, ResMan::ResourceDependencyList *pInSourceDependencyList)
			{
				auto mainSubresourceParams = GetDefault_LoadableMainSubresourceParams(pInSubresourceRole, pInResourceType);
				auto resourceLoaderParams = GetDefault_ResourceLoaderParams();
				auto pResultLoader = CreateDefault_TestLoader_LoadAll_NoFail(&mainSubresourceParams, &resourceLoaderParams, pInSourceDependencyList);
				return pResultLoader;
			}

			std::shared_ptr<TestLoader_LoadAll_NoFail> CreateDefault_TestLoader_LoadAll_NoFail
			(				
				const ResMan::LoadableMainSubresourceParams                         *pInMainResourceParams,
				const ResMan::ResourceLoaderParams                                  *pInResourceLoaderParams,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList
			)
			{
				auto pLoad = std::make_shared<TestLoader_LoadAll_NoFail>(pInMainResourceParams, pInResourceLoaderParams, boost::optional<std::string>(), pInSourceDependencyList);
				return pLoad;
			}
			std::shared_ptr<TestLoader_LoadAll_NoFail> CreateDefault_TestLoader_LoadAll_NoFail
			(
				const std::string                                                   &InURL,
				const ResMan::LoadableMainSubresourceParams                         *pInMainResourceParams,
				const ResMan::ResourceLoaderParams                                  *pInResourceLoaderParams,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList
			)
			{
				auto pLoad = std::make_shared<TestLoader_LoadAll_NoFail>(pInMainResourceParams, pInResourceLoaderParams, boost::optional<std::string>(InURL), pInSourceDependencyList);
				return pLoad;
			}


			/****************************************************************************************************************
			* TestLoaderBase implementation
			****************************************************************************************************************/
			TestLoaderBase::TestLoaderBase
			(
				const ResMan::LoadableMainSubresourceParams                    *pInMainSubresourceParams,
				const ResMan::ResourceLoaderParams                             *pInResourceLoaderParams,
				boost::optional<std::string>                                    InURL,
				ResMan::ResourceDependencyList                                 *pInDepedenciesToLoadFrom,
				const ResMan::AddressableSubresourceList                       *pInAddressableSubresourceListToLoadFrom
			) :
				_mainSubresourceParams(*pInMainSubresourceParams),
				_resourceLoaderParams(*pInResourceLoaderParams),
				_url(InURL),
				_pDependenciesToLoadFrom(pInDepedenciesToLoadFrom)
			{
				if(pInAddressableSubresourceListToLoadFrom)
				{
					_addressableSubresourcesToLoadFrom = *pInAddressableSubresourceListToLoadFrom;
				}
			}

			void TestLoaderBase::BeginLoadDependencyLists(ResMan::SubresourceRoleSet InRoleSet)
			{
				ResMan::BeginLoadExtraDependencyLists(&_dependencyList, InRoleSet);
			}

			void TestLoaderBase::EndLoadingDependencyLists()
			{
				ResMan::EndLoadingDependencyLists(&_dependencyList);
			}

			void *TestLoaderBase::CreateResourceAndGet(void *pOldPtrOrNull)
			{
				AppendMethodCall_CreateResourceAndGet(&_realType, pOldPtrOrNull);
				return new TestRealType(_realType);
			}

			bool TestLoaderBase::LoadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				AppendMethodCall_LoadTemporaryOwnContent(&_realType, InRoleSet);
				_loadedTemporaryOwnContent.MergeWith(InRoleSet);
				return true;
			}
			void TestLoaderBase::UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				AppendMethodCall_UnloadTemporaryOwnContent(&_realType, InRoleSet);
				_loadedTemporaryOwnContent.Subtract(InRoleSet);
			}

			void TestLoaderBase::LoadDirectDepdendencyListOf(ResMan::SubresourceRoleSet InRoleSet)
			{
				AppendMethodCall_LoadDirectDependencyListOf(&_realType, InRoleSet);
			}
			void TestLoaderBase::UnloadDirectDepedencyListOf(ResMan::SubresourceRoleSet InRoleSet)
			{
				AppendMethodCall_UnloadDirectDependencyListOf(&_realType, InRoleSet);
			}
			void TestLoaderBase::LinkDependencyTo(ResMan::SubresourceRoleIndex InRoleIndex, DependencyIndex InDependency, ResMan::IManagedResource *pInResource)
			{
				AppendMethodCall_LinkDependencyTo(&_realType, InRoleIndex, InDependency, pInResource);
				_dependencyList.LinkDependencyToManagedResource(InRoleIndex, InDependency, pInResource);
			}

			void TestLoaderBase::SetInitialState_WhatMustBeLoaded(ResMan::SubresourceRoleSet InInitialState)
			{
				_whatMustBeLoaded = InInitialState;
			}
			void TestLoaderBase::SetInitialState_LoadedTemporaryOwnContent(ResMan::SubresourceRoleSet InInitialState)
			{
				_loadedTemporaryOwnContent = InInitialState;
			}




			size_t TestLoaderBase::GetNumAddressableSubresources(ResMan::ISubresourceRole *pInSubresource) const
			{
				return _addressableSubresources.GetNumAddressableSubresources(pInSubresource->GetRoleIndex());
			}
			size_t TestLoaderBase::GetNumAddressableSubresources(ResMan::SubresourceRoleIndex InSubresourceRoleIndex) const
			{
				return _addressableSubresources.GetNumAddressableSubresources(InSubresourceRoleIndex);
			}
			ResMan::SubresourceRoleSet TestLoaderBase::GetAddressableSubresourceRoles() const
			{
				return _addressableSubresources.GetAddressableSubresourceRoles();
			}
			const ResMan::LoadableAddressableSubresourceParams &TestLoaderBase::GetAddressableSubresourceParams(ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex) const
			{
				auto pSubresource = _addressableSubresources.GetSubresource(pInSubresource->GetRoleIndex(), InIndex);
				return pSubresource->params;
			}
			const ResMan::LoadableAddressableSubresourceParams &TestLoaderBase::GetAddressableSubresourceParams(ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex) const
			{
				auto pSubresource = _addressableSubresources.GetSubresource(InSubresourceRoleIndex, InIndex);
				return pSubresource->params;
			}

			void TestLoaderBase::LinkAdressableSubresourceTo(ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex, ResMan::IManagedResource *pInResource)
			{
				_addressableSubresources.LinkAddressableSubresourceTo(pInSubresource->GetRoleIndex(), InIndex, pInResource);
			}
			void TestLoaderBase::LinkAdressableSubresourceTo(ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex, ResMan::IManagedResource *pInResource)
			{
				_addressableSubresources.LinkAddressableSubresourceTo(InSubresourceRoleIndex, InIndex, pInResource);
			}
			void *TestLoaderBase::LoadCreateAndGetAddressableSubresource(ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex, void *pOldPtrOrNull)
			{
				return new TestRealType(_realType);
			}
			void *TestLoaderBase::LoadCreateAndGetAddressableSubresource(ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex, void *pOldPtrOrNull)
			{
				return new TestRealType(_realType);
			}



			/******************************************************************************************************************************
			* Test loader implementation
			*******************************************************************************************************************************/

			/********************************************************************************************************************************
			* TestRealType implementation
			********************************************************************************************************************************/
			TestRealType::MethodCallCont::const_iterator Find_ByLoaderMethodCode(const TestRealType *pResult, ELoaderMethod InLoaderMethod)
			{
				return Find_ByLoaderMethodCode(pResult->methodCalls.cbegin(), pResult->methodCalls.cend(), InLoaderMethod);
			}
			TestRealType::MethodCallCont::const_iterator Find_ByLoaderMethodCode(TestRealType::MethodCallCont::const_iterator first, TestRealType::MethodCallCont::const_iterator last, ELoaderMethod InLoaderMethod)
			{
				for (auto it = first; it != last; ++it)
				{
					if (it->get()->GetCode() == InLoaderMethod)
					{
						return it;
					}
				}
				return last;
			}

			void AppendMethodCall_WhatMustBeLoaded(TestRealType *pResult, void *pOldPtrOrNull)
			{
				pResult->methodCalls.emplace_back(new LoaderMethodCall(ELoaderMethod::WhatMustBeLoaded));
			}
			void AppendMethodCall_LoadTemporaryOwnContent(TestRealType *pResult, ResMan::SubresourceRoleSet InRoleSet)
			{
				pResult->methodCalls.emplace_back(new LoaderMethodCall(ELoaderMethod::LoadTemporaryOwnContent));
			}
			void AppendMethodCall_UnloadTemporaryOwnContent(TestRealType *pResult, ResMan::SubresourceRoleSet InRoleSet)
			{
				pResult->methodCalls.emplace_back(new LoaderMethodCall(ELoaderMethod::UnloadTemporaryOwnContent));
			}
			void AppendMethodCall_CreateResourceAndGet(TestRealType *pResult, void *pOldPtrOrNull)
			{
				pResult->methodCalls.emplace_back(new LoaderMethodCall(ELoaderMethod::CreateResourceAndGet));
			}
			void AppendMethodCall_LoadDirectDependencyListOf(TestRealType *pResult, ResMan::SubresourceRoleSet InRoleSet)
			{
				pResult->methodCalls.emplace_back(new LoaderMethodCall(ELoaderMethod::LoadDirectDependencyListOf));
			}
			void AppendMethodCall_UnloadDirectDependencyListOf(TestRealType *pResult, ResMan::SubresourceRoleSet InRoleSet)
			{
				pResult->methodCalls.emplace_back(new LoaderMethodCall(ELoaderMethod::UnloadDirectDependencyListOf));
			}
			void AppendMethodCall_LinkDependencyTo(TestRealType *pResult, ResMan::SubresourceRoleIndex InRoleIndex, ResMan::IResourceLoader::DependencyIndex InDependency, ResMan::IManagedResource *pInResource)
			{
				pResult->methodCalls.emplace_back(new LoaderMethodCall(ELoaderMethod::LinkDependencyTo));
			}
		}
	}
}