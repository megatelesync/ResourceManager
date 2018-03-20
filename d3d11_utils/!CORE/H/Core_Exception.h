#pragma once

#include <string>
#include <d3d11.h>
#include <memory>
#include "Core_Dx11BaseObject.h"

namespace Dv
{
	namespace dx11
	{
		// Base class for all the exceptions of the dx11 library
		class Exception
		{
		public:
			/*****************************************************************************
			* ctors
			*****************************************************************************/
			// ctor: the inner exception (if spectified) will be moved to the new object
			inline Exception				
			(
				const std::string&						InMessage, 
				HRESULT									InHR,
				Exception								*InNullOrInnerException = nullptr
			);

			// move ctor			
			inline						Exception				(Exception&& InOther);


			/*****************************************************************************
			* getters
			*****************************************************************************/
			HRESULT						GetHr					()					const { return _hr; }
			const std::string&			GetMessage				()					const { return _message; }
			//Exception				   *GetInnerException		()					const { return _pInnerException.get(); }


		private:
			HRESULT									_hr;
			std::string								_message;
			//std::unique_ptr<Exception>				_pInnerException;			
		}; 
		
		// Throw the exception if the error is found, based on the hresult code
		void ThrowExceptionIfError(HRESULT InHr, const std::string& InMessage);

		// Get message string for the given exception
		std::string GetExceptionMessage(const Exception& InException);



		// IMPL
		inline Exception::Exception
		(
			const std::string&					InMessage, 
			HRESULT								InHR,
			Exception						   *InNullOrInnerException
		) :
			_hr						(InHR),
			_message				(InMessage)//,
			//_pInnerException		(nullptr)
		{
			if(InNullOrInnerException != nullptr)
			{
				//Exception *pCopiedInnerException = new Exception(std::move(*InNullOrInnerException));
				//_pInnerException.reset(pCopiedInnerException);
			}
		}

		inline Exception::Exception(Exception&& InOther) :
			_hr						(std::move(InOther.GetHr())),
			_message				(std::move(InOther.GetMessage()))//,
			//_pInnerException		(std::move(InOther._pInnerException))
		{
		}
	} // dx11
} // Dv