#pragma once

#include "Basic/MathUtil_Basic_IntSize.h"
#include <Windows.h>
#include <DXGI.h>
#include <cassert>
#include <d3d11.h>
#include <d3dcompiler.h>

namespace Dv
{
	namespace d3d_utils
	{
		/************************************************************************
		* Parameters of the clearing
		************************************************************************/
		struct ClearConfig
		{
			/********************************************************************
			* Public variables
			********************************************************************/
			// true if clearing render target is enabled 
			bool            bClearRenderTarget;
			// color to fill the render target on clearing
			float           renderTargetColor[4];

			/*******************************************************************
			* Ctors
			********************************************************************/
			// default ctor
			ClearConfig();
		};

		/************************************************************************
		* Parameters of the present function
		************************************************************************/
		struct PresentConfig
		{
			/************************************************************************
			* Public variables
			************************************************************************/
			UINT			SyncInterval;
			UINT			flags;

			/************************************************************************
			* Ctors
			************************************************************************/
			// default ctor
			PresentConfig();
		};
		void Present(IDXGISwapChain *pSwapChain, const PresentConfig& InConfig);

		/***************************************************************************
		* Viewport helper functions
		***************************************************************************/
		D3D11_VIEWPORT CreateViewport
		(
			FLOAT TopLeftX, FLOAT TopLeftY, FLOAT Width, FLOAT Height, FLOAT MinDepth, FLOAT MaxDepth
		);
		// Create viewport with zero origin and MaxZ = 1.0F, MinZ = 0.0F
		D3D11_VIEWPORT CreateScaleViewport(FLOAT Width, FLOAT Height);

		/***************************************************************************************
		* Parameters of the swap chain (the DXGI_SWAP_CHAIN_DESC) except the window handle.
		*
		* Default-constructible: initializes with default parameters.
		****************************************************************************************/
		struct SwapChainParams
		{
			/**************************************************************************
			* Public members
			**************************************************************************/
			BOOL							bWindowed;
			DXGI_FORMAT						backBufferFormat;
			DXGI_SWAP_EFFECT				swapEffect;
			// Flags field of the SwapChain parameter of the D3D11CreateDeviceAndSwapChain call.
			// Possible values are DXGI_SWAP_CHAIN_FLAG enumeration values			
			UINT							flags;

			/**************************************************************************
			* Getters
			**************************************************************************/
			DXGI_SWAP_CHAIN_DESC            GetDesc(HWND InOutputWindow) const;

			/**************************************************************************
			* Ctors
			**************************************************************************/
			SwapChainParams();
			SwapChainParams
			(
				BOOL                       bInWindowedIn,
				DXGI_FORMAT                InBackBufferFormat,
				DXGI_SWAP_EFFECT           InSwapEffect,
				UINT                       InFlags
			);
		};

		/******************************************************************************
		* Creates a swap chain desc with a buffer size corresponding a given window
		*******************************************************************************/
		DXGI_SWAP_CHAIN_DESC GetSwapChainDesc
		(
			HWND                            outputWindow,
			BOOL                            bWindowed,
			DXGI_FORMAT                     backBufferFormat,
			DXGI_SWAP_EFFECT                swapEffect,
			UINT                            flags
		);

		std::string GetFeatureLevelString(D3D_FEATURE_LEVEL featureLevel);
		std::string GetDeviceFlagsString(UINT InDeviceFlags);
		std::string GetSingleDeviceFlagString(UINT InDeviceFlag);
		std::string GetShaderCompileFlagsString(UINT InFlag);
		std::string GetSingleShaderCompileFlagString(UINT InShaderCompilerFlags);
		std::string GetDevceHRESULTString(HRESULT hr);
		std::string GetFormattedHRESULTString(HRESULT hr, std::string_view hrDescr);
		std::string GetCreateDeviceHRESULTDescription(HRESULT hr);
		std::string GetCommonHRESULTDescription(HRESULT hr);
		std::string GetSingleClearFlagString(UINT InFlag);
		std::string GetClearFlagsString(UINT InClearFlags);

