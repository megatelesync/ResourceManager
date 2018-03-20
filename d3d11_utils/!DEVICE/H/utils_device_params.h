#pragma once

#include "Basic/MathUtil_Basic_IntSize.h"
#include "utils_device_utils.h"
#include "../../!UTILS/H/utils_com.h"

#include <vector>
#include <memory>
#include <iterator>
#include <Windows.h>
#include <D3D11.h>

#include <cstddef>
#include <cmath>
#include <Windows.h>


namespace Dv
{
	namespace d3d_utils
	{
		/***********************************************************************************
		* Converting rect to IntSize
		***********************************************************************************/
		inline DvUtils::Math::IntSize            GetRectSize              (LPRECT rect);

		/***********************************************************************************
		* App parameters of the render device, excluding the window handle.
		*
		* Default-constructible with default parameters: 
		* DX11_0 feature level device, debuggable multithreaded device, windowed, swap effect discard, R8G8B8A. 
		***********************************************************************************/
		struct DeviceParams
		{
			/***********************************************************************************
			* Public variables
			***********************************************************************************/
			// Parameters of the swap chain
			SwapChainParams										swapChain;

			// Feature levels of the device
			std::vector<D3D_FEATURE_LEVEL>						featureLevels;

			// Flags of the D3D11CreateDeviceAndSwapChain call:
			// Possible values include:
			//		D3D11_CREATE_DEVICE_SINGLETHREADED,
			//		D3D11_CREATE_DEVICE_DEBUG
			UINT												deviceFlags;


			/***********************************************************************************
			* Getters
			***********************************************************************************/
			DXGI_SWAP_CHAIN_DESC                                GetSwapChainDesc          (HWND InOutputWindow) const                             { return swapChain.GetDesc(InOutputWindow); }

			/***********************************************************************************
			* ctors
			***********************************************************************************/
			DeviceParams
			(				
				D3D_FEATURE_LEVEL                       InFeatureLevel                     = D3D_FEATURE_LEVEL_11_0,
				const SwapChainParams&                  InSwapChainParams                  = SwapChainParams(),
				UINT                                    InDeviceFlags                      = D3D11_CREATE_DEVICE_DEBUG
			);
		};
	} // d3d_utils
} // Dv