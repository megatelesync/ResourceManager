#pragma once

#include "../../ResourceTypes/ResManD3D11_ResourceTypes.h"
#include "Basic/MathUtil_Basic_IntSize.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			class ResManD3D11_Group_BasicResources;
			class ResManD3D11_Main;

			class IDeployer_Group_BasicResources
			{
			public:
				/************************************************************************************************************************
				* Depth stencil loaders
				************************************************************************************************************************/
				ResMan::GroupParams                        GetGroupParams                                          () const;
				ResMan::GroupParams                        GetDefault_GroupParams                                  () const;
				ResMan::CombinableParams                   GetDefault_GroupCombinableParams                        () const;

				D3D11Texture2DResource                     RegisterDepthStencilTexture                             (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain);
				D3D11DepthStencilViewResource              RegisterDepthStencilView                                (const DvUtils::Math::IntSize& InWindowSize, D3D11Texture2DResource pInTexture, ResManD3D11_Main *pInOwnerMain);
				D3D11RasterizerStateResource               RegisterRasterizerState                                 (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain);

				D3D11BlendStateResource                    RegisterBlendState_Disabled                             (ResManD3D11_Main *pInOwnerMain);
				D3D11BlendStateResource                    RegisterBlendState_TransparentAdditive                  (ResManD3D11_Main *pInOwnerMain);

				void                                       UpdateResources                                         (const DvUtils::Math::IntSize& InNewWindowSize, ResManD3D11_Main *pInOwnerMain, ResManD3D11_Group_BasicResources *pOutResources);

				std::shared_ptr<ResMan::IResourceLoader>   CreateLoader_DepthStencilTexture                        (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain);
				std::shared_ptr<ResMan::IResourceLoader>   CreateLoader_DepthStencilView                           (D3D11Texture2DResource pInTexture, ResManD3D11_Main *pInOwnerMain);

			protected:
				virtual ResMan::GroupParams                Virt_GetGroupParams                                     () const { return Virt_GetDefault_GroupParams(); }
				virtual ResMan::GroupParams                Virt_GetDefault_GroupParams                             () const =0;
				virtual ResMan::CombinableParams           Virt_GetDefault_GroupCombinableParams                   () const =0;

				virtual void                               Virt_UpdateResources                                    (const DvUtils::Math::IntSize& InNewWindowSize, ResManD3D11_Main *pInOwnerMain, ResManD3D11_Group_BasicResources *pOutResources) =0;

				virtual D3D11Texture2DResource             Virt_RegisterDepthStencilTexture                        (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain) =0;
				virtual D3D11DepthStencilViewResource      Virt_RegisterDepthStencilView                           (const DvUtils::Math::IntSize& InWindowSize, D3D11Texture2DResource pInTexture, ResManD3D11_Main *pInOwnerMain) =0;
				virtual D3D11RasterizerStateResource       Virt_RegisterRasterizerState                            (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain) =0;

				virtual D3D11BlendStateResource            Virt_RegisterBlendState_Disabled                        (ResManD3D11_Main *pInOwnerMain) =0;
				virtual D3D11BlendStateResource            Virt_RegisterBlendState_TransparentAdditive             (ResManD3D11_Main *pInOwnerMain) =0;

				virtual std::shared_ptr<ResMan::IResourceLoader>   Virt_CreateLoader_DepthStencilTexture           (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain) =0;
				virtual std::shared_ptr<ResMan::IResourceLoader>  Virt_CreateLoader_DepthStencilView               (D3D11Texture2DResource pInTexture, ResManD3D11_Main *pInOwnerMain) =0;

				virtual GeneratedTexture2DParams           Virt_GetTextureParams                                   (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain) =0;

			private:
			};

			/************************************************************************************************************************
			* IMPLEMENTATION
			************************************************************************************************************************/
			inline ResMan::GroupParams IDeployer_Group_BasicResources::GetGroupParams() const
			{
				assert(this);
				return Virt_GetGroupParams();
			}
			inline ResMan::GroupParams IDeployer_Group_BasicResources::GetDefault_GroupParams() const
			{
				assert(this);
				return Virt_GetDefault_GroupParams();
			}
			inline ResMan::CombinableParams IDeployer_Group_BasicResources::GetDefault_GroupCombinableParams() const
			{
				assert(this);
				return Virt_GetDefault_GroupCombinableParams();
			}

			inline void IDeployer_Group_BasicResources::UpdateResources(const DvUtils::Math::IntSize& InNewWindowSize, ResManD3D11_Main *pInOwnerMain, ResManD3D11_Group_BasicResources *pOutResources)
			{
				assert(this);
				Virt_UpdateResources(InNewWindowSize, pInOwnerMain, pOutResources);
			}

			inline D3D11BlendStateResource IDeployer_Group_BasicResources::RegisterBlendState_Disabled(ResManD3D11_Main *pInOwnerMain)
			{
				assert(this);
				return Virt_RegisterBlendState_Disabled(pInOwnerMain);
			}

			inline D3D11BlendStateResource IDeployer_Group_BasicResources::RegisterBlendState_TransparentAdditive(ResManD3D11_Main *pInOwnerMain)
			{
				assert(this);
				return Virt_RegisterBlendState_TransparentAdditive(pInOwnerMain);
			}

			inline D3D11Texture2DResource IDeployer_Group_BasicResources::RegisterDepthStencilTexture(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain)
			{
				assert(this);
				assert(&InWindowSize);
				assert(pInOwnerMain);
				return Virt_RegisterDepthStencilTexture(InWindowSize, pInOwnerMain);
			}
			inline D3D11DepthStencilViewResource IDeployer_Group_BasicResources::RegisterDepthStencilView(const DvUtils::Math::IntSize& InWindowSize, D3D11Texture2DResource pInTexture, ResManD3D11_Main *pInOwnerMain)
			{
				assert(this);
				assert(&InWindowSize);
				assert(pInOwnerMain);
				return Virt_RegisterDepthStencilView(InWindowSize, pInTexture, pInOwnerMain);
			}

			inline D3D11RasterizerStateResource IDeployer_Group_BasicResources::RegisterRasterizerState(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain)
			{
				assert(this);
				assert(&InWindowSize);
				assert(pInOwnerMain);
				return Virt_RegisterRasterizerState(InWindowSize, pInOwnerMain);
			}

			inline std::shared_ptr<ResMan::IResourceLoader> IDeployer_Group_BasicResources::CreateLoader_DepthStencilTexture(const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain)
			{
				assert(this);
				return CreateLoader_DepthStencilTexture(InWindowSize, pInOwnerMain);
			}
			inline std::shared_ptr<ResMan::IResourceLoader> IDeployer_Group_BasicResources::CreateLoader_DepthStencilView(D3D11Texture2DResource pInTexture, ResManD3D11_Main *pInOwnerMain)
			{
				assert(this);
				return CreateLoader_DepthStencilView(pInTexture, pInOwnerMain);
			}
		}
	}
} // Dv