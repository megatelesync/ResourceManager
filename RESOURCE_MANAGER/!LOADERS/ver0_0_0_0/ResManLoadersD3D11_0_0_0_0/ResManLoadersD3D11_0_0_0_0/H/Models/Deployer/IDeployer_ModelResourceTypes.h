#pragma once

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/***********************************************************************************************
			* Fwd declarations
			***********************************************************************************************/
			class ResManD3D11_ResourceTypes_Models;

			/***********************************************************************************************
			* Deployer
			***********************************************************************************************/
			class IDeployer_ModelResourceTypes
			{
			public:
				/****************************************************************************
				* return false is skipped to deploy types for corresponding model type
				*****************************************************************************/
				virtual bool DeployMaxModel(ResManD3D11_ResourceTypes_Models *pOutModels) = 0;
				virtual bool DeployMax3DS(ResManD3D11_ResourceTypes_Models *pOutModels) = 0;
				virtual bool DeployEditableModel(ResManD3D11_ResourceTypes_Models *pOutModels) = 0;
			};
		}
	}
}