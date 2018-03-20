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
			using D3D11SamplerStateResource = ResMan::TypedManagedResource<ID3D11SamplerState>;

			/******************************************************************************************
			* Roles
			******************************************************************************************/
			struct ResTypeDescr_SamplerState_Roles : public ResTypeDescr_Roles_Base
			{
			};

			class ResTypeDescr_SamplerState :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_SamplerState_Roles, ID3D11SamplerState>
			{
			public:
				ResTypeDescr_SamplerState();

			private:
			};

			/*****************************************************************************************
			* Typed resource type
			******************************************************************************************/
			class D3D11SamplerStateResourceType : public ResMan::TypedResourceType<ResTypeDescr_SamplerState>
			{
			public:
			};

			class D3D11SamplerStateLoader : public TypedD3D11ResourceLoaderBase<D3D11SamplerStateResourceType>
			{
			public:
				D3D11SamplerStateLoader
				(
					const D3D11_SAMPLER_DESC                 &InDesc,
					ID3D11Device                             *pInDev,
					ResMan::ISubresourceRole                 *pInRole,
					D3D11SamplerStateResourceType            *pInResourceType
				);

				virtual RealResourceType *Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull) override;

			private:
				D3D11_SAMPLER_DESC _desc;
			};
		}
	}
}