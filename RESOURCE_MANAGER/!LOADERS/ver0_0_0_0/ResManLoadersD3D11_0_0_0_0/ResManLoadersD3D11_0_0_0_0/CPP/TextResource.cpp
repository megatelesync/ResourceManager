#pragma once

#include "../H/TextResource.h"
#include "StringUtil/Util_StringUtil_Master.h"

#include <fstream>

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/*********************************************************************************************************
			* TextLoaderFromFile implementation
			*********************************************************************************************************/
			TextLoaderFromFile::TextLoaderFromFile
			(
				const std::string                      &InFilename,
				ID3D11Device                           *pInDev,
				ResMan::ISubresourceRole               *pInRole,
				TextResourceType                       *pInTypedResourceType
			) : TypedD3D11ResourceLoaderBase(InFilename, pInDev, pInRole, pInTypedResourceType),
				_pResultText(nullptr)
			{
			}

			TextLoaderFromFile::~TextLoaderFromFile()
			{
				delete _pResultText;
			}

			TextLoaderFromFile::RealResourceType *TextLoaderFromFile::Typed_CreateResourceAndGet(RealResourceType *pOldPtrOrNull)
			{
				if (_pResultText != nullptr)
				{
					// Here we actually move the resource created on the loading stage
					return _MoveLoadedResource();
				}
				else
				{
					if (pOldPtrOrNull != nullptr)
					{
						return new Text(*pOldPtrOrNull);
					}
					else
					{
						return nullptr;
					}
				}
			}

			void LoadTextFromStream(Text *pResultText, std::ifstream *pStrm)
			{
				std::string curr_str;
				while (std::getline(*pStrm, curr_str))
				{
					pResultText->AppendLine(curr_str);
				}
			}

			bool TextLoaderFromFile::LoadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				auto pMainRole = GetRoles().pStd;
				if (_ShouldLoadTempContent(InRoleSet, pMainRole))
				{
					_pResultText = nullptr;
					std::ifstream strm(_GetURL(), std::ios::in);
					if (strm.fail())
					{
						return false;
					}
					// WARNING!!!
					// Result text is created in the heap,
					// but normally it must NOT be deleted as it will be owned by the resource manager.
					// However, when nullptr is to be returned, the result text is to be deleted.
					_pResultText = new Text();
					LoadTextFromStream(_pResultText, &strm);
					// if eof gained, we succeeded with loading file; otherwise we failed.
					if (!strm.eof())
					{						
						_pResultText = nullptr;
						delete _pResultText;
						return false;
					}
					else
					{
						_MarkTempContentRole_Loaded(pMainRole);
					}
				}
				return true;
			}

			void TextLoaderFromFile::UnloadTemporaryOwnContent(ResMan::SubresourceRoleSet InRoleSet)
			{
				auto pMainRole = GetRoles().pStd;
				if (_ShouldTempContentBeUnloaded(InRoleSet, pMainRole))
				{
					 delete _pResultText;
					_pResultText = nullptr;
					_MarkTempContentRole_Unloaded(pMainRole);
				}
			}

			Text *TextLoaderFromFile::_MoveLoadedResource()
			{
				_MarkTempContentRole_Unloaded(GetRoles().pStd);
				auto _pSavedResultText = _pResultText;
				_pResultText = nullptr;
				return _pSavedResultText;
			}

			/*********************************************************************************************************
			* ResTypeDescr_Text implementation
			*********************************************************************************************************/
			ResTypeDescr_Text::ResTypeDescr_Text() :
				TypedD3D11ResTypeDescrBase("Text", "Text")
			{
			}

			void ResTypeDescr_Text::Delete(void *pInResource)
			{
				delete pInResource;
			}
		}
	}
} // Dv