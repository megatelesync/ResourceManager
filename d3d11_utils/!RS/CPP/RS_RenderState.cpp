#include "../H/RS_RenderState.h"

#include <cassert>

namespace Dv
{
	namespace d3d_utils
	{
		// Fwd declarations
		void SnapshotDeviceShaderRenderState(ID3D11DeviceContext *pInDevContext, RS_ShaderState *pOutRS);
		void SnapshotDeviceRenderTargetRenderState(ID3D11DeviceContext *pInDevContext, RS_DepthStencilRenderTarget_ShaderState *pOutRS);

		void SnapshotShaderState_Compute(ID3D11DeviceContext *pInDevContext, RS_ComputeShaderState *pOutRS);
		void SnapshotShaderState_Geometry(ID3D11DeviceContext *pInDevContext, RS_GeometryShaderState *pOutRS);
		void SnapshotShaderState_Pixel(ID3D11DeviceContext *pInDevContext, RS_PixelShaderState *pOutRS);
		void SnapshotShaderState_Vertex(ID3D11DeviceContext *pInDevContext, RS_VertexShaderState *pOutRS);
		void SnapshotShaderState_Hull(ID3D11DeviceContext *pInDevContext, RS_HullShaderState *pOutRS);
		void SnapshotShaderState_Domain(ID3D11DeviceContext *pInDevContext, RS_DomainShaderState *pOutRS);

		void SetRS_ShaderState(ID3D11DeviceContext *pInDevContext, const RS_ShaderState *pRS);
		void SetRS_DepthStencilRenderTarget(ID3D11DeviceContext *pInDevContext, const RS_DepthStencilRenderTarget_ShaderState *pRS);

		void SetRS_ShaderCompute(ID3D11DeviceContext *pInDevContext, const RS_ComputeShaderState *pRS);
		void SetRS_ShaderGeometry(ID3D11DeviceContext *pInDevContext, const RS_GeometryShaderState *pRS);
		void SetRS_ShaderPixel(ID3D11DeviceContext *pInDevContext, const RS_PixelShaderState *pRS);
		void SetRS_ShaderVertex(ID3D11DeviceContext *pInDevContext, const RS_VertexShaderState *pRS);
		void SetRS_ShaderHull(ID3D11DeviceContext *pInDevContext, const RS_HullShaderState *pRS);
		void SetRS_ShadeDomain(ID3D11DeviceContext *pInDevContext, const RS_DomainShaderState *pRS);

		void SnapshotDeviceRenderState(ID3D11DeviceContext *pInDevContext, RS_RenderState *pOutRS)
		{
			RS_ShaderState shaderState;
			SnapshotDeviceShaderRenderState(pInDevContext, &shaderState);
			pOutRS->SetShaders(std::move(shaderState));

			RS_DepthStencilRenderTarget_ShaderState renderTargetState;
			SnapshotDeviceRenderTargetRenderState(pInDevContext, &renderTargetState);
			pOutRS->SetOutputMerger(std::move(renderTargetState));
			
			// TODO: Take snapshot
			ID3D11InputLayout *pInputLayout = nullptr;
			pInDevContext->IAGetInputLayout(&pInputLayout);
			pOutRS->SetInputLayout(pInputLayout);

			ID3D11Buffer *pIndexBuffer = nullptr;
			DXGI_FORMAT ib_format;
			UINT ib_offset;
			pInDevContext->IAGetIndexBuffer(&pIndexBuffer, &ib_format, &ib_offset);
			pOutRS->SetIndexBuffer(pIndexBuffer, ib_format, ib_offset);

			D3D11_PRIMITIVE_TOPOLOGY topology;
			pInDevContext->IAGetPrimitiveTopology(&topology);
			pOutRS->SetPrimTopology(topology);

			ID3D11BlendState *pBlendState = nullptr;
			float blendFactor[4];
			UINT sampleMask;
			pInDevContext->OMGetBlendState(&pBlendState, &(blendFactor[0]), &sampleMask);
			pOutRS->SetBlendState(pBlendState, blendFactor, sampleMask);

			ID3D11RasterizerState *pRasterizerState = nullptr;
			pInDevContext->RSGetState(&pRasterizerState);
			pOutRS->SetRasterizerState(pRasterizerState);
			
			std::vector<ID3D11Buffer *> vertexBuffers;
			RS_RenderState::VertexBufferStridesCont vertexBufferStrides;
			RS_RenderState::VertexBufferOffsetsCont vertexBufferOffsets;
			vertexBuffers.resize(D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT);
			vertexBufferStrides.resize(D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT);
			vertexBufferOffsets.resize(D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT);
			pInDevContext->IAGetVertexBuffers
			(
				0,
				D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT,
				&(vertexBuffers.front()),
				&(vertexBufferStrides.front()),
				&(vertexBufferOffsets.front())
			);
			RS_RenderState::VertexBuffersCont vertexBuffersUniquePtr;
			vertexBuffersUniquePtr.resize(D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT);
			for (int i = 0; i < D3D11_IA_VERTEX_INPUT_RESOURCE_SLOT_COUNT; i++)
			{
				vertexBuffersUniquePtr[i] = vertexBuffers[i];
			}
			pOutRS->SetVertexBuffers(std::move(vertexBuffersUniquePtr), std::move(vertexBufferStrides), std::move(vertexBufferOffsets));

			RS_RenderState::ViewportsCont viewports;
			size_t numViewports = 0;
			pInDevContext->RSGetViewports(&numViewports, nullptr);
			viewports.resize(numViewports);
			pInDevContext->RSGetViewports(&numViewports, &(viewports.front()));
			pOutRS->SetViewports(&(viewports.front()), numViewports);

			RS_RenderState::ScissorRects scissorRects;
			size_t numScissorRects = 0;
			pInDevContext->RSGetScissorRects(&numScissorRects, nullptr);
			scissorRects.resize(numScissorRects);
			pInDevContext->RSGetScissorRects(&numScissorRects, &(scissorRects.front()));
			pOutRS->SetScissorRects(std::move(scissorRects));
		}
		void SetRS(ID3D11DeviceContext *pInDevContext, const RS_RenderState *pRS)
		{
			assert(pInDevContext);
			assert(pRS);
			pInDevContext->IASetInputLayout(pRS->GetInputLayout());
			pInDevContext->IASetPrimitiveTopology(pRS->GetPrimTopology());
			pInDevContext->RSSetState(pRS->GetRasterizerState());
			pInDevContext->RSSetViewports(pRS->GetViewports()->size(), &(pRS->GetViewports()->front()));
			pInDevContext->OMSetBlendState(pRS->GetBlendState(), pRS->GetBlendFactor(), pRS->GetBlendSampleMask());
			pInDevContext->IASetIndexBuffer(pRS->GetIndexBuffer(), pRS->GetIndexBufferFormat(), pRS->GetIndexBufferOffset());
			pInDevContext->IASetVertexBuffers(0, pRS->GetNumVertexBuffers(), &(pRS->GetVertexBuffers()->front()), &(pRS->GetVertexBufferStrides()->front()), &(pRS->GetVertexBufferOffsets()->front()));
			pInDevContext->RSSetScissorRects(pRS->GetScissorRects()->size(), &(pRS->GetScissorRects()->front()));
			SetRS_ShaderState(pInDevContext, pRS->GetShaderState());
			SetRS_DepthStencilRenderTarget(pInDevContext, pRS->GetOutputMerger());
		}

