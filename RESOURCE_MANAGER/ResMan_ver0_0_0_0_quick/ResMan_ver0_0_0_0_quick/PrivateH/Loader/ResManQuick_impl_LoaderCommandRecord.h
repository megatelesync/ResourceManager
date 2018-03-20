#pragma once

#include "Fwd/ResManQuick_Fwd_Impl.h"
#include "../../PrivateH/Fwd/ResManQuick_Fwd_ManagedGroup.h"
#include "../ResManQuick_impl_System.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			class LoaderCommandRecord
			{
			public:
				LoaderCommandRecord
				(
					ResMan::LoadResourceCommandID                     InCommandID,
					ResMan::LoadResourceCommandID                     InInitialRequestCommandID,
					size_t                                            InActualPriority,
					LoadCommandBase                                  *pInCommand, 
					LoaderCommandMainThreadData                      *pInMainThreadData,
					Loader_PerResource_PerLoadingLoop                *pInLoopData
				);
				// Create child loader command record base on the given result record;
				// The initial resource command id will be set to the id of command corresponding to the given result.
				LoaderCommandRecord
				(
					ResMan::LoadResourceCommandID                     InCommandID,
					const LoaderCommandResultRecord&                  InResultRecord,
					size_t                                            InActualPriority,
					LoadCommandBase                                  *pInCommand
				);

				/******************************************************************************************************
				* IDs
				******************************************************************************************************/
				// Get ID of the current resource load command
				ResMan::LoadResourceCommandID                          GetResourceCommandID                          () const { return _resourceCommandId; }
				// Get ID of the initial resource load command
				ResMan::LoadResourceCommandID                          GetInitialResourceCommandID                   () const { return _initialResourceCommandId; }

				/**************************************************************************************
				* Getters
				***************************************************************************************/
				size_t                                                 GetActualPriority                                 () const { return _actualPriority; }
				LoadCommandBase                                       *GetCommand                                        () const { return _pCommand; }
				const LoaderCommandMainThreadData                     *GetAttachedMainThreadData                         () const { return static_cast<const LoaderCommandMainThreadData *>(_pAttachedMainThreadData); }
				Loader_PerResource_PerLoadingLoop                     *GetResource_LoadingLoop                           () const { return _pLoaderLoopData; }

				/**************************************************************************************
				* Helper getters
				***************************************************************************************/
				const Loader_PerResource_ForLoadingLoopThread          *GetResource_LoadingLoopThreadData                () const;
				Impl::Impl_ManagedGroup                                *GetGroup                                         () const;

			private:
				size_t                                                    _actualPriority;
				LoadCommandBase                                          *_pCommand;
				LoaderCommandMainThreadData                              *_pAttachedMainThreadData;
				Loader_PerResource_PerLoadingLoop                        *_pLoaderLoopData;

				ResMan::LoadResourceCommandID                             _resourceCommandId;
				ResMan::LoadResourceCommandID                             _initialResourceCommandId;
			};
			bool operator<(const LoaderCommandRecord& lhs, const LoaderCommandRecord& rhs);
		}
	}
}