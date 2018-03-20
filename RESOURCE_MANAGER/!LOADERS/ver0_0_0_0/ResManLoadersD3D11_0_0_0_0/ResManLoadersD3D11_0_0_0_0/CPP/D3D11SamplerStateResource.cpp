#pragma once

#include "../H/D3D11SamplerStateResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*********************************************************************************
			* D3D11SamplerStateLoader implementation
			*********************************************************************************/			
			D3D11SamplerStateLoader::D3D11SamplerStateLoader
			(
				const D3D11_SAMPLER_DESC                &InDesc,
				ID3D11Device                            *pInDev,
				ResMan::ISubresourceRole                *pInRole,
				D3D11SamplerStateResourceType           *pInResourceType
			) : 
				_desc(InDesc),
				TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInResourceType)
			{
				
			}

			typename D3D11SamplerStateLoader::RealResourceType *D3D11SamplerStateLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (pOldPtrOrNull != nullptr)
				{
					// We have a working object already, so we just use it;
					// Because the old object will be unloaded, we increment the ref counter
					// to prevent it from being destroyed.
					pOldPtrOrNull->AddRef();
					return pOldPtrOrNull;
				}
				{
					ID3D11SamplerState *pResSamplerState = nullptr;
					HRESULT hr_create = _GetDev()->CreateSamplerState(&_desc, &pResSamplerState);
					_SetHResult_Create(hr_create, "ID3D11DeviceContext::CreateSamplerState failed.");
					if (FAILED(_GetHResult_Create()))
					{
						return nullptr;
					}
					return pResSamplerState;
				}
			}

			/*********************************************************************************
			* ResTypeDescr_SamplerState implementation
			*********************************************************************************/
			ResTypeDescr_SamplerState::ResTypeDescr_SamplerState() :
				TypedD3D11ResTypeDescrBase("ID3D11SamplerState", "SamplerState")
			{
			}
		}
	}
} // Dv