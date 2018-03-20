#pragma once

#include "../Interface/CombinableParams.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			class ResourceCombinableParamsCalculator
			{
			public:
				ResourceCombinableParamsCalculator();

				/**************************************************************************************************************************
				* GETTING RESULT
				**************************************************************************************************************************/
				const CombinableParams                                            *GetResultAggregatedParams() const;

				/**************************************************************************************************************************
				* GETTERS
				**************************************************************************************************************************/
				EResourceCombinableParametersMode       GetMode                                               () const          { return _mode; }
				const CombinableParams                 *GetGroupParams                                        () const;
				const CombinableParams                 *GetOwnParams                                          () const          { return &_ownParams; }

				/**************************************************************************************************************************
				* SETTERS
				**************************************************************************************************************************/
				void                                    SetMode                                                (EResourceCombinableParametersMode InNewValue);
				void                                    SetOwnParams                                           (const CombinableParams *pInNewValue);

				/**************************************************************************************************************************
				* EVENT-HANDLING
				**************************************************************************************************************************/
				void                                    Notify_GroupParamsUpdated                             (const CombinableParams *pInNullOrNewValue);

			private:
				void 					                _RecalculateResultParams();

				CombinableParams                                                        _resultAgreggatedParams;
				boost::optional<CombinableParams>                                       _groupParams;
				CombinableParams                                                        _ownParams;
				EResourceCombinableParametersMode                                       _mode;
			};
		}
	}
}