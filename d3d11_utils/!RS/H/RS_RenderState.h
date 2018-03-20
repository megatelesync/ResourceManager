#pragma once

#include "../../!UTILS/H/utils_com.h"

#include <vector>

namespace Dv
{
	namespace d3d_utils
	{
		/********************************************************************************************************************
		* State of the shader
		********************************************************************************************************************/
		template<class ShaderObjectType>
		class RS_ShaderStageState
		{
		public:
			~RS_ShaderStageState();

			using ClassInstancesCont = std::vector<ID3D11ClassInstance *>;

			/***************************************************************************************************************************
			* Getters
			***************************************************************************************************************************/
			ShaderObjectType                                                                  *GetShader                       () const                          { return _pShader; }

			const ClassInstancesCont                                                           GetClassInstances               () const                          { return &_classInstances; }

			size_t                                                                             GetNumUnorderedAccessViews      () const                          { return _unorderedAccessViews.size(); }
			ID3D11UnorderedAccessView                                                         *GetUnorderedAccessView          (size_t InIndex) const            { return _unorderedAccessViews[InIndex]; }

			size_t                                                                             GetNumConstantBuffers           () const                          { return _constantBuffers.size(); }
			ID3D11Buffer                                                                      *GetConstantBuffer               (size_t InIndex) const            { return _constantBuffers[InIndex]; }

			size_t                                                                             GetNumShaderResourceViews       () const                          { return _shaderResourceViews.size(); }
			ID3D11ShaderResourceView                                                          *GetShaderResourceView           (size_t InIndex) const            { return _shaderResourceViews[InIndex]; }

			size_t                                                                             GetNumSamplers                  () const                          { return _samplers.size(); }
			ID3D11SamplerState                                                                *GetSampler                      (size_t InIndex) const            { return _samplers[InIndex]; }
			
			/***************************************************************************************************************************
			* Initialization
			***************************************************************************************************************************/
			void                                                                               SetShader                       (ShaderObjectType *pInNewValue)                               { _pShader.reset(pInNewValue); }
			void                                                                               SetUnorderedAccessViews         (ID3D11UnorderedAccessView **ppInSrc, size_t InCount);
			void                                                                               SetConstantBuffers              (ID3D11Buffer **ppInSrc, size_t InCount);
			void                                                                               SetShaderResourceViews          (ID3D11ShaderResourceView **ppInSrc, size_t InCount);
			void                                                                               SetSamplers                     (ID3D11SamplerState **ppInSamplers, size_t InCount);

		private:
			ShaderObjectType                                                                  *_pShader;
			ClassInstancesCont                                                                 _classInstances;
			std::vector<ID3D11UnorderedAccessView *>                                           _unorderedAccessViews;
			std::vector<ID3D11Buffer *>                                                        _constantBuffers;
			std::vector<ID3D11ShaderResourceView *>                                            _shaderResourceViews;
			std::vector<ID3D11SamplerState *>                                                  _samplers;
		};

		/*************************************************************************************************************************
		* Concrete shaders
		*************************************************************************************************************************/
		using RS_ComputeShaderState                                                            = RS_ShaderStageState<ID3D11ComputeShader>;
		using RS_DomainShaderState                                                             = RS_ShaderStageState<ID3D11DomainShader>;
		using RS_GeometryShaderState                                                           = RS_ShaderStageState<ID3D11GeometryShader>;
		using RS_HullShaderState                                                               = RS_ShaderStageState<ID3D11HullShader>;
		using RS_PixelShaderState                                                              = RS_ShaderStageState<ID3D11PixelShader>;
		using RS_VertexShaderState                                                             = RS_ShaderStageState<ID3D11VertexShader>;

		/*************************************************************************************************************************
		* State of shaders and their constants
		*************************************************************************************************************************/
		class RS_ShaderState
		{
		public:
			const RS_PixelShaderState                             *GetPixel                     () const                   { return &_pixel; }
			const RS_VertexShaderState                            *GetVertex                    () const                   { return &_vertex; }
			const RS_ComputeShaderState                           *GetCompute                   () const                   { return &_compute; }
			const RS_GeometryShaderState                          *GetGeometry                  () const                   { return &_geometry; }
			const RS_DomainShaderState                            *GetDomain                    () const                   { return &_domain; }
			const RS_HullShaderState                              *GetHull                      () const                   { return &_hull; }

			void                                                   SetPixel                     (RS_PixelShaderState&& InValue)              { _pixel = std::move(InValue); }
			void                                                   SetVertex                    (RS_VertexShaderState&& InValue)             { _vertex = std::move(InValue); }
			void                                                   SetCompute                   (RS_ComputeShaderState&& InValue)            { _compute = std::move(InValue); }
			void                                                   SetGeometry                  (RS_GeometryShaderState&& InValue)           { _geometry = std::move(InValue); }
			void                                                   SetDomain                    (RS_DomainShaderState&& InValue)             { _domain = std::move(InValue); }
			void                                                   SetHull                      (RS_DomainShaderState&& InValue)             { _domain = std::move(InValue); }

