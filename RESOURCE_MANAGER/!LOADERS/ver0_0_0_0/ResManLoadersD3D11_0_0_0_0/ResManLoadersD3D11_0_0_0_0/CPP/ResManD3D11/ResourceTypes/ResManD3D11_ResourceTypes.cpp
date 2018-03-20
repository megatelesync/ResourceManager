#include "../../../H/ResManD3D11/ResourceTypes/ResManD3D11_ResourceTypes.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/********************************************************************************************
			* ResManD3D11_ResourceTypes implementation
			********************************************************************************************/
			ResManD3D11_ResourceTypes::~ResManD3D11_ResourceTypes()
			{
			}

			void ResManD3D11_ResourceTypes::Initialize(ResManD3D11_ResourceTypes_IDeployer *pInDeployer, ResMan::IResourceManagerSubsystem *pInSubsystem)
			{
				pInDeployer->RegisterTestResource(pInSubsystem, &_testResource);
				pInDeployer->RegisterText(pInSubsystem, &_text);
				pInDeployer->RegisterBlendState(pInSubsystem, &_blendState);
				pInDeployer->RegisterBuffer(pInSubsystem, &_buffer);
				pInDeployer->RegisterDepthStencilState(pInSubsystem, &_depthStencilState);
				pInDeployer->RegisterDepthStencilView(pInSubsystem, &_depthStencilView);
				pInDeployer->RegisterInputLayout(pInSubsystem, &_inputLayout);
				pInDeployer->RegisterSamplerState(pInSubsystem, &_samplerState);
				//pInDeployer->RegisterVertexShader(pInSubsystem, &_vertexShader);
				//pInDeployer->RegisterPixelShader(pInSubsystem, &_pixelShader);
				//pInDeployer->RegisterComputeShader(pInSubsystem, &_computeShader);
				//pInDeployer->RegisterGeometryShader(pInSubsystem, &_geometryShader);
				//pInDeployer->RegisterHullShader(pInSubsystem, &_hullShader);
				//pInDeployer->RegisterDomainShader(pInSubsystem, &_domainShader);
				pInDeployer->RegisterTexture2D(pInSubsystem, &_texture2D);
				pInDeployer->RegisterRasterizerState(pInSubsystem, &_rasterizerState);
				pInDeployer->RegisterShaderView(pInSubsystem, &_shaderView);
				pInDeployer->RegisterUnorderedAccessView(pInSubsystem, &_unorderedAccessView);
				pInDeployer->RegisterRenderTargetView(pInSubsystem, &_renderTargetView);
				pInDeployer->RegisterCommandList(pInSubsystem, &_commandList);
			}
		}
	}
}