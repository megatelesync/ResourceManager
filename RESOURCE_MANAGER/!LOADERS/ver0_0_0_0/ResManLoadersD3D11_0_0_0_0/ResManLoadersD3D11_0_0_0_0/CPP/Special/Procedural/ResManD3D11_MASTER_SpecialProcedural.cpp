#include "../../../H/Special/Procedural/ResManD3D11_MASTER_SpecialProcedural.h"

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
			)
			{
				auto pLoader = new D3D11LinearPerlinNoiseLoader
				(
					*pInNoiseTextureParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetTexture2D()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11Texture2DResource(pResource);
			}
			D3D11Texture2DResource RegisterResource_Texture2D_LinearPerlinNoise
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const LinearPerlinNoiseParams             *pInNoiseTextureParams
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetTexture2D());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_Texture2D_LinearPerlinNoise(pInResMan, &resourceParams, pInNoiseTextureParams);
				pInGroup->AttachResource(pResource.GetManagedResource());
				return pResource;
			}
			D3D11Texture2DResource RegisterResource_Texture2D_LinearPerlinNoise
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const LinearPerlinNoiseParams             *pInNoiseTextureParams
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetTexture2D());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_Texture2D_LinearPerlinNoise(pInResMan, &resourceParams, pInNoiseTextureParams);
				pInGroup->AttachResource(pResource.GetManagedResource());
				return pResource;
			}
		}
	}
}