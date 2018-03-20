#pragma once

#include "../../ResManD3D11/ResManD3D11.h"
#include "PerlinNoise.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*********************************************************************************************************************
			* Perlin noise texture registration
			*********************************************************************************************************************/
			D3D11Texture2DResource RegisterResource_Texture2D_LinearPerlinNoise
			(				
				ResManD3D11_Main                          *pInResMan,
				ResMan::PublicResourceParams              *pInParams,
				const LinearPerlinNoiseParams             *pInNoiseTextureParams
			);
			D3D11Texture2DResource RegisterResource_Texture2D_LinearPerlinNoise
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const LinearPerlinNoiseParams             *pInNoiseTextureParams
			);
			D3D11Texture2DResource RegisterResource_Texture2D_LinearPerlinNoise
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const LinearPerlinNoiseParams             *pInNoiseTextureParams
			);
		}
	}
}