		void SetRS_ShaderState(ID3D11DeviceContext *pInDevContext, const RS_ShaderState *pRS)
		{
			SetRS_ShaderCompute(pInDevContext, pRS->GetCompute());
			SetRS_ShaderGeometry(pInDevContext, pRS->GetGeometry());
			SetRS_ShaderPixel(pInDevContext, pRS->GetPixel());
			SetRS_ShaderVertex(pInDevContext, pRS->GetVertex());
			SetRS_ShaderHull(pInDevContext, pRS->GetHull());
			SetRS_ShadeDomain(pInDevContext, pRS->GetDomain());
		}

		void SetRS_ShaderCompute(ID3D11DeviceContext *pInDevContext, const RS_ComputeShaderState *pRS)
		{
			//pInDevContext->CSSetShader(pRS->GetShader(), &(pRS->GetClassInstances()->front()), pRS->GetClassInstances()->size());
		}
		void SetRS_ShaderGeometry(ID3D11DeviceContext *pInDevContext, const RS_GeometryShaderState *pRS)
		{
		}
		void SetRS_ShaderPixel(ID3D11DeviceContext *pInDevContext, const RS_PixelShaderState *pRS)
		{
		}
		void SetRS_ShaderVertex(ID3D11DeviceContext *pInDevContext, const RS_VertexShaderState *pRS)
		{
		}
		void SetRS_ShaderHull(ID3D11DeviceContext *pInDevContext, const RS_HullShaderState *pRS)
		{
		}
		void SetRS_ShadeDomain(ID3D11DeviceContext *pInDevContext, const RS_DomainShaderState *pRS)
		{
		}

		/*************************************************************************************************************************
		* RS_DepthStencilRenderTarget_ShaderState implementation
		*************************************************************************************************************************/
		void RS_DepthStencilRenderTarget_ShaderState::SetRenderTargets(ID3D11RenderTargetView **ppInSrc, size_t InCount)
		{
			_renderTargets.resize(InCount);
			for (int i = 0; i < InCount; i++)
			{
				_renderTargets[i] = ppInSrc[i];
			}
		}
		void RS_DepthStencilRenderTarget_ShaderState::SetUnorderedAccessViews(ID3D11UnorderedAccessView **ppInSrc, size_t InCount)
		{
			_unorderedAccessViews.resize(InCount);
			for (int i = 0; i < InCount; i++)
			{
				_unorderedAccessViews[i] = ppInSrc[i];
			}
		}
	}
} // Dv