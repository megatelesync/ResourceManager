#pragma once

#include "../H/D3D11DepthStencilStateResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/**************************************************************************************
			* D3D11DepthStencilStateLoader
			**************************************************************************************/
			D3D11DepthStencilStateLoader::D3D11DepthStencilStateLoader
			(
				const D3D11_DEPTH_STENCIL_DESC                        &InDesc,
				ID3D11Device                                          *pInDev,
				ResMan::ISubresourceRole                              *pInRole,
				D3D11DepthStencilStateResourceType                    *pInResourceType
			) : TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInResourceType)
			{
				assert(&InDesc != nullptr);
				_desc = InDesc;
			}

			D3D11DepthStencilStateLoader::RealResourceType *D3D11DepthStencilStateLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (pOldPtrOrNull != nullptr)
				{
					pOldPtrOrNull->AddRef();
					return pOldPtrOrNull;
				}
				else
				{
					ID3D11DepthStencilState *pResultDepthStencilState = nullptr;
					HRESULT hr = _GetDev()->CreateDepthStencilState(&_desc, &pResultDepthStencilState);
					_SetHResult_Create(hr, "ID3D11DeviceContext::CreateDepthStencilState failed");
					if (FAILED(hr))
					{
						return nullptr;
					}
					else
					{
						return pResultDepthStencilState;
					}
				}
			}

			/**************************************************************************************
			* ResTypeDescr_BlendState
			**************************************************************************************/
			ResTypeDescr_DepthStencilState::ResTypeDescr_DepthStencilState() :
				TypedD3D11ResTypeDescrBase("D3D11DepthStencilState", "DepthStencilState")
			{
			}
		}
	}
} // Dv