		private:
			RS_PixelShaderState                                                                  _pixel;
			RS_VertexShaderState                                                                 _vertex;
			RS_ComputeShaderState                                                                _compute;
			RS_GeometryShaderState                                                               _geometry;
			RS_DomainShaderState                                                                 _domain;
			RS_HullShaderState                                                                   _hull;
		};

		/*************************************************************************************************************************
		* State of RT and depth stencil
		*************************************************************************************************************************/
		class RS_DepthStencilRenderTarget_ShaderState
		{
		public:
			using RenderTargetsCont                                 = std::vector<ID3D11RenderTargetView *>;
			using UnorderedAccessViewsCont                          = std::vector<ID3D11UnorderedAccessView *>;

			size_t                                                  GetNumRenderTargets          () const                                      { return _renderTargets.size(); }
			const RenderTargetsCont                                *GetRenderTargets             () const                                      { return &_renderTargets; }
			ID3D11DepthStencilView                                 *GetDepthStencilView          () const                                      { return _pDepthStencilView; }
			ID3D11DepthStencilState                                *GetDepthStencilState         () const                                      { return _pDepthStencilState; }
			const UnorderedAccessViewsCont                         *GetUnorderedAccessViews      () const                                      { return &_unorderedAccessViews; }

			void                                                    SetRenderTargets             (ID3D11RenderTargetView **ppInSrc, size_t InCount);
			void                                                    SetUnorderedAccessViews      (ID3D11UnorderedAccessView **ppInSrc, size_t InCount);
			void                                                    SetDepthStencilView          (ID3D11DepthStencilView *pInValue)            { _pDepthStencilView = pInValue; }
			void                                                    SetDepthStencilState         (ID3D11DepthStencilState *pInValue)           { _pDepthStencilState = pInValue; }

		private:
			RenderTargetsCont                                                                     _renderTargets;
			ID3D11DepthStencilView                                                               *_pDepthStencilView;
			ID3D11DepthStencilState                                                              *_pDepthStencilState;
			UnorderedAccessViewsCont                                                              _unorderedAccessViews;

		};



		/*************************************************************************************************************************
		* Render state saved by the rendering tester
		*************************************************************************************************************************/
		class RS_RenderState
		{
		public:
			using VertexBuffersCont                                                              = std::vector<ID3D11Buffer *>;
			using VertexBufferStridesCont                                                        = std::vector<UINT>;
			using VertexBufferOffsetsCont                                                        = std::vector<UINT>;
			using ViewportsCont                                                                  = std::vector<D3D11_VIEWPORT>;
			using ScissorRects                                                                   = std::vector<D3D11_RECT>;

			const RS_DepthStencilRenderTarget_ShaderState          *GetOutputMerger              () const                                       { return &_outputMerger; }
			const RS_ShaderState                                   *GetShaderState               () const                                       { return &_shaders; }
			ID3D11InputLayout                                      *GetInputLayout               () const                                       { return _pInputLayout; }
			ID3D11Buffer                                           *GetIndexBuffer               () const                                       { return _pIndexBuffer; }
			DXGI_FORMAT                                             GetIndexBufferFormat         () const                                       { return _indexBufferFormat; }
			UINT                                                    GetIndexBufferOffset         () const                                       { return _indexBufferOffset; }
			size_t                                                  GetNumVertexBuffers          () const                                       { return _vertexBuffers.size(); }
			const VertexBuffersCont                                *GetVertexBuffers             () const                                       { return &_vertexBuffers; }
			const VertexBufferStridesCont                          *GetVertexBufferStrides       () const                                       { return &_vertexBufferOffsets; }
			const VertexBufferOffsetsCont                          *GetVertexBufferOffsets       () const                                       { return &_vertexBufferStrides; }
			D3D11_PRIMITIVE_TOPOLOGY                                GetPrimTopology              () const                                       { return _primTopology; }
			ID3D11BlendState                                       *GetBlendState                () const                                       { return _pBlendState; }
			const float                                            *GetBlendFactor               () const                                       { return _blendFactor; }
			UINT                                                    GetBlendSampleMask           () const                                       { return _sampleMask; }

			ID3D11RasterizerState                                  *GetRasterizerState           () const                                       { return _pRasterizerState; }
			const ViewportsCont                                    *GetViewports                 () const                                       { return &_viewports; }
			const ScissorRects                                     *GetScissorRects              () const                                       { return &_scissorRects; }

