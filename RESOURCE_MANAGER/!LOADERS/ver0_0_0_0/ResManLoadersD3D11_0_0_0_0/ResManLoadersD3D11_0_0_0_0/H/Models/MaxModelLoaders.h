#pragma once

#include "MaxModel_0_0_0_1.h"
#include "../../H/ResManLoadersD3D11_System.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			namespace ModeModelFileResource_Ver0_0_0_1
			{
				namespace MaxModel = Dv::MaxModel::Ver_0_0_0_1;

				/******************************************************************************************
				* Typed resource
				******************************************************************************************/
				using MaxModelFileResource = ResMan::TypedManagedResource<MaxModel::File>;

				/******************************************************************************************
				* Set of subresource role types
				******************************************************************************************/
				struct ResTypeDescr_MaxModelFileResource_Roles : public ResTypeDescr_Roles_Base
				{
				};

				class ResTypeDescr_MaxModelFileResource :
					public TypedD3D11ResTypeDescrBase<ResTypeDescr_MaxModelFileResource_Roles, MaxModel::File>
				{
				public:
					ResTypeDescr_MaxModelFileResource();
					
					virtual void Delete(void *pInResource) override;

				private:
				};

				/*****************************************************************************************
				* Typed resource type
				******************************************************************************************/
				class MaxModelFileResourceType : public ResMan::TypedResourceType<ResTypeDescr_MaxModelFileResource>
				{
				public:
				};

				/****************************************************************************************
				* Loader that loads max model from the given file
				****************************************************************************************/
				class MaxModelFileResourceLoader : public TypedD3D11ResourceLoaderBase<MaxModelFileResourceType>
				{
				public:
					/*******************************************************************************************************
					* CONSTRUCTION
					*******************************************************************************************************/
					MaxModelFileResourceLoader
					(
						const std::string                      &InURL,
						ID3D11Device                           *pInDev,
						ResMan::ISubresourceRole               *pInRole,
						ResourceType                           *pInTypedResourceType
					);

					virtual RealResourceType                   *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override final;

					virtual bool                                LoadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet) override final;
					virtual void                                UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet) override final;

				private:
					MaxModel::File                             *_pLoadedFile = nullptr;
				};
			}
		}
	}
}