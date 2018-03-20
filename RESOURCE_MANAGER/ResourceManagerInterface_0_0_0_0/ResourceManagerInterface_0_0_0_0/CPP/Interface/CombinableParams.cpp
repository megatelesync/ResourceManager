#include "../../H/Interface/CombinableParams.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			std::string GetString_ResourceCombinableParamsMode(EResourceCombinableParametersMode InMode)
			{
				switch (InMode)
				{
				case EResourceCombinableParametersMode::Combine:
					return "Combine";

				case EResourceCombinableParametersMode::Override:
					return "Override";

				case EResourceCombinableParametersMode::UseGroup:
					return "UseGroup";

				default:
					return "{UNKNOWN}";
				}
			}

			bool operator==(const LoggingParams& lhs, const LoggingParams& rhs)
			{
				if (lhs.ShoudLogAutoLoad() != rhs.ShoudLogAutoLoad())
				{
					return false;
				}
				if (lhs.ShoudLogAutoUnload() != rhs.ShoudLogAutoUnload())
				{
					return false;
				}
				if (lhs.ShoudLogManualLoad() != rhs.ShoudLogManualLoad())
				{
					return false;
				}
				if (lhs.ShoudLogManualUnload() != rhs.ShoudLogManualUnload())
				{
					return false;
				}
				return true;
			}

			bool operator!=(const LoggingParams& lhs, const LoggingParams& rhs)
			{
				return false == operator==(lhs, rhs);
			}

			void Combine_LoggingParams(LoggingParams *pOutLoggingParams, const LoggingParams *pInLoggingParams)
			{
				assert(pOutLoggingParams != nullptr);
				assert(pInLoggingParams != nullptr);
				if (pInLoggingParams->ShoudLogAutoLoad())
				{
					pOutLoggingParams->SetLogAutoLoad(true);
				}
				if (pOutLoggingParams->ShoudLogManualLoad())
				{
					pOutLoggingParams->SetLogManualLoad(true);
				}

				if (pInLoggingParams->ShoudLogAutoUnload())
				{
					pOutLoggingParams->SetLogAutoUnload(true);
				}
				if (pOutLoggingParams->ShoudLogManualUnload())
				{
					pOutLoggingParams->SetLogManualUnload(true);
				}
			}

			bool IsLoadUnloadFreezed(const CombinableParams *pCombinableParams)
			{
				assert(pCombinableParams);
				bool bLoading            = IsLoadingFreezed(pCombinableParams);
				bool bUnload             = IsUnloadingFreezed(pCombinableParams);
				bool bResult             = bLoading && bUnload;
				return bResult;
			}

			bool IsLoadingFreezed(const CombinableParams *pCombinableParams)
			{
				assert(pCombinableParams);
				auto bManual             = pCombinableParams->IsManualLoadFreezed();
				auto bAuto               = pCombinableParams->IsAutoLoadFreezed();
				auto bResult             = bManual && bAuto;
				return bResult;
			}
			
			bool IsUnloadingFreezed(const CombinableParams *pCombinableParams)
			{
				assert(pCombinableParams);
				auto bManual             = pCombinableParams->IsManualUnloadFreezed();
				auto bAuto               = pCombinableParams->IsAutoUnloadFreezed();
				auto bResult             = bManual && bAuto;
				return bResult;
			}

			bool operator==(const CombinableParams& lhs, const CombinableParams& rhs)
			{
				if (lhs.GetLoggingParams() != rhs.GetLoggingParams())
				{
					return false;
				}
				if (*lhs.GetErrorHandlingParams() != *rhs.GetErrorHandlingParams())
				{
					return false;
				}
				if (lhs.GetBaseLoadPriority() != rhs.GetBaseLoadPriority())
				{
					return false;
				}
				if (lhs.GetBaseUnloadPriority() != rhs.GetBaseUnloadPriority())
				{
					return false;
				}
				if (lhs.GetSubresourceRolesToKeepDependencies() != rhs.GetSubresourceRolesToKeepDependencies())
				{
					return false;
				}
				if (lhs.GetSubresourceRolesToKeepLoaded() != rhs.GetSubresourceRolesToKeepLoaded())
				{
					return false;
				}
				if (lhs.GetSubresourceRolesToLoad() != rhs.GetSubresourceRolesToLoad())
				{
					return false;
				}
				if (lhs.IsActive() != rhs.IsActive())
				{
					return false;
				}
				if (lhs.IsAutoLoadFreezed() != rhs.IsAutoLoadFreezed())
				{
					return false;
				}
				if (lhs.IsAutoUnloadFreezed() != rhs.IsAutoUnloadFreezed())
				{
					return false;
				}
				if (lhs.IsManualLoadFreezed() != rhs.IsManualLoadFreezed())
				{
					return false;
				}
				if (lhs.IsManualUnloadFreezed() != rhs.IsManualUnloadFreezed())
				{
					return false;
				}
				if (lhs.IsLoadingSynchro() != rhs.IsLoadingSynchro())
				{
					return false;
				}
				if (lhs.IsUnloadingSynchro() != rhs.IsUnloadingSynchro())
				{
					return false;
				}
				return true;
			}
			bool operator!=(const CombinableParams& lhs, const CombinableParams& rhs)
			{
				return false == operator==(lhs, rhs);
			}

			bool operator==(const CombinableErrorHandlingParams& lhs, const CombinableErrorHandlingParams& rhs)
			{
				if (lhs.IsFatal() != rhs.IsFatal())
				{
					return false;
				}
				return true;
			}
			bool operator!=(const CombinableErrorHandlingParams& lhs, const CombinableErrorHandlingParams& rhs)
			{
				return false == operator==(lhs, rhs);
			}
			

			void Log_ErrorHandlingParams(Util::ILoggingDevice *pLog, const CombinableErrorHandlingParams *pInParams)
			{
				assert(pLog != nullptr);
				assert(pInParams != nullptr);

				Util::LogRecordScoper scoper(pLog, "Combinable Error-Handling params:", Util::EMessageType::Info);
				*pLog << "IsFatal: " << pInParams->IsFatal() << Util::endl;
			}

			void Combine_ErrorHandlingParams(CombinableErrorHandlingParams *pOutLoggingParams, const CombinableErrorHandlingParams *pInLoggingParams)
			{
				if (pInLoggingParams->IsFatal())
				{
					pOutLoggingParams->SetFatal(true);
				}
			}

			void Log_CombinableLoggingParams(Util::ILoggingDevice *pLog, const LoggingParams *pInLoggingParams)
			{
				Util::LogRecordScoper scoper(pLog, "Combinable logging params:", Util::EMessageType::Info);

				*pLog << "log auto load: " << pInLoggingParams->ShoudLogAutoLoad() << Util::endl;
				*pLog << "log maual load: " << pInLoggingParams->ShoudLogManualLoad() << Util::endl;
				*pLog << "log auto unload: " << pInLoggingParams->ShoudLogAutoUnload() << Util::endl;
				*pLog << "log maual unload: " << pInLoggingParams->ShoudLogManualUnload() << Util::endl;
			}

			void Combine_CombinableParams(CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams)
			{
				assert(pOutCombinedParams != nullptr);
				assert(pInCombinedParams != nullptr);

				Combine_LoggingParams(pOutCombinedParams->GetLoggingParams(), &(pInCombinedParams->GetLoggingParams()));
				Combine_ErrorHandlingParams(pOutCombinedParams->GetErrorHandlingParams(), pInCombinedParams->GetErrorHandlingParams());

				if (pInCombinedParams->IsActive())
				{
					pOutCombinedParams->SetIsActive(true);
				}

				Combine_BaseLoadPriority(pOutCombinedParams, pInCombinedParams);
				Combine_BaseUnloadPriority(pOutCombinedParams, pInCombinedParams);

				if (pInCombinedParams->IsAutoUnloadFreezed())
				{
					pOutCombinedParams->SetAutoUnloadingFreezed(true);
				}

				if (pInCombinedParams->IsManualUnloadFreezed())
				{
					pOutCombinedParams->SetManualUnloadingFreezed(true);
				}

				if (pInCombinedParams->IsAutoLoadFreezed())
				{
					pOutCombinedParams->SetAutoLoadingFreezed(true);
				}

				if (pInCombinedParams->IsManualLoadFreezed())
				{
					pOutCombinedParams->SetManualLoadingFreezed(true);
				}

				if (pInCombinedParams->IsLoadingSynchro())
				{
					pOutCombinedParams->SetLoadingSynchro(true);
				}

				if (pInCombinedParams->IsUnloadingSynchro())
				{
					pOutCombinedParams->SetUnloadingSynchro(true);
				}

				Combine_SubresourceRolesToKeepDependencies(pOutCombinedParams, pInCombinedParams);
				Combine_SubresourceRolesToLoad(pOutCombinedParams, pInCombinedParams);
				Combine_SubresourceRolesToKeepLoaded(pOutCombinedParams, pInCombinedParams);
			}

			void Combine_SubresourceRolesToLoad(CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams)
			{
				auto newSubresourceRolesToLoad = pOutCombinedParams->GetSubresourceRolesToLoad();
				newSubresourceRolesToLoad.MergeWith(pInCombinedParams->GetSubresourceRolesToLoad());
				pOutCombinedParams->SetSubresourceRolesToLoad(newSubresourceRolesToLoad);
			}

			void Combine_SubresourceRolesToKeepLoaded(CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams)
			{
				auto newSubresourceRolesToLoad = pOutCombinedParams->GetSubresourceRolesToKeepLoaded();
				newSubresourceRolesToLoad.MergeWith(pInCombinedParams->GetSubresourceRolesToKeepLoaded());
				pOutCombinedParams->SetSubresourceRolesToKeepLoaded(newSubresourceRolesToLoad);
			}

			void Combine_SubresourceRolesToKeepDependencies(CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams)
			{
				auto newSubresourceRolesToKeepDependencies = pOutCombinedParams->GetSubresourceRolesToKeepDependencies();
				newSubresourceRolesToKeepDependencies.MergeWith(pInCombinedParams->GetSubresourceRolesToKeepDependencies());
				pOutCombinedParams->SetSubresourceRolesToKeepDependencies(newSubresourceRolesToKeepDependencies);
			}

			void Combine_BaseLoadPriority(CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams)
			{
				auto outPrio = pOutCombinedParams->GetBaseLoadPriority();
				auto inputPrio = pInCombinedParams->GetBaseLoadPriority();
				auto combinedPrio = GetCombinedResourcePriority(outPrio, inputPrio);
				pOutCombinedParams->SetBaseLoadPriority(combinedPrio);
			}

			void Combine_BaseUnloadPriority(CombinableParams *pOutCombinedParams, const CombinableParams *pInCombinedParams)
			{
				auto outPrio = pOutCombinedParams->GetBaseUnloadPriority();
				auto inputPrio = pInCombinedParams->GetBaseUnloadPriority();
				auto combinedPrio = GetCombinedResourcePriority(outPrio, inputPrio);
				pOutCombinedParams->SetBaseUnloadPriority(combinedPrio);
			}

			ResourcePriority GetCombinedResourcePriority(ResourcePriority InFirst, ResourcePriority InSecond)
			{
				if (InFirst < InSecond)
				{
					return InSecond;
				}
				else
				{
					return InFirst;
				}
			}
		}
	}
}