			void                                                    SetOutputMerger              (RS_DepthStencilRenderTarget_ShaderState &&InValue) { _outputMerger = std::move(InValue); }
			void                                                    SetShaders                   (RS_ShaderState&& InValue)                          { _shaders = std::move(InValue); }
			void                                                    SetInputLayout               (ID3D11InputLayout *pInValue)                       { _pInputLayout = pInValue; }
			void                                                    SetIndexBuffer               (ID3D11Buffer *pInValue, DXGI_FORMAT InFormat, UINT InOffset) { _pIndexBuffer = pInValue; _indexBufferFormat = InFormat; _indexBufferOffset = InOffset; }
			void                                                    SetPrimTopology              (D3D11_PRIMITIVE_TOPOLOGY InValue)                  { _primTopology = InValue; }
			void                                                    SetBlendState                (ID3D11BlendState *pInValue, const float *pInBlendFactor, UINT InSampleMask) { _pBlendState = pInValue; _blendFactor[0] = pInBlendFactor[0]; _blendFactor[1] = pInBlendFactor[1]; _blendFactor[2] = pInBlendFactor[2]; _blendFactor[3] = pInBlendFactor[3]; _sampleMask = InSampleMask; }
			void                                                    SetRasterizerState           (ID3D11RasterizerState *pInValue)                   { _pRasterizerState = pInValue; }
			void                                                    SetVertexBuffers             (VertexBuffersCont&& InBuffers, VertexBufferStridesCont&& InStrides, VertexBufferOffsetsCont&& InOffsets);
			void                                                    SetViewports                 (const D3D11_VIEWPORT *ppInSrc, size_t InCount);
			void                                                    SetScissorRects              (ScissorRects&& InValue);

		private:
			RS_DepthStencilRenderTarget_ShaderState                                               _outputMerger;
			RS_ShaderState                                                                        _shaders;
			ID3D11InputLayout                                                                    *_pInputLayout;
			ID3D11Buffer                                                                         *_pIndexBuffer;
			DXGI_FORMAT                                                                           _indexBufferFormat;
			UINT                                                                                  _indexBufferOffset;
			VertexBuffersCont                                                                     _vertexBuffers;
			VertexBufferStridesCont                                                               _vertexBufferStrides;
			VertexBufferOffsetsCont                                                               _vertexBufferOffsets;

			D3D11_PRIMITIVE_TOPOLOGY                                                              _primTopology;
			ID3D11BlendState                                                                     *_pBlendState;
			float                                                                                 _blendFactor[4];
			UINT                                                                                  _sampleMask;
			ID3D11RasterizerState                                                                *_pRasterizerState;
			ViewportsCont                                                                         _viewports;
			ScissorRects                                                                          _scissorRects;
		};
		void                                      SnapshotDeviceRenderState                       (ID3D11DeviceContext *pInDevContext, RS_RenderState *pOutRS);
		void                                      SetRS                                           (ID3D11DeviceContext *pInDevContext, const RS_RenderState *pRS);


		/********************************************************************************************************************
		* RS_ShaderStageState implementation
		********************************************************************************************************************/
		template<class ShaderObjectType>
		void RS_ShaderStageState<ShaderObjectType>::SetUnorderedAccessViews(ID3D11UnorderedAccessView **ppInSrc, size_t InCount)
		{
			_unorderedAccessViews.resize(InCount);
			for (int i = 0; i < InCount; i++)
			{
				_unorderedAccessViews[i] = *ppInSrc[i];
			}
		}

		template<class ShaderObjectType>
		void RS_ShaderStageState<ShaderObjectType>::SetConstantBuffers(ID3D11Buffer **ppInSrc, size_t InCount)
		{
			_constantBuffers.resize(InCount);
			for (int i = 0; i < InCount; i++)
			{
				_constantBuffers[i] = *ppInSrc[i];
			}
		}

		template<class ShaderObjectType>
		void RS_ShaderStageState<ShaderObjectType>::SetShaderResourceViews(ID3D11ShaderResourceView **ppInSrc, size_t InCount)
		{
			_shaderResourceViews.resize(InCount);
			for (int i = 0; i < InCount; i++)
			{
				_shaderResourceViews[i] = *ppInSrc[i];
			}
		}

		template<class ShaderObjectType>
		void RS_ShaderStageState<ShaderObjectType>::SetSamplers(ID3D11SamplerState **ppInSamplers, size_t InCount)
		{
			_samplers.resize(InCount);
			for (int i = 0; i < InCount; i++)
			{
				_samplers[i] = *ppInSamplers[i];
			}
		}

		template<class ShaderObjectType>
		RS_ShaderStageState<ShaderObjectType>::~RS_ShaderStageState()
		{
		}
	} // d3d_utils
} // Dv