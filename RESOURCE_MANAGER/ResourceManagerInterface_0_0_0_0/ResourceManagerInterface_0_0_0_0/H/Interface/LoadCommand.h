#pragma once

#include "../Core/ResourceManagerTypes.h"
#include "../Utils/ISubresourceRole.h"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			/*******************************************************************************************************
			* Load resource command id (both precache dependency lists or subresources)
			*******************************************************************************************************/
			class LoadResourceCommandID
			{
			public:
				LoadResourceCommandID                                         ()                           : _id(0xFFFFFFFF) {}
				explicit                    LoadResourceCommandID             (size_t InID)                : _id(InID) {}
				size_t                      GetValue                          () const                     { return _id; }

			private:
				size_t                      _id;
			};
			bool operator==                 (LoadResourceCommandID lhs, LoadResourceCommandID rhs);
			bool operator!=                 (LoadResourceCommandID lhs, LoadResourceCommandID rhs);
			bool operator<                  (LoadResourceCommandID lhs, LoadResourceCommandID rhs);

			/*******************************************************************************************************
			* Load group command id
			*******************************************************************************************************/
			class LoadGroupCommandID
			{
			public:
				LoadGroupCommandID                                                      ()                              : _id(0xFFFFFFFF) {}
				explicit                    LoadGroupCommandID                          (size_t InID)                   : _id(InID) {}
				size_t                      GetValue                                    () const                        { return _id; }

			private:
				size_t                      _id;
			};
			bool operator==                 (LoadGroupCommandID lhs, LoadGroupCommandID rhs);
			bool operator!=                 (LoadGroupCommandID lhs, LoadGroupCommandID rhs);
			bool operator<                  (LoadGroupCommandID lhs, LoadGroupCommandID rhs);


			/*******************************************************************************************************
			* Command definitions
			*******************************************************************************************************/
			struct ResourceCommandArgs
			{
				Urgency						urgency;
				SubresourceRoleSet			subresources;
			};

			struct UnloadCommand
			{
				ResourceCommandArgs args;
			};

			struct LoadCommand
			{
				ResourceCommandArgs args;
			};

			struct PrecacheDependenciesCommand
			{
				ResourceCommandArgs args;
			};

			/********************************************************************************
			* Load resource command id implementation
			********************************************************************************/
			inline bool operator==(LoadResourceCommandID lhs, LoadResourceCommandID rhs)
			{
				return lhs.GetValue() < rhs.GetValue();
			}
			inline bool operator!=(LoadResourceCommandID lhs, LoadResourceCommandID rhs)
			{
				return lhs.GetValue() != rhs.GetValue();
			}
			inline bool operator<(LoadResourceCommandID lhs, LoadResourceCommandID rhs)
			{
				return lhs.GetValue() < rhs.GetValue();
			}

			/********************************************************************************
			* Load group command id implementation
			********************************************************************************/
			inline bool operator==(LoadGroupCommandID lhs, LoadGroupCommandID rhs)
			{
				return lhs.GetValue() < rhs.GetValue();
			}
			inline bool operator!=(LoadGroupCommandID lhs, LoadGroupCommandID rhs)
			{
				return lhs.GetValue() != rhs.GetValue();
			}
			inline bool operator<(LoadGroupCommandID lhs, LoadGroupCommandID rhs)
			{
				return lhs.GetValue() < rhs.GetValue();
			}
		}
	}
}