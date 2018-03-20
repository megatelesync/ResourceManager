#pragma once

#include "ResManD3D11_Group_BasicResources.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			class ResManD3D11_Groups
			{
			public:
				/***************************************************************************************************************************************
				* Initialization
				***************************************************************************************************************************************/
				void                                                            Initialize                       (const DvUtils::Math::IntSize& InWindowSize, ResManD3D11_Main *pInOwnerMain);

				/***************************************************************************************************************************************
				* Getters
				***************************************************************************************************************************************/
				const ResManD3D11_Group_BasicResources                         *GetBasicResources                () const                  { return &_group_BasicResources; }
				ResManD3D11_Group_BasicResources                               *GetBasicResources                ()                        { return &_group_BasicResources; }

				/***************************************************************************************************************************************
				* Helper getters
				***************************************************************************************************************************************/
				const ResManD3D11_Main                                         *GetOwnerMain                     () const                  { return _pOwnerMain; }
				ResManD3D11_Main                                               *GetOwnerMain                     ()                        { return _pOwnerMain; }

			private:
				ResManD3D11_Group_BasicResources                                _group_BasicResources;
				ResManD3D11_Main                                               *_pOwnerMain;
			};
		}
	}
}