		std::string GetFormatString(DXGI_FORMAT InFormat);

		std::string GetSwapEffectString(DXGI_SWAP_EFFECT InSwapEffect);

		inline std::string GetFormatString(DXGI_FORMAT InFormat)
		{
			switch (InFormat)
			{
			case DXGI_FORMAT_R32G32B32A32_TYPELESS:
				return "{R32G32B32A32_TYPELESS}";

			case DXGI_FORMAT_R32G32B32A32_FLOAT:
				return "{R32G32B32A32_FLOAT}";

			case DXGI_FORMAT_R32G32B32A32_UINT:
				return "{R32G32B32A32_UINT}";

			case DXGI_FORMAT_R32G32B32A32_SINT:
				return "{R32G32B32A32_SINT}";

			case DXGI_FORMAT_R32G32B32_TYPELESS:
				return "{R32G32B32_TYPELESS}";

			case DXGI_FORMAT_R32G32B32_FLOAT:
				return "{R32G32B32_FLOAT}";

			case DXGI_FORMAT_R32G32B32_UINT:
				return "{R32G32B32_UINT}";

			case DXGI_FORMAT_R32G32B32_SINT:
				return "{R32G32B32_SINT}";

			case DXGI_FORMAT_R16G16B16A16_TYPELESS:
				return "{R16G16B16A16_TYPELESS}";

			case DXGI_FORMAT_R16G16B16A16_FLOAT:
				return "{R16G16B16A16_FLOAT}";

			case DXGI_FORMAT_R16G16B16A16_UNORM:
				return "{R16G16B16A16_UNORM}";

			case DXGI_FORMAT_R16G16B16A16_UINT:
				return "{R16G16B16A16_UINT}";

			case DXGI_FORMAT_R16G16B16A16_SNORM:
				return "{R16G16B16A16_SNORM}";

			case DXGI_FORMAT_R16G16B16A16_SINT:
				return "{R16G16B16A16_SINT}";

			case DXGI_FORMAT_R32G32_TYPELESS:
				return "{R32G32_TYPELESS}";

			case DXGI_FORMAT_R32G32_FLOAT:
				return "{R32G32_FLOAT}";

			case DXGI_FORMAT_R32G32_UINT:
				return "{R32G32_UINT}";

			case DXGI_FORMAT_R32G32_SINT:
				return "{R32G32_SINT}";

			case DXGI_FORMAT_R32_TYPELESS:
				return "{R32_TYPELESS}";

			case DXGI_FORMAT_R32_FLOAT:
				return "{R32_FLOAT}";

			case DXGI_FORMAT_R32_UINT:
				return "{R32_UINT}";

			case DXGI_FORMAT_R32_SINT:
				return "{R32_SINT}";


			case DXGI_FORMAT_R8G8B8A8_TYPELESS:
				return "{R8G8B8A8_TYPELESS}";

			case DXGI_FORMAT_R8G8B8A8_UNORM:
				return "{R8G8B8A8_UNORM}";

			case DXGI_FORMAT_R8G8B8A8_SNORM:
				return "{R8G8B8A8_SNORM}";

			case DXGI_FORMAT_R8G8B8A8_UNORM_SRGB:
				return "{R8G8B8A8_UNORM_SRGB}";

			case DXGI_FORMAT_R8G8B8A8_UINT:
				return "{R8G8B8A8_UINT}";

			case DXGI_FORMAT_R8G8B8A8_SINT:
				return "{R8G8B8A8_SINT}";

			case DXGI_FORMAT_D32_FLOAT_S8X24_UINT:
				return "{D32_FLOAT_S8X24_UINT}";

			case DXGI_FORMAT_D32_FLOAT:
				return "{D32_FLOAT}";

			case DXGI_FORMAT_D24_UNORM_S8_UINT:
				return "{D24_UNORM_S8_UINT}";

			case DXGI_FORMAT_D16_UNORM:
				return "{D16_UNORM}";

			default:
				break;
			}

			return "{UNKNOWN}";
		}

