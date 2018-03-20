#pragma once

#include "ResManD3D11_Deployer_ResourceTypes.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			class ResManD3D11_ResourceTypes_BasicDeployer : public ResManD3D11_ResourceTypes_IDeployer
			{
			public:

			protected:
				virtual void                                       RegisterTestResource(ResMan::IResourceManagerSubsystem *pInSubsystem, TestResourceType *pOutType) override;
				virtual void                                       RegisterText(ResMan::IResourceManagerSubsystem *pInSubsystem, TextResourceType *pOutType) override;
				virtual void                                       RegisterBlendState(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11BlendStateResourceType *pOutType) override;
				virtual void                                       RegisterBuffer(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11BufferResourceType *pOutType) override;
				virtual void                                       RegisterDepthStencilState(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11DepthStencilStateResourceType *pOutType) override;
				virtual void                                       RegisterDepthStencilView(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11DepthStencilViewResourceType *pOutType) override;
				virtual void                                       RegisterInputLayout(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11InputLayoutResourceType *pOutType) override;
				virtual void                                       RegisterSamplerState(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11SamplerStateResourceType *pOutType) override;
				virtual void                                       RegisterVertexShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11VertexShader> *pOutType) override;
				virtual void                                       RegisterPixelShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11PixelShader> *pOutType) override;
				virtual void                                       RegisterComputeShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11ComputeShader> *pOutType) override;
				virtual void                                       RegisterGeometryShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11GeometryShader> *pOutType) override;
				virtual void                                       RegisterHullShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11HullShader> *pOutType) override;
				virtual void                                       RegisterDomainShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11DomainShader> *pOutType) override;
				virtual void                                       RegisterTexture2D(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11Texture2DResourceType *pOutType) override;
				virtual void                                       RegisterRasterizerState(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11RasterizerStateResourceType *pOutType) override;
				virtual void                                       RegisterShaderView(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderViewResourceType *pOutType) override;
				virtual void                                       RegisterUnorderedAccessView(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11UnorderedAccessViewResourceType *pOutType) override;
				virtual void                                       RegisterRenderTargetView(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11RenderTargetViewResourceType *pOutType) override;
				virtual void                                       RegisterCommandList(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11CommandListResourceType *pOutType) override;


			private:
			};
		}
	}
} // Dv