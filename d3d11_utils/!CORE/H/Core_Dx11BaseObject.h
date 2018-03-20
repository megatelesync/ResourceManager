#pragma once

#include "CoreUtil/CoreUtil_BaseObject.h"

namespace Dv
{
	namespace dx11
	{
		class Dx11BaseObject : public Util::BaseObject
		{
		public:
			/******************************************************************
			* ctors
			******************************************************************/
			inline Dx11BaseObject();
			inline Dx11BaseObject(const std::string& InName);

		protected:
		};





		/******************************************************************
		* implementation
		******************************************************************/
		inline Dx11BaseObject::Dx11BaseObject()
		{
		}
		inline Dx11BaseObject::Dx11BaseObject(const std::string& InName) :
			Util::BaseObject(InName)
		{
		}
	} // dx11
} // Dv