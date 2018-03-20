#include "../H/ResManQuick_ResourceManagerSubsystem.h"

namespace Dv
{
	namespace ResManQuick
	{
		ResourceManagerSubsystem::~ResourceManagerSubsystem()
		{
		}

		ResourceManagerSubsystem::ResourceManagerSubsystem(const ResManQuick_SubsystemParams& InParams) :
			_subsystem(InParams,this)
		{
		}

		void ResourceManagerSubsystem::InitializeAndStart() throw(ResMan::ResourceManagerException)
		{
			_subsystem.Initialize();
			_bInitialized = true;
		}
		void ResourceManagerSubsystem::Stop()
		{
			_subsystem.Stop();
		}

		void ResourceManagerSubsystem::Tick(double InDeltaSeconds)
		{
			assert(_bInitialized);
			_subsystem.Tick(InDeltaSeconds);
		}

		void ResourceManagerSubsystem::UpdateRuntimeParams(const ResMan::ResourceManagerPublicRuntimeParams& InParams)
		{
			_subsystem.UpdateRuntimeParams(InParams);
		}

		ResMan::IResourceType *ResourceManagerSubsystem::RegisterResourceType(ResMan::IResourceTypeDescr *pInType)
		{
			assert(pInType != nullptr);
			pInType->RegisterApplicableRoles(this);
			auto pRegisteredType = _subsystem.RegisterResourceType(pInType);
			return pRegisteredType;
		}
		ResMan::ISubresourceRole *ResourceManagerSubsystem::RegisterRole(const ResMan::SubresourceRolePublicParams& InParams)
		{
			assert(&InParams != nullptr);
			auto pRegisteredSubresourceRole = _subsystem.RegisterRole(InParams);
			return pRegisteredSubresourceRole;
		}

		/***********************************************************************************************************************
		* working with parameters
		***********************************************************************************************************************/
		void Initialize_Logging(ResManQuick_SubsystemParams *pOutParams, const std::string& InBaseDir, bool bShowOnlyFatalErrors)
		{
			assert(InBaseDir.size() > 0);

			std::string properBaseDir = InBaseDir;
			if(InBaseDir.back() != '/' &&InBaseDir.back() != '\\')
			{ 
				properBaseDir.append("/");
			}

			const std::string threadedLogFilename = "loader_thread_log.txt";
			const std::string mainThreadLogFilename = "mainThread_log.txt";

			const std::string threadedLogFullFilename = properBaseDir + threadedLogFilename;
			const std::string mainThreadLogFullFilename = properBaseDir + mainThreadLogFilename;

			auto pMainThreadLog = pOutParams->GetLogSettingsPtr_MainThread();
			pMainThreadLog->logName = "Log_MainThread";
			pMainThreadLog->logFilename = mainThreadLogFullFilename;

			auto pThreadedLog = pOutParams->GetLogSettingsPtr_Threaded();
			pThreadedLog->logFilename = threadedLogFullFilename;
			pThreadedLog->logName = "Log_Threaded";
		}
	}
}