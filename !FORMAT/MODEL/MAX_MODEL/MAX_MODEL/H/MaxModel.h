#pragma once

#include "MaxModel_Utils.h"

namespace Dv
{
	namespace MaxModel
	{
		namespace Ver_0_0_0_0
		{
			const Version CURRENT_VERSION              = {0,0,0,0};
			const std::string CURRENT_SIGNATURE        = "dml";
			const FileHeader REFERENCE_HEADER = FileHeader { CURRENT_SIGNATURE, CURRENT_VERSION };

			struct Vertex
			{
				Point3                                  position;
				Point3                                  normal;
			};

			struct Face
			{
				MaterialID                             materialId;
				Point3                                 normal;
				VertIndices3                           vertexIndices;
			};

			struct TextureVertex
			{
				Point3                                  UVW;
			};

			struct TextureFace
			{
				VertIndices3                            textureVertexIndices;
			};

			/*****************************************************************************************************************
			* Entire loaded file
			*****************************************************************************************************************/
			class File
			{
			public:
				const FileHeader                       *GetHeader                        () const                            { return &_header; }

				int                                     GetNumVertices                   () const                            { return _vertices.size(); }
				const Vertex                           *GetVertex                        (int InIndex) const                 { assert(InIndex >= 0);  assert(InIndex < GetNumVertices()); return &(_vertices[InIndex]); }

				int                                     GetNumFaces                      () const                            { return _faces.size(); }
				const Face                             *GetFace                          (int InIndex) const                 { assert(InIndex >= 0);  assert(InIndex < GetNumFaces()); return &(_faces[InIndex]); }

				int                                     GetNumTexturesVertices           () const                            { return _textureVertices.size(); }
				const TextureVertex                    *GetTextureVertex                 (int InIndex) const                 { return &(_textureVertices[InIndex]); }

				int                                     GetNumTextureFaces               () const                            { return _textureFaces.size(); }
				const TextureFace                      *GetTextureFace                   (int InIndex) const                 { return &(_textureFaces[InIndex]); }

				void                                    SetHeader                        (const FileHeader *pInHeader)                                    { assert(pInHeader);  _header = *pInHeader; }
				void                                    AppendVertex                     (const Vertex *pInVertex)                                        { _vertices.push_back(*pInVertex); }
				void                                    AppendFace                       (const Face *pInFace)                                            { _faces.push_back(*pInFace); }
				void                                    AppendTextureVertex              (const TextureVertex *pInTextureVertex)                          { _textureVertices.push_back(*pInTextureVertex); }
				void                                    AppendTextureFace                (const TextureFace *pInTextureFace)                              { _textureFaces.push_back(*pInTextureFace); }

			private:
				FileHeader                              _header;
				std::vector<Vertex>                     _vertices;
				std::vector<Face>                       _faces;
				std::vector<TextureVertex>              _textureVertices;
				std::vector<TextureFace>                _textureFaces;
			};

			// Loading from file: returns non-initialized value if successfully loaded
			boost::optional<Error> TryLoadFromFile(std::ifstream *pStrm, File *pOutFile);
		}
	}
} // Dv