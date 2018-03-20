#include "../../H/Interface/IResourceManager.h"
#include "../../H/Subsystem/IResourceManagerSubsystem.h"

#include <sys/stat.h>

namespace Dv
{
	namespace ResourceManager
	{
		namespace Ver_0_0_0_0
		{
			/*****************************************************************************************************************************
			* Working with resource paths fwd
			*****************************************************************************************************************************/
			bool CheckFileExists                                                     (const std::string& InFullFilename);
			bool IsAbsolutePath                                                      (const std::string& InPath);
			// Get full filename of file that is stored in registered base data directory with the given index
			std::string GetFullFilename_InBaseDataDir                                (IResourceManagerSubsystem *pInSubsystem, const std::string& InFilename, size_t InBaseDirIndex);
			// Get full filename of file that is stored in subdir for the given resource type
			boost::optional<std::string> GetFullFilename_InSubdirForResourceType     (IResourceManagerSubsystem *pInSubsystem, const std::string& InFilename, IResourceType *pInResourceType, size_t InBaseDirIndex);

			// Get default URL or return nullptr
			boost::optional<std::string> GetDefaultURL_ForType_OrNull                (IResourceManagerSubsystem *pInSubsystem, const std::string& InFilename, IResourceType *pInResourceType);
			std::string PostfixFilenameWithDirSeparator_IfNecessary                  (const std::string& InFilename);

			/*****************************************************************************************************************************
			* Working with resource paths implementation
			*****************************************************************************************************************************/
			boost::optional<std::string> GetExistingFullFilename                     (IResourceManagerSubsystem *pInSubsystem, const std::string& InFilename, IResourceType *pInResourceType)
			{
				if (IsAbsolutePath(InFilename) && CheckFileExists(InFilename))
				{
					return InFilename;
				}
				else
				{
					auto pFsParams = pInSubsystem->GetCommonParams()->GetFilesystem();

					size_t numRegisteredBasePaths = pFsParams->GetNumBaseDataDirs();
					for (int baseDirIndex = 0; baseDirIndex < numRegisteredBasePaths; baseDirIndex++)
					{
						boost::optional<std::string> fullFilename_InSubdirForResourceType = GetFullFilename_InSubdirForResourceType(pInSubsystem, InFilename, pInResourceType, baseDirIndex);
						if (fullFilename_InSubdirForResourceType.is_initialized() && CheckFileExists(fullFilename_InSubdirForResourceType.get()))
						{
							return fullFilename_InSubdirForResourceType;
						}
						else
						{
							std::string fullFilename_InBaseDataDir = GetFullFilename_InBaseDataDir(pInSubsystem, InFilename, baseDirIndex);
							if (CheckFileExists(fullFilename_InBaseDataDir))
							{
								return fullFilename_InBaseDataDir;
							}
						}
					}
				}
				// Here we have not found file in any of directories, so try to check default URL or return nullptr.
				return GetDefaultURL_ForType_OrNull(pInSubsystem, InFilename, pInResourceType);
			}

			std::string GetFullFilename_InBaseDataDir(IResourceManagerSubsystem *pInSubsystem, const std::string& InFilename, size_t InBaseDirIndex)
			{
				assert(pInSubsystem != nullptr);

				auto pFsParams                        = pInSubsystem->GetCommonParams()->GetFilesystem();
				auto baseDataDir                      = pFsParams->GetBaseDataDir(InBaseDirIndex);
				std::string result_fullFilename       = PostfixFilenameWithDirSeparator_IfNecessary(baseDataDir);

				result_fullFilename.append(InFilename);

				return result_fullFilename;
			}

			boost::optional<std::string> GetFullFilename_InSubdirForResourceType(IResourceManagerSubsystem *pInSubsystem, const std::string& InFilename, IResourceType *pInResourceType, size_t InBaseDirIndex)
			{
				auto subdirName = pInResourceType->GetParams().GetSubdirectoryName();
				if (subdirName.size() == 0)
				{
					return boost::optional<std::string>();
				}
				else
				{
					auto pFsParams                 = pInSubsystem->GetCommonParams()->GetFilesystem();
					auto baseDataDir               = pFsParams->GetBaseDataDir(InBaseDirIndex);
					std::string resultDir          = PostfixFilenameWithDirSeparator_IfNecessary(baseDataDir);

					resultDir.append(PostfixFilenameWithDirSeparator_IfNecessary(subdirName));
					resultDir.append(InFilename);

					return resultDir;
				}
			}

			boost::optional<std::string> GetDefaultURL_ForType_OrNull(IResourceManagerSubsystem *pInSubsystem, const std::string& InFilename, IResourceType *pInResourceType)
			{
				auto defaultURL = pInResourceType->GetParams().GetDefaultURL();
				if (defaultURL.size() == 0)
				{
					return boost::optional<std::string>();
				}

				auto pFsParams = pInSubsystem->GetCommonParams()->GetFilesystem();
				auto defaultUrl_BaseDataDir = pFsParams->GetDefaultURL_BaseDataDir();
				if (defaultUrl_BaseDataDir.size() == 0)
				{
					return boost::optional<std::string>();
				}
				
				auto resultFilename = PostfixFilenameWithDirSeparator_IfNecessary(defaultUrl_BaseDataDir);
				resultFilename.append(defaultURL);

				if (false == CheckFileExists(resultFilename))
				{
					return boost::optional<std::string>();
				}

				return resultFilename;
			}

			bool CheckFileExists(const std::string& InFullFilename)
			{
				struct stat fileinfo;
				int stat_result = stat(InFullFilename.c_str(), &fileinfo);
				bool bExists = (stat_result == 0);
				return bExists;
			}

			bool IsAbsolutePath(const std::string& InPath)
			{
				const size_t           ColonIndex                    = 1;
				const char             DriveSeparator                = '/';
				const char             AltDriveSeparator             = '\\';
				const char             ColonChar                     = ':';

				char charWhereColonShouldBe = InPath[ColonIndex];
				if (charWhereColonShouldBe != ColonChar)
				{
					return false;
				}
				return true;
			}

			std::string PostfixFilenameWithDirSeparator_IfNecessary(const std::string& InFilename)
			{
				std::string result;
				result.append(InFilename);
				if (result.back() != '\\' || result.back() != '/')
				{
					result.append("/");
				}
				return result;
			}

			/*****************************************************************************************************************************
			* Working with subresource roles
			*****************************************************************************************************************************/
			SubresourceRoleSet MakeSubresourceRoleSet_AllRoles(IResourceManager *pInManager)
			{
				assert(pInManager != nullptr);
				SubresourceRoleSet resultSet;
				auto pRoleEnumerator = std::move(pInManager->GetSubresourceRoles());
				while (true)
				{
					resultSet.Include(pRoleEnumerator->GetCurrent());
					if (false == pRoleEnumerator->MoveNext())
					{
						break;
					}
				}
				return resultSet;
			}

			std::string ToString_SubresourceRoleSet(SubresourceRoleSet InSet, IResourceManager *pInManager)
			{
				std::string result = "";

				result.append("{");
				auto enumerator = InSet.GetEnumerator();
				bool bFirst = true;	
				while (true)
				{
					if (enumerator.IsEnd())
					{
						break;
					}
					auto roleIndex = enumerator.GetRoleIndex();
					auto pRole = pInManager->GetSubresourceRoleByIndex(roleIndex);
					if(false == bFirst)
					{
						result += " ";
					}
					result += ToString_SubresourceRole(pRole);
					enumerator.MoveNext();
					bFirst = true;
				}
				result.append("}");
				return result;
			}
		}
	}
}