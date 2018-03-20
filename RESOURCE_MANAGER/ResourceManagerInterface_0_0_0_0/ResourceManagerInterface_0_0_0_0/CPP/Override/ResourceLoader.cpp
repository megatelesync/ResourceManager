#include "../../H/Override/IResourceLoader.h"
#include "../../H/Interface/IResourceManager.h"
#include "../../H/Utils/ResourceDependencyList.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			bool Check_URL_Correct(const std::string& InURL)
			{
				bool bNotEmpty = (false == InURL.empty());
				return bNotEmpty;
			}
			void AssertAddressableSubresourceParamsCorrect(const LoadableAddressableSubresourceParams *pInParams)
			{
				assert(pInParams);
				bool bURLCorrect = Check_URL_Correct(pInParams->GetURL());
				assert(bURLCorrect);
				assert(pInParams->GetResourceType());
			}

			/************************************************************************************************************************
			* IResourceLoader utils
			************************************************************************************************************************/
			void Log_ResourceLoaderParams(Util::ILoggingDevice *pInLog, const ResourceLoaderParams *pInParams)
			{
				Util::LogRecordScoper scoper(pInLog, "ResourceLoaderParams", Util::EMessageType::Info);
				*pInLog << "IsProcedural: " << pInParams->IsProcedural() << Util::endl;
				*pInLog << "CanBeThreaded: " << pInParams->CanBeThreaded() << Util::endl;
				*pInLog << "ShouldSearchURLsInDataDir: " << pInParams->ShouldSearchURLsInDataDir() << Util::endl;
			}

			void Log_LoadableMainSubresourceParams(Util::ILoggingDevice *pInLog, const LoadableMainSubresourceParams *pInParams)
			{
				Util::LogRecordScoper scoper(pInLog, "LoadableMainSubresourceParams", Util::EMessageType::Info);
				Util::NotYetImplVoid_Silent("Extract Log_PublicResourceParams from the ResManQuick library");
				Util::NotYetImplVoid_Silent("Extract Log_ResourceType from the ResManQuick library");

				*pInLog << "SubresourceRole: " << Util::endl;
				std::string subresourceRoleStr = ToString_SubresourceRole(pInParams->GetRole());

			}

			void Log_LoadableAddressableSubresourceParams(Util::ILoggingDevice *pInLog, AddressableSubresourceIndex addressableSubresourceIndex, const LoadableAddressableSubresourceParams *pInParams)
			{
				Util::LogRecordScoper scoper(pInLog, "Addressable subresource: ", Util::EMessageType::Info);
				*pInLog << "index=" << addressableSubresourceIndex;
				*pInLog << "; URL=" << pInParams->GetURL();
				*pInLog << "; Type=";
				Util::NotYetImplVoid_Silent("Output type of the resource");
				*pInLog << Util::endl;
			}

			void Log_AddressableSubresources(Util::ILoggingDevice *pInLog, const IResourceLoader *pInLoader, IResourceManager *pInResourceManager)
			{
				Util::LogRecordScoper scoper(pInLog, "Addressable subresources", Util::EMessageType::Info);
				auto addressableSubresourceRoles = pInLoader->GetAddressableSubresourceRoles();
				*pInLog << "AddressableSubresourceRoles" << ToString_SubresourceRoleSet(addressableSubresourceRoles, pInResourceManager) << Util::endl;

				auto pPreviousRole = static_cast<ISubresourceRole *>(nullptr);
				AdressableSubresourceEnumerator enumerator(const_cast<IResourceLoader *>(pInLoader), addressableSubresourceRoles.GetEnumerator());
				while(true)
				{
					if (enumerator.IsEnd())
					{
						break;
					}

					auto roleIndex = enumerator.GetSubresourceRoleIndex();
					auto pRole = pInResourceManager->GetSubresourceRoleByIndex(roleIndex);
					auto addressableSubresourceIndex = enumerator.GetAddressableSubresourceIndex();
					auto pSubresourceParams = &(pInLoader->GetAddressableSubresourceParams(roleIndex, addressableSubresourceIndex));

					auto numAddressableSubresources = pInLoader->GetNumAddressableSubresources(roleIndex);
					if (pRole != pPreviousRole && numAddressableSubresources > 0)
					{
						*pInLog << "Role" << ToString_SubresourceRole(pRole) << Util::endl;
						*pInLog << "Num addressable subresources of role: " << numAddressableSubresources << Util::endl;
						pPreviousRole = pRole;
					} 

					Log_LoadableAddressableSubresourceParams(pInLog, addressableSubresourceIndex, pSubresourceParams);
					enumerator.MoveNext();
				}
			}

			void Log_ResourceLoader(Util::ILoggingDevice *pInLog, const IResourceLoader *pInLoader, IResourceManager *pInResourceManager)
			{
				Util::LogRecordScoper scoper(pInLog, "IResourceLoader:", Util::EMessageType::Info);

				if (pInLoader == nullptr)
				{
					*pInLog << "{nullptr}" << Util::endl;
				}
				else
				{
					Log_ResourceLoaderParams(pInLog, &(pInLoader->GetParams()));
					Log_LoadableMainSubresourceParams(pInLog, &(pInLoader->GetMainResourceParams()));
					bool bHasNoURL = pInLoader->HasNoURL();
					*pInLog << "bHasNoURL: " << bHasNoURL << Util::endl;
					if (false == bHasNoURL)
					{
						*pInLog << "URL: " << pInLoader->GetURL() << Util::endl;
					}
					auto whatMustBeLoaded = pInLoader->WhatMustBeLoaded(nullptr);
					*pInLog << "WhatMustBeLoaded(nullptr) returned: " << ToString_SubresourceRoleSet(whatMustBeLoaded, pInResourceManager) << Util::endl;
					auto bHasCreationFailed = pInLoader->HasCreationFailed();
					*pInLog << "HasCreationFailed: " << bHasCreationFailed << Util::endl;
					auto subresourceRoles_FailedLoading = pInLoader->GetSubresourceRoles_FailedLoading();
					*pInLog << "SubresourceRoles_FailedLoading: " << ToString_SubresourceRoleSet(subresourceRoles_FailedLoading, pInResourceManager) << Util::endl;
					auto subresourceRoles_FailedLoadingDependencies = pInLoader->GetSubresourceRoles_FailedLoadingDependencies();
					*pInLog << "SubresourceRoles_FailedLoadingDependencies: " << ToString_SubresourceRoleSet(subresourceRoles_FailedLoadingDependencies, pInResourceManager) << Util::endl;
					Log_AddressableSubresources(pInLog, pInLoader, pInResourceManager);
					auto subresourceRole_OwnTemporaryContent = pInLoader->WhatOwnTemporaryContentLoaded();
					*pInLog << "OwnTemporaryContent loaded: " << ToString_SubresourceRoleSet(subresourceRole_OwnTemporaryContent, pInResourceManager) << Util::endl;
					Log_DependencyList(pInLog, &(pInLoader->GetDependencyList()), pInResourceManager);
				}
			}

			/************************************************************************************************************************
			* AddressableSubresourceEnumerator
			************************************************************************************************************************/
			bool AdressableSubresourceEnumerator::_IsAdressableSubresourceIndex_OutOfRange() const
			{
				auto bResult = _addressableSubresourceIndex >= GetNumAddressableSubresources_OfSubresourceRole();
				return bResult;
			}

			AdressableSubresourceEnumerator::AdressableSubresourceEnumerator(IResourceLoader *pInLoader, SubresourceRoleEnumerator InCurrentProcessingRole) :
				_pLoader(pInLoader),
				enumerator_currentProcessingRole(InCurrentProcessingRole),
				_addressableSubresourceIndex(0),
				_bMoved(false)
			{
				_Initialize_CurrentSubresourceRole();
			}

			void AdressableSubresourceEnumerator::_Initialize_CurrentSubresourceRole()
			{
				if (!IsEnd() && _IsAdressableSubresourceIndex_OutOfRange())
				{
					MoveNextSubresourceRole_WithDependencies();
				}
			}

			bool AdressableSubresourceEnumerator::MoveNextSubresourceRole()
			{
				_bMoved = true;
				auto nextRole = enumerator_currentProcessingRole.MoveNext();
				if (nextRole.is_initialized())
				{
					return true;
				}
				else
				{
					return false;
				}
			}

			bool AdressableSubresourceEnumerator::MoveNextSubresourceRole_WithDependencies()
			{
				while (true)
				{
					MoveNextSubresourceRole();
					if (IsEnd())
					{
						return false;
					}
					if (!_IsAdressableSubresourceIndex_OutOfRange())
					{
						return true;
					}
				}
			}

			bool AdressableSubresourceEnumerator::MoveNext()
			{
				++_addressableSubresourceIndex;
				if (_IsAdressableSubresourceIndex_OutOfRange())
				{
					auto bIsEnd = MoveNextSubresourceRole_WithDependencies();
					return bIsEnd;
				}
				else
				{
					return true;
				}
			}

			/************************************************************************************************************************
			* Accessors implementation
			************************************************************************************************************************/
			bool HasURL(IResourceLoader *pInLoader, std::string *pOutURL, SubresourceRoleIndex InRoleIndex, AddressableSubresourceIndex InIndex)
			{
				if (AddressableSubresourceIndex_IsNull(InIndex))
				{
					return HasMainSubresourceURL(pInLoader, pOutURL);
				}
				else
				{
					auto pAddressableSubresourceParams = &(pInLoader->GetAddressableSubresourceParams(InRoleIndex, InIndex));
					auto resultURL = pAddressableSubresourceParams->GetURL();
					*pOutURL = resultURL;
					return true;
				}
			}
			bool HasMainSubresourceURL(IResourceLoader *pInLoader, std::string *pOutURL)
			{
				bool bResourceWithURL = false;
				if (pInLoader != nullptr)
				{
					if (!pInLoader->HasNoURL() && pInLoader->GetURL().size() > 0)
					{
						bResourceWithURL = true;
						*pOutURL = pInLoader->GetURL();
					}
				}
				return bResourceWithURL;
			}
		}
	}
}