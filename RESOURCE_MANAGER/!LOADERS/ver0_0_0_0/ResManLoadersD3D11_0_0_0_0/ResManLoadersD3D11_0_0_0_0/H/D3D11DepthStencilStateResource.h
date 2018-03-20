#pragma once

#include "ResManLoadersD3D11_System.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/******************************************************************************************
			* Typed resource
			******************************************************************************************/
			using D3D11DepthStencilStateResource = ResMan::TypedManagedResource<ID3D11DepthStencilState>;

			/******************************************************************************************
			* Depth stencil state set of subresource role types
			******************************************************************************************/
			struct ResTypeDescr_DepthStencilState_Roles : public ResTypeDescr_Roles_Base
			{
			};

			class ResTypeDescr_DepthStencilState :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_DepthStencilState_Roles, ID3D11DepthStencilState>
			{
			public:
				ResTypeDescr_DepthStencilState();

			private:
			};

			/*****************************************************************************************
			* Typed resource type
			******************************************************************************************/
			class D3D11DepthStencilStateResourceType : public ResMan::TypedResourceType<ResTypeDescr_DepthStencilState>
			{
			public:
			};

			/*******************************************************************************************
			* Loads the given depth-stencil state according to the given depth-stencil descriptor
			*******************************************************************************************/
			class D3D11DepthStencilStateLoader : public TypedD3D11ResourceLoaderBase<D3D11DepthStencilStateResourceType>
			{
			public:
				D3D11DepthStencilStateLoader
				(
					const D3D11_DEPTH_STENCIL_DESC               &InDesc,
					ID3D11Device                                 *pInDev,
					ResMan::ISubresourceRole                     *pInRole,
					D3D11DepthStencilStateResourceType           *pInTypedResourceType
				);

				virtual RealResourceType                         *Typed_CreateResourceAndGet             (RealResourceType *pOldPtrOrNull) override;


			private:
				D3D11_DEPTH_STENCIL_DESC                         _desc;
			};
		}
	}
}