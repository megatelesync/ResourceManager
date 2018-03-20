#include "../H/utils_device_utils.h"

namespace Dv
{
	namespace d3d_utils
	{
		/*******************************************************************************************************
		* ClearConfig implementation
		*******************************************************************************************************/
		ClearConfig::ClearConfig()
		{
			bClearRenderTarget = true;
			renderTargetColor[0] = 0.0F;
			renderTargetColor[1] = 1.0F;
			renderTargetColor[2] = 0.0F;
			renderTargetColor[3] = 0.0F;
		}

		/*******************************************************************************************************
		* PresentConfig implementation
		*******************************************************************************************************/
		PresentConfig::PresentConfig() :
			SyncInterval(0),
			flags(0)
		{
		}

		void Present(IDXGISwapChain *pSwapChain, const PresentConfig& InConfig)
		{
			pSwapChain->Present(InConfig.SyncInterval, InConfig.flags);
		}

		/***************************************************************************
		* Viewport helper functions implementation
		***************************************************************************/
		D3D11_VIEWPORT CreateScaleViewport(FLOAT Width, FLOAT Height)
		{
			return CreateViewport(0.0F, 0.0F, Width, Height, 0.0F, 1.0F);
		}

		D3D11_VIEWPORT CreateViewport
		(
			FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth, FLOAT MaxDepth
		)
		{
			assert(MinDepth <= 1.0F);
			assert(MinDepth >= 0.0F);
			assert(MaxDepth <= 1.0f);
			assert(MaxDepth >= 0.0F);
			D3D11_VIEWPORT res;
			ZeroMemory(&res, sizeof(res));
			res.TopLeftX          = TopLeftX;
			res.TopLeftY          = TopLeftY;
			res.Width             = Width;
			res.Height            = Height;
			res.MinDepth          = MinDepth;
			res.MaxDepth          = MaxDepth;
			return res;
		}

		/***************************************************************************
		* SwapChainParams implementation
		***************************************************************************/
		SwapChainParams::SwapChainParams() :
			bWindowed(true),
			backBufferFormat(DXGI_FORMAT_R8G8B8A8_UNORM),
			swapEffect(DXGI_SWAP_EFFECT_DISCARD),
			flags(0)
		{
		}

		SwapChainParams::SwapChainParams
		(
			BOOL bWindowedIn,
			DXGI_FORMAT backBufferFormatIn,
			DXGI_SWAP_EFFECT swapEffectIn,
			UINT flagsIn
		) :
			bWindowed(bWindowedIn),
			backBufferFormat(backBufferFormatIn),
			swapEffect(swapEffectIn),
			flags(flagsIn)
		{
		}


		DXGI_SWAP_CHAIN_DESC SwapChainParams::GetDesc(HWND outputWindowIn) const
		{
			return GetSwapChainDesc(outputWindowIn, bWindowed, backBufferFormat, swapEffect, flags);
		}

		/***************************************************************************
		* SwapChainParams external functions implementation
		***************************************************************************/
		DXGI_SWAP_CHAIN_DESC GetSwapChainDesc
		(
			HWND outputWindow,
			BOOL bWindowed,
			DXGI_FORMAT backBufferFormat,
			DXGI_SWAP_EFFECT swapEffect,
			UINT flags
		)
		{
			RECT wndRect;
			GetWindowRect(outputWindow, &wndRect);
			UINT wndWidth = wndRect.right - wndRect.left + 1;
			UINT wndHeight = wndRect.bottom - wndRect.top + 1;
			DXGI_SWAP_CHAIN_DESC res;
			ZeroMemory(&res, sizeof(res));
			res.OutputWindow = outputWindow;
			res.Windowed = bWindowed;
			res.Flags = flags;
			// Back buffer desc
			res.BufferCount = 1;
			res.BufferDesc.Format = backBufferFormat;
			res.BufferDesc.Width = wndWidth;
			res.BufferDesc.Height = wndHeight;
			res.BufferDesc.RefreshRate.Numerator = 60;
			res.BufferDesc.RefreshRate.Denominator = 1;
			res.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			// Swap effect
			res.SwapEffect = swapEffect;
			// Multisampling
			res.SampleDesc.Count = 1;
			res.SampleDesc.Quality = 0;
			return res;
		}
	} // d3d_utils
} // Dv