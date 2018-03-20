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
			using D3D11RasterizerStateResource = ResMan::TypedManagedResource<ID3D11RasterizerState>;

			/******************************************************************************************
			* RasterizerState set of subresource role types
			******************************************************************************************/
			struct ResTypeDescr_RasterizerState_Roles : public ResTypeDescr_Roles_Base
			{
			};

			class ResTypeDescr_RasterizerState : 
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_RasterizerState_Roles, ID3D11RasterizerState>
			{ 
			public:
				ResTypeDescr_RasterizerState();

			private:
			};

			/*****************************************************************************************
			* Typed resource type
			******************************************************************************************/
			class D3D11RasterizerStateResourceType : public ResMan::TypedResourceType<ResTypeDescr_RasterizerState>
			{
			public:
				~D3D11RasterizerStateResourceType();
			};

			/******************************************************************************************
			* Loads the given rasterizer state for the given description structure
			******************************************************************************************/
			class D3D11RasterizerStateLoader : public TypedD3D11ResourceLoaderBase<D3D11RasterizerStateResourceType>
			{
			public:
				/***************************************************************************************
				* CONSTRUCTION:
				***************************************************************************************/
				D3D11RasterizerStateLoader
				(
					const D3D11_RASTERIZER_DESC                *pInDesc,
					ID3D11Device                               *pInDev,
					ResMan::ISubresourceRole                   *pInRole,
					D3D11RasterizerStateResourceType           *pInTypedResourceType
				);
				

				virtual RealResourceType *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;
				

			private:
				D3D11_RASTERIZER_DESC                                   _desc;
			};
		}
	}
} // Dv