#pragma once

#include "Subsystem/ResourceManagerSubsystem_Params.h"
#include "ResManQuick_System.h"

namespace Dv
{
	namespace ResManQuick
	{
		struct ResManQuick_SubsystemParams
		{
		public:
			const ResMan::ResourceManagerPublicRuntimeParams&              GetInitialRuntimeParams                    () const             { return _initial_runtime_params; }
			const ResMan::ResourceManagerSubsystemParams&                  GetCommonSubsystemParams                   () const             { return _common; }

			/*******************************************************************************************************************************************************************
			* Common parameters
			*******************************************************************************************************************************************************************/
			bool                                                           IsSingleThreaded                            () const            { return _common.IsSingleThreaded(); }
			void                                                           SetSingleThreadedMode                       (bool bInValue)     { return _common.SetSingleThreadedMode(bInValue); }

			bool                                                           ShouldDoAllCommandsSynchro                  () const            { return _common.ShouldDoAllCommandsSynchro(); }
			void                                                           SetDoAllCommandsSynchro_EnableState         (bool bInValue)     { _common.SetDoAllCommandsSynchro_EnableState(bInValue); }

			Util::LogSettings                                             *GetLogSettingsPtr_MainThread               ()                   { return _common.GetLogSettingsPtr_MainThread(); }
			Util::LogSettings                                             *GetLogSettingsPtr_Threaded                 ()                   { return _common.GetLogSettingsPtr_Threaded(); }

			// Get log settings for logs that run on the main thread
			const Util::LogSettings                                       *GetLogSettings_MainThread                  () const             { return _common.GetLogSettings_MainThread(); }

			// Get log settings for logs that run on any thread other than the main thread
			const Util::LogSettings                                       *GetLogSettings_Threaded                    () const             { return _common.GetLogSettings_Threaded(); }

			/*******************************************************************************************************************************************************************
			* Getters
			*******************************************************************************************************************************************************************/
			ResMan::ResourceManagerSubsystemParams                        *GetCommonSubsystemParams                   ()                   { return &_common; }

			/*******************************************************************************************************************************************************************
			* Helper getters
			*******************************************************************************************************************************************************************/
			const ResMan::ResourceManagerSubsystem_ErrorHandlingParams     *GetErrorHandlingParams                    () const             { return _common.GetErrorHandling(); }
			ResMan::ResourceManagerSubsystem_ErrorHandlingParams           *GetErrorHandlingParams                    ()                   { return _common.GetErrorHandling(); }

			/*******************************************************************************************************************************************************************
			* Helper getters
			*******************************************************************************************************************************************************************/
			const ResMan::ResourceManager_FilesystemParams                *GetFilesystem                              () const             { return _common.GetFilesystem(); }
			ResMan::ResourceManager_FilesystemParams                      *GetFilesystem                              ()                   { return _common.GetFilesystem(); }

		private:
			ResMan::ResourceManagerPublicRuntimeParams         _initial_runtime_params;
			// Params defined in the interface
			ResMan::ResourceManagerSubsystemParams             _common;
		};
		// Initialize logging:
		//		bShowOnlyFatalErrors - true if only fatal message to be outputted on the screen 
		void Initialize_Logging(ResManQuick_SubsystemParams *pOutParams, const std::string& InBaseDir, bool bShowOnlyFatalErrors = true);
	}
}