#pragma once

// ResourceTypes is included in the ResManD3D11_Groups.h
#include "Groups\ResManD3D11_Groups.h"
#include "Deployer\ResManD3D11_BasicDeployer.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/****************************************************************************
			* The main class of the ResManD3D11 library:
			* - contains all types;
			* - the resource manager subsystem pointer will be attached after "Initialize" is called.
			*****************************************************************************/
			class ResManD3D11_Main
			{
			public:
				ResManD3D11_Main();
				~ResManD3D11_Main();

				/*****************************************************************************************************
				* Initialization
				*****************************************************************************************************/
				void                                              Initialize                       (ResMan::IResourceManagerSubsystem *pInSubsystem);
				// Call this function to substitute the default deployer
				void                                              SetDeployer                      (ResManD3D11_IDeployer *pInDeployer);

				/*****************************************************************************************************
				* D3D11 Device and device context can be attached to be able to pass one function argument
				* that is instance of this class vs three (pDev, pDevCon and pResManD3D11_Main).
				*****************************************************************************************************/
				void                                              AttachD3D                        (ID3D11Device *pInDev, ID3D11DeviceContext *pInDevCon);

				/*****************************************************************************************************
				* Getters
				*****************************************************************************************************/
				ID3D11Device                                     *GetDev                           () const { return _pDev; }
				ID3D11DeviceContext                              *GetDevCon                        () const { return _pDevCon; }

				const ResMan::IResourceManagerSubsystem          *GetSubsystem                     () const { return _pSubsystem; }
				ResMan::IResourceManagerSubsystem                *GetSubsystem                     () { return _pSubsystem; }

				const ResManD3D11_ResourceTypes                  *GetResourceTypes                 () const { return &_resourceTypes; }
				ResManD3D11_ResourceTypes                        *GetResourceTypes                 () { return &_resourceTypes; }

				const TextureFormats                             *GetTextureFormats                () const { return &_textureFormats; }

			private:
				TextureFormats                                   _textureFormats;
				ResManD3D11_ResourceTypes                        _resourceTypes;
				ResMan::IResourceManagerSubsystem               *_pSubsystem = nullptr;

				ID3D11Device                                    *_pDev = nullptr;
				ID3D11DeviceContext                             *_pDevCon = nullptr;

				ResManD3D11_IDeployer                           *_pDeployer;
				ResManD3D11_BasicDeployer                        _basicDeployer;
			};

			/********************************************************************************************************************************
			* Subresource role access functions
			********************************************************************************************************************************/
			ResMan::ISubresourceRole                           *GetSubresourceRole_TestResource_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_Text_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_Buffer_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_BlendState_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_DepthStencilState_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_DepthStencilView_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_InputLayout_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_RasterizerState_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_SamplerState_Std(const ResManD3D11_Main *pResMan);
			//ResMan::ISubresourceRole                           *GetSubresourceRole_VertexShader_Std(const ResManD3D11_Main *pResMan);
			//ResMan::ISubresourceRole                           *GetSubresourceRole_PixelShader_Std(const ResManD3D11_Main *pResMan);
			//ResMan::ISubresourceRole                           *GetSubresourceRole_ComputeShader_Std(const ResManD3D11_Main *pResMan);
			//ResMan::ISubresourceRole                           *GetSubresourceRole_GeometryShader_Std(const ResManD3D11_Main *pResMan);
			//ResMan::ISubresourceRole                           *GetSubresourceRole_HullShader_Std(const ResManD3D11_Main *pResMan);
			//ResMan::ISubresourceRole                           *GetSubresourceRole_DomainShader_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_Texture2D_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_ShaderView_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_UnorderedAccessView_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_RenderTargetView_Std(const ResManD3D11_Main *pResMan);
			ResMan::ISubresourceRole                           *GetSubresourceRole_CommandList_Std(const ResManD3D11_Main *pResMan);

			/********************************************************************************************************************************
			* Resource registration functions arguments:
			* 1. First argument is D3D11 resource manager instance (eResManD3D11_Main instance);
			* 2. Second argument is parameters of the managed resource to create;
			* 3. Third argument(s) are size arguments (for buffer: size in bytes)
			* 4. Description argument(s)
			* 5. Initial content (if any) - must be situtated after the description argument(s), because it's optional
			*
			* Dynamic resources always created with "CPU Write access" flag,
			* because it makes no sense to create dynamic resources that cannot be accessed for writing from CPU
			* (read flag is not specified, of course, because dynamic resourced in D3D 11 cannot be read from by CPU).
			*
			* The staging resource are always created both with read and write CPU access flags specified together automatically.
			********************************************************************************************************************************/

			// Compute appropriate access flags for the given usage
			UINT GetAccessFlags_ForUsage(D3D11_USAGE InUsage);

			/*******************************************************************************************************************************
			* Test resource registration
			*******************************************************************************************************************************/
			TestResource RegisterResource_Test_NoFail
			(
				ResManD3D11_Main                          *pInResMan,
				TestLoader_LoadAll_NoFail                 **pOutLoader = nullptr,
				ResMan::ResourceDependencyList            *pInSourceDependencyList = nullptr
			);
			TestResource RegisterResource_Test_NoFail
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::CombinableParams            *pInCombinableParams,
				TestLoader_LoadAll_NoFail                 **pOutLoader = nullptr,
				ResMan::ResourceDependencyList            *pInSourceDependencyList = nullptr
			);

			/*******************************************************************************************************************************
			* Text
			*******************************************************************************************************************************/
			TextResource RegisterResource_TextFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				const std::string                         &InURL
			);
			// Register resource in group with default role
			TextResource RegisterResource_TextFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				ResMan::IManagedGroup                     *pInGroup,
				const std::string                         &InURL
			);
			// Register resource in group with default role
			TextResource RegisterResource_TextFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const std::string                         &InURL
			);

			/*******************************************************************************************************************************
			* Command list
			*******************************************************************************************************************************/
			D3D11CommandListResource RegisterResource_CommandList
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string                         &InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				ID3D11CommandList                         *pInTemplCommandList
			);
			D3D11CommandListResource RegisterResource_CommandList
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ID3D11CommandList                         *pInTemplCommandList
			);
			D3D11CommandListResource RegisterResource_CommandList
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				ID3D11CommandList                         *pInTemplCommandList
			);

			/*******************************************************************************************************************************
			* Input layout
			*******************************************************************************************************************************/
			// Add non-instanced layout element
			void AppendLayoutElement
			(
				std::vector<D3D11_INPUT_ELEMENT_DESC>     *pOutLayout,
				const char                                *InSemanticName,
				UINT                                       InSemanticIndex,
				DXGI_FORMAT                                InFormat,
				UINT                                       InInputSlot,
				UINT                                       InAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT
			);
			void AppendInstancedLayoutElement
			(
				std::vector<D3D11_INPUT_ELEMENT_DESC>     *pOutLayout,
				const char                                *InSemanticName,
				UINT                                       InSemanticIndex,
				DXGI_FORMAT                                InFormat,
				UINT                                       InInputSlot,
				UINT                                       InInstanceDataStepRate = 1,
				UINT                                       InAlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT
			);
			D3D11InputLayoutResource                               RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				const void                                *pInShaderBytecode,
				SIZE_T                                     InByteCodeLength
			);
			D3D11InputLayoutResource                               RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				D3D11VertexShaderResource                  pInVertexShader
			);
			// Register resource in group with default role
			D3D11InputLayoutResource                               RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				const void                                *pInShaderBytecode,
				SIZE_T                                     InByteCodeLength
			);
			D3D11InputLayoutResource                               RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				D3D11VertexShaderResource                  pInVertexShader
			);
			// Register resource in group with default role
			D3D11InputLayoutResource                               RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				const void                                *pInShaderBytecode,
				SIZE_T                                     InByteCodeLength
			);
			D3D11InputLayoutResource                               RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				D3D11VertexShaderResource                  pInVertexShader
			);

			/*******************************************************************************************************************************
			* Rasterizer state registration
			*******************************************************************************************************************************/
			D3D11RasterizerStateResource                          RegisterResource_RasterizerState
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams       *pInParams,
				const D3D11_RASTERIZER_DESC              *pInDesc
			);
			D3D11RasterizerStateResource                          RegisterResource_RasterizerState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3D11_RASTERIZER_DESC              *pInDesc
			);
			D3D11RasterizerStateResource                          RegisterResource_RasterizerState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3D11_RASTERIZER_DESC              *pInDesc
			);

			/*******************************************************************************************************************************
			* Blend states
			*******************************************************************************************************************************/
			// Register classical additive blend state considering only render target zero
			// (alpha to coverage is always disabled)
			D3D11BlendStateResource RegisterResource_AdditiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams             *pInParams
			);
			D3D11BlendStateResource RegisterResource_AdditiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup
			);
			D3D11BlendStateResource RegisterResource_AdditiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole
			);
			// Register classical subtractive blend state
			// (alpha to coverage is always disabled)
			D3D11BlendStateResource RegisterResource_SubtractiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams       *pInParams
			);
			D3D11BlendStateResource RegisterResource_SubtractiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup
			);
			D3D11BlendStateResource RegisterResource_SubtractiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole
			);
			// Register blend state where blending is disabled
			// (alpha to coverage is always disabled)
			D3D11BlendStateResource RegisterResource_BlendStateDisabled
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams       *pInParams
			);
			D3D11BlendStateResource RegisterResource_BlendStateDisabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup
			);
			D3D11BlendStateResource RegisterResource_BlendStateDisabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole
			);
			// Register parameterized blend state considering only render target zero
			// (alpha to coverage is always disabled)
			D3D11BlendStateResource RegisterResource_BlendState_ForTargetZero
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams       *pInParams,
				D3D11_BLEND                             InSrcBlend,
				D3D11_BLEND                             InDestBlend,
				D3D11_BLEND_OP                          InBlendOp,
				D3D11_BLEND                             InAlphaSrcBlend = D3D11_BLEND_ONE,
				D3D11_BLEND                             InAlphaDestBlend = D3D11_BLEND_ZERO,
				D3D11_BLEND_OP                          InAlphaBlendOp = D3D11_BLEND_OP_ADD
			);
			D3D11BlendStateResource RegisterResource_BlendState_ForTargetZero
			(
				ResManD3D11_Main                       *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                  *pInGroup,
				D3D11_BLEND                             InSrcBlend,
				D3D11_BLEND                             InDestBlend,
				D3D11_BLEND_OP                          InBlendOp,
				D3D11_BLEND                             InAlphaSrcBlend = D3D11_BLEND_ONE,
				D3D11_BLEND                             InAlphaDestBlend = D3D11_BLEND_ZERO,
				D3D11_BLEND_OP                          InAlphaBlendOp = D3D11_BLEND_OP_ADD
			);
			D3D11BlendStateResource RegisterResource_BlendState_ForTargetZero
			(
				ResManD3D11_Main                       *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                  *pInGroup,
				ResMan::ISubresourceRole               *pInRole,
				D3D11_BLEND                             InSrcBlend,
				D3D11_BLEND                             InDestBlend,
				D3D11_BLEND_OP                          InBlendOp,
				D3D11_BLEND                             InAlphaSrcBlend = D3D11_BLEND_ONE,
				D3D11_BLEND                             InAlphaDestBlend = D3D11_BLEND_ZERO,
				D3D11_BLEND_OP                          InAlphaBlendOp = D3D11_BLEND_OP_ADD
			);
			D3D11BlendStateResource RegisterResource_BlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams       *pInParams,
				const D3D11_BLEND_DESC                   *pInDesc
			);
			D3D11BlendStateResource RegisterResource_BlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                   *pInGroup,
				const D3D11_BLEND_DESC                   *pInDesc
			);
			D3D11BlendStateResource RegisterResource_BlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole,
				const D3D11_BLEND_DESC                   *pInDesc
			);

			/*******************************************************************************************************************************
			* Depth stencil state
			*******************************************************************************************************************************/
			// Register depth-stencil state with disabled with disabled both depth and stencil buffers.
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_Disabled
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams       *pInParams
			);
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_Disabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup
			);
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_Disabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole
			);
			// Register normal depth-stencil state with enabled depth.
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams       *pInParams
			);
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup
			);
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole
			);
			// Register depth-stencil state with disabled stencil.
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams       *pInParams,
				D3D11_COMPARISON_FUNC                     InDepthFunc,
				D3D11_DEPTH_WRITE_MASK                    InDepthWriteMask
			);
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				D3D11_COMPARISON_FUNC                     InDepthFunc,
				D3D11_DEPTH_WRITE_MASK                    InDepthWriteMask
			);
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole,
				D3D11_COMPARISON_FUNC                     InDepthFunc,
				D3D11_DEPTH_WRITE_MASK                    InDepthWriteMask
			);
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams       *pInParams,
				const D3D11_DEPTH_STENCIL_DESC           *pInDesc
			);
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				const D3D11_DEPTH_STENCIL_DESC           *pInDesc
			);
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole,
				const D3D11_DEPTH_STENCIL_DESC           *pInDesc
			);

			/*******************************************************************************************************************************
			* Shader resource view
			*******************************************************************************************************************************/
			D3D11ShaderViewResource RegisterResource_ShaderView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMostDefailedMip,
				UINT                                       InNumMipLevels
			);

			D3D11ShaderViewResource RegisterResource_ShaderView_Texture2DArray
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMostDefailedMip,
				UINT                                       InNumMipLevels,	
				UINT                                       InFirstArraySlice,
				UINT                                       InArraySize
			);

			D3D11ShaderViewResource RegisterResource_ShaderView_Texture2DCube
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMostDefailedMip,
				UINT                                       InNumMipLevels
			);

			D3D11ShaderViewResource RegisterResource_ShaderView_Texture2DCubeArray
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMostDefailedMip,
				UINT                                       InNumMipLevels,
				UINT                                       InFirst2DArrayFace,
			    UINT                                       InNumCubes
			);

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				D3D11Texture2DResource                     InResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			);

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				ResMan::IManagedResource                  *pInResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			);

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			);

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				ResMan::IManagedResource                  *pInResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			);

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			);

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::IManagedResource                  *pInResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			);

			/*******************************************************************************************************************************
			* Unordered access view
			*******************************************************************************************************************************/
			D3D11UnorederedAccessViewResource RegisterResource_UnorderedAccessView
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				D3D11Texture2DResource                     InResource,
				const D3D11_UNORDERED_ACCESS_VIEW_DESC    *pInNullOrDesc
			);

			D3D11UnorederedAccessViewResource RegisterResource_UnorderedAccessView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				const D3D11_UNORDERED_ACCESS_VIEW_DESC     *pInNullOrDesc
			);

			D3D11UnorederedAccessViewResource RegisterResource_UnorderedAccessView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				const D3D11_UNORDERED_ACCESS_VIEW_DESC     *pInNullOrDesc
			);

			/*******************************************************************************************************************************
			* Render target view
			*******************************************************************************************************************************/
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMipSlice
			);
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMipSlice
			);
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMipSlice
			);
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				D3D11Texture2DResource                     InResource,
				const D3D11_RENDER_TARGET_VIEW_DESC       *pInNullOrDesc
			);
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				const D3D11_RENDER_TARGET_VIEW_DESC       *pInNullOrDesc
			);
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				const D3D11_RENDER_TARGET_VIEW_DESC       *pInNullOrDesc
			);

			/*******************************************************************************************************************************
			* Depth stencil view
			*******************************************************************************************************************************/
			// Register depth stencil view for depth stencil of texture 2d
			D3D11DepthStencilViewResource RegisterResource_DepthStencilView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InFlags,
				UINT                                       InMipSlice
			);
			D3D11DepthStencilViewResource RegisterResource_DepthStencilView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InFlags,
				UINT                                       InMipSlice
			);
			D3D11DepthStencilViewResource RegisterResource_DepthStencilView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InFlags,
				UINT                                       InMipSlice
			);
			D3D11DepthStencilViewResource RegisterResource_DepthStencilView
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				D3D11Texture2DResource                     InResource,
				const D3D11_DEPTH_STENCIL_VIEW_DESC       *pInNullOrDesc
			);
			D3D11DepthStencilViewResource RegisterResource_DepthStencilView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				const D3D11_DEPTH_STENCIL_VIEW_DESC       *pInNullOrDesc
			);
			D3D11DepthStencilViewResource RegisterResource_DepthStencilView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				const D3D11_DEPTH_STENCIL_VIEW_DESC       *pInNullOrDesc
			);

			/*******************************************************************************************************************************
			* SamplerState
			*******************************************************************************************************************************/
			D3D11SamplerStateResource RegisterResource_SamplerState
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				const D3D11_SAMPLER_DESC                  *pInDesc
			);
			D3D11SamplerStateResource RegisterResource_SamplerState
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3D11_SAMPLER_DESC                  *pInDesc
			);
			D3D11SamplerStateResource RegisterResource_SamplerState
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3D11_SAMPLER_DESC                  *pInDesc
			);

			/*******************************************************************************************************************************
			* General shader registration functions
			*******************************************************************************************************************************/
			// This function is to be specialized for each shader type
			/*
			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);

			// This function is to be specialized for each shader type
			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);

			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);

			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);


			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);
			*/

			/*******************************************************************************************************************************
			* Concrete shader registration functions - from memory;
			*
			* WARNING!!! Only ONE template specialization for each shader type is to be provided
			*******************************************************************************************************************************/
			/*
			template<>
			D3D11ShaderResource<ID3D11VertexShader> RegisterResource_ShaderFromMemory<ID3D11VertexShader>
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			template<>
			D3D11ShaderResource<ID3D11PixelShader> RegisterResource_ShaderFromMemory<ID3D11PixelShader>
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			template<>
			D3D11ShaderResource<ID3D11ComputeShader> RegisterResource_ShaderFromMemory<ID3D11ComputeShader>
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			template<>
			D3D11ShaderResource<ID3D11GeometryShader> RegisterResource_ShaderFromMemory<ID3D11GeometryShader>
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			template<>
			D3D11ShaderResource<ID3D11HullShader> RegisterResource_ShaderFromMemory<ID3D11HullShader>
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			template<>
			D3D11ShaderResource<ID3D11DomainShader> RegisterResource_ShaderFromMemory<ID3D11DomainShader>
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);	
			*/

			/*******************************************************************************************************************************
			* Concrete shader registration functions - from file;
			*
			* WARNING!!! Only ONE template specialization for each shader type is to be provided
			*******************************************************************************************************************************/
			/*
			template<>
			D3D11ShaderResource<ID3D11VertexShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			template<>
			D3D11ShaderResource<ID3D11PixelShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			template<>
			D3D11ShaderResource<ID3D11ComputeShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			template<>
			D3D11ShaderResource<ID3D11GeometryShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			template<>
			D3D11ShaderResource<ID3D11HullShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			template<>
			D3D11ShaderResource<ID3D11DomainShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);
			*/

			/*******************************************************************************************************************************
			* Deprecated vertex shader functions
			*******************************************************************************************************************************/
			/*
			D3D11VertexShaderResource RegisterResource_VertexShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			D3D11VertexShaderResource RegisterResource_VertexShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			D3D11VertexShaderResource RegisterResource_VertexShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			D3D11VertexShaderResource RegisterResource_VertexShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);
			D3D11VertexShaderResource RegisterResource_VertexShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);
			D3D11VertexShaderResource RegisterResource_VertexShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);
			*/

			/*******************************************************************************************************************************
			* Implementing functions
			*******************************************************************************************************************************/
			/*
			D3D11ComputeShaderResource RegisterResource_ComputeShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			D3D11ComputeShaderResource RegisterResource_ComputeShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);

			D3D11GeometryShaderResource RegisterResource_GeometryShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			D3D11GeometryShaderResource RegisterResource_GeometryShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);

			D3D11HullShaderResource RegisterResource_HullShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			D3D11HullShaderResource RegisterResource_HullShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);

			D3D11DomainShaderResource RegisterResource_DomainShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);

			D3D11DomainShaderResource RegisterResource_DomainShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			*/

			/*******************************************************************************************************************************
			* Deprecated pixel shader functions
			*******************************************************************************************************************************/
			/*
			D3D11PixelShaderResource RegisterResource_PixelShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			D3D11PixelShaderResource RegisterResource_PixelShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			D3D11PixelShaderResource RegisterResource_PixelShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			);
			D3D11PixelShaderResource RegisterResource_PixelShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);
			D3D11PixelShaderResource RegisterResource_PixelShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);
			D3D11PixelShaderResource RegisterResource_PixelShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			);
			*/

			/*******************************************************************************************************************************
			* Texture2D
			*******************************************************************************************************************************/
			D3D11Texture2DResource RegisterResource_Texture2DFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const Texture2DParams                     *pInTextureParams,
				const std::string                         &InURL
			);

			D3D11Texture2DResource RegisterResource_Texture2DFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				const Texture2DParams                     *pInTextureParams,
				const std::string                         &InURL
			);

			D3D11Texture2DResource RegisterResource_Texture2DFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const Texture2DParams                     *pInTextureParams,
				const std::string                         &InURL
			);

			// Use this call to create unitialized texture
			// (typically to be used for dynamic resources and depth stencil targets)
			D3D11Texture2DResource RegisterResource_Texture2D_Uninitialized
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const GeneratedTexture2DParams            *pInTextureParams
			);

			// Use this call to create unitialized texture
			// (typically to be used for dynamic resources and depth stencil targets)
			D3D11Texture2DResource RegisterResource_Texture2D_Uninitialized
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				const GeneratedTexture2DParams            *pInTextureParams
			);

			// Use this call to create unitialized texture
			// (typically to be used for dynamic resources and depth stencil targets)
			D3D11Texture2DResource RegisterResource_Texture2D_Uninitialized
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const GeneratedTexture2DParams            *pInTextureParams
			);

			/*******************************************************************************************************************************
			* Buffers with the given usage (correct CPU access flags are to be assigned)
			*
			* Default managed resource parameters for buffer resource are so that it must be kept loaded.
			*
			* WARNING!!! Do NOT specialize functions to register managed resource with the given parameters,
			* because the parameterize functions (see below) are made for that purpose.
			* That functions take just a little more parameters.
			*******************************************************************************************************************************/
			D3D11BufferResource RegisterResource_VertexBuffer
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                       InName,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_VertexBuffer
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                       InName,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_VertexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_VertexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_IndexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_IndexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_IndexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_ConstantBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_ConstantBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_IndexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_ConstantBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent = nullptr
			);
			D3D11BufferResource RegisterResource_ConstantBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			);

			/*****************************************************************************************************************************
			* Parameterized buffer resource registration
			*****************************************************************************************************************************/
			// Register buffer with the given binld flags and the given usage
			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				const ResMan::PublicResourceParams      *pInParams,
				size_t                                   InByteWidth,
				UINT                                     InBindFlags,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			);
			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				size_t                                   InByteWidth,
				UINT                                     InBindFlags,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			);
			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				UINT                                     InBindFlags,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			);


			// Register buffer with the given description and initial content
			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				const ResMan::PublicResourceParams      *pInParams,
				size_t                                   InByteWidth,
				const D3D11_BUFFER_DESC                 *pInDesc,
				const D3D11_SUBRESOURCE_DATA            *pInNullOrSubresourceData
			);
			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				ResMan::IManagedGroup                   *pInGroup,
				size_t                                   InByteWidth,
				const D3D11_BUFFER_DESC                 *pInDesc,
				const D3D11_SUBRESOURCE_DATA            *pInNullOrSubresourceData
			);
			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				ResMan::IManagedGroup                   *pInGroup,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				const D3D11_BUFFER_DESC                 *pInDesc,
				const D3D11_SUBRESOURCE_DATA            *pInNullOrSubresourceData
			);
		}
	}
}

