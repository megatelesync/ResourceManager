#pragma once

#include "../ResourceTypes/Deployer/ResManD3D11_Deployer_ResourceTypes.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			class ResManD3D11_IDeployer
			{
			public:
				const ResManD3D11_ResourceTypes_IDeployer               *GetDeployer_ResourceTypes                       () const;
				ResManD3D11_ResourceTypes_IDeployer                     *GetDeployer_ResourceTypes                       ();
				const ResManD3D11_ResourceTypes_IDeployer               *GetDefaultDeployer_ResourceTypes                () const;

			protected:
				virtual const ResManD3D11_ResourceTypes_IDeployer        *Virt_GetDeployer_ResourceTypes                 () const =0;
				virtual ResManD3D11_ResourceTypes_IDeployer              *Virt_GetDeployer_ResourceTypes() =0;
				virtual const ResManD3D11_ResourceTypes_IDeployer        *Virt_GetDefaultDeployer_ResourceTypes          () const =0;

			private:
			};

			/*******************************************************************************************************************************
			* IMPLEMENTATION
			*******************************************************************************************************************************/
			inline const ResManD3D11_ResourceTypes_IDeployer *ResManD3D11_IDeployer::GetDeployer_ResourceTypes() const
			{
				assert(this);
				return Virt_GetDeployer_ResourceTypes();
			}
			inline ResManD3D11_ResourceTypes_IDeployer *ResManD3D11_IDeployer::GetDeployer_ResourceTypes()
			{
				assert(this);
				return Virt_GetDeployer_ResourceTypes();
			}
			inline const ResManD3D11_ResourceTypes_IDeployer *ResManD3D11_IDeployer::GetDefaultDeployer_ResourceTypes() const
			{
				assert(this);
				return Virt_GetDefaultDeployer_ResourceTypes();
			}
		}
	}
}