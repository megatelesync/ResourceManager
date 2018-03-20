#pragma once

#include "../Core/ResourceManagerTypes.h"
#include "../Utils/ISubresourceRole.h"
#include "Log/Util_ILoggingDevice.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class IResourceManager;

			/**********************************************************************************************
			* Mode of the combinable params of the resource
			**********************************************************************************************/
			enum class EResourceCombinableParametersMode
			{
				// UseGroup can be set ever if resource is not binded to group 
				// - in that case the behaviour will correspond to behaviour of the Override mode.
				UseGroup, 
				Combine,
				Override
			};
			std::string GetString_ResourceCombinableParamsMode(EResourceCombinableParametersMode InMode);

			/*************************************************************************************************
			* Parameters of logging
			**************************************************************************************************/
			class LoggingParams
			{
			public:
				/***********************************************************************************************************
				* Getters
				***********************************************************************************************************/
				bool                         ShoudLogAutoLoad                      () const                            { return _bLogAutoLoad; }
				bool                         ShoudLogManualLoad                    () const                            { return _bLogManualLoad; }
				bool                         ShoudLogAutoUnload                    () const                            { return _bLogAutoUnload; }
				bool                         ShoudLogManualUnload                  () const                            { return _bLogManualUnload; }

				/***********************************************************************************************************
				* Setters
				***********************************************************************************************************/
				void                         SetLogAutoLoad                       (bool bInValue)                      { _bLogAutoLoad = bInValue; }
				void                         SetLogManualLoad                     (bool bInValue)                      { _bLogManualLoad = bInValue; }
				void                         SetLogAutoUnload                     (bool bInValue)                      { _bLogAutoUnload = bInValue; }
				void                         SetLogManualUnload                   (bool bInValue)                      { _bLogManualUnload = bInValue; }

			private:
				bool                         _bLogAutoLoad = true;
				bool                         _bLogManualLoad = true;
				bool                         _bLogAutoUnload = true;
				bool                         _bLogManualUnload = true;
			};
			bool operator==                                                 (const LoggingParams& lhs, const LoggingParams& rhs);
			bool operator!=                                                 (const LoggingParams& lhs, const LoggingParams& rhs);

			void Log_CombinableLoggingParams                                (Util::ILoggingDevice *pLog, const LoggingParams *pInLoggingParams);

			void Combine_LoggingParams                                      (LoggingParams *pOutLoggingParams, const LoggingParams *pInLoggingParams);

			/*************************************************************************************************
			* Parameters of error-handling
			**************************************************************************************************/
			class CombinableErrorHandlingParams
			{
			public:
				/************************************************************************************************************
				* Getters
				************************************************************************************************************/
				// If this flag is set, any loading error to be regarded as fatal
				bool                          IsFatal                             () const                                 { return _bFatal; }

				/************************************************************************************************************
				* Setters
				************************************************************************************************************/
				void                          SetFatal                            (bool bInValue)                          { _bFatal = bInValue; }

			private:
				bool                         _bFatal = true;
			};

			bool operator==                                                 (const CombinableErrorHandlingParams& lhs, const CombinableErrorHandlingParams& rhs);
			bool operator!=                                                 (const CombinableErrorHandlingParams& lhs, const CombinableErrorHandlingParams& rhs);

			void                             Log_ErrorHandlingParams              (Util::ILoggingDevice *pLog, const CombinableErrorHandlingParams *pInParams);

			void                             Combine_ErrorHandlingParams          (CombinableErrorHandlingParams *pOutLoggingParams, const CombinableErrorHandlingParams *pInLoggingParams);

			/*************************************************************************************************
			* Parameters of groups, group combinations and resources.
			**************************************************************************************************/
			class CombinableParams
			{
			public:
				CombinableParams() :
					_bActive(true),
					_baseLoadPriority(0),
					_baseUnloadPriority(0),
					_bLoadSynchro(false),
					_bUnloadSynchro(false),
					_bFreezeAutoUnload(false),
					_bFreezeManualUnload(false),
					_bFreezeAutoLoad(false),
					_bFreezeManualLoad(false),
					_subresourceRolesToLoad(SubresourceRoleSet()),
					_subresourceRolesKeepLoaded(SubresourceRoleSet()),
					_subresourceRolesToKeepDependencies(SubresourceRoleSet())
				{
				}

				bool                         IsActive() const { return _bActive; }
				void                         SetIsActive(bool bInValue) { _bActive = bInValue; }

				ResourcePriority             GetBaseLoadPriority() const { return _baseLoadPriority; }
				ResourcePriority             GetBaseUnloadPriority() const { return _baseLoadPriority; }

				void                         SetBaseLoadPriority(ResourcePriority InValue) { _baseLoadPriority = InValue; }
				void                         SetBaseUnloadPriority(ResourcePriority InValue) { _baseUnloadPriority = InValue; }

				// true if loading of resources and their dependencies is always performed  synchronously
				bool                         IsLoadingSynchro() const { return _bLoadSynchro; }
				// true if unloading resources or their dependencies is always performed synchronously
				bool                         IsUnloadingSynchro() const { return _bUnloadSynchro; }

				void                         SetLoadingSynchro(bool bInValue) { _bLoadSynchro = true; }
				void                         SetUnloadingSynchro(bool bInValue) { _bUnloadSynchro = true; }

				bool                         IsAutoUnloadFreezed() const { return _bFreezeAutoUnload; }
				bool                         IsManualUnloadFreezed() const { return _bFreezeManualUnload; }
				bool                         IsAutoLoadFreezed() const { return _bFreezeAutoLoad; }
				bool                         IsManualLoadFreezed() const { return _bFreezeManualLoad; }

				void                         SetAutoLoadingFreezed(bool bInValue) { _bFreezeAutoLoad = bInValue; }
				void                         SetManualLoadingFreezed(bool bInValue) { _bFreezeManualLoad = bInValue; }
				void                         SetAutoUnloadingFreezed(bool bInValue) { _bFreezeAutoUnload = bInValue; }
				void                         SetManualUnloadingFreezed(bool bInValue) { _bFreezeManualUnload = bInValue; }

				SubresourceRoleSet           GetSubresourceRolesToLoad() const { return _subresourceRolesToLoad; }
				SubresourceRoleSet           GetSubresourceRolesToKeepLoaded() const { return _subresourceRolesKeepLoaded; }
				SubresourceRoleSet           GetSubresourceRolesToKeepDependencies() const { return _subresourceRolesToKeepDependencies; }

				void                         SetSubresourceRolesToLoad(SubresourceRoleSet InValue) { _subresourceRolesToLoad = InValue; }
				void                         SetSubresourceRolesToKeepLoaded(SubresourceRoleSet InValue) { _subresourceRolesKeepLoaded = InValue; }
				void                         SetSubresourceRolesToKeepDependencies(SubresourceRoleSet InValue) { _subresourceRolesToKeepDependencies = InValue; }
				 
				const LoggingParams&         GetLoggingParams() const { return _logging; }
				LoggingParams               *GetLoggingParams() { return &_logging; }
				void                         SetLoggingParams(const LoggingParams& InValue) { _logging = InValue; }

				const CombinableErrorHandlingParams             *GetErrorHandlingParams                    () const                      { return &_errorHandling; }
				CombinableErrorHandlingParams                   *GetErrorHandlingParams                    ()                            { return &_errorHandling; }

			private:
				bool                         _bActive;
				ResourcePriority             _baseLoadPriority;
				ResourcePriority             _baseUnloadPriority;
				bool                         _bFreezeAutoUnload;
				bool                         _bFreezeManualUnload;
				bool                         _bFreezeAutoLoad;
				bool                         _bFreezeManualLoad;
				bool                         _bLoadSynchro;
				bool                         _bUnloadSynchro;
				SubresourceRoleSet           _subresourceRolesToLoad;
				SubresourceRoleSet           _subresourceRolesKeepLoaded;
				SubresourceRoleSet           _subresourceRolesToKeepDependencies;
				LoggingParams                _logging;
				CombinableErrorHandlingParams _errorHandling;
			};
			bool operator==                                                 (const CombinableParams& lhs, const CombinableParams& rhs);
			bool operator!=                                                 (const CombinableParams& lhs, const CombinableParams& rhs);

			// Are both loading and unloading (manual an automatic) freezed
			bool IsLoadUnloadFreezed                                        (const CombinableParams *pCombinableParams);
			// Are both automatic and manual loading freezed
			bool IsLoadingFreezed                                           (const CombinableParams *pCombinableParams);
			// Are both automatic and manual unloading freezed
			bool IsUnloadingFreezed                                         (const CombinableParams *pCombinableParams);

			void Combine_CombinableParams                                   (CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams);
			void Combine_BaseLoadPriority                                   (CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams);
			void Combine_BaseUnloadPriority                                 (CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams);

			void Combine_SubresourceRolesToLoad                             (CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams);
			void Combine_SubresourceRolesToKeepLoaded                       (CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams);
			void Combine_SubresourceRolesToKeepDependencies                 (CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams);

			ResourcePriority GetCombinedResourcePriority(ResourcePriority InFirst, ResourcePriority InSecond);

			template<class Cont>
			CombinableParams GetCombinedParams(const Cont& InCont);

			/***********************************************************************************************************
			* implementation
			***********************************************************************************************************/
			template<class Cont>
			CombinableParams GetCombinedParams(const Cont& InCont)
			{
				assert(&resultParams != nullptr);
				assert(resultParams.size() > 0);
				auto it = InCont.begin();
				CombinableParams resultParams = InCont.front();
				while (true)
				{
					++it;
					if (it == InCont.end())
					{
						return resultParams;
					}
					resultParams = Combine_CombinableParams(&resultParams, *it);
				}
				return resultParams;
			}
		}
	}
}