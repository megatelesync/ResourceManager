#pragma once

#include "utils_device_params.h"

namespace Dv
{
	namespace d3d_utils
	{
		/**************************************************************************************
		* Fwd declarations
		**************************************************************************************/
		inline DvUtils::Math::IntSize GetRectSize(LPRECT rect);

		inline DvUtils::Math::IntSize GetRectSize(LPRECT rect)
		{
			return DvUtils::Math::IntSize(rect->right - rect->left, rect->bottom - rect->top);
		}


		class D3D11RenderDevice
		{
		public:
			/**************************************************************************************
			* Ctors
			**************************************************************************************/
			// WARNING: Creates UNINITIALIZED device. Init(HWND) must be called!
			D3D11RenderDevice();

			/**************************************************************************************
			* Initialization
			**************************************************************************************/
			// Inits device and returns HRESULT code of the D3D11CreateDeviceAndSwapChain call.
			// Only the output windows must be passed (the default parameters to be used in that case).
			// WARNING: This function may be called ever if the device is in initialized state
			// (the function will reinitilizes the device with the new parameters).			
			HRESULT                                 Init                       (HWND InOutputWindow, const DeviceParams& InParams = DeviceParams());
			bool                                    IsInitialized              () const						{ return _pDev.get() != nullptr; }

			/***************************************************************************************
			*
			***************************************************************************************/
			HRESULT                                 GetCreateDeviceHResult     () const						{ assert(IsInitialized());  return _hr_create_device; }
			
			/**************************************************************************************
			* Device getters
			**************************************************************************************/
			ID3D11Device							*GetDev                    ()                           { assert(IsInitialized());  return _pDev.get(); }
			ID3D11DeviceContext						*GetContext                ()							{ assert(IsInitialized());  return _pDevContext.get(); }
			IDXGISwapChain							*GetSwapChain              ()							{ assert(IsInitialized());  return _pSwapChain.get(); }

			const ID3D11Device						*GetDev                    () const						{ assert(IsInitialized());  return _pDev.get(); }
			const ID3D11DeviceContext				*GetContext                () const						{ assert(IsInitialized());  return _pDevContext.get(); }
			const IDXGISwapChain					*GetSwapChain              () const						{ assert(IsInitialized());  return _pSwapChain.get(); }

			/**************************************************************************************
			* Window getters
			**************************************************************************************/
			HWND									GetHWND                   () const						{ assert(IsInitialized());  return _hOutputWindow; }
			RECT									GetRect                   () const;
			DvUtils::Math::IntSize					GetWindowSize             () const						{ assert(IsInitialized());  return GetRectSize(&GetRect()); }

			/**************************************************************************************
			* Device parameters getters
			**************************************************************************************/
			D3D_FEATURE_LEVEL						GetRealFeatureLevel       () const						{ assert(IsInitialized());  return _realFeatureLevel; }
			const DeviceParams&						GetParams                 () const						{ assert(IsInitialized());  return _params; }
			

		private:			
			std::unique_ptr<ID3D11Device,Releaser>					_pDev;
			std::unique_ptr<ID3D11DeviceContext,Releaser>			_pDevContext;
			std::unique_ptr<IDXGISwapChain,Releaser>				_pSwapChain;			
			HRESULT													_hr_create_device;
			D3D_FEATURE_LEVEL										_realFeatureLevel;
			DeviceParams											_params;
			HWND													_hOutputWindow;
		};
	} // d3d_utils
} // Dv