#pragma once

//#include "..\..\..\..\H\ResManD3D11\ResourceTypes\Deployer\ResManD3D11_BasicDeployer_ResourceTypes.h"
#include "ResManD3D11\ResourceTypes\Deployer\ResManD3D11_BasicDeployer_ResourceTypes.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterTestResource(ResMan::IResourceManagerSubsystem *pInSubsystem, TestResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterText(ResMan::IResourceManagerSubsystem *pInSubsystem, TextResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterBlendState(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11BlendStateResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterBuffer(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11BufferResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterDepthStencilState(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11DepthStencilStateResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterDepthStencilView(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11DepthStencilViewResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterInputLayout(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11InputLayoutResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterSamplerState(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11SamplerStateResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterVertexShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11VertexShader> *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterPixelShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11PixelShader> *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterComputeShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11ComputeShader> *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterGeometryShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11GeometryShader> *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterHullShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11HullShader> *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterDomainShader(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11DomainShader> *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterTexture2D(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11Texture2DResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterRasterizerState(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11RasterizerStateResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}

			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterShaderView(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderViewResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterUnorderedAccessView(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11UnorderedAccessViewResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterRenderTargetView(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11RenderTargetViewResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
			void ResManD3D11_ResourceTypes_BasicDeployer::RegisterCommandList(ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11CommandListResourceType *pOutType)
			{
				ResMan::RegisterType(pInSubsystem, pOutType);
			}
		}
	}
} // Dv