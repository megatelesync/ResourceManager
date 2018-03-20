#pragma once

#include "../H/ResManQuick_ManagedGroup.h"

#include <list>
#include <vector>
#include <memory>

/********************************************************************************************************
* TODO:
*********************************************************************************************************/

namespace Dv
{
	namespace ResManQuick
	{
		namespace Impl
		{
			// Maximal number of groups;
			// WARNING!!! This value must be consistent with the ResourceGroupCombiCont container type
			// (that is typed as ResourceGroupCombiCont_Cont)
			const size_t MAX_GROUPS = 32;

			using GroupCont_SequentialCont                           = std::list<std::shared_ptr<ManagedGroup>>;
			using GroupCont_SequentialIterator                       = GroupCont_SequentialCont::iterator;

			/*********************************************************************************
			* Element of the group container.
			*********************************************************************************/
			class GroupContElement
			{
			public:
				friend class GroupCont;

				GroupContElement                                                     ();

				int                                       GetID                      () const                       { assert(_index >= 0); return _index;  }
				GroupCont_SequentialIterator              GetSequentialIterator      () const                       { assert(_index >= 0); return _it_sequential; }
				ManagedGroup                             *GetGroup                   () const                       { return GetSequentialIterator()->get(); }
				std::shared_ptr<ManagedGroup>             GetGroupPtr                () const                       { return *GetSequentialIterator(); }

				bool                                      IsFree                     () const                       { return _bFree; }

			private:
				void                                      _Initialize                (size_t InIndex);
				void                                      _BindGroupIterator         (GroupCont_SequentialIterator InIterator);
				void                                      _MakeFree();

				GroupCont_SequentialIterator                          _it_sequential;
				int                                                    _index = -1;
				bool                                                   _bFree = true;
			};
			using GroupCont_Cont = GroupContElement[MAX_GROUPS];

			/**********************************************************************************
			* Container for group storage
			**********************************************************************************/
			class GroupCont
			{
			public:
				const int NumGroupSlots                              = MAX_GROUPS;

				GroupCont                                                                        ();

				ManagedGroup                                         *GetGroupByID               (size_t InGroupID) const;
				std::shared_ptr<ManagedGroup>                         GetGroupPtrByID            (size_t InGroupID) const;

				// Get any free element
				GroupContElement                                     *GetFree                    ();
				void                                                  Register                   (GroupContElement *pFreeElem, ManagedGroup *pGroup);
				void                                                  Delete                     (GroupContElement *pFreeElem);

				// Get list of all groups
				const GroupCont_SequentialCont&                       GetSequentialGroups        () const                             { return _sequentialGroups; }

			private:
				GroupCont_Cont                                        _groups;
				GroupCont_SequentialCont                              _sequentialGroups;
			};
		}
	}
}