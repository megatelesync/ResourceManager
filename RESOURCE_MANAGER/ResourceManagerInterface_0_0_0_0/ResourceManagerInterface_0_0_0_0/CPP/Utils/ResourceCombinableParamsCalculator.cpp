#include "../../H/Utils/ResourceCombinableParamsCalculator.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			ResourceCombinableParamsCalculator::ResourceCombinableParamsCalculator() : 
				_mode(EResourceCombinableParametersMode::Combine)
			{
			}

			const CombinableParams *ResourceCombinableParamsCalculator::GetResultAggregatedParams() const
			{
				return &_resultAgreggatedParams;
			}

			const CombinableParams *ResourceCombinableParamsCalculator::GetGroupParams() const
			{
				return _groupParams.get_ptr();
			}

			void ResourceCombinableParamsCalculator::SetMode(EResourceCombinableParametersMode InNewValue)
			{
				_mode = InNewValue;
				_RecalculateResultParams();
			}

			void ResourceCombinableParamsCalculator::SetOwnParams(const CombinableParams *pInNewValue)
			{
				assert(pInNewValue != nullptr);
				_ownParams = *pInNewValue;
				_RecalculateResultParams();
			}

			void ResourceCombinableParamsCalculator::Notify_GroupParamsUpdated(const CombinableParams *pInNullOrNewValue)
			{
				if (pInNullOrNewValue == nullptr)
				{
					_groupParams = boost::optional<CombinableParams>();
				}
				else
				{
					_groupParams = *pInNullOrNewValue;
				}
				_RecalculateResultParams();
			}

			void ResourceCombinableParamsCalculator::_RecalculateResultParams()
			{
				if (_mode == EResourceCombinableParametersMode::Override || !_groupParams.is_initialized())
				{
					_resultAgreggatedParams = _ownParams;
				}
				else
				{
					_resultAgreggatedParams = _groupParams.get();
					if (_mode == EResourceCombinableParametersMode::Combine)
					{
						Combine_CombinableParams(&_resultAgreggatedParams, &_ownParams);
					}
				}
			}
		}
	}
}