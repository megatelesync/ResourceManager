#pragma once

#include "../ResManLoadersD3D11_System.h"
#include "Utils/AddressableSubresourceList.h"
#include <memory>

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/****************************************************************************************
			* Roles
			****************************************************************************************/
			struct ResTypeDescr_Test_Roles : public ResTypeDescr_Roles_Base
			{
			};


			/****************************************************************************************
			* Real test type
			****************************************************************************************/
			enum class ELoaderMethod
			{
				WhatMustBeLoaded,
				LoadTemporaryOwnContent,
				UnloadTemporaryOwnContent,
				WhaOwnTemporaryContentLoaded,
				CreateResourceAndGet,
				LoadDirectDependencyListOf,
				UnloadDirectDependencyListOf,
				LinkDependencyTo
			};

			class LoaderMethodCall
			{
			public:
				LoaderMethodCall(ELoaderMethod InCode) : _code(InCode) {}

				ELoaderMethod                GetCode               () const               { return _code; }

			private:
				ELoaderMethod                             _code;
				//std::vector<std::unique_ptr<void>>        _args;
			};

			struct TestRealType
			{
				using MethodCallCont = std::vector<std::shared_ptr<LoaderMethodCall>>;

				MethodCallCont       methodCalls;
			};
			
			//  return end() if not found
			TestRealType::MethodCallCont::const_iterator Find_ByLoaderMethodCode(const TestRealType *pResult, ELoaderMethod InLoaderMethod);
			//  return last if not found
			TestRealType::MethodCallCont::const_iterator Find_ByLoaderMethodCode(TestRealType::MethodCallCont::const_iterator first, TestRealType::MethodCallCont::const_iterator last, ELoaderMethod InLoaderMethod);

			void AppendMethodCall_WhatMustBeLoaded                                 (TestRealType *pResult, void *pOldPtrOrNull);
			void AppendMethodCall_LoadTemporaryOwnContent                          (TestRealType *pResult, ResMan::SubresourceRoleSet InRoleSet);
			void AppendMethodCall_UnloadTemporaryOwnContent                        (TestRealType *pResult, ResMan::SubresourceRoleSet InRoleSet);
			void AppendMethodCall_CreateResourceAndGet                             (TestRealType *pResult, void *pOldPtrOrNull);
			void AppendMethodCall_LoadDirectDependencyListOf                       (TestRealType *pResult, ResMan::SubresourceRoleSet InRoleSet);
			void AppendMethodCall_UnloadDirectDependencyListOf                     (TestRealType *pResult, ResMan::SubresourceRoleSet InRoleSet);
			void AppendMethodCall_LinkDependencyTo                                 (TestRealType *pResult, ResMan::SubresourceRoleIndex InRoleIndex, ResMan::IResourceLoader::DependencyIndex InDependency, ResMan::IManagedResource *pInResource);

			/******************************************************************************************
			* Typed resource
			******************************************************************************************/
			using TestResource = ResMan::TypedManagedResource<TestRealType>;

			/****************************************************************************************
			* Test type descriptor
			****************************************************************************************/
			class ResTypeDescr_Test :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_Test_Roles, TestRealType>
			{
			public:
				ResTypeDescr_Test() : TypedD3D11ResTypeDescrBase<ResTypeDescr_Test_Roles, TestRealType>("Test", "TEST_RESOURCE"){}

				//virtual void       RegisterApplicableRoles(ResMan::IResourceManagerSubsystem *pInSubsys)    override
				virtual void       Delete(void *pInResource)                               override { delete pInResource; }

			private:
			};

			/**************************************************************************************************************
			* Typed test resource type
			**************************************************************************************************************/
			class TestResourceType : public ResMan::TypedResourceType<ResTypeDescr_Test>
			{
			public:
			};

			/****************************************************************************************
			* TestLoader base class
			*
			* Dependency lists to load from can be loaded from the given dependency list container.
			* The container is not copied, the pointer to it is kept.
			****************************************************************************************/
			class TestLoaderBase : public ResMan::IResourceLoader
			{
			public:
				TestLoaderBase
				(
					const ResMan::LoadableMainSubresourceParams                    *pInMainSubresourceParams,
					const ResMan::ResourceLoaderParams                             *pInResourceLoaderParams,
					boost::optional<std::string>                                    InURL,
					ResMan::ResourceDependencyList                                 *pInDepedenciesToLoadFrom = nullptr,
					const ResMan::AddressableSubresourceList                       *pInAddressableSubresourceListToLoadFrom = nullptr
				);

				/****************************************************************************************************************************
				* Working with loader state
				*****************************************************************************************************************************/
				void                                                                 SetInitialState_WhatMustBeLoaded            (ResMan::SubresourceRoleSet InInitialState);
				void                                                                 SetInitialState_LoadedTemporaryOwnContent   (ResMan::SubresourceRoleSet InInitialState);

				ResMan::SubresourceRoleSet                                           GetState_WhatMustBeLoaded                   () const { return _whatMustBeLoaded; }
				ResMan::SubresourceRoleSet                                           GetState_LoadedTemporaryOwnContent          () const { return _loadedTemporaryOwnContent; }

				/***************************************************************************************
				* Parameters of the loader itself
				***************************************************************************************/
				virtual const ResMan::ResourceLoaderParams&                         GetParams                                    () const override { return _resourceLoaderParams; }

				/***************************************************************************************
				* Parameters of the main resource to be created
				***************************************************************************************/
				virtual const ResMan::LoadableMainSubresourceParams&                GetMainResourceParams                        () const override { return _mainSubresourceParams; }

				/***************************************************************************************
				* Source identification
				***************************************************************************************/
				virtual bool                                                         HasNoURL                                    () const override { return !_url.is_initialized(); }
				virtual const std::string&                                           GetURL                                      () const override { return _url.get(); }

				/****************************************************************************************
				* Create NEW INSTANCE of the resource
				****************************************************************************************/
				virtual void                                                        *CreateResourceAndGet                        (void *pOldPtrOrNull) override;
				virtual ResMan::SubresourceRoleSet                                   WhatMustBeLoaded                            (void *pOldPtrOrNull) const override { return _whatMustBeLoaded; }
				
				/****************************************************************************************
				* Working with adressable subresources
				****************************************************************************************/
				virtual size_t                                                       GetNumAddressableSubresources              (ResMan::ISubresourceRole *pInSubresource) const override;
				virtual size_t                                                       GetNumAddressableSubresources              (ResMan::SubresourceRoleIndex InSubresourceRoleIndex) const override;
				virtual ResMan::SubresourceRoleSet                                   GetAddressableSubresourceRoles             () const override;

				virtual const ResMan::LoadableAddressableSubresourceParams          &GetAddressableSubresourceParams            (ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex) const override;
				virtual const ResMan::LoadableAddressableSubresourceParams          &GetAddressableSubresourceParams            (ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex) const override;

				virtual void                                                         LinkAdressableSubresourceTo                (ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex, ResMan::IManagedResource *pInResource) override;
				virtual void                                                         LinkAdressableSubresourceTo                (ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex, ResMan::IManagedResource *pInResource) override;

				virtual void                                                        *LoadCreateAndGetAddressableSubresource(ResMan::ISubresourceRole *pInSubresource, ResMan::AddressableSubresourceIndex InIndex, void *pOldPtrOrNull) override;
				virtual void                                                        *LoadCreateAndGetAddressableSubresource(ResMan::SubresourceRoleIndex InSubresourceRoleIndex, ResMan::AddressableSubresourceIndex InIndex, void *pOldPtrOrNull) override;

				/****************************************************************************************
				* Loading and unloading temporary data
				****************************************************************************************/
				virtual bool                                                         LoadTemporaryOwnContent                     (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                         UnloadTemporaryOwnContent                   (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual ResMan::SubresourceRoleSet                                   WhatOwnTemporaryContentLoaded               () const override { return _loadedTemporaryOwnContent; }

				/****************************************************************************************
				* Working with dependencies
				****************************************************************************************/
				//  This function to be called before loading dependencies of the given role (typically from the main thread)
				virtual void                                                        BeginLoadDependencyLists(ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                        EndLoadingDependencyLists() override;

				virtual const ResMan::ResourceDependencyList&                        GetDependencyList                           () const override { return _dependencyList; }
				virtual void                                                         LoadDirectDepdendencyListOf                 (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                         UnloadDirectDepedencyListOf                 (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                         LinkDependencyTo                            (ResMan::SubresourceRoleIndex InRoleIndex, DependencyIndex InDependency, ResMan::IManagedResource *pInResource) override;

				/****************************************************************************************
				* Error-handling
				****************************************************************************************/
				virtual bool                                         HasCreationFailed                                           () const override { return false; }
				virtual ResMan::SubresourceRoleSet                   GetSubresourceRoles_FailedLoading                           () const override { return ResMan::SubresourceRoleSet(); }
				virtual ResMan::SubresourceRoleSet                   GetSubresourceRoles_FailedLoadingDependencies               () const override { return ResMan::SubresourceRoleSet(); }

			protected:
				TestRealType                                                     _realType;
				ResMan::ResourceDependencyList                                  *_pDependenciesToLoadFrom;
				ResMan::AddressableSubresourceList                               _addressableSubresourcesToLoadFrom;

				ResMan::LoadableMainSubresourceParams                            _mainSubresourceParams;
				ResMan::ResourceLoaderParams                                     _resourceLoaderParams;
				boost::optional<std::string>                                     _url;

				ResMan::ResourceDependencyList                                   _dependencyList;
				ResMan::SubresourceRoleSet                                       _whatMustBeLoaded;
				ResMan::SubresourceRoleSet                                       _loadedTemporaryOwnContent;
				ResMan::AddressableSubresourceList                               _addressableSubresources;
 			};

			/****************************************************************************************
			* TestLoader that loads all what it asked for, without fail codes.
			****************************************************************************************/
			class TestLoader_LoadAll_NoFail : public TestLoaderBase
			{
			public:
				TestLoader_LoadAll_NoFail
				(
					const ResMan::LoadableMainSubresourceParams                    *pInMainSubresourceParams,
					const ResMan::ResourceLoaderParams                             *pInResourceLoaderParams,
					boost::optional<std::string>                                    InURL,
					ResMan::ResourceDependencyList                                 *pInDepedenciesToLoadFrom = nullptr,
					const ResMan::AddressableSubresourceList                       *pInAddressableSubresourceListToLoadFrom = nullptr
				);				

				/****************************************************************************************************************************
				* IResourceLoader implementation
				*****************************************************************************************************************************/
				virtual bool                                                         LoadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                         UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet) override;

				virtual void                                                         LoadDirectDepdendencyListOf(ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                                         UnloadDirectDepedencyListOf(ResMan::SubresourceRoleSet InRoleSet) override;
			};

			ResMan::ResourceLoaderParams GetDefault_ResourceLoaderParams();
			ResMan::PublicResourceParams GetDefault_PublicResourceParams
			(
				ResMan::ISubresourceRole                                            *pInSubresourceRole
			);
			ResMan::LoadableMainSubresourceParams GetDefault_LoadableMainSubresourceParams
			(
				ResMan::ISubresourceRole                                            *pInSubresourceRole,
				ResMan::IResourceType                                               *pInResourceType
			);

			// Create default test loader with ALL parameters set to default values
			std::shared_ptr<TestLoader_LoadAll_NoFail> CreateDefault_TestLoader_LoadAll_NoFail
			(
				ResMan::ISubresourceRole                                            *pInSubresourceRole, 
				ResMan::IResourceType                                               *pInResourceType,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList = nullptr
			);
			std::shared_ptr<TestLoader_LoadAll_NoFail> CreateDefault_TestLoader_LoadAll_NoFail
			(
				const ResMan::LoadableMainSubresourceParams                         *pInMainResourceParams,
				const ResMan::ResourceLoaderParams                                  *pInResourceLoaderParams,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList = nullptr
			);
			std::shared_ptr<TestLoader_LoadAll_NoFail> CreateDefault_TestLoader_LoadAll_NoFail
			(
				const std::string                                                   &InURL, 
				const ResMan::LoadableMainSubresourceParams                         *pInMainResourceParams,
				const ResMan::ResourceLoaderParams                                  *pInResourceLoaderParams,
				ResMan::ResourceDependencyList                                      *pInSourceDependencyList = nullptr
			);
		}
	}
}