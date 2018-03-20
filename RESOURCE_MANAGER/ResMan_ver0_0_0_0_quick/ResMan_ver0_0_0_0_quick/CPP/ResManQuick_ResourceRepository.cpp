#include "../PrivateH/ResManQuick_impl_ResourceRepository.h"
#include "../PrivateH/ResManQuick_impl_ResourceManagerSubsystem.h"
#include "../PrivateH/ResManQuick_impl_access_SubresourceRole.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			bool IsRegisterFail_IsFatalError
			(
				ResourceRepository                                                    *pRepository,
				const ResMan::PublicResourceParams                                    *pInParams,
				ResMan::IResourceLoader                                               *pInLoader,
				ResMan::NullableAddressableSubresourceIndex                            InAdressableSubresourceIndex
			);
			void Log_ResourceRepository_ResourceRegistration_Failed_IfNecessary
			(
				ResourceRepository                                                    *pRepository,
				const ResMan::PublicResourceParams                                    *pInParams,
				ResMan::IResourceLoader                                               *pInLoader,
				ResMan::NullableAddressableSubresourceIndex                            InAdressableSubresourceIndex
			);
			void Log_ResourceRepositry_ReturningFromDestructor_IfNecessary(ResourceRepository *pRepository);
			void Log_ResourceRepositry_ReturningFromDestructor(ResourceRepository *pRepository);
			void Log_ResourceRepository_Initialized_IfNecessary(ResourceRepository *pRepository);
			void Log_ResourceRepository_Initialized(ResourceRepository *pRepository);
			void Log_ResourceRepository_ResourceRegistered_IfNecessary
			(
				ResourceRepository                                                    *pRepository,
				ManagedResource                                                       *pRegisteredResource,
				const ResMan::PublicResourceParams                                    *pInParams,
				ResMan::IResourceLoader                                               *pInLoader,
				ResMan::NullableAddressableSubresourceIndex                            InAdressableSubresourceIndex
			);
			void Log_ResourceRepository_ResourceRegistered
			(
				ResourceRepository                                                    *pRepository,
				ManagedResource                                                       *pRegisteredResource,
				const ResMan::PublicResourceParams                                    *pInParams,
				ResMan::IResourceLoader                                               *pInLoader,
				ResMan::NullableAddressableSubresourceIndex                            InAdressableSubresourceIndex
			);

			/***************************************************************************************************************
			* Utils fwd declarations
			***************************************************************************************************************/
			ResMan::ResourceParams                      CreateResourceParams(const ResMan::PublicResourceParams& InParams, std::shared_ptr<ResMan::IResourceLoader>pInLoader, ResMan::NullableAddressableSubresourceIndex InAdressableSubresourceIndex);

			/***************************************************************************************************************
			* ResourceRepository implementation
			***************************************************************************************************************/
			ResourceRepository::~ResourceRepository()
			{
				Log_ResourceRepositry_ReturningFromDestructor_IfNecessary(this);
			}
			ResourceRepository::ResourceRepository() :
				_pOwnerManager(nullptr)
			{
			}
			void ResourceRepository::Initialize(Impl_ResourceManager *pInOwnerManager)
			{
				assert(_pOwnerManager == nullptr);
				_pOwnerManager = pInOwnerManager;

				Log_ResourceRepository_Initialized_IfNecessary(this);
			}

			// Checks that the given desired URL is valid.
			// Returns URL that is to be actually used for the given resource.
			// For example, if URL was a filename URL and was not absolute, returns absolute URL.
			// If the given URL cannot be modified to be useful return nullptr.
			boost::optional<std::string> CheckURLValidity_And_GetActualURL(Impl_ResourceManagerSubsystem *pInResourceManagerSubsystem, std::shared_ptr<ResMan::IResourceLoader> pInLoader, const std::string& InDesiredURL, ResMan::IResourceType *pInResourceType, ResMan::NullableAddressableSubresourceIndex InAdressableSubresourceIndex)
			{
				if (false == ResMan::AddressableSubresourceIndex_IsNull(InAdressableSubresourceIndex))
				{
					return InDesiredURL;
				}
				if (pInLoader->GetParams().ShouldSearchURLsInDataDir())
				{
					auto pSubsystemInterface = pInResourceManagerSubsystem->GetSubsystem();
					boost::optional<std::string> resultActualURL = ResMan::GetExistingFullFilename(pSubsystemInterface, InDesiredURL, pInResourceType);
					return resultActualURL;
				}
				else
				{
					return InDesiredURL;
				}
			}

			void ResourceRepository::UnregisterResource(Impl_ManagedResource *pInResource)
			{
				_resources.UnregisterResource(pInResource->GetIter());
				auto pRole = pInResource->GetParams().pub.GetRole();
				CastTyped(pRole)->GetImpl()->UnregisterResource(pInResource);
			}

			ManagedResource *ResourceRepository::FindByURL(const std::string& InURL)
			{
				return _resources.FindByUrl(InURL);
			}

			ManagedResource *ResourceRepository::RegisterAddressableResource(std::shared_ptr<ResMan::IResourceLoader> pInLoader, const ResMan::LoadableAddressableSubresourceParams *pInAddressableParams, ResMan::NullableAddressableSubresourceIndex InAdressableSubresourceIndex)
			{
				auto pPublicResourceParams = pInAddressableParams->GetPublicResourceParams();
				auto pResultResource = RegisterResource(*pPublicResourceParams, pInLoader, pInAddressableParams->GetResourceType(), InAdressableSubresourceIndex);
				return pResultResource;
			}
			
			ManagedResource *ResourceRepository::RegisterResource(const ResMan::PublicResourceParams& InParams, std::shared_ptr<ResMan::IResourceLoader> pInLoader, ResMan::IResourceType *pInResourceType, ResMan::NullableAddressableSubresourceIndex InAdressableSubresourceIndex)
			{
				assert(&InParams != nullptr);

				ManagedResource       *pResultRegisteredResource            = nullptr;

				std::string URL;
				bool bResourceWithURL = ResMan::HasURL(pInLoader.get(), &URL, InParams.GetRole()->GetRoleIndex(), InAdressableSubresourceIndex);

				bool bShouldRegisterNewResource = false;
				bool bError = false;

				// By default actual URL is not initialized - it's intended;
				boost::optional<std::string> actualURL;
				if (bResourceWithURL)
				{
					actualURL = CheckURLValidity_And_GetActualURL(GetSubsystem(), pInLoader, URL, pInResourceType, InAdressableSubresourceIndex);
					if (false == actualURL.is_initialized())
					{
						pResultRegisteredResource = nullptr;
						bError = true;
					}
					else
					{
						auto pResource = _resources.FindByUrl(actualURL.get());
						if (pResource != nullptr)
						{
							pResultRegisteredResource = pResource;
							bShouldRegisterNewResource = false;
						}
						else
						{
							bShouldRegisterNewResource = true;
						}
					}
				}
				else
				{
					bShouldRegisterNewResource = true;
				}

				if(bShouldRegisterNewResource)
				{
					pResultRegisteredResource = _RegisterNewResource(InParams, pInLoader, actualURL, InAdressableSubresourceIndex);
					if (pResultRegisteredResource == nullptr)
					{
						bError = true;
					}
				}

				if (bError)
				{
					Log_ResourceRepository_ResourceRegistration_Failed_IfNecessary(this, &InParams, pInLoader.get(), InAdressableSubresourceIndex);
					if (IsRegisterFail_IsFatalError(this, &InParams, pInLoader.get(), InAdressableSubresourceIndex))
					{
						exit(-1);
					}
				}
				else
				{
					Log_ResourceRepository_ResourceRegistered_IfNecessary(this, pResultRegisteredResource, &InParams, pInLoader.get(), InAdressableSubresourceIndex);
				}
				return pResultRegisteredResource;
			}

			ManagedResource *ResourceRepository::_RegisterNewResource(const ResMan::PublicResourceParams& InParams, std::shared_ptr<ResMan::IResourceLoader> pInLoader, boost::optional<std::string> InURL, ResMan::NullableAddressableSubresourceIndex InAdressableSubresourceIndex)
			{
				auto pTypedResourceRole                          = CastTyped(InParams.GetRole());

				size_t id                                        = _resources.GenerateID();

				auto it = _resources.ReserveIterator();
				auto it_subresource_in_role_container            = pTypedResourceRole->GetImpl()->ReserveResourceIterator();

				ResMan::ResourceParams resourceParams            = CreateResourceParams(InParams, pInLoader, InAdressableSubresourceIndex);
				auto pNewResource                                = std::make_shared<ManagedResource>(_pOwnerManager, resourceParams, id, it, it_subresource_in_role_container);

				_resources.BindIteratorToResource(it, InURL, pNewResource);
				pTypedResourceRole->GetImpl()->BindIteratorToResource(it_subresource_in_role_container, InURL, pNewResource);

				pNewResource->GetImpl()->Notify_AfterRegisteredInResourceRepo();

				return pNewResource.get();
			}

			Impl_ResourceManagerSubsystem *ResourceRepository::GetSubsystem() const
			{
				return _pOwnerManager->GetSubsystem();
			}

			/*******************************************************************************************************************
			* Utils implementation
			*******************************************************************************************************************/
			ResMan::ResourceParams CreateResourceParams(const ResMan::PublicResourceParams& InParams, std::shared_ptr<ResMan::IResourceLoader> pInLoader, ResMan::NullableAddressableSubresourceIndex InAdressableSubresourceIndex)
			{
				ResMan::ResourceParams resourceParams;
				resourceParams.pub = InParams;
				resourceParams.pLoader = pInLoader;
				resourceParams.addressable_subresource_index = InAdressableSubresourceIndex;
				return resourceParams;
			}

			void Log_ResourceRepository_ResourceRegistration_Failed
			(
				ResourceRepository                                                    *pRepository,
				const ResMan::PublicResourceParams                                    *pInParams,
				ResMan::IResourceLoader                                               *pInLoader,
				ResMan::NullableAddressableSubresourceIndex                            InAdressableSubresourceIndex
			)
			{
				auto pManager = pRepository->GetSubsystem()->GetManager();
				auto pLogger = pRepository->GetSubsystem()->GetLogger_MainThread();
				Util::LogRecordScoper scoper(pLogger, "ResourceRepository: Registration failed", Util::EMessageType::FatalError);

				*pLogger << "AddressableSubresourceIndex: " << InAdressableSubresourceIndex << Util::endl;

				*pLogger << "Loader: " << Util::endl;
				ResMan::Log_ResourceLoader(pLogger, pInLoader, pManager->GetInterface());
			}
			void Log_ResourceRepository_ResourceRegistration_Failed_IfNecessary
			(
				ResourceRepository                                                    *pRepository,				
				const ResMan::PublicResourceParams                                    *pInParams,
				ResMan::IResourceLoader                                               *pInLoader,
				ResMan::NullableAddressableSubresourceIndex                            InAdressableSubresourceIndex
			)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceRepository_ResourceRegistration_Failed
					(
						pRepository,
						pInParams,
						pInLoader,
						InAdressableSubresourceIndex
					);
				}
			}

			void Log_ResourceRepository_ResourceRegistered_IfNecessary
			(
				ResourceRepository                                                    *pRepository,
				ManagedResource                                                       *pInRegisteredResource,
				const ResMan::PublicResourceParams                                    *pInParams,
				ResMan::IResourceLoader                                               *pInLoader,
				ResMan::NullableAddressableSubresourceIndex                            InAdressableSubresourceIndex
			)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceRepository_ResourceRegistered(pRepository, pInRegisteredResource, pInParams, pInLoader, InAdressableSubresourceIndex);
				}
			}

			void Log_ResourceRepository_Initialized_IfNecessary(ResourceRepository *pRepository)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceRepository_Initialized(pRepository);
				}
			}

			void Log_ResourceRepositry_ReturningFromDestructor_IfNecessary(ResourceRepository *pRepository)
			{
				bool bShouldLog = true;
				if (bShouldLog)
				{
					Log_ResourceRepositry_ReturningFromDestructor(pRepository);
				}
			}

			void Log_ResourceRepositry_ReturningFromDestructor(ResourceRepository *pRepository)
			{
				auto pLogger = pRepository->GetSubsystem()->GetLogger_MainThread();
				Util::LogRecordScoper scoper(pLogger, "ResourceRepository: returning from destructor", Util::EMessageType::Status);
			}

			void Log_ResourceRepository_Initialized(ResourceRepository *pRepository)
			{
				auto pLogger = pRepository->GetSubsystem()->GetLogger_MainThread();
				Util::LogRecordScoper scoper(pLogger, "ResourceRepository: initialized", Util::EMessageType::Status);
			}

			void Log_ResourceRepository_ResourceRegistered
			(
				ResourceRepository                                                    *pRepository,
				ManagedResource                                                       *pInRegisteredResource,
				const ResMan::PublicResourceParams                                    *pInParams,
				ResMan::IResourceLoader                                               *pInLoader,
				ResMan::NullableAddressableSubresourceIndex                            InAdressableSubresourceIndex
			)
			{
				auto pManager = pRepository->GetSubsystem()->GetManager();
				auto pLogger = pRepository->GetSubsystem()->GetLogger_MainThread();
				Util::LogRecordScoper scoper(pLogger, "ResourceRepository: resource registered", Util::EMessageType::Status);

				*pLogger << "AddressableSubresourceIndex: " << InAdressableSubresourceIndex << Util::endl;

				*pLogger << "Resource:" << Util::endl;
				Log_Resource(pLogger, pInRegisteredResource->GetImpl(), Util::EMessageType::FatalError);

				*pLogger << "Loader: " << Util::endl;
				ResMan::Log_ResourceLoader(pLogger, pInLoader, pManager->GetInterface());
			}

			bool IsRegisterFail_IsFatalError
			(
				ResourceRepository                                                    *pRepository,
				const ResMan::PublicResourceParams                                    *pInParams,
				ResMan::IResourceLoader                                               *pInLoader,
				ResMan::NullableAddressableSubresourceIndex                            InAdressableSubresourceIndex
			)
			{
				auto pSubsystemParams = &(pRepository->GetSubsystem()->GetParams());
				bool bTreatAllErrors_AsFatal = pSubsystemParams->GetCommonSubsystemParams().GetErrorHandling()->ShouldAlwaysTreatErrorsAsFatal();
				bool bResult = bTreatAllErrors_AsFatal;
				return bResult;
			}
		}
	}
}