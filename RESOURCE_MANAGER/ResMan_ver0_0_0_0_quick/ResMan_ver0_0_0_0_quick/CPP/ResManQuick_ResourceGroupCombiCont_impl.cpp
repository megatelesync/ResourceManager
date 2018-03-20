#pragma once

#include "../PrivateH/ResManQuick_impl_ResourceGroupCombiCont.h"
#include "../PrivateH/ResManQuick_impl_access_ManagedGroup.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			ResMan::CombinableParams CalculateCombinedParamsOfGroups(const ResourceGroupCombi_SequentialGroupCont& InCont)
			{
				auto it = InCont.begin();
				auto pFirstGroupParams = (*it)->GetCombinableParams();
				ResMan::CombinableParams resultParams = *pFirstGroupParams;
				while (true)
				{
					++it;
					if (it == InCont.end())
					{
						return resultParams;
					}
					auto pGroupParams = (*it)->GetCombinableParams();
					ResMan::Combine_CombinableParams(&resultParams, pGroupParams);
				}
				return resultParams;
			}

			// Find the first combi that includes the given group
			ResourceGroupCombiCont_iterator Find_FirstCombi_ThatIncludesGroup(ResourceGroupCombiCont_Cont *pInCombiMap, ManagedGroup *pInGroup)
			{				
				for (auto it = pInCombiMap->begin(); it != pInCombiMap->end(); ++it)
				{
					GroupMaskUInt combi_group_mask = it->first;
					
					if (IsGroupSet(combi_group_mask, GetID(pInGroup)))
					{
						return it;
					}
				}
				return pInCombiMap->end();
			}

			/*********************************************************************************************
			* ResourceGroupCombi implementation
			*********************************************************************************************/
			ResourceGroupCombi::ResourceGroupCombi(ResourceGroupCombiCont *pInOwner, GroupMask InGroupMask) :
				_pOwnerCont(pInOwner),
				_groupMask(InGroupMask)
			{
			}

			void ResourceGroupCombi::RecalculateAll()
			{
				_RecalculateActiveGroupSequence();
				RecalculateCombinedParams();
			}
			void ResourceGroupCombi::RecalculateCombinedParams()
			{
				_combinedParams = CalculateCombinedParamsOfGroups(_activeGroups);
			}

			void ResourceGroupCombi::_RecalculateActiveGroupSequence()
			{
				_activeGroups.clear();
				for (int group_slot_index = 0; group_slot_index < MAX_GROUPS; group_slot_index++)
				{
					if (_groupMask[group_slot_index])
					{
						auto pGroup = _pOwnerCont->GetGroupByID(group_slot_index);
						if (IsActive(pGroup))
						{
							_activeGroups.push_back(pGroup);
						}
					}
				}
			}


			/*********************************************************************************************
			* ResourceGroupCombiCont implementation
			*********************************************************************************************/
			GroupContElement *ResourceGroupCombiCont::GetFreeGroupElement()
			{
				return _groups.GetFree();
			}

			void ResourceGroupCombiCont::RegisterGroup(GroupContElement *pFreeElement, ManagedGroup *pNewGroup)
			{
				_groups.Register(pFreeElement, pNewGroup);
			}

			void ResourceGroupCombiCont::DeleteGroup(ManagedGroup *pInGroup)
			{
				assert(pInGroup != nullptr);
				auto it_first_combi = Find_FirstCombi_ThatIncludesGroup(&_groupCombis, pInGroup);
				for (auto it_combi = it_first_combi; it_combi != _groupCombis.end(); )
				{
					GroupMaskUInt mask = it_combi->first;
					if (IsGroupSet(mask,GetID(pInGroup)))
					{
						it_combi = _groupCombis.erase(it_combi);
					}
					else
					{
						++it_combi;
					}
				}
				_groups.Delete(GetSlot(pInGroup));
			}

			void ResourceGroupCombiCont::RecalculateCombinedParams_ForGroup(ManagedGroup *pInGroup)
			{
				assert(pInGroup != nullptr);
				auto it_first_combi = Find_FirstCombi_ThatIncludesGroup(&_groupCombis, pInGroup);
				for (auto it_combi = it_first_combi; it_combi != _groupCombis.end(); ++it_combi)
				{
					GroupMaskUInt mask = it_combi->first;
					if (mask & (1 << GetID(pInGroup)))
					{
						it_combi->second.RecalculateCombinedParams();
					}
				}	
			}

			ResourceGroupCombi *ResourceGroupCombiCont::PrecalculateCombi(GroupMask InGroupMask)
			{
				auto it_existing_combi = _groupCombis.find(GroupMaskToUInt(InGroupMask));
				if (it_existing_combi != _groupCombis.end())
				{
					return &(it_existing_combi->second);
				}
				else
				{
					auto insert_res = _groupCombis.insert(std::pair<GroupMaskUInt, ResourceGroupCombi>(GroupMaskToUInt(InGroupMask), ResourceGroupCombi(this, InGroupMask)));
					auto it_new_combi = insert_res.first;
					auto& new_combi = it_new_combi->second;
					new_combi.RecalculateAll();
					return &new_combi;
				}
			}
		}
	}
}