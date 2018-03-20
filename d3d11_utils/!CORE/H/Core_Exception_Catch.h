#pragma once

#include "Core_Exception.h"
#include "Util_Output.h"

namespace Dv
{
	namespace dx11
	{
		void ShowExceptionMessage(const Exception& InException, const Dv::Utils::OutputParams& InParams);
	} // dx11
} // Dv