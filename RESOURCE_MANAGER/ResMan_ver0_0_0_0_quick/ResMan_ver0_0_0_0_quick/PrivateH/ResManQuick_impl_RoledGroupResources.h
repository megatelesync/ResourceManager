#pragma once

#include <list>
#include <cassert>

/********************************************************************************************************
* TODO:
*********************************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		class ManagedResource;

		namespace Impl
		{
			using RoledGroupResources_ResourceListCont = std::list<ManagedResource *>;
			using RoledGroupResources_Iterator         = RoledGroupResources_ResourceListCont::iterator;

			/********************************************************************************************
			* State of the resource load
			*********************************************************************************************/
			enum class EResourceLoadState
			{
				NotPrecached = 0,
				Precached,
				PrecacheFailed,
				NUM_STATES
			};
			const size_t NumResourceLoadStates = static_cast<size_t>(EResourceLoadState::NUM_STATES);

			/********************************************************************************************
			* Pointer to group resource record
			*
			* To be always passed by value
			*********************************************************************************************/
			class GroupResourceRecordPointer
			{
			public:
				GroupResourceRecordPointer              (RoledGroupResources_Iterator InIterator, EResourceLoadState InLoadState);

				ManagedResource                         *operator*                        () const { return *_iterator; }
				ManagedResource                         *operator->                       () const { return *_iterator; }

				RoledGroupResources_Iterator             GetIterator                      () const { return _iterator; }
				EResourceLoadState                       GetLoadState                     () const { return _loadState; }

			private:
				RoledGroupResources_Iterator                                              _iterator;
				EResourceLoadState                                                        _loadState;
			};

			/*********************************************************************************************
			* Contains all resources references for the given role of the given group.
			*********************************************************************************************/
			struct RoledGroupResources
			{
			public:
				using resource_list          = RoledGroupResources_ResourceListCont;

				const resource_list           *GetResourceList_Precached                  () const       { return GetResourceList_ForState(EResourceLoadState::Precached); }
				resource_list                 *GetResourceList_Precached                  ()             { return GetResourceList_ForState(EResourceLoadState::Precached); }

				const resource_list           *GetResourceList_PrecachedFailed            () const       { return GetResourceList_ForState(EResourceLoadState::PrecacheFailed); }
				resource_list                 *GetResourceList_PrecachedFailed            ()             { return GetResourceList_ForState(EResourceLoadState::PrecacheFailed); }

				const resource_list           *GetResourceList_NotPrecached               () const       { return GetResourceList_ForState(EResourceLoadState::NotPrecached); }
				resource_list                 *GetResourceList_NotPrecached               ()             { return GetResourceList_ForState(EResourceLoadState::NotPrecached); }

				const resource_list           *GetResourceList_ForState                  (EResourceLoadState InLoadState) const { assert(static_cast<size_t>(InLoadState) < NumResourceLoadStates); return &(stateResources[static_cast<size_t>(InLoadState)]); }
				resource_list                 *GetResourceList_ForState                  (EResourceLoadState InLoadState) { assert(static_cast<size_t>(InLoadState) < NumResourceLoadStates); return &(stateResources[static_cast<size_t>(InLoadState)]); }

				GroupResourceRecordPointer     Register                                  (EResourceLoadState InLoadState, ManagedResource *pInResource);
				GroupResourceRecordPointer     MoveState                                 (GroupResourceRecordPointer InPointer, EResourceLoadState InNewLoadState);
				void                           Erase                                     (GroupResourceRecordPointer InPointer);
				void                           Clear                                     ();

			private:
				resource_list                  stateResources[NumResourceLoadStates];
			};


			/********************************************************************************************
			* Pointer to group resource record implementation
			*********************************************************************************************/
			inline GroupResourceRecordPointer::GroupResourceRecordPointer(RoledGroupResources_Iterator InIterator, EResourceLoadState InLoadState) :
				_iterator(InIterator),
				_loadState(InLoadState)
			{
			}

			inline GroupResourceRecordPointer RoledGroupResources::Register(EResourceLoadState InLoadState, ManagedResource *pInResource)
			{
				auto pCont = GetResourceList_ForState(InLoadState);
				auto it_cont = pCont->insert(pCont->end(), pInResource);
				return GroupResourceRecordPointer(it_cont, InLoadState);
			}
			inline GroupResourceRecordPointer RoledGroupResources::MoveState(GroupResourceRecordPointer InPointer, EResourceLoadState InNewLoadState)
			{
				auto pResource = *InPointer.GetIterator();
				Erase(InPointer);
				auto newResourceRecordPointer = Register(InNewLoadState, pResource);
				return newResourceRecordPointer;
			}
			inline void RoledGroupResources::Erase(GroupResourceRecordPointer InPointer)
			{
				GetResourceList_ForState(InPointer.GetLoadState())->erase(InPointer.GetIterator());
			}
			inline void RoledGroupResources::Clear()
			{
				for (int i = 0; i < NumResourceLoadStates; i++)
				{
					stateResources[i].clear();
				}
			}
		}
	}
}