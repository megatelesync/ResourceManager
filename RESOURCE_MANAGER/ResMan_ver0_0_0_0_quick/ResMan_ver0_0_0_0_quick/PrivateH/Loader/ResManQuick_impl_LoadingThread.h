#pragma once

#include "Fwd\ResManQuick_Fwd_Impl.h"
#include "../ResManQuick_impl_System.h"
#include "ResManQuick_impl_LoadingLoop.h"
#include "../../H/ResManQuick_SubsystemParams.h"

#include <future>

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			class LoadingThread
			{
			public:
				LoadingThread                                                                                (LoaderMain *pInOwnerMain, const std::string& InName);

				void                                                        InitializeAndStart               (const ResManQuick_SubsystemParams& InParams, Impl::Logger *pInLogger) throw(ResMan::ResourceManagerException);
				void                                                        UpdateRuntimeParams              (const ResMan::ResourceManagerPublicRuntimeParams& InParams);

				// Pushing and popping commands and results is thread-safe
				LoadingLoopCommandQueues                                   *GetCommands                      ()                                                            { return _loop.GetCommands(); }
				LoadingLoop                                                *GetLoop                          ()                                                            { return &_loop; }

			private:
				ResManQuick_SubsystemParams                                 _params; // params must be immutable (for thread-safety)
				LoadingLoop                                                 _loop;
				std::future<void>                                           _task;
			};
		} // Loader
	} // ResManQuick
} // Dv