#pragma once

#include "../H/D3D11CommandListResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*********************************************************************************
			* D3D11CommandListLoader implementation
			*********************************************************************************/
			D3D11CommandListLoader::D3D11CommandListLoader
			(
				ID3D11Device                            *pInDev,
				ID3D11CommandList                       *pInTemplCommandList,
				ResMan::ISubresourceRole                *pInRole,
				D3D11CommandListResourceType            *pInResourceType
			) :
				TypedD3D11ResourceLoaderBase(pInDev, pInRole, pInResourceType),
				_pTemplCommandList(pInTemplCommandList)
			{

			}

			typename D3D11CommandListLoader::RealResourceType *D3D11CommandListLoader::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				// COMMENT: We cannot just return existing command list, 
				// because when we release created main list, template command list will be released also - it's wrong!!!.
				// That's why we must call AddRef:
				_pTemplCommandList->AddRef();
				return _pTemplCommandList.get();
			}

			/*********************************************************************************
			* ResTypeDescr_CommandList implementation
			*********************************************************************************/
			ResTypeDescr_CommandList::ResTypeDescr_CommandList() :
				TypedD3D11ResTypeDescrBase("ID3D11CommandList", "CommandList")
			{
			}
		}
	}
} // Dv