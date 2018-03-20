#pragma once

#include "../H/D3D11InputLayoutResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*********************************************************************************
			* D3D11InputLayoutLoader implementation
			*********************************************************************************/			
			D3D11InputLayoutLoader::D3D11InputLayoutLoader
			(
				const D3D11_INPUT_ELEMENT_DESC         *pInLayoutDesc,
				UINT                                    InNumDescElems,
				const void                             *pInShaderBytecode,
				SIZE_T                                  InByteCodeLength,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				D3D11InputLayoutResourceType           *pInResourceType
			) : TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInResourceType)
			{
				_shaderBytecodeLength = InByteCodeLength;
				_pLayoutDesc.reset(new D3D11_INPUT_ELEMENT_DESC[InNumDescElems]);
				_numDescElems = InNumDescElems;
				std::copy(pInLayoutDesc, pInLayoutDesc + InNumDescElems, _pLayoutDesc.get());
				_pShaderBytecode.reset(new BYTE[_shaderBytecodeLength]);
				memcpy(_pShaderBytecode.get(), pInShaderBytecode, _shaderBytecodeLength);
			}

			typename D3D11InputLayoutLoader::RealResourceType *D3D11InputLayoutLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
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
					ID3D11InputLayout *pResInputLayout;
					HRESULT hr_create = _GetDev()->CreateInputLayout(_pLayoutDesc.get(), _numDescElems, _pShaderBytecode.get(), _shaderBytecodeLength, &pResInputLayout);
					_SetHResult_Create(hr_create, "ID3D11DeviceContext::CreateInputLayout failed");
					if (FAILED(_GetHResult_Create()))
					{
						return nullptr;
					}
					return pResInputLayout;
				}
			}

			/*********************************************************************************
			* ResTypeDescr_InputLayout implementation
			*********************************************************************************/
			ResTypeDescr_InputLayout::ResTypeDescr_InputLayout() :
				TypedD3D11ResTypeDescrBase("ID3D11InputLayout", "InputLayout")
			{
			}
		}
	}
} // Dv