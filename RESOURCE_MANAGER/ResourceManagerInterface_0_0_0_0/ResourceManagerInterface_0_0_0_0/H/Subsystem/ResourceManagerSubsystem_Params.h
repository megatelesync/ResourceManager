#pragma once

#include "Log/Util_LoggingDevice.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			/*****************************************************************************
			* Params of the resource manager related to filesystem
			*****************************************************************************/
			class ResourceManager_FilesystemParams
			{
			public:
				// Default ctor: automatically registers "C:\!DV\!DATA" directory
				ResourceManager_FilesystemParams();

				size_t                                                  GetNumBaseDataDirs                   ()                             const { return _baseDataDirs.size(); }
				const std::string&                                      GetBaseDataDir                       (size_t InIndex)               const { return _baseDataDirs[InIndex]; }
				const std::string&                                      GetDefaultURL_BaseDataDir            () const                             { return _defaultURL_BaseDataDir; }

				// Register base data directory
				void                                                    RegisterBaseDataDir                  (const std::string& InBaseDataDir);
				void                                                    SetDefaultURL_BaseDataDir            (const std::string& InDefaultURLBaseDataDir);

			private:
				std::vector<std::string>                                _baseDataDirs;
				std::string                                             _defaultURL_BaseDataDir = "";
			};
			inline ResourceManager_FilesystemParams::ResourceManager_FilesystemParams()
			{
				RegisterBaseDataDir("C:\\!DV\\!DATA\\");
			}

			/******************************************************************************
			* Params of logging of initialization and finalization
			******************************************************************************/
			struct InitializationLoggingParams
			{
				bool                                                   bShouldLogInitialization = true;
				bool                                                   bShouldLogShutdown = true;
			};
			void Log_InitializationLoggingParams                       (Util::ILoggingDevice *pLog, const InitializationLoggingParams *pInParams);

			/******************************************************************************
			* Params of logging of commands
			******************************************************************************/
			struct CommandLoggingParams
			{
				// Should we log command pushing to queue
				bool                                                    bLogPushToQueue = true;
				// Should we log command result dequeueing
				bool                                                    bLogDequeueCommandResult = true;

				// Should we log loading commands (both manual and automatic)
				bool                                                    bLogLoadingCommands = true;
				// Should we log unloading commands (both manual and automatic)
				bool                                                    bLogUnloadingCommands = true;

				// Should we log manual commands
				bool                                                    bLogManualCommands = true;

				// Should we log automatic commands
				bool                                                    bLogAutomaticCommands = true;

				// Should we log only Non-synchronized commands
				bool                                                    bLogNonSynchroOnly = false;

				// Should we log recursive child commands
				bool                                                    bLogRecursiveChildCommands = true;
			};
			bool                                                        ShouldLog_LoadCommandPushing
			(
				const                                                   CommandLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive
			);
			bool                                                        ShouldLog_LoadDequeueing
			(
				const                                                   CommandLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive
			);
			bool                                                        ShouldLog_UnloadCommandPushing
			(
				const                                                   CommandLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive
			);
			// Should we perform command (without considering type of the command)
			bool                                                        ShouldPerformCommandLogging
			(
				const                                                   CommandLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive
			);

			void Log_CommandLoggingParams(Util::ILoggingDevice *pLog, const CommandLoggingParams *pInParams, const std::string& InTitle);

			/******************************************************************************
			* Params of the logging
			******************************************************************************/
			struct SubsystemLoggingParams
			{
				/*************************************************************************************************************************
				* Logging initialization and finalization
				*************************************************************************************************************************/
				InitializationLoggingParams                             initialization;

				/*************************************************************************************************************************
				* Command logging
				*************************************************************************************************************************/
				CommandLoggingParams                                    commands;

				/*************************************************************************************************************************
				* Logging group commands
				*************************************************************************************************************************/
				CommandLoggingParams                                    groupCommands;

				/*************************************************************************************************************************
				* Logging resource commands
				*************************************************************************************************************************/
				CommandLoggingParams                                    resourceCommands;
			};
			void Log_LoggingParams(Util::ILoggingDevice *pLog, const SubsystemLoggingParams *pInParams);

			bool                                                        ShouldLog_LoadCommandPushing
			(
				const                                                   SubsystemLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive,
				bool                                                    bGroupCommand
			);
			bool                                                        ShouldLog_LoadDequeueing
			(
				const                                                   SubsystemLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive,
				bool                                                    bGroupCommand
			);
			bool                                                        ShouldLog_UnloadCommandPushing
			(
				const                                                   SubsystemLoggingParams *pInParams,
				bool                                                    bIsManual,
				bool                                                    bLoadOnlyDependencies,
				bool                                                    bIsSynchro,
				bool                                                    bIsRecursive,
				bool                                                    bGroupCommand
			);

			/**************************************************************************************************************
			* Error handling params
			**************************************************************************************************************/
			class ResourceManagerSubsystem_ErrorHandlingParams
			{
			public:
				// If this flag is true, all errors to be treated as fatal errors (means "exiting application when error occured")
				bool ShouldAlwaysTreatErrorsAsFatal                            () const                  { return _bAlwaysTreatErrorsAsFatal; }

				// If this flag is true, then no normally non-fatal errors to be treated as fatal
				// (exception: when "always treat as fatal errors" flag of this params is set, current flag is ignored,
				// and all errors are treated fatal)
				//
				// This flag is initially designed for unit tests.
				bool ShouldNeverTreatErrorsAsFatal                             () const                  { return _bNeverTreatErrorsAsFatal; };

				void SetEnableState_AlwaysTreatErrorsAsFatal                   (bool bInEnabled)         { _bAlwaysTreatErrorsAsFatal = bInEnabled; }
				void SetEnableState_NeverTreatErrorsAsFatal                    (bool bInEnabled)         { _bNeverTreatErrorsAsFatal = bInEnabled; }

			private:
				bool                                                           _bAlwaysTreatErrorsAsFatal = true;
				bool                                                           _bNeverTreatErrorsAsFatal = false;
			};
			// Calculates, whether we actually never should treat errors as fatal
			bool ShouldActuallyNeverTreatErrorsAsFatal(const ResourceManagerSubsystem_ErrorHandlingParams *pInParams);

			/**************************************************************************************************************
			* Parameters of the resource manager subsystem
			* to be passed during the initialization
			*
			* WARNING! The parameters are not passed in the Initialize function
			* because they are to be passed in the constructor,
			* - to eliminate the inconsistency with passing the derived class's parameters in the constructor.
			***************************************************************************************************************/
			class ResourceManagerSubsystemParams
			{
			public:
				ResourceManagerSubsystemParams() :
					_bSingleThreaded(true),
					_bDoAllCommandsSynchro(true)
				{
				}

				// Getting params of the filesystem
				const ResourceManager_FilesystemParams     *GetFilesystem     () const                { return &_filesystem; }

				// true, if manager used in the single-threaded mode
				bool               IsSingleThreaded                           () const                { return _bSingleThreaded; }
				void               SetSingleThreadedMode                      (bool bInValue)         { _bSingleThreaded = bInValue; }

				// Should wait for answer of any command before return it
				bool               ShouldDoAllCommandsSynchro                 () const                { return _bDoAllCommandsSynchro; }
				void               SetDoAllCommandsSynchro_EnableState        (bool bInNewValue)      { _bDoAllCommandsSynchro = bInNewValue; }

				/******************************************************************************************
				* Gettings non-const logging settings for initialization
				*******************************************************************************************/
				Util::LogSettings                                *GetLogSettingsPtr_MainThread()      { return &_logMainThread; }
				Util::LogSettings                                *GetLogSettingsPtr_Threaded()        { return &_logThreaded; }

				/******************************************************************************************
				* Logging settings
				******************************************************************************************/
				// Get log settings for logs that run on the main thread
				const Util::LogSettings                          *GetLogSettings_MainThread          () const { return &_logMainThread; }

				// Get log settings for logs that run on any thread other than the main thread
				const Util::LogSettings                          *GetLogSettings_Threaded            () const { return &_logMainThread; }

				const SubsystemLoggingParams                     *GetLogging                         () const { return &_logging; }
				SubsystemLoggingParams                           *GetLogging                         ()       { return &_logging; }

				/*****************************************************************************************
				* Get ptr to the filsystem for initialization
				*****************************************************************************************/
				ResourceManager_FilesystemParams                 *GetFilesystem                      ()       { return &_filesystem; }

				/*****************************************************************************************
				* Handling errors
				*****************************************************************************************/
				const ResourceManagerSubsystem_ErrorHandlingParams *GetErrorHandling() const { return &_errorHandling; }
				ResourceManagerSubsystem_ErrorHandlingParams *GetErrorHandling() { return &_errorHandling; }

			private:
				ResourceManagerSubsystem_ErrorHandlingParams                 _errorHandling;
				ResourceManager_FilesystemParams                             _filesystem;
				bool                                                         _bSingleThreaded;
				bool                                                         _bDoAllCommandsSynchro;
				Util::LogSettings                                            _logMainThread;
				Util::LogSettings                                            _logThreaded;
				SubsystemLoggingParams                                       _logging;
			};

			/******************************************************************************
			* Public parameters of the resource manager those can be changed at runtime
			******************************************************************************/
			class ResourceManagerPublicRuntimeParams
			{
			public:
				/************************************************************************************
				* CONSTRUCTION
				************************************************************************************/
				// default ctor
				ResourceManagerPublicRuntimeParams();

				/************************************************************************************
				* GETTERS
				************************************************************************************/
				// true, if no resource precaching or unloading to be performed at all
				// (ever if a direct command for precaching/unloading is issued);
				// Loop parameters (current time, current tick), however, are updated.
				bool                               IsFreezed() const;

				// true, if resource can be precached are unloaded
				// according to the rules of the resource manager
				bool                               IsDecisionMakingEnabled() const;

				/************************************************************************************
				* SETTERS
				************************************************************************************/
				void                               SetFreezed(bool bInValue);
				void                               SetDecisionMakingEnabled(bool bInEnabled);

			private:
				bool                               _bFreezed;
				bool                               _bDecisionMakingEnabled;
			};

			void Log_SubsystemRuntimeParams(Util::ILoggingDevice *pInLogger, const ResourceManagerPublicRuntimeParams *pInParams);
		}
	}
}