#include "../PrivateH/ResManQuick_impl_GroupCont.h"

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			/********************************************************************************************************
			* GroupContElement implementation
			*********************************************************************************************************/
			GroupContElement::GroupContElement() : _index(-1)
			{
			}
			void GroupContElement::_Initialize(size_t InIndex)
			{				
				assert(InIndex >= 0);
				assert(_index < 0);
				_index = InIndex;
			}
			void GroupContElement::_BindGroupIterator(GroupCont_SequentialIterator InIterator)
			{
				assert(_index >= 0);
				_it_sequential = InIterator;
				_bFree = false;
			}
			void GroupContElement::_MakeFree()
			{
				_bFree = true;
			}


			/********************************************************************************************************
			* GroupCont implementation
			*********************************************************************************************************/
			GroupCont::GroupCont()
			{
				for (int i = 0; i < NumGroupSlots; i++)
				{
					_groups[i]._Initialize(i);
				}
			}

			ManagedGroup *GroupCont::GetGroupByID(size_t InGroupID) const
			{
				assert(InGroupID >= 0);
				assert(InGroupID < NumGroupSlots);
				auto& slot = _groups[InGroupID];
				assert(!slot.IsFree());
				return slot.GetSequentialIterator()->get();
			}

			std::shared_ptr<ManagedGroup> GroupCont::GetGroupPtrByID(size_t InGroupID) const
			{
				assert(InGroupID >= 0);
				assert(InGroupID < NumGroupSlots);
				auto& slot = _groups[InGroupID];
				assert(!slot.IsFree());
				return *slot.GetSequentialIterator();
			}

			GroupContElement *GroupCont::GetFree()
			{
				for (int group_slot_index = 0; group_slot_index < NumGroupSlots; ++group_slot_index)
				{
					auto& group_slot = _groups[group_slot_index];
					if (group_slot.IsFree())
					{
						return &group_slot;
					}
				}
				assert(!"No free slots to register new group");
				return nullptr;
			}

			void GroupCont::Register(GroupContElement *pFreeElem, ManagedGroup *pGroup)
			{
				assert(pFreeElem != nullptr);
				assert(pGroup != nullptr);
				assert(pFreeElem->IsFree());
				const size_t curr_slot_index = pFreeElem->GetID();
				GroupCont_SequentialIterator it_group_next_pos = _sequentialGroups.end();
				if (curr_slot_index < NumGroupSlots)
				{
					// Find next group in the list of sequential groups to insert new element in the list
					auto next_slot_index = curr_slot_index;
					while (true)
					{
						++next_slot_index;
						if (next_slot_index >= NumGroupSlots)
						{
							break;
						}
						auto& next_slot = _groups[next_slot_index];
						if (!next_slot.IsFree())
						{
							it_group_next_pos = next_slot.GetSequentialIterator();
						}
					}
				}
				auto it_new_group_pos = _sequentialGroups.insert(it_group_next_pos, std::shared_ptr<ManagedGroup>(pGroup));
				_groups[curr_slot_index]._BindGroupIterator(it_new_group_pos);
			}

			void GroupCont::Delete(GroupContElement *pFreeElem)
			{
				assert(pFreeElem != nullptr);
				assert(!pFreeElem->IsFree());
				auto it_group = pFreeElem->GetSequentialIterator();
				_groups[pFreeElem->GetID()]._MakeFree();
				_sequentialGroups.erase(it_group);
			}
		}
	}
}