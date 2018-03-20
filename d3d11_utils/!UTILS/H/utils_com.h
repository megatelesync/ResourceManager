#pragma once

#include <D3D11.h>
#include <memory>

namespace Dv
{
	namespace d3d_utils
	{
		struct Releaser
		{
		public:
			void operator() (IUnknown *pObj)
			{
				if(pObj != nullptr)
				{
					pObj->Release();
				}
			}
		};

		template<class T> class HResultValue
		{
		public:
			HResultValue(const T& valueIn) : _hr(S_OK), _value(valueIn) {}
			HResultValue(HRESULT hrIn, const T& valueIn) : _hr(hrIn), _value(valueIn) {}

			const T&				GetValue		()					const			 { return _value; }
			HRESULT					GetHResult		()					const			 { return _hr; }

		private:
			HRESULT			   _hr;
			T				_value;
		};

		template<class T> bool IsFail(const HResultValue<T>& hrValue);




		// HResultValue impl
		template<class T> bool IsFail(const HResultValue<T>& hrValue)
		{
			return FAILED(hrValue.GetHResult());
		}
	} // d3d_utils
} // Dv