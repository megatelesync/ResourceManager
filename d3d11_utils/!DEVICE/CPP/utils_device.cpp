#include "../H/utils_device.h"

namespace Dv
{
	namespace d3d_utils
	{
		/*****************************************************************************************
		* device params implementation
		******************************************************************************************/
		DeviceParams::DeviceParams
		(
			D3D_FEATURE_LEVEL			featureLevelIn,
			const SwapChainParams&		swapChainParamsIn,
			UINT						deviceFlagsIn
		) : swapChain(swapChainParamsIn),
			deviceFlags(deviceFlagsIn)
		{
			featureLevels.push_back(featureLevelIn);
		}

		/***********************************************************************************
		* D3D11 render device implementation
		***********************************************************************************/
		D3D11RenderDevice::D3D11RenderDevice() :
			_pDev(nullptr),
			_pDevContext(nullptr),
			_pSwapChain(nullptr),
			_hr_create_device(S_OK)
		{
		}

		HRESULT D3D11RenderDevice::Init
		(
			HWND								InOutputWindow,
			const DeviceParams&		            InParams
		)
		{

			_hOutputWindow = InOutputWindow;
			_params = InParams;
			IDXGISwapChain *pSwapChain = nullptr;
			ID3D11Device *pDev = nullptr;
			ID3D11DeviceContext *pDevContext = nullptr;
			DXGI_SWAP_CHAIN_DESC swapChainDesc = _params.GetSwapChainDesc(_hOutputWindow);
			_hr_create_device = D3D11CreateDeviceAndSwapChain
			(
				nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, _params.deviceFlags,
				&(_params.featureLevels.front()), _params.featureLevels.size(),
				D3D11_SDK_VERSION, &swapChainDesc,
				&pSwapChain, &pDev, &_realFeatureLevel, &pDevContext
			);
			_pSwapChain.reset(pSwapChain);
			_pDev.reset(pDev);
			_pDevContext.reset(pDevContext);
			return _hr_create_device;
		}

		RECT D3D11RenderDevice::GetRect() const
		{
			RECT rect;
			GetWindowRect(GetHWND(), &rect);
			return rect;
		}
	}
}