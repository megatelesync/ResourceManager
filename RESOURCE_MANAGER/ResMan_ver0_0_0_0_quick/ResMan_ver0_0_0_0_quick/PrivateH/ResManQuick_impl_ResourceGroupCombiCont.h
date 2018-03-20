#pragma once

#include "../H/ResManQuick_System.h"
#include "ResManQuick_impl_GroupCont.h"

#include <bitset>
#include <map>

/********************************************************************************************
* TODO:
*
* 1. 
*********************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			using GroupMask                                     = std::bitset<MAX_GROUPS>;
			using ResourceGroupCombi_SequentialGroupCont        = std::list<ManagedGroup *>;

			class ResourceGroupCombiCont;

			class ResourceGroupCombi
			{
			public:
				ResourceGroupCombi                                                               (ResourceGroupCombiCont *pInOwner, GroupMask InGroupMask);

				const GroupMask                                     &GetGroupMask                () const               { return _groupMask; }
				const ResMan::CombinableParams                      &GetCombinedParams           () const               { return _combinedParams; }
				const ResourceGroupCombi_SequentialGroupCont        &GetActiveGroups             () const               { return _activeGroups;  }

				// Recalculate both set of active groups and combined params
				void                                                 RecalculateAll              ();
				void                                                 RecalculateCombinedParams   ();

			private:
				void                                                _RecalculateActiveGroupSequence ();

				ResourceGroupCombiCont                              *_pOwnerCont;
				GroupMask                                            _groupMask;
				ResMan::CombinableParams                             _combinedParams;
				ResourceGroupCombi_SequentialGroupCont               _activeGroups;
			};			

			// This map will only work if count of groups is no more than 32.
			using GroupMaskUInt                            = uint32_t;
			using ResourceGroupCombiCont_Cont              = std::map<GroupMaskUInt, ResourceGroupCombi>;
			using ResourceGroupCombiCont_iterator          = ResourceGroupCombiCont_Cont::iterator;

			// This function always must be used to convert the given bitset group mask representation to its UInt counterpart.
			GroupMaskUInt                  GroupMaskToUInt             (GroupMask InMask);
			bool                           IsGroupSet                  (GroupMaskUInt InMask, size_t InGroupID);

			/************************************************************************************************************
			* ResourceGroupCombiCont
			*
			* Stores all groups, their combinations and precalculated state of the combinations.
			************************************************************************************************************/
			class ResourceGroupCombiCont
			{
			public:
				ResourceGroupCombiCont() {}

				ManagedGroup                                        *GetGroupByID                                (size_t InGroupID) const                                     { return _groups.GetGroupByID(InGroupID);  }

				GroupContElement                                    *GetFreeGroupElement                         ();
				void                                                 RegisterGroup                               (GroupContElement *pFreeElement, ManagedGroup *pNewGroup);

				// Delete the group and old combis containing the group
				void                                                 DeleteGroup                                 (ManagedGroup *pInGroup);
				// Recalculate combis that must be recalculated when the given group changed
				void                                                 RecalculateCombinedParams_ForGroup          (ManagedGroup *pInGroup);
				ResourceGroupCombi                                  *PrecalculateCombi                           (GroupMask InGroupMask);

				const GroupCont_SequentialCont&                      GetAllGroups                                () const { return _groups.GetSequentialGroups(); }

			private:
				GroupCont                                            _groups;
				ResourceGroupCombiCont_Cont                          _groupCombis;
			};

			/************************************************************************************************************
			* mask operations implementation
			************************************************************************************************************/
			inline GroupMaskUInt GroupMaskToUInt(GroupMask InMask)
			{
				return static_cast<GroupMaskUInt>(InMask.to_ulong());
			}
			inline bool IsGroupSet(GroupMaskUInt InMask, size_t InGroupID)
			{
				return (InMask >> InGroupID) & 1;
			}
		}
	}
}