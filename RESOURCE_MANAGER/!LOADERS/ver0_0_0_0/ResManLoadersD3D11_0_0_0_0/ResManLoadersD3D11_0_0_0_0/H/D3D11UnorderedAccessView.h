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
			using D3D11UnorederedAccessViewResource = ResMan::TypedManagedResource<ID3D11UnorderedAccessView>;

			/*****************************************************************************************
			* Roles
			******************************************************************************************/
			struct ResTypeDescr_UnorderedAccessView_Roles : public ResTypeDescr_Roles_Base
			{
			};

			class ResTypeDescr_UnorderedAccessView :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_UnorderedAccessView_Roles, ID3D11UnorderedAccessView>
			{
			public:
				ResTypeDescr_UnorderedAccessView();

			private:
			};

			/*****************************************************************************************
			* Typed resource type
			******************************************************************************************/
			class D3D11UnorderedAccessViewResourceType : public ResMan::TypedResourceType<ResTypeDescr_UnorderedAccessView>
			{
			public:
			};

			/************************************************************************************************
			* D3D11ShaderViewLoader
			*************************************************************************************************/
			class D3D11UnorderedAccessViewLoader : public TypedD3D11ResourceLoaderBase<D3D11UnorderedAccessViewResourceType>
			{
			public:
				/*********************************************************************************************
				* Construction
				*
				* Arguments:
				* 1. Pointer to texture resource (cannot be nullptr), for which the view is to be created;
				* 2. Pointer to the depth stencil desc structure (pInNullOrDesc),
				* that may be nullptr (if nullptr to be passed to the corresponding argument
				* in the d3d11 CreateDepthStencilView call);
				* Will be copied and owned.
				*********************************************************************************************/
				D3D11UnorderedAccessViewLoader
				(
					D3D11Texture2DResource                     InResource,
					const D3D11_UNORDERED_ACCESS_VIEW_DESC    *pInNullOrDesc,
					ID3D11Device                              *pInDev,
					ResMan::ISubresourceRole                  *pInRole,
					D3D11UnorderedAccessViewResourceType      *pInTypedResourceType
				);

				virtual RealResourceType *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;


			private:
				void                                    _ResetDependencyLists();

				D3D11Texture2DResource                  _resource;
				bool                                    _bPassNullptrToDesc;
				D3D11_UNORDERED_ACCESS_VIEW_DESC        _desc;
			};
		}
	}
}