		inline std::string GetSwapEffectString(DXGI_SWAP_EFFECT InSwapEffect)
		{
			switch (InSwapEffect)
			{
			case DXGI_SWAP_EFFECT_DISCARD:
				return "{DXGI_SWAP_EFFECT_DISCARD}";

			case DXGI_SWAP_EFFECT_SEQUENTIAL:
				return "{DXGI_SWAP_EFFECT_SEQUENTIAL}";

			case DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL:
				return "{DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL}";

			case DXGI_SWAP_EFFECT_FLIP_DISCARD:
				return "{DXGI_SWAP_EFFECT_FLIP_DISCARD";

			default:
				break;
			}

			return "{UNKNOWN}";
		}


		inline std::string GetFormattedHRESULTString(HRESULT hr, const std::string& hrDescr)
		{
			std::string failOrSuccessDescr = SUCCEEDED(hr) ? "SUCC" : "FAILED";
			return std::to_string(hr) + std::string("(") + 
				failOrSuccessDescr + std::string(":") + 
				hrDescr + std::string(")");
		}

		inline std::string GetCommonHRESULTDescription(HRESULT hr)
		{
			switch (hr)
			{
			case DXGI_ERROR_DEVICE_HUNG:
				return "{DXGI_ERROR_DEVICE_HUNG}";

			case DXGI_ERROR_DEVICE_REMOVED:
				return "{DXGI_ERROR_DEVICE_REMOVED}";

			case DXGI_ERROR_DEVICE_RESET:
				return "{DXGI_ERROR_DEVICE_RESET}";

			case DXGI_ERROR_DRIVER_INTERNAL_ERROR:
				return "{DXGI_ERROR_DRIVER_INTERNAL_ERROR}";

			case DXGI_ERROR_FRAME_STATISTICS_DISJOINT:
				return "{DXGI_ERROR_FRAME_STATISTICS_DISJOINT}";

			case DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE:
				return "{DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE}";

			case DXGI_ERROR_INVALID_CALL:
				return "{DXGI_ERROR_INVALID_CALL}";

			case DXGI_ERROR_MORE_DATA:
				return "{DXGI_ERROR_MORE_DATA}";

			case DXGI_ERROR_NONEXCLUSIVE:
				return "{DXGI_ERROR_NONEXCLUSIVE}";

			case DXGI_ERROR_NOT_CURRENTLY_AVAILABLE:
				return "{DXGI_ERROR_NOT_CURRENTLY_AVAILABLE}";

			case DXGI_ERROR_NOT_FOUND:
				return "{DXGI_ERROR_NOT_FOUND}";

			case DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED:
				return "{DXGI_ERROR_REMOTE_CLIENT_DISCONNECTED}";

			case DXGI_ERROR_REMOTE_OUTOFMEMORY:
				return "{DXGI_ERROR_REMOTE_OUTOFMEMORY}";

			case DXGI_ERROR_WAS_STILL_DRAWING:
				return "{DXGI_ERROR_WAS_STILL_DRAWING}";

			case DXGI_ERROR_UNSUPPORTED:
				return "{DXGI_ERROR_UNSUPPORTED}";

			case DXGI_ERROR_ACCESS_LOST:
				return "{DXGI_ERROR_ACCESS_LOST}";

			case DXGI_ERROR_WAIT_TIMEOUT:
				return "{DXGI_ERROR_WAIT_TIMEOUT}";

			case DXGI_ERROR_SESSION_DISCONNECTED:
				return "{DXGI_ERROR_SESSION_DISCONNECTED}";

			case DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE:
				return "{DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE}";

			case DXGI_ERROR_CANNOT_PROTECT_CONTENT:
				return "{DXGI_ERROR_CANNOT_PROTECT_CONTENT}";

			case DXGI_ERROR_ACCESS_DENIED:
				return "{DXGI_ERROR_ACCESS_DENIED}";

			case DXGI_ERROR_NAME_ALREADY_EXISTS:
				return "{DXGI_ERROR_NAME_ALREADY_EXISTS}";

			case DXGI_ERROR_SDK_COMPONENT_MISSING:
				return "{DXGI_ERROR_SDK_COMPONENT_MISSING}";

			case S_OK:
				return "{S_OK}";

			default:
				break;
			}
			return "{UNKNOWN}";
		}
	
