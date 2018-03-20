#pragma once

#include "EditableModelLoaders.h"
#include "Max3DSLoaders.h"
#include "MaxModelLoaders.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/***********************************************************************************************************
			* Fwd declarations
			***********************************************************************************************************/
			class IDeployer_ModelResourceTypes;

			/***********************************************************************************************************
			* Contains all (registered or not) 3D model resource types
			***********************************************************************************************************/
			class ResManD3D11_ResourceTypes_Models
			{
			public:
				/*****************************************************************************************************************
				* Initialization
				*****************************************************************************************************************/
				void                                                                   Initialize                    (IDeployer_ModelResourceTypes *pInDeployer);

				/******************************************************************************************************************
				* Max model files
				******************************************************************************************************************/
				const ModeModelFileResource_Ver0_0_0_1::MaxModelFileResourceType       *GetMaxModel_Ver0_0_0_1       () const { return &_maxModel_0_0_0_1; }
				ModeModelFileResource_Ver0_0_0_1::MaxModelFileResourceType             *GetMaxModel_Ver0_0_0_1       () { return &_maxModel_0_0_0_1; }

			private:
				ModeModelFileResource_Ver0_0_0_1::MaxModelFileResourceType          _maxModel_0_0_0_1;
			};
		}
	}
}