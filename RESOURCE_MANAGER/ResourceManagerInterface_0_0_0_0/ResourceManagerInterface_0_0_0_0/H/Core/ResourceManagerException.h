#pragma once

#include <string>
#include <thread>
#include <sstream>
#include "boost/optional.hpp"
#include "boost/format.hpp"

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			/*******************************************************************************************
			* Helper utlis
			*******************************************************************************************/
			std::string thread_id_to_string(boost::optional<std::thread::id> InID);
			std::string thread_id_to_string(std::thread::id InID);

			/*******************************************************************************************
			* Exception class
			*******************************************************************************************/
			class ResourceManagerException
			{
			public:
				/*******************************************************************************************
				* SPECIAL OPERATIONS
				*******************************************************************************************/
				~ResourceManagerException                              ()                                = default;

				ResourceManagerException                               (const ResourceManagerException&) = default;
				ResourceManagerException&                operator=     (const ResourceManagerException&) = default;
				ResourceManagerException                               (ResourceManagerException&&)      = default;
				ResourceManagerException&                operator=     (ResourceManagerException&&)      = default;

				/*******************************************************************************************
				* Construction
				*******************************************************************************************/
				ResourceManagerException                               (const std::string& InMessage);

				/*******************************************************************************************
				* GETTERS
				*******************************************************************************************/
				const std::string&                      GetMessage     () const;

			private:
				std::string _message;
			};

			/********************************************************************************
			* Exception connected with some threading error
			********************************************************************************/
			class ResourceManagerThreadingError : public ResourceManagerException
			{
			public:
				// For thread id you may pass non-initialized value 
				// (for the case when the thread id is not known)
				ResourceManagerThreadingError(const std::system_error *pInSystemError, boost::optional<std::thread::id> InThreadID, std::string& InMessage);

				const boost::optional<std::system_error>&     GetSystemError() const                { return _systemError; }

			private:
				boost::optional<std::thread::id>              _threadID;
				boost::optional<std::system_error>            _systemError;
			};

			/********************************************************************************
			* Exception to be thrown when it's not possible to create some threads 
			* during initialization
			********************************************************************************/
			class ResourceManagerCreateThreadException : public ResourceManagerThreadingError
			{
			public:
				ResourceManagerCreateThreadException(const std::system_error *pInSystemError, boost::optional<std::thread::id> InThreadID, std::string& InMessage) :
					ResourceManagerThreadingError(pInSystemError, InThreadID, InMessage) {}

			private:
			};
			
			/********************************************************************************
			* ResourceManagerThreadingError IMPLEMENTATION
			********************************************************************************/
			inline ResourceManagerThreadingError::ResourceManagerThreadingError(const std::system_error *pInSystemError, boost::optional<std::thread::id> InThreadID, std::string& InMessage) :
				ResourceManagerException(boost::str(boost::format("{Threading error for thread with ThreadID=%1%; std::system_error:what() returns:\n;message=\n:%2%}") % thread_id_to_string(InThreadID) % InMessage % (pInSystemError ? pInSystemError->what() : "NULL"))),
				_threadID(InThreadID),
				_systemError(*pInSystemError)
			{
			}

			/********************************************************************************
			* ResourceManagerException IMPLEMENTATION
			********************************************************************************/
			inline ResourceManagerException::ResourceManagerException(const std::string& InMessage) :
				_message(InMessage)
			{				
			}

			inline const std::string& ResourceManagerException::GetMessage() const
			{
				return _message;
			}

			/********************************************************************************
			* Helper utils IMPLEMENTATION
			********************************************************************************/
			inline std::string thread_id_to_string(boost::optional<std::thread::id> InID)
			{
				if (InID.is_initialized())
				{
					return thread_id_to_string(InID.get());
				}
				else
				{
					return "{UNITIALIZED}";
				}
			}

			inline std::string thread_id_to_string(std::thread::id InID)
			{
				std::stringstream ss;
				ss << InID;
				return ss.str();
			}
		} // Ver_0_0_0_0
	} // ResourceManager
} // Dv