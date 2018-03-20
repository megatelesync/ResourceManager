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
			using D3D11RenderTargetViewResource = ResMan::TypedManagedResource<ID3D11RenderTargetView>;

			/*****************************************************************************************
			* Roles
			******************************************************************************************/
			struct ResTypeDescr_RenderTargetView_Roles : public ResTypeDescr_Roles_Base
			{
			};

			class ResTypeDescr_RenderTargetView :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_RenderTargetView_Roles, ID3D11RenderTargetView>
			{
			public:
				ResTypeDescr_RenderTargetView();

			private:
			};

			/*****************************************************************************************
			* Typed resource type
			******************************************************************************************/
			class D3D11RenderTargetViewResourceType : public ResMan::TypedResourceType<ResTypeDescr_RenderTargetView>
			{
			public:
			};

			/************************************************************************************************
			* D3D11RenderTargetViewLoader:
			*
			* Questions:
			* 1. How to specify the resource to be used as the surface?
			* 1.1. It must be pointer to the manageable resource instance
			*************************************************************************************************/
			class D3D11RenderTargetViewLoader : public TypedD3D11ResourceLoaderBase<D3D11RenderTargetViewResourceType>
			{
			public:
				/*********************************************************************************************
				* Construction
				*
				* Arguments:
				* 1. Pointer to texture resource (cannot be nullptr), for which the view is to be created;
				* 2. Pointer to the render target view desc structure (pInNullOrDesc),
				* that may be nullptr (if nullptr to be passed to the corresponding argument
				* in the d3d11 CreateRenderTargetView call);
				* Will be copied and owned.
				*********************************************************************************************/
				D3D11RenderTargetViewLoader
				(
					D3D11Texture2DResource                  InResource,
					const D3D11_RENDER_TARGET_VIEW_DESC    *pInNullOrDesc,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11RenderTargetViewResourceType      *pInTypedResourceType
				);


				virtual RealResourceType *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;


			private:
				void                                    _ResetDependencyLists();

				D3D11Texture2DResource                  _resource;
				bool                                    _bPassNullptrToDesc;
				D3D11_RENDER_TARGET_VIEW_DESC           _desc;
			};
		}
	}
}