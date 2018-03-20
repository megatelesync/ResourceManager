#include "../../H/Override/IResourceTypeDescr.h"
#include "../../H/Interface/IResourceManager.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			void Log_ResourceTypeDescr(Util::ILoggingDevice *pInLogger, const IResourceTypeDescr *pInDescr, IResourceManager *pInResourceManager)
			{
				assert(pInLogger != nullptr);
				
				Util::LogRecordScoper scoper(pInLogger, "ResourceTypeDescr", Util::EMessageType::Info);

				Log_RegisteredResourceType_PublicParams(pInLogger, &(pInDescr->GetParams()), pInResourceManager);
			}


			void Log_ResourceTypeSubresourceRoleParams(Util::ILoggingDevice *pInLogger, const ResourceTypeSubresourceRoleParams *pInParams, IResourceManager *pInResourceManager)
			{
				assert(pInLogger != nullptr);

				Util::LogRecordScoper scoper(pInLogger, "ResourceTypeSubresourceRoleParams", Util::EMessageType::Info);

				auto applicableSubresourceRoles                       = pInParams->GetApplicableSubresourceRoles();
				auto rolesThatMustBeLoadedToCreateResource            = pInParams->GetRolesThatMustBeLoadedToCreateResource();
				
				auto applicableSubresourceRoles_str                   = ToString_SubresourceRoleSet(applicableSubresourceRoles, pInResourceManager);
				auto rolesThatMustBeLoadedToCreateResource_str        = ToString_SubresourceRoleSet(rolesThatMustBeLoadedToCreateResource, pInResourceManager);

				*pInLogger << "applicableSubresourceRoles: " << applicableSubresourceRoles_str << Util::endl;
				*pInLogger << "rolesThatMustBeLoadedToCreateResource: " << rolesThatMustBeLoadedToCreateResource_str << Util::endl;
			}

			void Log_RegisteredResourceType_PublicParams(Util::ILoggingDevice *pInLogger, const ResourceTypePublicParams *pInParams, IResourceManager *pInResourceManager)
			{
				assert(pInLogger != nullptr);

				Util::LogRecordScoper scoper(pInLogger, "ResourceTypePublicParams", Util::EMessageType::Info);

				Util::NotYetImplVoid_Silent("logging");
				*pInLogger << "Name: " << pInParams->GetName() << Util::endl;
				auto defaultUrlStr = pInParams->GetDefaultURL();
				if (defaultUrlStr.empty())
				{
					defaultUrlStr = "{EMPTY_STRING}";
				}
				*pInLogger << "DefaultURL: " << defaultUrlStr << Util::endl;
				*pInLogger << "SubdirectoryName: " << pInParams->GetSubdirectoryName() << Util::endl;
				*pInLogger << "SubresourceRoles: ";
				Log_ResourceTypeSubresourceRoleParams(pInLogger, &(pInParams->GetSubresourceRoles()), pInResourceManager);
			}


			void Log_RegisteredResourceType(Util::ILoggingDevice *pInLogger, const IResourceType *pInRegisteredResourceType, IResourceManager *pInResourceManager)
			{
				assert(pInLogger != nullptr);

				Util::LogRecordScoper scoper(pInLogger, "IResourceType", Util::EMessageType::Info);

				*pInLogger << "index: " << pInRegisteredResourceType->GetIndex() << Util::endl;
				*pInLogger << "public params: " << Util::endl;
				Log_RegisteredResourceType_PublicParams(pInLogger, &(pInRegisteredResourceType->GetParams()), pInResourceManager);
				Log_ResourceTypeDescr(pInLogger, pInRegisteredResourceType->GetDescr(), pInResourceManager);
			}
		}
	}
}