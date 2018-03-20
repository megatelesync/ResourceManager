#pragma once

#include "../H/D3D11BlendStateResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/**************************************************************************************
			* D3D11BlendStateLoader
			**************************************************************************************/
			D3D11BlendStateLoader::D3D11BlendStateLoader
			(
				const D3D11_BLEND_DESC                 &InDesc,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11BlendStateResourceType            *pInResourceType
			) : TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInResourceType)
			{
				assert(&InDesc != nullptr);
				_desc = InDesc;
			}

			D3D11BlendStateLoader::RealResourceType *D3D11BlendStateLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (pOldPtrOrNull != nullptr)
				{
					pOldPtrOrNull->AddRef();
					return pOldPtrOrNull;
				}
				else
				{
					ID3D11BlendState *pResBlendState = nullptr;
					HRESULT hr = _GetDev()->CreateBlendState(&_desc, &pResBlendState);
					_SetHResult_Create(hr, "ID3D11DeviceContext::CreateBlendState failed");
					if (FAILED(hr))
					{
						return nullptr;
					}
					else
					{
						return pResBlendState;
					}
				}
			}

			/**************************************************************************************
			* ResTypeDescr_BlendState
			**************************************************************************************/
			ResTypeDescr_BlendState::ResTypeDescr_BlendState() :
				TypedD3D11ResTypeDescrBase("D3D11BlendState", "BlendState")
			{
			}
		}
	}
} // Dv