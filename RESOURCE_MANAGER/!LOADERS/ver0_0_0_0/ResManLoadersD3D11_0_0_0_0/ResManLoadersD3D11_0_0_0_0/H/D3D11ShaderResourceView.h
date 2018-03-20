#pragma once

#include "ResManLoadersD3D11_System.h"
#include "D3D11Texture2DResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*****************************************************************************************
			* Resource typedefs
			******************************************************************************************/
			using D3D11ShaderViewResource = ResMan::TypedManagedResource<ID3D11ShaderResourceView>;

			/*****************************************************************************************
			* Roles
			******************************************************************************************/
			struct ResTypeDescr_ShaderView_Roles : public ResTypeDescr_Roles_Base
			{
			};

			class ResTypeDescr_ShaderView :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_ShaderView_Roles, ID3D11ShaderResourceView>
			{
			public:
				ResTypeDescr_ShaderView();

			private:
			};

			/*****************************************************************************************
			* Typed resource type
			******************************************************************************************/
			class D3D11ShaderViewResourceType : public ResMan::TypedResourceType<ResTypeDescr_ShaderView>
			{
			public:
			};

			/************************************************************************************************
			* D3D11ShaderViewLoader
			*************************************************************************************************/
			class D3D11ShaderViewLoader : public TypedD3D11ResourceLoaderBase<D3D11ShaderViewResourceType>
			{
			public:
				/*********************************************************************************************
				* Construction
				*
				* Arguments:
				* 1. Pointer to resource (cannot be nullptr), for which the view is to be created;
				* 2. Pointer to the shader resource view desc structure (pInNullOrDesc),
				* that may be nullptr (if nullptr to be passed to the corresponding argument
				* in the d3d11 CreateShaderResourceView call);
				* Will be copied and owned.
				*********************************************************************************************/
				D3D11ShaderViewLoader
				(
					ResMan::IManagedResource                 *pInResource,
					const D3D11_SHADER_RESOURCE_VIEW_DESC    *pInNullOrDesc,
					ID3D11Device                             *pInDev,
					ResMan::ISubresourceRole                 *pInRole,
					D3D11ShaderViewResourceType              *pInTypedResourceType
				);

				virtual void LoadDirectDepdendencyListOf(ResMan::SubresourceRoleSet InRoleSet) override;
				virtual RealResourceType *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;


			private:
				ResMan::SubresourceRoleIndex            _GetDependencyListRoleIndex() const;
				void                                    _ResetDependencyLists();
				ResMan::IManagedResource                *_pResource = nullptr;
				bool                                    _bPassNullptrToDesc;
				D3D11_SHADER_RESOURCE_VIEW_DESC         _desc;
			};
		}
	}
}