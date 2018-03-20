#pragma once

#include "ResManQuick_Fwd_LoaderMain.h"
#include "ResManQuick_Fwd_LoadUpdate.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			class LoadingLoopCommandQueues;
			struct LoadCommandParams;
			class LoadRequestParams;
			class LoadingLoop;

			class LoadCommandBase;
			struct LoaderCommandMainThreadData;
			class Loader_PerResourceData;
			class Loader_PerResource_ForLoadingLoopThread;
			class Loader_PerResource_PerLoadingLoop;
			class Loader_PerResource_ForMainThread;

			class LoaderCommandResultRecord;
		}
	}
}