		inline std::string GetCreateDeviceHRESULTDescription(HRESULT hr)
		{
			return GetCommonHRESULTDescription(hr);
		}
		inline std::string GetDevceHRESULTString(HRESULT hr)
		{			
			return GetFormattedHRESULTString(hr, GetCreateDeviceHRESULTDescription(hr));
		}

		inline std::string GetSingleShaderCompileFlagString(UINT InFlag)
		{
			switch (InFlag)
			{
			case D3DCOMPILE_DEBUG:
				return "{DEBUG}";

			case D3DCOMPILE_SKIP_VALIDATION:
				return "{SKIP_VALIDATION}";

			case D3DCOMPILE_SKIP_OPTIMIZATION:
				return "{SKIP_OPTIMIZATION}";

			case D3DCOMPILE_PACK_MATRIX_ROW_MAJOR:
				return "{ROW_MAJOR}";

			case D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR:
				return "{COLUMN_MAJOR}";

			case D3DCOMPILE_PARTIAL_PRECISION:
				return "{PARTIAL_PRECISION}";

			case D3DCOMPILE_FORCE_VS_SOFTWARE_NO_OPT:
				return "{FORCE_VS_SOFTWARE_NO_OPT}";

			case D3DCOMPILE_FORCE_PS_SOFTWARE_NO_OPT:
				return "{FORCE_PS_SOFTWARE_NO_OPT}";

			case D3DCOMPILE_NO_PRESHADER:
				return "{NO_PRESHADER}";

			case D3DCOMPILE_AVOID_FLOW_CONTROL:
				return "{AVOID_FLOW_CONTROL}";

			case D3DCOMPILE_PREFER_FLOW_CONTROL:
				return "{PREFER_FLOW_CONTROL}";

			case D3DCOMPILE_ENABLE_STRICTNESS:
				return "{ENABLE_STRICTNESS}";

			case D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY:
				return "{ENABLE_BACKWARDS_COMPATIBILITY}";

			case D3DCOMPILE_IEEE_STRICTNESS:
				return "{IEEE_STRICTNESS}";

			case D3DCOMPILE_OPTIMIZATION_LEVEL0:
				return "{OPTIMIZATION_LEVEL0}";

			case D3DCOMPILE_OPTIMIZATION_LEVEL1:
				return "{OPTIMIZATION_LEVEL1}";

			case D3DCOMPILE_OPTIMIZATION_LEVEL2:
				return "{OPTIMIZATION_LEVEL2}";

			case D3DCOMPILE_OPTIMIZATION_LEVEL3:
				return "{OPTIMIZATION_LEVEL3}";

			case D3DCOMPILE_WARNINGS_ARE_ERRORS:
				return "{WARNINGS_ARE_ERRORS}";

			case D3DCOMPILE_RESOURCES_MAY_ALIAS:
				return "{RESOURCES_MAY_ALIAS}";

			case D3DCOMPILE_ENABLE_UNBOUNDED_DESCRIPTOR_TABLES:
				return "{ENABLE_UNBOUNDED_DESCRIPTOR_TABLES}";

			case D3DCOMPILE_ALL_RESOURCES_BOUND:
				return "{ALL_RESOURCES_BOUND}";

			default:
				break;
			}
			return "{UNKNOWN_FLAG}";
		}

