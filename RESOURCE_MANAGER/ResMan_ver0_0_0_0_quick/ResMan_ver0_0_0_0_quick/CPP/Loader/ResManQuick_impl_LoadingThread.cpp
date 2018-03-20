#include "../../PrivateH/Loader/ResManQuick_impl_LoadingThread.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			void ThreadFunc_LoadingLoopThread(LoadingLoop *pInLoadingLoop);

			LoadingThread::LoadingThread(LoaderMain *pInOwnerMain, const std::string& InName) :
				_loop(pInOwnerMain, InName)
			{
			}

			void LoadingThread::InitializeAndStart(const ResManQuick_SubsystemParams& InParams, Impl::Logger *pInLogger)
			{
				_params = InParams;

				_loop.Initialize(InParams, pInLogger);

				try
				{
					_task = std::async(std::launch::async, ThreadFunc_LoadingLoopThread, &_loop);
				}
				catch (const std::system_error& InException)
				{
					throw ResMan::ResourceManagerCreateThreadException(&InException, boost::optional<std::thread::id>(), std::string("LoadingThread: Cannot initialize thread"));
				}
			}

			void LoadingThread::UpdateRuntimeParams(const ResMan::ResourceManagerPublicRuntimeParams& InParams)
			{
				// no update is performed, it's ok
			}

			void ThreadFunc_LoadingLoopThread(LoadingLoop *pInLoadingLoop)
			{
				while (true)
				{
					pInLoadingLoop->Tick();
				}
			}
		} // Loader
	} // ResManQuick
} // Dv