#include "../../!Core/H/Core_Exception.h"
#include "../../!Core/H/Core_Exception_Catch.h"

namespace Dv
{
	namespace dx11
	{
		// Get message string for the given exception
		std::string GetExceptionMessage(const Exception& InException)
		{
			std::string res_str;
			res_str.append("hr=");
			res_str.append(std::to_string(InException.GetHr()));
			res_str.append("\n");
			res_str.append("message:");
			res_str.append(InException.GetMessage());
			return res_str;
		}

		void ThrowExceptionIfError(HRESULT InHr, const std::string& InMessage)
		{
			if(FAILED(InHr))
			{
				throw Exception(InMessage, InHr);
			}
		}

		void ShowExceptionMessage(const Exception& InException, const Dv::Utils::OutputParams& InParams)
		{
			std::string message = GetExceptionMessage(InException);
			Dv::Utils::ShowPageMessageBox(InParams, message.c_str(), "Dv::dx11::Exception");
		}
	} // dx11
} // Dv