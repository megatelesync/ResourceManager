#pragma once

#include "ResManLoadersD3D11_System.h"

namespace Dv
{
	namespace ResManLoadersD3D11
	{
		namespace Ver_0_0_0_0
		{
			/******************************************************************************************
			* Read-only text resource
			******************************************************************************************/
			class Text
			{
			public:
				Text                                                           () : _bAppendable(true)                  {}

				size_t                           GetNumLines                   () const                                 { return _text.size(); }
				const std::string&               GetLine                       (int InLineIndex) const                  { return _text[InLineIndex]; }

				// Can we add line at the end of the text?
				bool                             IsAppendable                  () const                                 { return _bAppendable; }
				// Append line at the end of the text
				void                             AppendLine(const std::string& InNewLine)                              { assert(_bAppendable);  _text.emplace_back(InNewLine); }
				// Finish initialization: to be called when the resource initialization is finished
				// (makes text non-appendable)
				void                             FinishInit                    ()                                      { _bAppendable = false; };

				// C++ stl interface
				using cont                     = std::vector<std::string>;
				using const_iterator           = cont::const_iterator;
				using iterator                 = const_iterator;

				const_iterator                   cbegin                        () const                                 { return _text.cbegin(); }
				const_iterator                   cend                          () const                                 { return _text.cend(); }
				const_iterator                   begin                         () const                                 { return _text.cbegin(); }
				const_iterator                   end                           () const                                 { return _text.cend(); }

			private:
				cont                            _text;
				bool                            _bAppendable;
			};

			/******************************************************************************************
			* Typed resource
			******************************************************************************************/
			using TextResource = ResMan::TypedManagedResource<Text>;

			/******************************************************************************************
			* set of text resource subresource role types
			******************************************************************************************/
			struct ResTypeDescr_Text_Roles : public ResTypeDescr_Roles_Base
			{
			};

			/******************************************************************************************
			* Text resource description
			******************************************************************************************/
			class ResTypeDescr_Text :
				public TypedD3D11ResTypeDescrBase<ResTypeDescr_Text_Roles, Text>
			{
			public:
				ResTypeDescr_Text();

				virtual void                         Delete(void *pInResource) override;

			private:				
			};

			/*****************************************************************************************
			* Typed resource
			******************************************************************************************/
			class TextResourceType : public ResMan::TypedResourceType<ResTypeDescr_Text>
			{
			public:
			};

			/******************************************************************************************
			* Loads text from file
			*******************************************************************************************/
			class TextLoaderFromFile : public TypedD3D11ResourceLoaderBase<TextResourceType>
			{
			public:
				// Custom destructor is needed, because result text needs to be removed, if not moved
				virtual ~TextLoaderFromFile();

				/**********************************************************************************************
				* Filename is full filename (including path and extension)
				**********************************************************************************************/
				TextLoaderFromFile
				(
					const std::string                      &InFilename,
					ID3D11Device                           *pInDev,
					ResMan::ISubresourceRole               *pInRole,
					TextResourceType                       *pInTypedResourceType
				);

				/**********************************************************************************************
				* When created, the loaded content will be moved,
				* and will be further marked as unloaded.
				**********************************************************************************************/
				virtual RealResourceType                   *Typed_CreateResourceAndGet                (RealResourceType *pOldPtrOrNull) override;

				virtual bool                                LoadTemporaryOwnContent                   (ResMan::SubresourceRoleSet InRoleSet) override;
				virtual void                                UnloadTemporaryOwnContent                 (ResMan::SubresourceRoleSet InRoleSet) override;

			private:
				Text                                       *_pResultText;

				// Move loaded resource and return the moved loaded resource
				// (moved resource will be treated as unloaded in this resource)
				Text                                       *_MoveLoadedResource();
			};
		}
	}
}