		inline std::string GetSingleClearFlagString(UINT InFlag)
		{
			switch (InFlag)
			{
			case D3D11_CLEAR_DEPTH:
				return "{CLEAR_DEPTH}";

			case D3D11_CLEAR_STENCIL:
				return "{CLEAR_STENCIL}";

			default:
				BOOST_ASSERT_MSG(false, "GetSingleClearFlagString: wrong clear flag value");
				return"";
			}
		}

		inline std::string GetClearFlagsString(UINT InClearFlags)
		{
			std::string result_string;
			auto numSetFlags = 0;
			for (auto flagIndex = 0; flagIndex < 32; flagIndex++)
			{
				auto flagValue = InClearFlags & (1 << flagIndex);
				if (flagValue)
				{
					if (numSetFlags > 0)
					{
						result_string.append(" | ");
					}
					result_string.append(GetSingleClearFlagString(flagValue));
					numSetFlags++;
				}
			}
			return result_string;
		}

		inline std::string GetShaderCompileFlagsString(UINT InShaderCompilerFlags)
		{
			std::string result_string;
			auto numSetFlags = 0;
			for (auto flagIndex = 0; flagIndex < 32; flagIndex++)
			{
				auto flagValue = InShaderCompilerFlags & (1 << flagIndex);
				if (flagValue)
				{
					if (numSetFlags > 0)
					{
						result_string.append(" | ");
					}
					result_string.append(GetSingleShaderCompileFlagString(flagValue));
					numSetFlags++;
				}
			}
			return result_string;
		}

		inline std::string GetSingleDeviceFlagString(UINT InDeviceFlag)
		{
			switch (InDeviceFlag)
			{
			case D3D11_CREATE_DEVICE_SINGLETHREADED:
				return "{SINGLE_THREADED}";

			case D3D11_CREATE_DEVICE_DEBUG:
				return "{DEBUG}";

			case D3D11_CREATE_DEVICE_SWITCH_TO_REF:
				return "{SWITCH_TO_REF}";

			case D3D11_CREATE_DEVICE_PREVENT_INTERNAL_THREADING_OPTIMIZATIONS:
				return "{PREVENT_INTERNAL_THREADING_OPTIMIZATIONS}";

			case D3D11_CREATE_DEVICE_BGRA_SUPPORT:
				return "{BGRA_SUPPORT}";

			case D3D11_CREATE_DEVICE_DEBUGGABLE:
				return "{DEBUGGABLE}";

			case D3D11_CREATE_DEVICE_PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY:
				return "{PREVENT_ALTERING_LAYER_SETTINGS_FROM_REGISTRY}";

			case D3D11_CREATE_DEVICE_DISABLE_GPU_TIMEOUT:
				return "{DISABLE_GPU_TIMEOUT}";

			case D3D11_CREATE_DEVICE_VIDEO_SUPPORT:
				return "{VIDEO_SUPPORT}";

			default:
				break;
			}
			return "{UNKNOWN_FLAG}";
		}

		inline std::string GetDeviceFlagsString(UINT InDeviceFlags)
		{
			std::string result_string;
			auto numSetFlags = 0;
			for (auto flagIndex = 0; flagIndex < 32; flagIndex++)
			{
				auto flagValue = InDeviceFlags & (1 << flagIndex);
				if(flagValue)
				{
					if (numSetFlags > 0)
					{
						result_string.append(" | ");
					}
					result_string.append(GetSingleDeviceFlagString(flagValue));
					numSetFlags++;
				}
			}
			return result_string;
		}

		inline std::string GetFeatureLevelString(D3D_FEATURE_LEVEL featureLevel)
		{
			switch (featureLevel)
			{
			case D3D_FEATURE_LEVEL_11_0:
				return "{11_0}";

			case D3D_FEATURE_LEVEL_11_1:
				return "{11_1}";

			case D3D_FEATURE_LEVEL_10_0:
				return "{10_0}";

			case D3D_FEATURE_LEVEL_10_1:
				return "{10_1}";

			default:
				break;
			}
			return "{UNKNOWN FEATURE LEVEL}";
		}
	} // d3d_utils
} // Dv