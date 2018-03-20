#pragma once

#include "ResManLoadersD3D11_System.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*****************************************************************************************
			* Resource typedefs
			******************************************************************************************/
			using D3D11BlendStateResource = ResMan::TypedManagedResource<ID3D11BlendState>;

			/*****************************************************************************************
			* Roles
			******************************************************************************************/
			struct ResTypeDescr_BlendState_Roles : public ResTypeDescr_Roles_Base
			{
			};

			class ResTypeDescr_BlendState :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_BlendState_Roles, ID3D11BlendState>
			{
			public:
				ResTypeDescr_BlendState();

			private:
			};

			/*****************************************************************************************
			* Typed resource type
			******************************************************************************************/
			class D3D11BlendStateResourceType : public ResMan::TypedResourceType<ResTypeDescr_BlendState>
			{
			public:
			};

			class D3D11BlendStateLoader : public TypedD3D11ResourceLoaderBase<D3D11BlendStateResourceType>
			{
			public:
				D3D11BlendStateLoader
				(
					const D3D11_BLEND_DESC                 &InDesc,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					D3D11BlendStateResourceType            *pInResourceType
				);

				virtual RealResourceType *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;

			private:
				D3D11_BLEND_DESC _desc;
			};
		}
	}
}