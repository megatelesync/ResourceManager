#include "../../H/Models/MaxModelLoaders.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			namespace ModeModelFileResource_Ver0_0_0_1
			{
				ResTypeDescr_MaxModelFileResource::ResTypeDescr_MaxModelFileResource() :
					TypedD3D11ResTypeDescrBase<ResTypeDescr_MaxModelFileResource_Roles, MaxModel::File>("MaxModelFile_Ver0_0_0_1", "MaxModelFile\\Ver0_0_0_1")
				{
				}

				void ResTypeDescr_MaxModelFileResource::Delete(void *pInResource)
				{
					delete pInResource;
				}

				MaxModelFileResourceLoader::MaxModelFileResourceLoader
				(
					const std::string                      &InURL,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					ResourceType                           *pInTypedResourceType
				) : TypedD3D11ResourceLoaderBase<MaxModelFileResourceType>(InURL, pInDev, pInRole, pInTypedResourceType)
				{
				}

				MaxModelFileResourceLoader::RealResourceType *MaxModelFileResourceLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
				{
					auto pRole = GetRoles().pStd;
					auto pSavedPtr = _pLoadedFile;
					_pLoadedFile = nullptr;
					_MarkTempContentRole_Unloaded(pRole);
					return pSavedPtr;
				}

				bool MaxModelFileResourceLoader::LoadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
				{
					auto pRole = GetRoles().pStd;
					if (_ShouldLoadTempContent(InRoleSet, pRole))
					{
						std::ifstream ifs(GetURL(), std::ios::in);
						if (false == ifs.is_open())
						{
							return false;
						}
						auto loadError = MaxModel::TryLoadFromFile(&ifs, _pLoadedFile);
						if (loadError.is_initialized())
						{
							return false;
						}
						_MarkTempContentRole_Loaded(pRole);
					}
					return true;
				}
				void MaxModelFileResourceLoader::UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
				{
					auto pRole = GetRoles().pStd;
					if (_ShouldTempContentBeUnloaded(InRoleSet, pRole))
					{
						delete _pLoadedFile;
						_MarkTempContentRole_Unloaded(pRole);
					}
				}
			}
		}
	}
}