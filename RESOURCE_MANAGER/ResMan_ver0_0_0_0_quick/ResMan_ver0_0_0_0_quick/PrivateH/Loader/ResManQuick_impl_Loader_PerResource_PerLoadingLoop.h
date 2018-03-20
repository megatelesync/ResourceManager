#pragma once

#include "ResManQuick_impl_LoaderCommandRecord.h"
#include <queue>
#include <cassert>
#include <boost/optional.hpp>

namespace Dv
{
	namespace ResManQuick
	{
		namespace Loader
		{
			/*****************************************************************************************
			* Part of the data to be connected with each of the managed resource objects
			* for each loading loop - the part that is TO BE ACCESSIBLE ONLY FROM THE MAIN THREAD
			*****************************************************************************************/
			class Loader_PerResource_ForMainThread
			{
			public:
				// Num unprocessed commands:
				// Number of commands that were added to the queue and not yet returned as result
				size_t                          GetNumUnprocessedCommands() const { return _numUnprocessedCommands; }

				void                            IncrUnprocessedCommands() { _numUnprocessedCommands++; }
				size_t                          DecrementUnprocessedCommands() { assert(_numUnprocessedCommands > 0); --_numUnprocessedCommands; return _numUnprocessedCommands; }

			private:
				size_t                         _numUnprocessedCommands = 0;
			};

			/*****************************************************************************************
			* Part of the data to be connected with each of the managed resource objects
			* that is TO BE accesible only from thread that the corresponding runs on
			* (it may be main thread or dedicated thread).
			*****************************************************************************************/
			class Loader_PerResource_ForLoadingLoopThread
			{
			public:
				Loader_PerResource_ForLoadingLoopThread() : _numLocks_FromUsingByThisLoop(0) {}

				/**************************************************************************************************
				* Getters
				**************************************************************************************************/
				size_t                 GetNumLocks_FromUsingByThisLoop                  () const                                { return _numLocks_FromUsingByThisLoop; }

				/**************************************************************************************************
				* Updaters
				**************************************************************************************************/
				void                   AddNumLocks_FromUsingByThisLoop                  (size_t InNumLocks)                     { _numLocks_FromUsingByThisLoop += InNumLocks; }
				void                   SubtractNumLocks_FromUsingByThisLoop             (size_t InNumUnlocks)                   { assert(_numLocks_FromUsingByThisLoop >= InNumUnlocks); _numLocks_FromUsingByThisLoop -= _numLocks_FromUsingByThisLoop; }

				void EnqueueDefferedCommand(const LoaderCommandRecord& InCommand) { _deferredCommands.push(InCommand); }
				boost::optional<LoaderCommandRecord> PopDefferedCommand();

			private:
				size_t                                                  _numLocks_FromUsingByThisLoop;
				std::priority_queue<LoaderCommandRecord>                _deferredCommands;
			};

			inline boost::optional<LoaderCommandRecord> Loader_PerResource_ForLoadingLoopThread::PopDefferedCommand()
			{
				if (_deferredCommands.size() == 0)
				{
					return boost::optional<LoaderCommandRecord>();
				}
				else
				{
					auto top = _deferredCommands.top();
					_deferredCommands.pop();
					return top;
				}
			}

			/*****************************************************************************************
			* Data to be connected with each of the managed resource objects for each loading loop
			*****************************************************************************************/
			class Loader_PerResource_PerLoadingLoop
			{
			public:
				const Loader_PerResource_ForMainThread                 *GetMainThreadData                              () const                { return &_mainThreadData; }
				Loader_PerResource_ForMainThread                       *GetMainThreadData                              ()                      { return &_mainThreadData; }
				const Loader_PerResource_ForLoadingLoopThread          *GetPerResourceData_ForLoadingLoopThread        () const                { return &_dataForThreadThatLoaderRunsOn; }
				Loader_PerResource_ForLoadingLoopThread                *GetPerResourceData_ForLoadingLoopThread        ()                      { return &_dataForThreadThatLoaderRunsOn; }

				/**************************************************************************************
				* Helper functions
				**************************************************************************************/
				size_t                                                  GetNumUnprocessedCommands                      () const                { return _mainThreadData.GetNumUnprocessedCommands(); }

				void                                                    IncrUnprocessedCommands                        ()                      { _mainThreadData.IncrUnprocessedCommands(); }
				size_t                                                  DecrementUnprocessedCommands                   ()                      { return _mainThreadData.DecrementUnprocessedCommands(); }

			private:
				Loader_PerResource_ForMainThread                        _mainThreadData;
				Loader_PerResource_ForLoadingLoopThread                 _dataForThreadThatLoaderRunsOn;
			};
		}
	}
}