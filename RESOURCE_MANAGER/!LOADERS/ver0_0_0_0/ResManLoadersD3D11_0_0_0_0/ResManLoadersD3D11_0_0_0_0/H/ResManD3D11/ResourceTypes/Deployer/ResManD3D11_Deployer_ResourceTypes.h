#pragma once

#include "../../../Test/TestLoader.h"
#include "../../../Test/TestLoaderFailable.h"

#include "../../../TextResource.h"

#include "../../../D3D11BlendStateResource.h"
#include "../../../D3D11InputLayoutResource.h"
#include "../../../D3D11BufferResource.h"
#include "../../../D3D11DepthStencilStateResource.h"
#include "../../../D3D11DepthStencilViewResource.h"
#include "../../../D3D11ShaderResourceView.h"
#include "../../../D3D11UnorderedAccessView.h"
#include "../../../D3D11ShaderPairResource.h"
#include "../../../D3D11ShaderResource.h"
#include "../../../D3D11SamplerStateResource.h"
#include "../../../D3D11Texture2DResource.h"
#include "../../../D3D11RasterizerStateResource.h"
#include "../../../D3D11RenderTargetViewResource.h"
#include "../../../D3D11CommandListResource.h"

/*****************************************************************************************************
* TODO:
* 1. RenderTargetViewResource:
* 1.1. Create register (+DONE)
* 1.2. Test
* 2. Shaders:
* 2.1. Modify shader resource to support dynamic class linkage (+DONE: )12.07.2017)
* 2.2. Shader program (amalgamate many shader stages as one resource)
* 2.3. Create all shader programs
* 2.3.1. Geometry
* 2.3.2. Tesselation
* 2.3.2.1. Hull
* 2.3.2.2. Domain
* 2.3.3. Compute
* 3. Command list
* 3.1. POSSIBLE BUG: If we copy command list as-is (without doing AddRef), then we will release source object!!!
* (+DONE: 12.07.2017)
* 4. Texture resources:
* 4.1. Registration methods:
* 4.1.1. Arrays
* 4.1.2. Cubes
* 4.2. Texture array support inside texture loaders;
* 4.3. 1D textures:
* 4.4. 3D textures:
* 4.5. Texture cubes
* 4.6. Views for other textures:
* 4.6.1. 1D textures:
* 4.6.2. 3D textures:
* 4.6.3. texture cubes
*****************************************************************************************************/

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			class ResManD3D11_ResourceTypes_IDeployer
			{
			public:

				/**************************************************************************************************************************************
				* Implementation of each function may register corresponding resource type or may omit its registration
				**************************************************************************************************************************************/
				virtual void                                       RegisterTestResource                  (ResMan::IResourceManagerSubsystem *pInSubsystem, TestResourceType *pOutType) =0;
				virtual void                                       RegisterText                          (ResMan::IResourceManagerSubsystem *pInSubsystem, TextResourceType *pOutType) =0;
				virtual void                                       RegisterBlendState                    (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11BlendStateResourceType *pOutType) =0;
				virtual void                                       RegisterBuffer                        (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11BufferResourceType *pOutType) =0;
				virtual void                                       RegisterDepthStencilState             (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11DepthStencilStateResourceType *pOutType) =0;
				virtual void                                       RegisterDepthStencilView              (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11DepthStencilViewResourceType *pOutType) =0;
				virtual void                                       RegisterInputLayout                   (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11InputLayoutResourceType *pOutType) =0;
				virtual void                                       RegisterSamplerState                  (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11SamplerStateResourceType *pOutType) =0;
				virtual void                                       RegisterVertexShader                  (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11VertexShader> *pOutType) =0;
				virtual void                                       RegisterPixelShader                   (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11PixelShader> *pOutType) =0;
				virtual void                                       RegisterComputeShader                 (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11ComputeShader> *pOutType) = 0;				
				virtual void                                       RegisterGeometryShader                (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11GeometryShader> *pOutType) = 0;
				virtual void                                       RegisterHullShader                    (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11HullShader> *pOutType) = 0;
				virtual void                                       RegisterDomainShader                  (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderResourceType<ID3D11DomainShader> *pOutType) = 0;
				virtual void                                       RegisterTexture2D                     (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11Texture2DResourceType *pOutType) =0;
				virtual void                                       RegisterRasterizerState               (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11RasterizerStateResourceType *pOutType) =0;
				virtual void                                       RegisterShaderView                    (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11ShaderViewResourceType *pOutType) =0;
				virtual void                                       RegisterUnorderedAccessView           (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11UnorderedAccessViewResourceType *pOutType) =0;
				virtual void                                       RegisterRenderTargetView              (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11RenderTargetViewResourceType *pOutType) = 0;
				virtual void                                       RegisterCommandList                   (ResMan::IResourceManagerSubsystem *pInSubsystem, D3D11CommandListResourceType *pOutType) = 0;

			private:
			};
		}
	}
} // Dv