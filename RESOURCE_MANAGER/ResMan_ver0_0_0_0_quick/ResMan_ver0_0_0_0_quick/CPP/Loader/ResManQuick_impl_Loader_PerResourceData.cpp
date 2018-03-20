#include "../../PrivateH/Loader/ResManQuick_impl_Loader_PerResourceData.h"
#include "../../PrivateH/Loader/ResManQuick_impl_LoadingLoop.h"


namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			Loader_PerResourceData::Loader_PerResourceData()
			{
			}

			const Loader_PerResource_PerLoadingLoop *Loader_PerResourceData::GetLoaderLoopData(LoadingLoop *pLoadingLoop) const
			{
				return pLoadingLoop->GetPerResourceData(this);
			}
			Loader_PerResource_PerLoadingLoop *Loader_PerResourceData::GetLoaderLoopData(LoadingLoop *pLoadingLoop)
			{
				return pLoadingLoop->GetPerResourceData(this);
			}
		}
	}
}