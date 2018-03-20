#pragma once

#include "Deployer/ResManD3D11_Deployer_ResourceTypes.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*****************************************************************************
			* Computing default resource param
			******************************************************************************/

			// Compute default parameters for the given resource type:
			// By default, all applicable subresource roles are to be loaded, 
			// only depedencies (not subresources) are to be kept loaded.
			template<class ResTypeDescrArg>
			ResMan::PublicResourceParams GetDefaultResourceParams_ForResourceType
			(
				ResMan::TypedResourceType<ResTypeDescrArg>                        *pInResourceTypeDescr
			);

			template<class ResTypeDescrArg>
			ResMan::PublicResourceParams GetDefaultResourceParams_ForResourceType
			(
				ResMan::TypedResourceType<ResTypeDescrArg>                        *pInResourceTypeDescr,
				const ResMan::CombinableParams                                    *pInCombinableParams
			);

			// Compute default resource params for resource to be kept loaded
			template<class ResTypeDescrArg>
			ResMan::PublicResourceParams GetDefaultResourceParams_ForResourceType_KeepLoaded
			(
				ResMan::TypedResourceType<ResTypeDescrArg>                        *pInResourceTypeDescr
			);

			// Compute default resource params for resource with the given combinable params
			template<class ResTypeDescrArg>
			ResMan::PublicResourceParams GetDefaultResourceParams_ForResourceType_KeepLoaded
			(
				ResMan::TypedResourceType<ResTypeDescrArg>                        *pInResourceTypeDescr,
				const ResMan::CombinableParams                                    *pInCombinableParams
			);

			/****************************************************************************
			* All resource types
			*****************************************************************************/
			class ResManD3D11_ResourceTypes
			{
			public:
				~ResManD3D11_ResourceTypes();

				/********************************************************************************************
				* Initializes resource types:
				*
				* Registers types using the given subsystem
				********************************************************************************************/
				void                                                          Initialize(ResManD3D11_ResourceTypes_IDeployer *pInDeployer, ResMan::IResourceManagerSubsystem *pInSubsystem);

				/********************************************************************************************
				* Getters
				********************************************************************************************/
				const TestResourceType                                       *GetTestResource() const { return &_testResource; }
				TestResourceType                                             *GetTestResource() { return &_testResource; }

				const TextResourceType                                       *GetText() const { return &_text; }
				TextResourceType                                             *GetText() { return &_text; }
				const D3D11BlendStateResourceType                            *GetBlendState() const { return &_blendState; }
				D3D11BlendStateResourceType                                  *GetBlendState() { return &_blendState; }
				const D3D11BufferResourceType                                *GetBuffer() const { return &_buffer; }
				D3D11BufferResourceType                                      *GetBuffer() { return &_buffer; }
				const D3D11DepthStencilStateResourceType                     *GetDepthStencilState() const { return &_depthStencilState; }
				D3D11DepthStencilStateResourceType                           *GetDepthStencilState() { return &_depthStencilState; }
				const D3D11DepthStencilViewResourceType                      *GetDepthStencilView() const { return &_depthStencilView; }
				D3D11DepthStencilViewResourceType                            *GetDepthStencilView() { return &_depthStencilView; }
				const D3D11ShaderViewResourceType                            *GetShaderView() const { return &_shaderView; }
				D3D11ShaderViewResourceType                                  *GetShaderView() { return &_shaderView; }
				const D3D11UnorderedAccessViewResourceType                   *GetUnorderedAccessView() const { return &_unorderedAccessView; }
				D3D11UnorderedAccessViewResourceType                         *GetUnorderedAccessView() { return &_unorderedAccessView; }
				const D3D11InputLayoutResourceType                           *GetInputLayout() const { return &_inputLayout; }
				D3D11InputLayoutResourceType                                 *GetInputLayout() { return &_inputLayout; }
				const D3D11SamplerStateResourceType                          *GetSamplerState() const { return &_samplerState; }
				D3D11SamplerStateResourceType                                *GetSamplerState() { return &_samplerState; }
				const D3D11RasterizerStateResourceType                       *GetRasterizerState() const { return &_rasterizerState; }
				D3D11RasterizerStateResourceType                             *GetRasterizerState() { return &_rasterizerState; }
				const D3D11RenderTargetViewResourceType                      *GetRenderTargetView() const { return &_renderTargetView; }
				D3D11RenderTargetViewResourceType                            *GetRenderTargetView() { return &_renderTargetView; }
				const D3D11CommandListResourceType                           *GetCommandList() const { return &_commandList; }
				D3D11CommandListResourceType                                 *GetCommandList() { return &_commandList; }

				//template<class ShaderObjectType>
				//const D3D11ShaderResourceType<ShaderObjectType>              *GetShader() const;

				//template<class ShaderObjectType>
				//D3D11ShaderResourceType<ShaderObjectType>                    *GetShader();

				//template<>
				//const D3D11ShaderResourceType<ID3D11VertexShader>            *GetShader<ID3D11VertexShader>() const { return GetVertexShader(); }

				//template<>
				//D3D11ShaderResourceType<ID3D11VertexShader>                  *GetShader<ID3D11VertexShader>() { return GetVertexShader(); }

				//template<>
				//const D3D11ShaderResourceType<ID3D11PixelShader>            *GetShader<ID3D11PixelShader>() const { return GetPixelShader(); }

				//template<>
				//D3D11ShaderResourceType<ID3D11PixelShader>                  *GetShader<ID3D11PixelShader>() { return GetPixelShader(); }

				//template<>
				//const D3D11ShaderResourceType<ID3D11ComputeShader>          *GetShader<ID3D11ComputeShader>() const { return GetComputeShader(); }

				//template<>
				//D3D11ShaderResourceType<ID3D11ComputeShader>                *GetShader<ID3D11ComputeShader>() { return GetComputeShader(); }

				//template<>
				//const D3D11ShaderResourceType<ID3D11GeometryShader>          *GetShader<ID3D11GeometryShader>() const { return GetGeometryShader(); }

				//template<>
				//D3D11ShaderResourceType<ID3D11GeometryShader>                *GetShader<ID3D11GeometryShader>() { return GetGeometryShader(); }

				//template<>
				//const D3D11ShaderResourceType<ID3D11HullShader>              *GetShader<ID3D11HullShader>() const { return GetHullShader(); }

				//template<>
				//D3D11ShaderResourceType<ID3D11HullShader>                    *GetShader<ID3D11HullShader>() { return GetHullShader(); }

				//template<>
				//const D3D11ShaderResourceType<ID3D11DomainShader>            *GetShader<ID3D11DomainShader>() const { return GetDomainShader(); }

				//template<>
				//D3D11ShaderResourceType<ID3D11DomainShader>                  *GetShader<ID3D11DomainShader>() { return GetDomainShader(); }

				//const D3D11ShaderResourceType<ID3D11VertexShader>            *GetVertexShader() const { return &_vertexShader; }
				//D3D11ShaderResourceType<ID3D11VertexShader>                  *GetVertexShader() { return &_vertexShader; }
				//const D3D11ShaderResourceType<ID3D11PixelShader>             *GetPixelShader() const { return &_pixelShader; }
				//D3D11ShaderResourceType<ID3D11PixelShader>                   *GetPixelShader() { return &_pixelShader; }
				//const D3D11ShaderResourceType<ID3D11ComputeShader>           *GetComputeShader() const { return &_computeShader; }
				//D3D11ShaderResourceType<ID3D11ComputeShader>                 *GetComputeShader() { return &_computeShader; }
				//const D3D11ShaderResourceType<ID3D11GeometryShader>          *GetGeometryShader() const { return &_geometryShader; }
				//D3D11ShaderResourceType<ID3D11GeometryShader>                *GetGeometryShader() { return &_geometryShader; }
				//const D3D11ShaderResourceType<ID3D11HullShader>              *GetHullShader() const { return &_hullShader; }
				//D3D11ShaderResourceType<ID3D11HullShader>                    *GetHullShader() { return &_hullShader; }
				//const D3D11ShaderResourceType<ID3D11DomainShader>            *GetDomainShader() const { return &_domainShader; }
				//D3D11ShaderResourceType<ID3D11DomainShader>                  *GetDomainShader() { return &_domainShader; }

				const D3D11Texture2DResourceType                             *GetTexture2D() const { return &_texture2D; }
				D3D11Texture2DResourceType                                   *GetTexture2D() { return &_texture2D; }

			private:
				TestResourceType                                              _testResource;

				TextResourceType                                              _text;

				D3D11BlendStateResourceType                                   _blendState;
				D3D11BufferResourceType                                       _buffer;
				D3D11DepthStencilStateResourceType                            _depthStencilState;
				D3D11DepthStencilViewResourceType                             _depthStencilView;
				D3D11InputLayoutResourceType                                  _inputLayout;
				D3D11SamplerStateResourceType                                 _samplerState;
				//D3D11ShaderResourceType<ID3D11VertexShader>                   _vertexShader;
				//D3D11ShaderResourceType<ID3D11PixelShader>                    _pixelShader;
				//D3D11ShaderResourceType<ID3D11ComputeShader>                  _computeShader;
				//D3D11ShaderResourceType<ID3D11GeometryShader>                 _geometryShader;
				//D3D11ShaderResourceType<ID3D11HullShader>                     _hullShader;
				//D3D11ShaderResourceType<ID3D11DomainShader>                   _domainShader;
				D3D11Texture2DResourceType                                    _texture2D;
				D3D11RasterizerStateResourceType                              _rasterizerState;
				D3D11ShaderViewResourceType                                   _shaderView;
				D3D11UnorderedAccessViewResourceType                          _unorderedAccessView;
				D3D11RenderTargetViewResourceType                             _renderTargetView;
				D3D11CommandListResourceType                                  _commandList;
			};

			/*******************************************************************************************************************
			* ResourceParams computation implementation
			*******************************************************************************************************************/
			template<class ResTypeDescrArg>
			ResMan::PublicResourceParams GetDefaultResourceParams_ForResourceType
			(
				ResMan::TypedResourceType<ResTypeDescrArg>                        *pInResourceTypeDescr
			)
			{
				ResMan::PublicResourceParams params;
				params.SetRole(pInResourceTypeDescr->GetRoles().pStd);
				auto applicableSubresourceRoles = pInResourceTypeDescr->GetApplicableSubresources();
				params.GetCombinableParams()->SetSubresourceRolesToKeepDependencies(applicableSubresourceRoles);
				return params;
			}

			template<class ResTypeDescrArg>
			ResMan::PublicResourceParams GetDefaultResourceParams_ForResourceType
			(
				ResMan::TypedResourceType<ResTypeDescrArg>                        *pInResourceTypeDescr,
				const ResMan::CombinableParams                                    *pInCombinableParams
			)
			{
				ResMan::PublicResourceParams params;
				params.SetRole(pInResourceTypeDescr->GetRoles().pStd);
				*params.GetCombinableParams() = *pInCombinableParams;
				return params;
			}

			template<class ResTypeDescrArg>
			ResMan::PublicResourceParams GetDefaultResourceParams_ForResourceType_KeepLoaded
			(
				ResMan::TypedResourceType<ResTypeDescrArg>                        *pInResourceTypeDescr
			)
			{
				auto params = GetDefaultResourceParams_ForResourceType(pInResourceTypeDescr);
				auto applicableSubresourceRoles = pInResourceTypeDescr->GetApplicableSubresources();
				params.GetCombinableParams()->SetSubresourceRolesToLoad(applicableSubresourceRoles);
				params.GetCombinableParams()->SetSubresourceRolesToKeepLoaded(applicableSubresourceRoles);
				return params;
			}

			template<class ResTypeDescrArg>
			ResMan::PublicResourceParams GetDefaultResourceParams_ForResourceType_KeepLoaded
			(
				ResMan::TypedResourceType<ResTypeDescrArg>                        *pInResourceTypeDescr,
				const ResMan::CombinableParams                                    *pInCombinableParams
			)
			{
				auto params = GetDefaultResourceParams_ForResourceType(pInResourceTypeDescr);
				*params.GetCombinableParams() = *pInCombinableParams;
				return params;
			}
		}
	}
}
