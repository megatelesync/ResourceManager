#include "../../H/ResManD3D11/ResManD3D11.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*******************************************************************************************************************************
			* Test resource registration
			*******************************************************************************************************************************/
			TestResource RegisterResource_Test_NoFail(ResManD3D11_Main *pInResMan, TestLoader_LoadAll_NoFail **pOutLoader, ResMan::ResourceDependencyList *pInSourceDependencyList)
			{
				auto pTestResourceType = pInResMan->GetResourceTypes()->GetTestResource();

				auto resourceParams = GetDefaultResourceParams_ForResourceType(pTestResourceType);

				auto pLoader = CreateDefault_TestLoader_LoadAll_NoFail(pTestResourceType->GetRoles().pStd, pTestResourceType->GetResourceType(), pInSourceDependencyList);

				ResMan::IManagedResource *pManagedResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(resourceParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				if (pOutLoader != nullptr)
				{
					*pOutLoader = pLoader.get();
				}
				return TestResource(pManagedResource);
			}

			TestResource RegisterResource_Test_NoFail(ResManD3D11_Main *pInResMan, const ResMan::CombinableParams *pInCombinableParams, TestLoader_LoadAll_NoFail **pOutLoader, ResMan::ResourceDependencyList *pInSourceDependencyList)
			{
				auto pTestResourceType = pInResMan->GetResourceTypes()->GetTestResource();

				auto resourceParams = GetDefaultResourceParams_ForResourceType(pTestResourceType, pInCombinableParams);

				auto pLoader = CreateDefault_TestLoader_LoadAll_NoFail(pTestResourceType->GetRoles().pStd, pTestResourceType->GetResourceType(), pInSourceDependencyList);

				ResMan::IManagedResource *pManagedResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(resourceParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				if (pOutLoader != nullptr)
				{
					*pOutLoader = pLoader.get();
				}
				return TestResource(pManagedResource);
			}

			/********************************************************************************************
			* ResManD3D11_Main text functions implementation
			********************************************************************************************/
			TextResource RegisterResource_TextFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				const std::string                         &InURL
			)
			{
				auto pLoader = new TextLoaderFromFile
				(
					InURL,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetText()
				);

				ResMan::IManagedResource *pTextResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return TextResource(pTextResource);
			}
			TextResource RegisterResource_TextFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				ResMan::IManagedGroup                     *pInGroup,
				const std::string                         &InURL
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetText());
				auto pResource = RegisterResource_TextFromFile(pInResMan, &resourceParams, InURL);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			TextResource RegisterResource_TextFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const std::string                         &InURL
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetText());
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_TextFromFile(pInResMan, &resourceParams, InURL);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			/*******************************************************************************************************************************
			* ResManD3D11_Main input layout functions implementation
			*******************************************************************************************************************************/
			void AppendLayoutElement
			(
				std::vector<D3D11_INPUT_ELEMENT_DESC>     *pOutLayout,
				const char                                *InSemanticName,
				UINT                                       InSemanticIndex,
				DXGI_FORMAT                                InFormat,
				UINT                                       InInputSlot,
				UINT                                       InAlignedByteOffset
			)
			{
				assert(pOutLayout != nullptr);
				D3D11_INPUT_ELEMENT_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.AlignedByteOffset = InAlignedByteOffset;
				desc.Format = InFormat;
				desc.InputSlot = InInputSlot;
				desc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
				desc.InstanceDataStepRate = 0;
				desc.SemanticIndex = InSemanticIndex;
				desc.SemanticName = InSemanticName;
				pOutLayout->push_back(desc);

			}
			void AppendInstancedLayoutElement
			(
				std::vector<D3D11_INPUT_ELEMENT_DESC>     *pOutLayout,
				const char                                *InSemanticName,
				UINT                                       InSemanticIndex,
				DXGI_FORMAT                                InFormat,
				UINT                                       InInputSlot,
				UINT                                       InInstanceDataStepRate,
				UINT                                       InAlignedByteOffset
			)
			{
				assert(pOutLayout != nullptr);
				D3D11_INPUT_ELEMENT_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.AlignedByteOffset = InAlignedByteOffset;
				desc.Format = InFormat;
				desc.InputSlot = InInputSlot;
				desc.InputSlotClass = D3D11_INPUT_PER_INSTANCE_DATA;
				desc.InstanceDataStepRate = InInstanceDataStepRate;
				desc.SemanticIndex = InSemanticIndex;
				desc.SemanticName = InSemanticName;
				pOutLayout->push_back(desc);
			}
			D3D11InputLayoutResource                               RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				D3D11VertexShaderResource                  pInVertexShader
			)
			{
				return RegisterResource_InputLayout(pInResMan, pInParams, pInLayoutDesc, InNumDescElems, pInVertexShader.GetTypedRealResource()->GetBytecodeBlob()->GetBufferPointer(), pInVertexShader.GetTypedRealResource()->GetBytecodeBlobSize());
			}
			D3D11InputLayoutResource                               RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				const void                                *pInShaderBytecode,
				SIZE_T                                     InByteCodeLength
			)
			{
				auto pLoader = std::make_shared<D3D11InputLayoutLoader>
				(
					pInLayoutDesc,
					InNumDescElems,
					pInShaderBytecode,
					InByteCodeLength,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetInputLayout()
				);

				ResMan::IManagedResource *pInputLayoutResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, pLoader);
				return D3D11InputLayoutResource(pInputLayoutResource);
			}
			D3D11InputLayoutResource                               RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				D3D11VertexShaderResource                  pInVertexShader
			)
			{
				return RegisterResource_InputLayout(pInResMan, InName, pInGroup, pInLayoutDesc, InNumDescElems, pInVertexShader.GetTypedRealResource()->GetBytecodeBlob()->GetBufferPointer(), pInVertexShader.GetTypedRealResource()->GetBytecodeBlobSize());
			}
			// Register resource in group with default role
			D3D11InputLayoutResource RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				const void                                *pInShaderBytecode,
				SIZE_T                                     InByteCodeLength
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetInputLayout());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_InputLayout(pInResMan, &resourceParams, pInLayoutDesc, InNumDescElems, pInShaderBytecode, InByteCodeLength);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11InputLayoutResource                               RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				D3D11VertexShaderResource                  pInVertexShader
			)
			{
				return RegisterResource_InputLayout(pInResMan, InName, pInGroup, pInRole, pInLayoutDesc, InNumDescElems, pInVertexShader.GetTypedRealResource()->GetBytecodeBlob()->GetBufferPointer(), pInVertexShader.GetTypedRealResource()->GetBytecodeBlobSize());
			}
			D3D11InputLayoutResource RegisterResource_InputLayout
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3D11_INPUT_ELEMENT_DESC            *pInLayoutDesc,
				UINT                                       InNumDescElems,
				const void                                *pInShaderBytecode,
				SIZE_T                                     InByteCodeLength
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetInputLayout());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_InputLayout(pInResMan, &resourceParams, pInLayoutDesc, InNumDescElems, pInShaderBytecode, InByteCodeLength);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			/*******************************************************************************************************************************
			* ResManD3D11_Main Rasterizer state functions implementation
			*******************************************************************************************************************************/
			D3D11RasterizerStateResource RegisterResource_RasterizerState
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3D11_RASTERIZER_DESC              *pInDesc
			)
			{
				auto pLoader = std::make_shared<D3D11RasterizerStateLoader>
				(
					pInDesc,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetRasterizerState()
				);
				ResMan::IManagedResource *pRasterizerStateResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, pLoader);
				return D3D11RasterizerStateResource(pRasterizerStateResource);
			}
			D3D11RasterizerStateResource RegisterResource_RasterizerState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                    *pInGroup,
				const D3D11_RASTERIZER_DESC              *pInDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetRasterizerState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_RasterizerState(pInResMan, &resourceParams, pInDesc);
				if(pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11RasterizerStateResource RegisterResource_RasterizerState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3D11_RASTERIZER_DESC              *pInDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetRasterizerState());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_RasterizerState(pInResMan, &resourceParams, pInDesc);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			/*******************************************************************************************************************************
			* ResManD3D11_Main command list resource implementation
			*******************************************************************************************************************************/
			D3D11CommandListResource RegisterResource_CommandList
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string                         &InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				ID3D11CommandList                         *pInTemplCommandList
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetCommandList());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_CommandList(pInResMan, &resourceParams, pInTemplCommandList);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11CommandListResource RegisterResource_CommandList
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ID3D11CommandList                         *pInTemplCommandList
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetCommandList());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_CommandList(pInResMan, &resourceParams, pInTemplCommandList);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11CommandListResource RegisterResource_CommandList
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				ID3D11CommandList                         *pInTemplCommandList
			)
			{
				auto pLoader = new D3D11CommandListLoader
				(
					pInResMan->GetDev(),
					pInTemplCommandList,
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetCommandList()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11CommandListResource(pResource);
			}

			/*******************************************************************************************************************************
			* ResManD3D11_Main Blend states functions implementation
			*******************************************************************************************************************************/
			D3D11BlendStateResource RegisterResource_AdditiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams             *pInParams
			)
			{
				auto resource = RegisterResource_BlendState_ForTargetZero(pInResMan, pInParams, D3D11_BLEND_ONE, D3D11_BLEND_ONE, D3D11_BLEND_OP_ADD);
				return resource;
			}
			D3D11BlendStateResource RegisterResource_AdditiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBlendState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_AdditiveBlendState(pInResMan, &resourceParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11BlendStateResource RegisterResource_AdditiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBlendState());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_AdditiveBlendState(pInResMan, &resourceParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11BlendStateResource RegisterResource_SubtractiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams             *pInParams
			)
			{
				auto resource = RegisterResource_BlendState_ForTargetZero(pInResMan, pInParams, D3D11_BLEND_SRC_ALPHA, D3D11_BLEND_INV_DEST_ALPHA, D3D11_BLEND_OP_ADD);
				return resource;
			}

			D3D11BlendStateResource RegisterResource_SubtractiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBlendState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_SubtractiveBlendState(pInResMan, &resourceParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11BlendStateResource RegisterResource_SubtractiveBlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBlendState());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_SubtractiveBlendState(pInResMan, &resourceParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			D3D11BlendStateResource RegisterResource_BlendStateDisabled
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams             *pInParams
			)
			{
				D3D11_BLEND_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.AlphaToCoverageEnable = FALSE;
				desc.IndependentBlendEnable = TRUE;
				desc.RenderTarget[0].BlendEnable = FALSE;
				auto pResource = RegisterResource_BlendState(pInResMan, pInParams, &desc);
				return pResource;
			}
			D3D11BlendStateResource RegisterResource_BlendStateDisabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBlendState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_BlendStateDisabled(pInResMan, &resourceParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11BlendStateResource RegisterResource_BlendStateDisabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBlendState());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_BlendStateDisabled(pInResMan, &resourceParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11BlendStateResource RegisterResource_BlendState_ForTargetZero
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams             *pInParams,
				D3D11_BLEND                               InSrcBlend,
				D3D11_BLEND                               InDestBlend,
				D3D11_BLEND_OP                            InBlendOp,
				D3D11_BLEND                               InAlphaSrcBlend,
				D3D11_BLEND                               InAlphaDestBlend,
				D3D11_BLEND_OP                            InAlphaBlendOp
			)
			{
				D3D11_BLEND_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.AlphaToCoverageEnable = FALSE;
				desc.IndependentBlendEnable = TRUE;
				desc.RenderTarget[0].BlendEnable = TRUE;
				desc.RenderTarget[0].BlendOp = InBlendOp;
				desc.RenderTarget[0].SrcBlend = InSrcBlend;
				desc.RenderTarget[0].DestBlend = InDestBlend;
				desc.RenderTarget[0].SrcBlendAlpha = InAlphaSrcBlend;
				desc.RenderTarget[0].DestBlendAlpha = InAlphaDestBlend;
				desc.RenderTarget[0].BlendOpAlpha = InAlphaBlendOp;
				desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
				auto resource = RegisterResource_BlendState(pInResMan, pInParams, &desc);
				return resource;
			}
			D3D11BlendStateResource RegisterResource_BlendState_ForTargetZero
			(
				ResManD3D11_Main                       *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                  *pInGroup,
				D3D11_BLEND                             InSrcBlend,
				D3D11_BLEND                             InDestBlend,
				D3D11_BLEND_OP                          InBlendOp,
				D3D11_BLEND                             InAlphaSrcBlend,
				D3D11_BLEND                             InAlphaDestBlend,
				D3D11_BLEND_OP                          InAlphaBlendOp
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBlendState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_BlendState_ForTargetZero(pInResMan, &resourceParams, InSrcBlend, InDestBlend, InBlendOp, InAlphaSrcBlend, InAlphaDestBlend, InAlphaBlendOp);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11BlendStateResource RegisterResource_BlendState_ForTargetZero
			(
				ResManD3D11_Main                       *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                  *pInGroup,
				ResMan::ISubresourceRole               *pInRole,
				D3D11_BLEND                             InSrcBlend,
				D3D11_BLEND                             InDestBlend,
				D3D11_BLEND_OP                          InBlendOp,
				D3D11_BLEND                             InAlphaSrcBlend,
				D3D11_BLEND                             InAlphaDestBlend,
				D3D11_BLEND_OP                          InAlphaBlendOp
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBlendState());
				resourceParams.SetRole(pInRole);
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_BlendState_ForTargetZero(pInResMan, &resourceParams, InSrcBlend, InDestBlend, InBlendOp, InAlphaSrcBlend, InAlphaDestBlend, InAlphaBlendOp);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11BlendStateResource RegisterResource_BlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams             *pInParams,
				const D3D11_BLEND_DESC                   *pInDesc
			)
			{
				auto pLoader = new D3D11BlendStateLoader
				(
					*pInDesc,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetBlendState()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11BlendStateResource(pResource);
			}

			D3D11BlendStateResource RegisterResource_BlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                   *pInGroup,
				const D3D11_BLEND_DESC                   *pInDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBlendState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_BlendState(pInResMan, &resourceParams, pInDesc);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11BlendStateResource RegisterResource_BlendState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole,
				const D3D11_BLEND_DESC                   *pInDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBlendState());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_BlendState(pInResMan, &resourceParams, pInDesc);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			/*******************************************************************************************************************************
			* ResManD3D11_Main buffer depth stencil state
			*******************************************************************************************************************************/
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_Disabled
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams             *pInParams
			)
			{
				D3D11_DEPTH_STENCIL_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.DepthEnable = FALSE;
				desc.StencilEnable = FALSE;
				auto pResource = RegisterResource_DepthStencilState(pInResMan, pInParams, &desc);
				return pResource;
			}

			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_Disabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_DepthStencilState_Disabled(pInResMan, &resourceParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_Disabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilState());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_DepthStencilState_Disabled(pInResMan, &resourceParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams             *pInParams
			)
			{
				auto pResource = RegisterResource_DepthStencilState_DepthEnabled(pInResMan, pInParams, D3D11_COMPARISON_LESS, D3D11_DEPTH_WRITE_MASK_ALL);
				return pResource;
			}

			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_DepthStencilState_DepthEnabled(pInResMan, &resourceParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilState());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_DepthStencilState_DepthEnabled(pInResMan, &resourceParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			// Register depth-stencil state with disabled stencil.
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams             *pInParams,
				D3D11_COMPARISON_FUNC                     InDepthFunc,
				D3D11_DEPTH_WRITE_MASK                    InDepthWriteMask
			)
			{
				D3D11_DEPTH_STENCIL_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.DepthEnable = TRUE;
				desc.StencilEnable = FALSE;
				desc.DepthWriteMask = InDepthWriteMask;
				desc.DepthFunc = InDepthFunc;
				auto pResource = RegisterResource_DepthStencilState(pInResMan, pInParams, &desc);
				return pResource;
			}

			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				D3D11_COMPARISON_FUNC                     InDepthFunc,
				D3D11_DEPTH_WRITE_MASK                    InDepthWriteMask
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_DepthStencilState_DepthEnabled(pInResMan, &resourceParams, InDepthFunc, InDepthWriteMask);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState_DepthEnabled
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole,
				D3D11_COMPARISON_FUNC                     InDepthFunc,
				D3D11_DEPTH_WRITE_MASK                    InDepthWriteMask
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilState());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_DepthStencilState_DepthEnabled(pInResMan, &resourceParams, InDepthFunc, InDepthWriteMask);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			D3D11DepthStencilStateResource RegisterResource_DepthStencilState
			(
				ResManD3D11_Main                         *pInResMan,
				const ResMan::PublicResourceParams             *pInParams,
				const D3D11_DEPTH_STENCIL_DESC           *pInDesc
			)
			{
				auto pLoader = new D3D11DepthStencilStateLoader
				(
					*pInDesc,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetDepthStencilState()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11DepthStencilStateResource(pResource);
			}

			D3D11DepthStencilStateResource RegisterResource_DepthStencilState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				const D3D11_DEPTH_STENCIL_DESC           *pInDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_DepthStencilState(pInResMan, &resourceParams, pInDesc);
				if(pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11DepthStencilStateResource RegisterResource_DepthStencilState
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                    *pInGroup,
				ResMan::ISubresourceRole                 *pInRole,
				const D3D11_DEPTH_STENCIL_DESC           *pInDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilState());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_DepthStencilState(pInResMan, &resourceParams, pInDesc);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			/*******************************************************************************************************************************
			* Unordered access view implementation
			*******************************************************************************************************************************/
			D3D11UnorederedAccessViewResource RegisterResource_UnorderedAccessView
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				D3D11Texture2DResource                     InResource,
				const D3D11_UNORDERED_ACCESS_VIEW_DESC    *pInNullOrDesc
			)
			{
				auto pLoader = new D3D11UnorderedAccessViewLoader
				(
					InResource,
					pInNullOrDesc,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetUnorderedAccessView()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11UnorederedAccessViewResource(pResource);
			}

			D3D11UnorederedAccessViewResource RegisterResource_UnorderedAccessView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				const D3D11_UNORDERED_ACCESS_VIEW_DESC     *pInNullOrDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetUnorderedAccessView());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_UnorderedAccessView(pInResMan, &resourceParams, InResource, pInNullOrDesc);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			D3D11UnorederedAccessViewResource RegisterResource_UnorderedAccessView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				const D3D11_UNORDERED_ACCESS_VIEW_DESC     *pInNullOrDesc
			)
			{
				auto pRole = GetSubresourceRole_UnorderedAccessView_Std(pInResMan);
				return RegisterResource_UnorderedAccessView(pInResMan, InName, pInGroup, InResource, pInNullOrDesc);
			}

			/*******************************************************************************************************************************
			* Shader view implementation
			*******************************************************************************************************************************/
			D3D11ShaderViewResource RegisterResource_ShaderView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMostDetailedMip,
				UINT                                       InNumMipLevels
			)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Format = InFormat;
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				desc.Texture2D.MostDetailedMip = InMostDetailedMip;
				desc.Texture2D.MipLevels = InNumMipLevels;
				return RegisterResource_ShaderView(pInResMan, pInParams, InResource, &desc);
			}

			D3D11ShaderViewResource RegisterResource_ShaderView_Texture2DArray
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMostDetailedMip,
				UINT                                       InNumMipLevels,
				UINT                                       InFirstArraySlice,
				UINT                                       InArraySize
			)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Format = InFormat;
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
				desc.Texture2DArray.MostDetailedMip = InMostDetailedMip;
				desc.Texture2DArray.MipLevels = InNumMipLevels;
				desc.Texture2DArray.FirstArraySlice = InFirstArraySlice;
				desc.Texture2DArray.ArraySize = InArraySize;
				return RegisterResource_ShaderView(pInResMan, pInParams, InResource, &desc);
			}

			D3D11ShaderViewResource RegisterResource_ShaderView_Texture2DCube
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMostDetailedMip,
				UINT                                       InNumMipLevels
			)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Format = InFormat;
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
				desc.TextureCube.MipLevels = InNumMipLevels;
				desc.TextureCube.MostDetailedMip = InMostDetailedMip;
				return RegisterResource_ShaderView(pInResMan, pInParams, InResource, &desc);
			}

			D3D11ShaderViewResource RegisterResource_ShaderView_Texture2DCubeArray
			(
				ResManD3D11_Main                          *pInResMan,
				ResMan::PublicResourceParams              *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMostDetailedMip,
				UINT                                       InNumMipLevels,
				UINT                                       InFirst2DArrayFace,
				UINT                                       InNumCubes
			)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.Format = InFormat;
				desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBEARRAY;
				desc.TextureCubeArray.MipLevels = InNumMipLevels;
				desc.TextureCubeArray.MostDetailedMip = InMostDetailedMip;
				desc.TextureCubeArray.NumCubes = InNumCubes;
				desc.TextureCubeArray.First2DArrayFace = InFirst2DArrayFace;
				return RegisterResource_ShaderView(pInResMan, pInParams, InResource, &desc);
			}

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				D3D11Texture2DResource                     InResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			)
			{
				return RegisterResource_ShaderView(pInResMan, pInParams, InResource.GetManagedResource(), pInNullOrDesc);
			}

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				ResMan::IManagedResource                  *pInResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			)
			{
				assert(pInResource);
				auto pLoader = new D3D11ShaderViewLoader
				(
					pInResource,
					pInNullOrDesc,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetShaderView()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11ShaderViewResource(pResource);
			}

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			)
			{
				return RegisterResource_ShaderView(pInResMan, InName, pInGroup, pInRole, InResource.GetManagedResource(), pInNullOrDesc);
			}

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				ResMan::IManagedResource                  *pInResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetShaderView());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_ShaderView(pInResMan, &resourceParams, pInResource, pInNullOrDesc);
				if (pInGroup != nullptr)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			)
			{
				return RegisterResource_ShaderView(pInResMan, InName, pInGroup, InResource.GetManagedResource(), pInNullOrDesc);
			}

			D3D11ShaderViewResource RegisterResource_ShaderView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::IManagedResource                  *pInResource,
				const D3D11_SHADER_RESOURCE_VIEW_DESC     *pInNullOrDesc
			)
			{
				auto pRole = GetSubresourceRole_ShaderView_Std(pInResMan);
				return RegisterResource_ShaderView(pInResMan, InName, pInGroup, pRole, pInResource, pInNullOrDesc);
			}

			/*******************************************************************************************************************************
			* Render target view implementation
			*******************************************************************************************************************************/
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMipSlice
			)
			{
				D3D11_RENDER_TARGET_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
				desc.Format = InFormat;
				desc.Texture2D.MipSlice = InMipSlice;
				auto pResource = RegisterResource_RenderTargetView(pInResMan, pInParams, InResource, &desc);
				return pResource;
			}
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMipSlice
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetRenderTargetView());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_RenderTargetView_Texture2D(pInResMan, &resourceParams, InResource, InFormat, InMipSlice);
				if (pInGroup != nullptr)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InMipSlice
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetRenderTargetView());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_RenderTargetView_Texture2D(pInResMan, &resourceParams, InResource, InFormat, InMipSlice);
				if (pInGroup != nullptr)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				D3D11Texture2DResource                     InResource,
				const D3D11_RENDER_TARGET_VIEW_DESC       *pInNullOrDesc
			)
			{
				auto pLoader = new D3D11RenderTargetViewLoader
				(
					InResource,
					pInNullOrDesc,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetRenderTargetView()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11RenderTargetViewResource(pResource);
			}
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				const D3D11_RENDER_TARGET_VIEW_DESC       *pInNullOrDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetRenderTargetView());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_RenderTargetView(pInResMan, &resourceParams, InResource, pInNullOrDesc);
				if (pInGroup != nullptr)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11RenderTargetViewResource RegisterResource_RenderTargetView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				const D3D11_RENDER_TARGET_VIEW_DESC       *pInNullOrDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetRenderTargetView());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_RenderTargetView(pInResMan, &resourceParams, InResource, pInNullOrDesc);
				if (pInGroup != nullptr)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			/*******************************************************************************************************************************
			* ResManD3D11_Main buffer depth stencil view
			*******************************************************************************************************************************/
			D3D11DepthStencilViewResource RegisterResource_DepthStencilView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InFlags,
				UINT                                       InMipSlice
			)
			{
				D3D11_DEPTH_STENCIL_VIEW_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
				desc.Flags = InFlags;
				desc.Format = InFormat;
				desc.Texture2D.MipSlice = InMipSlice;
				auto pResource = RegisterResource_DepthStencilView(pInResMan, pInParams, InResource, &desc);
				return pResource;
			}

			D3D11DepthStencilViewResource RegisterResource_DepthStencilView_Texture2D
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				DXGI_FORMAT                                InFormat,
				UINT                                       InFlags,
				UINT                                       InMipSlice
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilView());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_DepthStencilView_Texture2D(pInResMan, &resourceParams, InResource, InFormat, InFlags, InMipSlice);
				if (pInGroup != nullptr)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
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
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilView());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_DepthStencilView_Texture2D(pInResMan, &resourceParams, InResource, InFormat, InFlags, InMipSlice);
				if (pInGroup != nullptr)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			D3D11DepthStencilViewResource RegisterResource_DepthStencilView
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				D3D11Texture2DResource                            InResource,
				const D3D11_DEPTH_STENCIL_VIEW_DESC       *pInNullOrDesc
			)
			{
				auto pLoader = new D3D11DepthStencilViewLoader
				(
					InResource,
					pInNullOrDesc,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetDepthStencilView()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11DepthStencilViewResource(pResource);
			}
			D3D11DepthStencilViewResource RegisterResource_DepthStencilView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				D3D11Texture2DResource                     InResource,
				const D3D11_DEPTH_STENCIL_VIEW_DESC       *pInNullOrDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilView());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_DepthStencilView(pInResMan, &resourceParams, InResource, pInNullOrDesc);
				if(pInGroup != nullptr)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11DepthStencilViewResource RegisterResource_DepthStencilView
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				D3D11Texture2DResource                     InResource,
				const D3D11_DEPTH_STENCIL_VIEW_DESC       *pInNullOrDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetDepthStencilView());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_DepthStencilView(pInResMan, &resourceParams, InResource, pInNullOrDesc);
				if (pInGroup != nullptr)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			/*******************************************************************************************************************************
			*  ResManD3D11_Main SamplerState functions implementation
			*******************************************************************************************************************************/
			D3D11SamplerStateResource RegisterResource_SamplerState
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3D11_SAMPLER_DESC                  *pInDesc
			)
			{
				auto pLoader = new D3D11SamplerStateLoader
				(
					*pInDesc,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetSamplerState()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11SamplerStateResource(pResource);
			}

			D3D11SamplerStateResource RegisterResource_SamplerState
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3D11_SAMPLER_DESC                  *pInDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetSamplerState());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_SamplerState(pInResMan, &resourceParams, pInDesc);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}
			D3D11SamplerStateResource RegisterResource_SamplerState
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3D11_SAMPLER_DESC                  *pInDesc
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetSamplerState());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_SamplerState(pInResMan, &resourceParams, pInDesc);
				if (pInGroup)
				{
					pInGroup->AttachResource(pResource.GetManagedResource());
				}
				return pResource;
			}

			/*******************************************************************************************************************************
			* ResManD3D11_Main Shader functions implementation
			*******************************************************************************************************************************/
			/*
			D3D11VertexShaderResource RegisterResource_VertexShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto pLoader = new D3D11VertexShaderLoaderFromMemory
				(
					pInSourceCode,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetVertexShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11VertexShaderResource(pResource);
			}
			D3D11VertexShaderResource RegisterResource_VertexShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetVertexShader());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_VertexShaderFromMemory(pInResMan, &resourceParams, pInCompileParams, pInSourceCode);
				pInGroup->AttachResource(pResource.GetManagedResource());
				return pResource;
			}
			D3D11VertexShaderResource RegisterResource_VertexShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetVertexShader());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_VertexShaderFromMemory(pInResMan, &resourceParams, pInCompileParams, pInSourceCode);
				pInGroup->AttachResource(pResource.GetManagedResource());
				return pResource;
			}
			D3D11VertexShaderResource RegisterResource_VertexShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto pLoader = new D3D11VertexShaderLoaderFromFile
				(
					InURL,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetVertexShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11VertexShaderResource(pResource);
			}
			D3D11VertexShaderResource RegisterResource_VertexShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetVertexShader());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_VertexShaderFromFile(pInResMan, &resourceParams, pInCompileParams, InURL);
				pInGroup->AttachResource(pResource.GetManagedResource());
				return pResource;
			}
			D3D11VertexShaderResource RegisterResource_VertexShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetVertexShader());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_VertexShaderFromFile(pInResMan, &resourceParams, pInCompileParams, InURL);
				pInGroup->AttachResource(pResource.GetManagedResource());
				return pResource;
			}
			D3D11PixelShaderResource RegisterResource_PixelShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto pLoader = new D3D11PixelShaderLoaderFromMemory
				(
					pInSourceCode,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetPixelShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11PixelShaderResource(pResource);
			}
			D3D11PixelShaderResource RegisterResource_PixelShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetPixelShader());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_PixelShaderFromMemory(pInResMan, &resourceParams, pInCompileParams, pInSourceCode);
				pInGroup->AttachResource(pResource.GetManagedResource());
				return pResource;
			}
			D3D11PixelShaderResource RegisterResource_PixelShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetPixelShader());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_PixelShaderFromMemory(pInResMan, &resourceParams, pInCompileParams, pInSourceCode);
				pInGroup->AttachResource(pResource.GetManagedResource());
				return pResource;
			}
			D3D11PixelShaderResource RegisterResource_PixelShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams        *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto pLoader = new D3D11PixelShaderLoaderFromFile
				(
					InURL,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetPixelShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11PixelShaderResource(pResource);
			}

			D3D11PixelShaderResource RegisterResource_PixelShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetPixelShader());
				resourceParams.SetName(InName);
				auto pResource = RegisterResource_PixelShaderFromFile(pInResMan, &resourceParams, pInCompileParams, InURL);
				pInGroup->AttachResource(pResource.GetManagedResource());
				return pResource;
			}
			D3D11PixelShaderResource RegisterResource_PixelShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetPixelShader());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pResource = RegisterResource_PixelShaderFromFile(pInResMan, &resourceParams, pInCompileParams, InURL);
				pInGroup->AttachResource(pResource.GetManagedResource());
				return pResource;
			}

			template<>
			D3D11ShaderResource<ID3D11PixelShader> RegisterResource_ShaderFromMemory<ID3D11PixelShader>
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				return RegisterResource_PixelShaderFromMemory(pInResMan, pInParams, pInCompileParams, pInSourceCode);
			}

			template<>
			D3D11ShaderResource<ID3D11VertexShader> RegisterResource_ShaderFromMemory<ID3D11VertexShader>
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				return RegisterResource_VertexShaderFromMemory(pInResMan, pInParams, pInCompileParams, pInSourceCode);
			}			
			template<>
			D3D11ShaderResource<ID3D11ComputeShader> RegisterResource_ShaderFromMemory<ID3D11ComputeShader>
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				return RegisterResource_ComputeShaderFromMemory(pInResMan, pInParams, pInCompileParams, pInSourceCode);;
			}
			template<>
			D3D11ShaderResource<ID3D11GeometryShader> RegisterResource_ShaderFromMemory<ID3D11GeometryShader>
			(
					ResManD3D11_Main                          *pInResMan,
					const ResMan::PublicResourceParams              *pInParams,
					const D3DCompile_Params                   *pInCompileParams,
					const std::vector<std::string>            *pInSourceCode
			)
			{
				return RegisterResource_GeometryShaderFromMemory(pInResMan, pInParams, pInCompileParams, pInSourceCode);;
			}
			template<>
			D3D11ShaderResource<ID3D11HullShader> RegisterResource_ShaderFromMemory<ID3D11HullShader>
			(
					ResManD3D11_Main                          *pInResMan,
					const ResMan::PublicResourceParams              *pInParams,
					const D3DCompile_Params                   *pInCompileParams,
					const std::vector<std::string>            *pInSourceCode
			)
			{
				return RegisterResource_HullShaderFromMemory(pInResMan, pInParams, pInCompileParams, pInSourceCode);;
			}
			template<>
			D3D11ShaderResource<ID3D11DomainShader> RegisterResource_ShaderFromMemory<ID3D11DomainShader>
			(
					ResManD3D11_Main                          *pInResMan,
					const ResMan::PublicResourceParams              *pInParams,
					const D3DCompile_Params                   *pInCompileParams,
					const std::vector<std::string>            *pInSourceCode
			)
			{
				return RegisterResource_DomainShaderFromMemory(pInResMan, pInParams, pInCompileParams, pInSourceCode);;
			}

			template<>
			D3D11ShaderResource<ID3D11VertexShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				return RegisterResource_VertexShaderFromFile(pInResMan, pInParams, pInCompileParams, InURL);
			}

			template<>
			D3D11ShaderResource<ID3D11PixelShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				return RegisterResource_PixelShaderFromFile(pInResMan, pInParams, pInCompileParams, InURL);
			}

			template<>
			D3D11ShaderResource<ID3D11ComputeShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				return RegisterResource_ComputeShaderFromFile(pInResMan, pInParams, pInCompileParams, InURL);
			}

			template<>
			D3D11ShaderResource<ID3D11GeometryShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				return RegisterResource_GeometryShaderFromFile(pInResMan, pInParams, pInCompileParams, InURL);
			}

			template<>
			D3D11ShaderResource<ID3D11HullShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				return RegisterResource_HullShaderFromFile(pInResMan, pInParams, pInCompileParams, InURL);
			}

			template<>
			D3D11ShaderResource<ID3D11DomainShader> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				return RegisterResource_DomainShaderFromFile(pInResMan, pInParams, pInCompileParams, InURL);
			}


			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetShader<ShaderObjectType>());
				resourceParams.SetName(InName);
				auto pShader = RegisterResource_ShaderFromMemory<ShaderObjectType>(pInResMan, &resourceParams, pInCompileParams, pInSourceCode);
				pInGroup->AttachResource(pShader.GetManagedResource());
				return pShader;
			}

			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetShader<ShaderObjectType>());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pShader = RegisterResource_ShaderFromMemory<ShaderObjectType>(pInResMan, &resourceParams, pInCompileParams, pInSourceCode);
				pInGroup->AttachResource(pShader.GetManagedResource());
				return pShader;
			}

			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetShader<ShaderObjectType>());
				resourceParams.SetRole(pInRole);
				auto pShader = RegisterResource_ShaderFromFile<ShaderObjectType>(pInResMan, &resourceParams, pInCompileParams, InURL);
				pInGroup->AttachResource(pShader.GetManagedResource());
				return pShader;
			}

			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetShader<ShaderObjectType>());
				resourceParams.SetName(InName);
				auto pShader = RegisterResource_ShaderFromFile<ShaderObjectType>(pInResMan, &resourceParams, pInCompileParams, InURL);
				pInGroup->AttachResource(pShader.GetManagedResource());
				return pShader;
			}

			template<class ShaderObjectType>
			D3D11ShaderResource<ShaderObjectType> RegisterResource_ShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                         InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetShader<ShaderObjectType>());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInRole);
				auto pShader = RegisterResource_ShaderFromFile<ShaderObjectType>(pInResMan, &resourceParams, pInCompileParams, InURL);
				pInGroup->AttachResource(pShader.GetManagedResource());
				return pShader;
			}

			D3D11ComputeShaderResource RegisterResource_ComputeShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto pLoader = new D3D11ComputeShaderLoaderFromFile
				(
					InURL,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetComputeShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11ComputeShaderResource(pResource);
			}

			D3D11ComputeShaderResource RegisterResource_ComputeShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto pLoader = new D3D11ComputeShaderLoaderFromMemory
				(
					pInSourceCode,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetComputeShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11ComputeShaderResource(pResource);
			}

			D3D11GeometryShaderResource RegisterResource_GeometryShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto pLoader = new D3D11GeometryShaderLoaderFromFile
				(
					InURL,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetGeometryShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11GeometryShaderResource(pResource);
			}

			D3D11GeometryShaderResource RegisterResource_GeometryShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto pLoader = new D3D11GeometryShaderLoaderFromMemory
				(
					pInSourceCode,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetGeometryShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11GeometryShaderResource(pResource);
			}

			D3D11HullShaderResource RegisterResource_HullShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto pLoader = new D3D11HullShaderLoaderFromFile
				(
					InURL,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetHullShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11HullShaderResource(pResource);
			}

			D3D11HullShaderResource RegisterResource_HullShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto pLoader = new D3D11HullShaderLoaderFromMemory
				(
					pInSourceCode,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetHullShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11HullShaderResource(pResource);
			}

			D3D11DomainShaderResource RegisterResource_DomainShaderFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::string                         &InURL
			)
			{
				auto pLoader = new D3D11DomainShaderLoaderFromFile
				(
					InURL,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetDomainShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11DomainShaderResource(pResource);
			}

			D3D11DomainShaderResource RegisterResource_DomainShaderFromMemory
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const D3DCompile_Params                   *pInCompileParams,
				const std::vector<std::string>            *pInSourceCode
			)
			{
				auto pLoader = new D3D11DomainShaderLoaderFromMemory
				(
					pInSourceCode,
					*pInCompileParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetDomainShader()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11DomainShaderResource(pResource);
			}
			*/

			/*******************************************************************************************************************************
			* ResManD3D11_Main Texture2D functions implementation
			*******************************************************************************************************************************/
			D3D11Texture2DResource RegisterResource_Texture2DFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const ResMan::PublicResourceParams              *pInParams,
				const Texture2DParams                     *pInTextureParams,
				const std::string                         &InURL
			)
			{
				auto pLoader = new D3D11Texture2DLoaderFromFile
				(
					InURL,
					*pInTextureParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetTexture2D()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11Texture2DResource(pResource);
			}

			D3D11Texture2DResource RegisterResource_Texture2DFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				const Texture2DParams                     *pInTextureParams,
				const std::string                         &InURL
			)
			{
				ResMan::PublicResourceParams params = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetTexture2D());
				params.SetName(InName);
				auto pTexture2DResource = RegisterResource_Texture2DFromFile(pInResMan, &params, pInTextureParams, InURL);
				if (pInGroup)
				{
					pInGroup->AttachResource(pTexture2DResource.GetManagedResource());
				}
				return pTexture2DResource;
			}

			D3D11Texture2DResource RegisterResource_Texture2DFromFile
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const Texture2DParams                     *pInTextureParams,
				const std::string                         &InURL
			)
			{
				ResMan::PublicResourceParams params = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetTexture2D());
				params.SetName(InName);
				params.SetRole(pInRole);
				auto pTexture2DResource = RegisterResource_Texture2DFromFile(pInResMan, &params, pInTextureParams, InURL);
				if (pInGroup)
				{
					pInGroup->AttachResource(pTexture2DResource.GetManagedResource());
				}
				return pTexture2DResource;
			}

			D3D11Texture2DResource RegisterResource_Texture2D_Uninitialized
			(
				ResManD3D11_Main                          *pInResMan,
				ResMan::PublicResourceParams              *pInParams,
				const GeneratedTexture2DParams            *pInTextureParams
			)
			{
				auto pLoader = new D3D11Texture2DLoader_CreateUninitialized
				(
					*pInTextureParams,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetTexture2D()
				);
				ResMan::IManagedResource *pResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11Texture2DResource(pResource);
			}
			
			D3D11Texture2DResource RegisterResource_Texture2D_Uninitialized
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				const GeneratedTexture2DParams            *pInTextureParams
			)
			{
				ResMan::PublicResourceParams params = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetTexture2D());
				params.SetName(InName);
				auto pTexture2DResource = RegisterResource_Texture2D_Uninitialized(pInResMan, &params, pInTextureParams);
				if (pInGroup != nullptr)
				{
					pInGroup->AttachResource(pTexture2DResource.GetManagedResource());
				}
				return pTexture2DResource;
			}

			D3D11Texture2DResource RegisterResource_Texture2D_Uninitialized
			(
				ResManD3D11_Main                          *pInResMan,
				const std::string&                        InName,
				ResMan::IManagedGroup                     *pInGroup,
				ResMan::ISubresourceRole                  *pInRole,
				const GeneratedTexture2DParams            *pInTextureParams
			)
			{
				ResMan::PublicResourceParams params = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetTexture2D());
				params.SetName(InName);
				params.SetRole(pInRole);
				auto pTexture2DResource = RegisterResource_Texture2D_Uninitialized(pInResMan, &params, pInTextureParams);
				if (pInGroup)
				{
					pInGroup->AttachResource(pTexture2DResource.GetManagedResource());
				}
				return pTexture2DResource;
			}

			/********************************************************************************************
			* ResManD3D11_Main buffer functions implementation
			********************************************************************************************/
			D3D11BufferResource RegisterResource_VertexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				ResMan::PublicResourceParams params = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBuffer());
				params.SetName(InName);
				auto pRegisteredResource = RegisterResource_Buffer(pInResMan, &params, InByteWidth, D3D11_BIND_VERTEX_BUFFER, InUsage, pInNullOrInitialContent);
				return pRegisteredResource;
			}
			D3D11BufferResource RegisterResource_VertexBuffer
			(
				ResManD3D11_Main                         *pInResMan,
				const std::string&                       InName,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent 
			)
			{
				auto pRole = GetSubresourceRole_Buffer_Std(pInResMan);
				return RegisterResource_VertexBuffer(pInResMan, InName, pRole, InByteWidth, InUsage, pInNullOrInitialContent);
			}

			D3D11BufferResource RegisterResource_VertexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				ResMan::PublicResourceParams params = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBuffer());
				params.SetName(InName);
				auto pRegisteredResource = RegisterResource_Buffer(pInResMan, &params, InByteWidth, D3D11_BIND_VERTEX_BUFFER, InUsage, pInNullOrInitialContent);
				if (pInGroup)
				{
					pInGroup->AttachResource(pRegisteredResource.GetManagedResource());
				}
				return pRegisteredResource;
			}

			D3D11BufferResource RegisterResource_VertexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				auto pRole = GetSubresourceRole_Buffer_Std(pInResMan);
				return RegisterResource_VertexBuffer(pInResMan, InName, pInGroup, pRole, InByteWidth, InUsage, pInNullOrInitialContent);
			}

			D3D11BufferResource RegisterResource_IndexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				ResMan::PublicResourceParams params = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBuffer());
				params.SetName(InName);
				auto pRegisteredResource = RegisterResource_Buffer(pInResMan, &params, InByteWidth, D3D11_BIND_INDEX_BUFFER, InUsage, pInNullOrInitialContent);
				return pRegisteredResource;
			}

			D3D11BufferResource RegisterResource_IndexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				auto pRole = GetSubresourceRole_Buffer_Std(pInResMan);
				return RegisterResource_IndexBuffer(pInResMan, InName, pRole, InByteWidth, InUsage, pInNullOrInitialContent);
			}

			D3D11BufferResource RegisterResource_IndexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				auto pRole = GetSubresourceRole_Buffer_Std(pInResMan);
				return RegisterResource_IndexBuffer(pInResMan, InName, pInGroup, pRole, InByteWidth, InUsage, pInNullOrInitialContent);
			}

			D3D11BufferResource RegisterResource_IndexBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				ResMan::PublicResourceParams params = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBuffer());
				params.SetName(InName);
				auto pRegisteredResource = RegisterResource_Buffer(pInResMan, &params, InByteWidth, D3D11_BIND_INDEX_BUFFER, InUsage, pInNullOrInitialContent);
				if (pInGroup)
				{
					pInGroup->AttachResource(pRegisteredResource.GetManagedResource());
				}
				return pRegisteredResource;
			}

			D3D11BufferResource RegisterResource_ConstantBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				auto pRole = GetSubresourceRole_Buffer_Std(pInResMan);
				return RegisterResource_ConstantBuffer(pInResMan, InName, pRole, InByteWidth, InUsage, pInNullOrInitialContent);
			}

			D3D11BufferResource RegisterResource_ConstantBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				ResMan::PublicResourceParams params = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBuffer());
				params.SetName(InName);
				auto pRegisteredResource = RegisterResource_Buffer(pInResMan, &params, InByteWidth, D3D11_BIND_CONSTANT_BUFFER, InUsage, pInNullOrInitialContent);
				return pRegisteredResource;
			}
			D3D11BufferResource RegisterResource_ConstantBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				auto pRole = GetSubresourceRole_Buffer_Std(pInResMan);
				return RegisterResource_ConstantBuffer(pInResMan, InName, pInGroup, pRole, InByteWidth, InUsage, pInNullOrInitialContent);
			}
			D3D11BufferResource RegisterResource_ConstantBuffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				ResMan::PublicResourceParams params = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBuffer());
				params.SetName(InName);
				auto pRegisteredResource = RegisterResource_Buffer(pInResMan, &params, InByteWidth, D3D11_BIND_CONSTANT_BUFFER, InUsage, pInNullOrInitialContent);
				if (pInGroup)
				{
					pInGroup->AttachResource(pRegisteredResource.GetManagedResource());
				}
				return pRegisteredResource;
			}

			D3D11_SUBRESOURCE_DATA *GetInitialContent_SubresourceData(const void *pInNullOrInitialContent, D3D11_SUBRESOURCE_DATA *pOutSubresourceData)
			{
				D3D11_SUBRESOURCE_DATA *pSubresourceDataArg = nullptr;
				if (pInNullOrInitialContent != nullptr)
				{
					ZeroMemory(pOutSubresourceData, sizeof(*pOutSubresourceData));
					pOutSubresourceData->pSysMem = pInNullOrInitialContent;
					pOutSubresourceData->SysMemPitch = 0;
					pOutSubresourceData->SysMemSlicePitch = 0;
					pSubresourceDataArg = pOutSubresourceData;
				}
				return pSubresourceDataArg;
			}

			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				const ResMan::PublicResourceParams      *pInParams,
				size_t                                   InByteWidth,
				UINT                                     InBindFlags,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				D3D11_BUFFER_DESC desc;
				ZeroMemory(&desc, sizeof(desc));
				desc.BindFlags = InBindFlags;
				desc.MiscFlags = 0;
				desc.Usage = InUsage;
				desc.ByteWidth = InByteWidth;
				desc.StructureByteStride = 0;
				desc.CPUAccessFlags = GetAccessFlags_ForUsage(InUsage);

				D3D11_SUBRESOURCE_DATA subresourceData;
				D3D11_SUBRESOURCE_DATA *pSubresourceDataArg = GetInitialContent_SubresourceData(pInNullOrInitialContent, &subresourceData);
				D3D11BufferResource pBufferResource = RegisterResource_Buffer(pInResMan, pInParams, InByteWidth, &desc, pSubresourceDataArg);
				return pBufferResource;
			}

			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				const std::string&                       InName,
				ResMan::IManagedGroup                   *pInGroup,
				size_t                                   InByteWidth,
				UINT                                     InBindFlags,
				D3D11_USAGE                              InUsage,
				const void                              *pInNullOrInitialContent
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBuffer());
				resourceParams.SetName(InName);
				D3D11BufferResource pBufferResource = RegisterResource_Buffer(pInResMan, &resourceParams, InByteWidth, InBindFlags,InUsage, pInNullOrInitialContent);
				if (pInGroup)
				{
					pInGroup->AttachResource(pBufferResource.GetManagedResource());
				}
				return pBufferResource;
			}
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
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBuffer());
				resourceParams.SetName(InName);
				resourceParams.SetRole(pInSubresourceRole);
				D3D11BufferResource pBufferResource = RegisterResource_Buffer(pInResMan, &resourceParams, InByteWidth, InBindFlags, InUsage, pInNullOrInitialContent);
				if (pInGroup)
				{
					pInGroup->AttachResource(pBufferResource.GetManagedResource());
				}
				return pBufferResource;
			}

			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				const ResMan::PublicResourceParams      *pInParams,
				size_t                                   InByteWidth,
				const D3D11_BUFFER_DESC                 *pInDesc,
				const D3D11_SUBRESOURCE_DATA            *pInNullOrSubresourceData
			)
			{
				auto pLoader = new D3D11BufferLoader
				(
					pInNullOrSubresourceData,
					*pInDesc,
					pInResMan->GetDev(),
					pInParams->GetRole(),
					pInResMan->GetResourceTypes()->GetBuffer()
				);

				ResMan::IManagedResource *pBufferResource = pInResMan->GetSubsystem()->GetManager()->RegisterResource(*pInParams, std::shared_ptr<ResMan::IResourceLoader>(pLoader));
				return D3D11BufferResource(pBufferResource);
			}

			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				ResMan::IManagedGroup                   *pInGroup,
				size_t                                   InByteWidth,
				const D3D11_BUFFER_DESC                 *pInDesc,
				const D3D11_SUBRESOURCE_DATA            *pInNullOrSubresourceData
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBuffer());
				auto pBufferResource = RegisterResource_Buffer(pInResMan, &resourceParams, InByteWidth, pInDesc, pInNullOrSubresourceData);
				if (pInGroup)
				{
					pInGroup->AttachResource(pBufferResource.GetManagedResource());
				}
				return pBufferResource;
			}
			D3D11BufferResource RegisterResource_Buffer
			(
				ResManD3D11_Main                        *pInResMan,
				ResMan::IManagedGroup                   *pInGroup,
				ResMan::ISubresourceRole                *pInSubresourceRole,
				size_t                                   InByteWidth,
				const D3D11_BUFFER_DESC                 *pInDesc,
				const D3D11_SUBRESOURCE_DATA            *pInNullOrSubresourceData
			)
			{
				auto resourceParams = GetDefaultResourceParams_ForResourceType_KeepLoaded(pInResMan->GetResourceTypes()->GetBuffer());
				resourceParams.SetRole(pInSubresourceRole);
				auto pBufferResource = RegisterResource_Buffer(pInResMan, &resourceParams, InByteWidth, pInDesc, pInNullOrSubresourceData);
				if (pInGroup)
				{
					pInGroup->AttachResource(pBufferResource.GetManagedResource());
				}
				return pBufferResource;
			}

			UINT GetAccessFlags_ForUsage(D3D11_USAGE InUsage)
			{
				auto res_flags = (InUsage == D3D11_USAGE_DYNAMIC) ? D3D11_CPU_ACCESS_WRITE : 0;
				if (InUsage == D3D11_USAGE_STAGING)
				{
					res_flags |= D3D11_CPU_ACCESS_WRITE;
					res_flags |= D3D11_CPU_ACCESS_READ;
				}
				return res_flags;
			}

			/********************************************************************************************************************************
			* Subresource role access functions
			********************************************************************************************************************************/
			ResMan::ISubresourceRole *GetSubresourceRole_TestResource_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetTestResource()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_Text_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetText()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_Buffer_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetBuffer()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_BlendState_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetBlendState()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_DepthStencilState_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetDepthStencilState()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_DepthStencilView_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetDepthStencilView()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_InputLayout_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetInputLayout()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_RasterizerState_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetRasterizerState()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_SamplerState_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetSamplerState()->GetRoles().pStd;
			}
			/*
			ResMan::ISubresourceRole *GetSubresourceRole_VertexShader_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetVertexShader()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_PixelShader_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetPixelShader()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_ComputeShader_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetComputeShader()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_GeometryShader_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetGeometryShader()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_HullShader_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetHullShader()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_DomainShader_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetDomainShader()->GetRoles().pStd;
			}
			*/
			ResMan::ISubresourceRole *GetSubresourceRole_Texture2D_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetTexture2D()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_ShaderView_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetShaderView()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_UnorderedAccessView_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetUnorderedAccessView()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_RenderTargetView_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetRenderTargetView()->GetRoles().pStd;
			}
			ResMan::ISubresourceRole *GetSubresourceRole_CommandList_Std(const ResManD3D11_Main *pResMan)
			{
				return pResMan->GetResourceTypes()->GetCommandList()->GetRoles().pStd;
			}

			/********************************************************************************************
			* ResManD3D11_Main implementation
			********************************************************************************************/
			ResManD3D11_Main::ResManD3D11_Main() :
				_pDeployer(&_basicDeployer)
			{
			}
			ResManD3D11_Main::~ResManD3D11_Main()
			{
			}

			void ResManD3D11_Main::Initialize(ResMan::IResourceManagerSubsystem *pInSubsystem)
			{
				assert(_pDeployer);

				Initialize_Texture2DResource_Class();

				_pSubsystem = pInSubsystem;
				_resourceTypes.Initialize(_pDeployer->GetDeployer_ResourceTypes(), pInSubsystem);
			}

			void ResManD3D11_Main::SetDeployer(ResManD3D11_IDeployer *pInDeployer)
			{
				assert(pInDeployer);
				_pDeployer = pInDeployer;
			}

			void ResManD3D11_Main::AttachD3D(ID3D11Device *pInDev, ID3D11DeviceContext *pInDevCon)
			{
				assert(pInDev != nullptr);
				assert(pInDevCon != nullptr);
				_pDev = pInDev;
				_pDevCon = pInDevCon;
			}
		}
	}
}