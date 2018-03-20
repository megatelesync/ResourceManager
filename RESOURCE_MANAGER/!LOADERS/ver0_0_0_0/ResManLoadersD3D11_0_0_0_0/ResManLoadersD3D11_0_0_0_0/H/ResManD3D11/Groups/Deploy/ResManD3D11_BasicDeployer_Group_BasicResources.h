#pragma once

#include "ResManD3D11_IDeployer_Group_BasicResources.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			class BasicDeployer_Group_BasicResources :
				public IDeployer_Group_BasicResources
			{
			public:
			protected:				
				virtual ResMan::GroupParams                Virt_GetDefault_GroupParams                                  () const override;
				virtual ResMan::CombinableParams           Virt_GetDefault_GroupCombinableParams                        () const override;

				virtual void                               Virt_UpdateResources                                         (const DvUtils::Math::IntSize& InNewWindowSize, ResManD3D11_Main *pInOwnerMain, ResManD3D11_Group_BasicResources *pOutResources) override;

				std::shared_ptr<ResMan::IResourceLoader>   Virt_CreateLoader_DepthStencilTexture                        (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain) override;
				std::shared_ptr<ResMan::IResourceLoader>   Virt_CreateLoader_DepthStencilView                           (D3D11Texture2DResource pInTexture, ResManD3D11_Main *pInOwnerMain) override;

				virtual D3D11Texture2DResource             Virt_RegisterDepthStencilTexture                             (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain) override;
				virtual D3D11DepthStencilViewResource      Virt_RegisterDepthStencilView                                (const DvUtils::Math::IntSize& InWindowSize, D3D11Texture2DResource pInTexture, ResManD3D11_Main *pInOwnerMain) override;
				virtual D3D11RasterizerStateResource       Virt_RegisterRasterizerState                                 (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain) override;

				virtual D3D11BlendStateResource            Virt_RegisterBlendState_Disabled                             (ResManD3D11_Main *pInOwnerMain) override;
				virtual D3D11BlendStateResource            Virt_RegisterBlendState_TransparentAdditive                  (ResManD3D11_Main *pInOwnerMain) override;

				virtual GeneratedTexture2DParams           Virt_GetTextureParams                                        (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain) override;
				D3D11_RASTERIZER_DESC                      Virt_GetRasterizerDesc                                       ();

			private:
			};
		}
	}
} // Dv