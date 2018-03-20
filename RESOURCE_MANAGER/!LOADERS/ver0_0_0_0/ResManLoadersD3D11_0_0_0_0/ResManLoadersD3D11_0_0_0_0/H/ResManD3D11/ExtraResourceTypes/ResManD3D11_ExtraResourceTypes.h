#pragma once

#include "../../Models/Models_ResourceTypes.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*****************************************************************************************************
			* Contains all resource types that are not included in NORMAL resource types
			******************************************************************************************************/
			class ResManD3D11_ExtraResourceTypes
			{
			public:
				/***********************************************************************************************************************
				* Initialize
				***********************************************************************************************************************/
				void                                                   Initialize    ();

				/***********************************************************************************************************************
				* Getters
				***********************************************************************************************************************/
				const ResManD3D11_ResourceTypes_Models                *GetModels     () const                    { return &_models; }
				ResManD3D11_ResourceTypes_Models                      *GetModels     ()                          { return &_models; }

			private:
				ResManD3D11_ResourceTypes_Models                       _models;
			};
		}
	}
}
