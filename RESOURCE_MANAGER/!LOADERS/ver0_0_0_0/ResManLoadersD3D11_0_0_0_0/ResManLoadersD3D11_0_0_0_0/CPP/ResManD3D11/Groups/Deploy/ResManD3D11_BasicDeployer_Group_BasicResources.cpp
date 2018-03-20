#include "../../../../H/ResManD3D11/Groups/Deploy/ResManD3D11_BasicDeployer_Group_BasicResources.h"
#include "../../../../H/ResManD3D11/ResManD3D11.h"


namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			ResMan::GroupParams BasicDeployer_Group_BasicResources::Virt_GetDefault_GroupParams() const
			{
				ResMan::GroupParams groupParams;
				groupParams.SetName("D3D11_BasicResources");
				*groupParams.GetCombinablePtr() = Virt_GetDefault_GroupCombinableParams();
				return groupParams;
			}

			ResMan::CombinableParams BasicDeployer_Group_BasicResources::Virt_GetDefault_GroupCombinableParams() const
			{
				ResMan::CombinableParams result_combinable_params;
				result_combinable_params.SetIsActive(true);
				result_combinable_params.SetManualLoadingFreezed(false);
				result_combinable_params.SetManualUnloadingFreezed(false);
				result_combinable_params.SetAutoLoadingFreezed(true);
				result_combinable_params.SetAutoUnloadingFreezed(true);
				result_combinable_params.GetErrorHandlingParams()->SetFatal(true);
				return result_combinable_params;
			}

			void BasicDeployer_Group_BasicResources::Virt_UpdateResources(const DvUtils::Math::IntSize& InNewWindowSize, ResManD3D11_Main *pInOwnerMain, ResManD3D11_Group_BasicResources *pOutResources)
			{
				auto pLoader_DepthStencilTexture = Virt_CreateLoader_DepthStencilTexture(InNewWindowSize, pInOwnerMain);
				pOutResources->GetDepthStencilTexture()->SetLoader(pLoader_DepthStencilTexture);

				auto pLoader_DepthStencilView = Virt_CreateLoader_DepthStencilView(pOutResources->GetDepthStencilTexture(), pInOwnerMain);
				pOutResources->GetDepthStencilView()->SetLoader(pLoader_DepthStencilView);
			}

			std::shared_ptr<ResMan::IResourceLoader> BasicDeployer_Group_BasicResources::Virt_CreateLoader_DepthStencilTexture(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain)
			{
				auto textureParams = Virt_GetTextureParams(InWindowSize, pInOwnerMain);
				auto pDev = pInOwnerMain->GetDev();
				auto pRole = GetSubresourceRole_DepthStencilView_Std(pInOwnerMain);
				auto pResourceType = pInOwnerMain->GetResourceTypes()->GetTexture2D();
				auto pLoader = new D3D11Texture2DLoader_CreateUninitialized(textureParams, pDev, pRole, pResourceType);
				return std::shared_ptr<ResMan::IResourceLoader>(pLoader);
			}

			std::shared_ptr<ResMan::IResourceLoader> BasicDeployer_Group_BasicResources::Virt_CreateLoader_DepthStencilView(D3D11Texture2DResource pInTexture, ResManD3D11_Main *pInOwnerMain)
			
			{
				auto pDesc = nullptr;
				auto pResourceType = pInOwnerMain->GetResourceTypes()->GetDepthStencilView();
				auto pDev = pInOwnerMain->GetDev();
				auto pDevCon = pInOwnerMain->GetDevCon();
				auto pRole = GetSubresourceRole_DepthStencilView_Std(pInOwnerMain);
				auto pDepthStencilView = new D3D11DepthStencilViewLoader(pInTexture, pDesc, pDev, pRole, pResourceType);
				return std::shared_ptr<ResMan::IResourceLoader>(pDepthStencilView);
			}

			D3D11Texture2DResource BasicDeployer_Group_BasicResources::Virt_RegisterDepthStencilTexture(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain)
			{
				auto pLoader = Virt_CreateLoader_DepthStencilTexture(InWindowSize, pInOwnerMain);
								
				auto pRole = GetSubresourceRole_Texture2D_Std(pInOwnerMain);
				ResMan::PublicResourceParams resourceParams = GetDefault_PublicResourceParams(pRole);
				ResMan::IManagedResource *pResource = pInOwnerMain->GetSubsystem()->GetManager()->RegisterResource(resourceParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11Texture2DResource(pResource);
			}
			D3D11DepthStencilViewResource BasicDeployer_Group_BasicResources::Virt_RegisterDepthStencilView(const DvUtils::Math::IntSize& InWindowSize, D3D11Texture2DResource pInTexture, ResManD3D11_Main *pInOwnerMain)
			{
				auto pLoader = Virt_CreateLoader_DepthStencilView(pInTexture, pInOwnerMain);
				auto pRole = GetSubresourceRole_Texture2D_Std(pInOwnerMain);
				ResMan::PublicResourceParams resourceParams = GetDefault_PublicResourceParams(pRole);
				ResMan::IManagedResource *pResource = pInOwnerMain->GetSubsystem()->GetManager()->RegisterResource(resourceParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11DepthStencilViewResource(pResource);
			}

			D3D11RasterizerStateResource BasicDeployer_Group_BasicResources::Virt_RegisterRasterizerState(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain)
			{
				D3D11_RASTERIZER_DESC desc = Virt_GetRasterizerDesc();
				auto pRole = GetSubresourceRole_RasterizerState_Std(pInOwnerMain);
				ResMan::PublicResourceParams resourceParams = GetDefault_PublicResourceParams(pRole);
				auto resultResource = RegisterResource_RasterizerState(pInOwnerMain, &resourceParams, &desc);
				return resultResource;
			}

			D3D11BlendStateResource BasicDeployer_Group_BasicResources::Virt_RegisterBlendState_Disabled(ResManD3D11_Main *pInOwnerMain)
			{
				auto pRole = GetSubresourceRole_BlendState_Std(pInOwnerMain);
				ResMan::PublicResourceParams resourceParams = GetDefault_PublicResourceParams(pRole);
				auto resultResource = RegisterResource_BlendStateDisabled(pInOwnerMain, &resourceParams);
				return resultResource;
			}
			D3D11BlendStateResource BasicDeployer_Group_BasicResources::Virt_RegisterBlendState_TransparentAdditive(ResManD3D11_Main *pInOwnerMain)
			{
				auto pRole = GetSubresourceRole_BlendState_Std(pInOwnerMain);
				ResMan::PublicResourceParams resourceParams = GetDefault_PublicResourceParams(pRole);
				auto resultResource = RegisterResource_AdditiveBlendState(pInOwnerMain, &resourceParams);
				return resultResource;
			}

			D3D11_RASTERIZER_DESC BasicDeployer_Group_BasicResources::Virt_GetRasterizerDesc()
			{
				D3D11_RASTERIZER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.AntialiasedLineEnable                = TRUE;
				desc.CullMode                             = D3D11_CULL_NONE;
				desc.FillMode                             = D3D11_FILL_SOLID;
				desc.ScissorEnable                        = FALSE;
				desc.MultisampleEnable                    = FALSE;
				desc.DepthClipEnable                      = FALSE;
				desc.FrontCounterClockwise                = FALSE;
				return desc;
			}

			GeneratedTexture2DParams BasicDeployer_Group_BasicResources::Virt_GetTextureParams(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain)
			{
				Texture2DParams res_params (pInOwnerMain->GetTextureFormats()->GetD24_FLOAT_S8_UINT());
				res_params.bGenerateMipMaps = false;
				res_params.bindFlags = D3D11_BIND_DEPTH_STENCIL;
				res_params.usage = D3D11_USAGE_DEFAULT;
				res_params.CPUAccessFlags = 0;
				res_params.miscFlags = 0;
				GeneratedTexture2DParams res_generated_params(pInOwnerMain->GetTextureFormats()->GetD24_FLOAT_S8_UINT());
				res_generated_params.texture = res_params;
				res_generated_params.width = InWindowSize.width;
				res_generated_params.height = InWindowSize.height;
				return res_generated_params;
			}
		}
	}
}