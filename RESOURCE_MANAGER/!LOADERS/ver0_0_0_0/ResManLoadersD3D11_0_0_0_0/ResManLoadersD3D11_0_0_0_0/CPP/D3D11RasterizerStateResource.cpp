#include "../H/D3D11RasterizerStateResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*********************************************************************************
			* D3D11InputLayoutLoader implementation
			*********************************************************************************/
			D3D11RasterizerStateLoader::D3D11RasterizerStateLoader
			(
				const D3D11_RASTERIZER_DESC                *pInDesc,
				ID3D11Device                               *pInDev,
				ResMan::ISubresourceRole                   *pInRole,
				D3D11RasterizerStateResourceType           *pInTypedResourceType
			) : TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInTypedResourceType),
				_desc(*pInDesc)
			{
				
			}

			D3D11RasterizerStateResourceType::~D3D11RasterizerStateResourceType()
			{
			}

			typename D3D11RasterizerStateLoader::RealResourceType *D3D11RasterizerStateLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (pOldPtrOrNull != nullptr)
				{
					// We have a working object already, so we just use it;
					// Because the old object will be unloaded, we increment the ref counter
					// to prevent it from being destroyed.
					pOldPtrOrNull->AddRef();
					return pOldPtrOrNull;
				}
				else
				{
					ID3D11RasterizerState *pRasterizerState = nullptr;
					HRESULT hr_create = _GetDev()->CreateRasterizerState(&_desc, &pRasterizerState);
					_SetHResult_Create(hr_create, "ID3D11DeviceCOntext::CreateRasterizerState");
					if (FAILED(_GetHResult_Create()))
					{
						return nullptr;
					}
					return pRasterizerState;
				}
			}

			/*********************************************************************************
			* ResTypeDescr_InputLayout implementation
			*********************************************************************************/
			ResTypeDescr_RasterizerState::ResTypeDescr_RasterizerState() :
				TypedD3D11ResTypeDescrBase("ID3D11RasterizerState", "RasterizerState")
			{
			}
		}
	}
}