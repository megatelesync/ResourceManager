#include "../../H//Subsystem/IResourceManagerSubsystem.h"

#include <cassert>

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			/************************************************************************************
			* ResourceManagerPublicRuntimeParams implementation
			************************************************************************************/
			// default ctor
			ResourceManagerPublicRuntimeParams::ResourceManagerPublicRuntimeParams() :
				_bFreezed(false),
				_bDecisionMakingEnabled(true)
			{
			}

			void Log_SubsystemRuntimeParams(Util::ILoggingDevice *pInLogger, const ResourceManagerPublicRuntimeParams *pInParams)
			{
				*pInLogger << "IsDecisionMakingEnabled: " << pInParams->IsDecisionMakingEnabled() << Util::endl;
				*pInLogger << "IsFreezed: " << pInParams->IsFreezed() << Util::endl;
			}

			/************************************************************************************
			* ResourceManager_FilesystemParams implementation
			************************************************************************************/
			void ResourceManager_FilesystemParams::RegisterBaseDataDir(const std::string& InBaseDataDir)
			{
				_baseDataDirs.push_back(InBaseDataDir);
			}

			void ResourceManager_FilesystemParams::SetDefaultURL_BaseDataDir(const std::string& InDefaultURLBaseDataDir)
			{
				_defaultURL_BaseDataDir = InDefaultURLBaseDataDir;
			}

			/************************************************************************************
			* Error-handling params implementation
			************************************************************************************/
			bool ShouldActuallyNeverTreatErrorsAsFatal(const ResourceManagerSubsystem_ErrorHandlingParams *pInParams)
			{
				if (pInParams->ShouldAlwaysTreatErrorsAsFatal())
				{
					return false;
				}

				bool bShouldNeverTreatErrorsAsFatal = pInParams->ShouldNeverTreatErrorsAsFatal();
				return bShouldNeverTreatErrorsAsFatal;
			}

			/************************************************************************************
			* Global functions to work with the resource manager subsystem interface
			* IMPLEMENTATION.
			************************************************************************************/
			// Register subresource role that may be not loaded
			ISubresourceRole *RegisterSubresourceRole(IResourceManagerSubsystem *pInSubsys, ResourceTypeSubresourceRoleParams *pOutSubresourceRoles, const std::string& InName)
			{
				assert(pInSubsys != nullptr);
				assert(&InName != nullptr);
				assert(pOutSubresourceRoles != nullptr);
				SubresourceRolePublicParams new_role_descr (InName);
				auto pResult = pInSubsys->RegisterRole(new_role_descr);
				pOutSubresourceRoles->IncludeApplicable(pResult);
				return pResult;
			}
			// Register subresource role that must be loaded to create the resource
			ISubresourceRole *RegisterSubresourceRole_ThatMustBeLoaded(IResourceManagerSubsystem *pInSubsys, ResourceTypeSubresourceRoleParams *pOutSubresourceRoles, const std::string& InName, bool bMustBeLoadedWhileResourceIsLoaded)
			{
				auto pResult = RegisterSubresourceRole(pInSubsys, pOutSubresourceRoles, InName);
				pOutSubresourceRoles->IncludeIn_MustBeLoadedToCreateResource(pResult);
				if (bMustBeLoadedWhileResourceIsLoaded)
				{
					pOutSubresourceRoles->IncludeIn_MustBeLoadedWhileRealResourceIsLoaded(pResult);
				}
				return pResult;
			}
			ISubresourceRole *RegisterRealResourceSubresourceRole(IResourceManagerSubsystem *pInSubsys, ResourceTypeSubresourceRoleParams *pOutSubresourceRoles, const std::string& InName)
			{
				auto pResult = RegisterSubresourceRole_ThatMustBeLoaded(pInSubsys, pOutSubresourceRoles, InName, true);
				pOutSubresourceRoles->SetRealResourceSubresourceRole(pResult);
				return pResult;
			}


			/******************************************************************************************************************************
			* Params logging
			******************************************************************************************************************************/
			void Log_LoggingParams(Util::ILoggingDevice *pLog, const SubsystemLoggingParams *pInParams)
			{
				Util::LogRecordScoper scope(pLog, "Logging", Util::EMessageType::Info);

				Log_InitializationLoggingParams(pLog, &(pInParams->initialization));
				Log_CommandLoggingParams(pLog, &(pInParams->commands), "Command logging");
				Log_CommandLoggingParams(pLog, &(pInParams->resourceCommands), "Resource command logging");
				Log_CommandLoggingParams(pLog, &(pInParams->groupCommands), "Group command logging");
			}

			void Log_CommandLoggingParams(Util::ILoggingDevice *pLog, const CommandLoggingParams *pInParams, const std::string& InTitle)
			{
				Util::LogRecordScoper scope(pLog, InTitle, Util::EMessageType::Info);

				*pLog << "bLogPushToQueue: " << pInParams->bLogPushToQueue << Util::endl;
				*pLog << "bLogDequeueCommandResult: " << pInParams->bLogDequeueCommandResult << Util::endl;
				*pLog << "bLogLoadingCommands: " << pInParams->bLogLoadingCommands << Util::endl;
				*pLog << "bLogUnloadingCommands: " << pInParams->bLogUnloadingCommands << Util::endl;
				*pLog << "bLogManualCommands: " << pInParams->bLogManualCommands << Util::endl;
				*pLog << "bLogAutomaticCommands: " << pInParams->bLogAutomaticCommands << Util::endl;
				*pLog << "bLogNonSyncroOnly: " << pInParams->bLogNonSynchroOnly << Util::endl;
				*pLog << "bLogRecursiveChildCommands: " << pInParams->bLogRecursiveChildCommands << Util::endl;
			}

			void Log_InitializationLoggingParams(Util::ILoggingDevice *pLog, const InitializationLoggingParams *pInParams)
			{
				Util::LogRecordScoper scope(pLog, "Initialization logging params", Util::EMessageType::Info);
				*pLog << "bShouldLogInitialization: " << pInParams->bShouldLogInitialization << Util::endl;
				*pLog << "bShouldLogShutdown: " << pInParams->bShouldLogShutdown << Util::endl;
			}

			bool                                                        ShouldLog_LoadCommandPushing
			(
				const                                                   SubsystemLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive,
				bool                                                    bGroupCommand
			)
			{
				bool bShouldLog_Command = ShouldLog_LoadCommandPushing(&(pInParams->commands), bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
				bool bShouldLog_Resource = ShouldLog_LoadCommandPushing(&(pInParams->resourceCommands), bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
				if (bGroupCommand)
				{
					bool bShouldLog_Group = ShouldLog_LoadCommandPushing(&(pInParams->groupCommands), bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
					if (false == bShouldLog_Group)
					{
						return false;
					}
				}
				bool bShouldLog = bShouldLog_Command && bShouldLog_Resource;
				return bShouldLog;
			}
			bool                                                        ShouldLog_LoadDequeueing
			(
				const                                                   SubsystemLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive,
				bool                                                    bGroupCommand
			)
			{
				bool bShouldLog_Command = ShouldLog_LoadDequeueing(&(pInParams->commands), bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
				bool bShouldLog_Resource = ShouldLog_LoadDequeueing(&(pInParams->resourceCommands), bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
				if (bGroupCommand)
				{
					bool bShouldLog_Group = ShouldLog_LoadDequeueing(&(pInParams->groupCommands), bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
					if (false == bShouldLog_Group)
					{
						return false;
					}
				}
				bool bShouldLog = bShouldLog_Command && bShouldLog_Resource;
				return bShouldLog;
			}
			bool                                                        ShouldLog_UnloadCommandPushing
			(
				const                                                   SubsystemLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive,
				bool                                                    bGroupCommand
			)
			{
				bool bShouldLog_Command = ShouldLog_UnloadCommandPushing(&(pInParams->commands), bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
				bool bShouldLog_Resource = ShouldLog_UnloadCommandPushing(&(pInParams->resourceCommands), bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
				if (bGroupCommand)
				{
					bool bShouldLog_Group = ShouldLog_UnloadCommandPushing(&(pInParams->groupCommands), bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
					if (false == bShouldLog_Group)
					{
						return false;
					}
				}
				bool bShouldLog = bShouldLog_Command && bShouldLog_Resource;
				return bShouldLog;
			}

			bool                                                        ShouldLog_LoadCommandPushing
			(
				const                                                   CommandLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive
			)
			{
				if (false == pInParams->bLogLoadingCommands)
				{
					return false;
				}

				if (false == pInParams->bLogPushToQueue)
				{
					return false;
				}

				auto bShouldLog = ShouldPerformCommandLogging(pInParams, bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
				return bShouldLog;
			}

			bool                                                        ShouldLog_LoadDequeueing
			(
				const                                                   CommandLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive
			)
			{
				if (false == pInParams->bLogLoadingCommands)
				{
					return false;
				}

				if (false == pInParams->bLogDequeueCommandResult)
				{
					return false;
				}

				auto bShouldLog = ShouldPerformCommandLogging(pInParams, bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
				return bShouldLog;
			}

			bool                                                        ShouldLog_UnloadCommandPushing
			(
				const                                                   CommandLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive
			)
			{
				if (false == pInParams->bLogUnloadingCommands)
				{
					return false;
				}

				if (false == pInParams->bLogPushToQueue)
				{
					return false;
				}

				auto bShouldLog = ShouldPerformCommandLogging(pInParams, bIsManual, bLoadOnlyDependencies, bIsSynchro, bIsRecursive);
				return bShouldLog;
			}
			
			bool                                                        ShouldPerformCommandLogging
			(
				const                                                   CommandLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive
			)
			{
				bool bShouldLog = true;
				if (bIsManual && false == pInParams->bLogManualCommands)
				{
					return false;
				}
				if (false == bIsManual && false == pInParams->bLogAutomaticCommands)
				{
					return false;
				}
				if (true == bIsSynchro && pInParams->bLogNonSynchroOnly)
				{
					return false;
				}
				if(true == bIsRecursive && false == pInParams->bLogRecursiveChildCommands)
				{
					return false;
				}
				return bShouldLog;
			}
		}
	}
}