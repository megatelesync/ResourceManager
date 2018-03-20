#pragma once

#include "../H/D3D11ShaderPairResource.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*********************************************************************************************
			* D3D11ShaderPair implementation
			*********************************************************************************************/
			D3D11ShaderPair::~D3D11ShaderPair()
			{
				if (_pVertex != nullptr)
				{
					_pVertex->Release();
				}
				if (_pPixel != nullptr)
				{
					_pPixel->Release();
				}
			}
			D3D11ShaderPair::D3D11ShaderPair(ID3D11VertexShader *pInVertex, ID3D11PixelShader *pInPixel)
			{
				assert(pInVertex != nullptr);
				assert(pInPixel != nullptr);
				_pVertex = pInVertex;
				_pPixel = pInPixel;
			}

			/*********************************************************************************
			* ResTypeDescr_ShaderPair implementation
			*********************************************************************************/
			ResTypeDescr_ShaderPair::ResTypeDescr_ShaderPair() :
				TypedD3D11ResTypeDescrBase("D3D11ShaderPair", "ShaderPair")
			{
			}

			void ResTypeDescr_ShaderPair::RegisterApplicableRoles(ResMan::IResourceManagerSubsystem *pInSubsys)
			{
				_roles.pTempBlob = ResMan::RegisterSubresourceRole_ThatMustBeLoaded(pInSubsys, _params.GetSubresourceRolesPtr(), "TempBlob");
				_roles.pTempSourceCode = ResMan::RegisterSubresourceRole(pInSubsys, _params.GetSubresourceRolesPtr(), "TempSourceCode");
			}

			void ResTypeDescr_ShaderPair::Delete(void *pInResource)
			{
				delete pInResource;
			}
		}
	}
} // Dv