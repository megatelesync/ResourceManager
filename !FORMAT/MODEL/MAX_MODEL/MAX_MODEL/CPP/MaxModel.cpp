#include "../H/MaxModel.h"
#include "File/Util_File.h"
#include "CoreUtil/CoreUtil_Assert.h"

#include "../PrivateH/MatModel_Utils_Private.h"

namespace Dv
{
	namespace MaxModel
	{
		namespace Ver_0_0_0_0
		{
			/***************************************************************************************************************************
			* Fwd declarations
			***************************************************************************************************************************/
			boost::optional<Error>        TryLoadVertices(std::ifstream *pStrm, File *pOutFile);
			boost::optional<Error>        TryLoadFaces(std::ifstream *pStrm, File *pOutFile);
			boost::optional<Error>        TryLoadTextureVertices(std::ifstream *pStrm, File *pOutFile);
			boost::optional<Error>        TryLoadTextureFaces(std::ifstream *pStrm, File *pOutFile);

			bool                          TryLoadMeshVertex(std::ifstream& strm, Vertex *pOutVertex);
			bool                          TryLoadFace(std::ifstream& strm, Face *pOutFace);

			bool                          TryLoadTextureVertex(std::ifstream& strm, TextureVertex *pOutVertex);
			bool                          TryLoadTextureFace(std::ifstream& strm, TextureFace *pOutFace);

			/***************************************************************************************************************************
			* implementation
			***************************************************************************************************************************/
			boost::optional<Error> TryLoadFromFile(std::ifstream *pStrm, File *pOutFile)
			{
				assert(pOutFile);

				FileHeader fileHeader;
				auto resultError = TryLoadAndCheckHeader(pStrm, &fileHeader, &REFERENCE_HEADER);
				if (resultError.is_initialized())
				{
					return resultError;
				}
				pOutFile->SetHeader(&fileHeader);

				auto verticesLoadedResult = TryLoadVertices(pStrm, pOutFile);
				if (verticesLoadedResult.is_initialized())
				{
					return verticesLoadedResult;
				}

				auto facesLoadedResult = TryLoadFaces(pStrm, pOutFile);
				if (facesLoadedResult.is_initialized())
				{
					return facesLoadedResult;
				}

				auto textureVerticesLoadedResult = TryLoadTextureVertices(pStrm, pOutFile);
				if (textureVerticesLoadedResult.is_initialized())
				{
					return textureVerticesLoadedResult;
				}

				auto textureFacesLoadedResult = TryLoadTextureFaces(pStrm, pOutFile);
				if (textureFacesLoadedResult.is_initialized())
				{
					return textureFacesLoadedResult;
				}

				// Successfully loaded here
				return boost::optional<Error>();
			}

			boost::optional<Error> TryLoadVertices(std::ifstream *pStrm, File *pOutFile)
			{
				assert(pStrm);
				assert(pOutFile);

				uint32_t numVertices = 0;
				bool bCountLoaded = Dv::Util::TryLoadUInt32(pStrm, &numVertices);
				if (false == bCountLoaded)
				{
					const uint32_t countShouldBe = 0;
					const uint32_t countReallyLoaded = 0;
					return Error_EofWhileReadingVertices(countShouldBe, countReallyLoaded);
				}
				for (int i = 0; i < numVertices; i++)
				{
					Vertex vert;
					bool bVertexLoaded = TryLoadMeshVertex(*pStrm, &vert);
					if (false == bVertexLoaded)
					{
						// WARNING!!! Here "i" is written correctly (NOT "i+1")
						const uint32_t countReallyLoaded = i;
						return Error_EofWhileReadingVertices(numVertices, countReallyLoaded);
					}
					pOutFile->AppendVertex(&vert);
				}
				return boost::optional<Error>();
			}

			boost::optional<Error> TryLoadFaces(std::ifstream *pStrm, File *pOutFile)
			{
				assert(pStrm);
				assert(pOutFile);

				uint32_t numFaces = 0;
				bool bCountLoaded = Dv::Util::TryLoadUInt32(pStrm, &numFaces);
				if (false == bCountLoaded)
				{
					const uint32_t countShouldBe = 0;
					const uint32_t countReallyLoaded = 0;
					return Error_EofWhileReadingFaces(countShouldBe, countReallyLoaded);
				}
				for (int i = 0; i < numFaces; i++)
				{
					Face face;
					bool bFaceLoaded = TryLoadFace(*pStrm, &face);
					if (false == bFaceLoaded)
					{
						// WARNING!!! Here "i" is written correctly (NOT "i+1")
						const uint32_t countReallyLoaded = i;
						return Error_EofWhileReadingFaces(numFaces, countReallyLoaded);
					}
					pOutFile->AppendFace(&face);
				}
				return boost::optional<Error>();
			}

			boost::optional<Error> TryLoadTextureVertices(std::ifstream *pStrm, File *pOutFile)
			{
				assert(pStrm);
				assert(pOutFile);

				uint32_t numVertices = 0;
				bool bCountLoaded = Dv::Util::TryLoadUInt32(pStrm, &numVertices);
				if (false == bCountLoaded)
				{
					const uint32_t countShouldBe = 0;
					const uint32_t countReallyLoaded = 0;
					return Error_EofWhileReadingTextureVertices(countShouldBe, countReallyLoaded);
				}
				for (int i = 0; i < numVertices; i++)
				{
					TextureVertex vert;
					bool bVertexLoaded = TryLoadTextureVertex(*pStrm, &vert);
					if (false == bVertexLoaded)
					{
						// WARNING!!! Here "i" is written correctly (NOT "i+1")
						const uint32_t countReallyLoaded = i;
						return Error_EofWhileReadingTextureVertices(numVertices, countReallyLoaded);
					}
					pOutFile->AppendTextureVertex(&vert);
				}
				return boost::optional<Error>();
			}

			boost::optional<Error> TryLoadTextureFaces(std::ifstream *pStrm, File *pOutFile)
			{
				assert(pStrm);
				assert(pOutFile);

				uint32_t numFaces = 0;
				bool bCountLoaded = Dv::Util::TryLoadUInt32(pStrm, &numFaces);
				if (false == bCountLoaded)
				{
					const uint32_t countShouldBe = 0;
					const uint32_t countReallyLoaded = 0;
					return Error_EofWhileReadingTextureFaces(countShouldBe, countReallyLoaded);
				}
				for (int i = 0; i < numFaces; i++)
				{
					TextureFace face;
					bool bFaceLoaded = TryLoadTextureFace(*pStrm, &face);
					if (false == bFaceLoaded)
					{
						// WARNING!!! Here "i" is written correctly (NOT "i+1")
						const uint32_t countReallyLoaded = i;
						return Error_EofWhileReadingTextureFaces(numFaces, countReallyLoaded);
					}
					pOutFile->AppendTextureFace(&face);
				}
				return boost::optional<Error>();
			}

			bool TryLoadTextureVertex(std::ifstream& strm, TextureVertex *pOutVertex)
			{
				Point3 uvw;
				bool bUVWloaded = TryLoadPoint3(strm, &uvw);
				pOutVertex->UVW = uvw; // It can be helpful when debugging to copy ever partly-loaded value
				if (false == bUVWloaded)
				{
					return false;
				}
				return true;
			}

			bool TryLoadTextureFace(std::ifstream& strm, TextureFace *pOutFace)
			{
				VertIndices3 vertIndices;
				bool bIndicesLoaded = TryLoadVertexIndices3(&strm, &vertIndices);
				pOutFace->textureVertexIndices[0] = vertIndices[0];
				pOutFace->textureVertexIndices[1] = vertIndices[1];
				pOutFace->textureVertexIndices[2] = vertIndices[2];
				if (false == bIndicesLoaded)
				{
					return false;
				}
				return true;
			}

			bool TryLoadMeshVertex(std::ifstream& strm, Vertex *pOutVertex)
			{
				assert(pOutVertex);
				Point3 position, normal;
				bool bLoadPositionSucceeded = TryLoadPoint3(strm, &position);
				if (false == bLoadPositionSucceeded)
				{
					return false;
				}
				bool bLoadNormalSucceeded = TryLoadPoint3(strm, &normal);
				if (false == bLoadNormalSucceeded)
				{
					return false;
				}
				pOutVertex->position = position;
				pOutVertex->normal = normal;
				return true;
			}

			bool TryLoadFace(std::ifstream& strm, Face *pOutFace)
			{
				assert(pOutFace);
				bool bMaterialID_loaded = Util::TryLoadUInt32(&strm, &(pOutFace->materialId));
				if (false == bMaterialID_loaded)
				{
					return false;
				}
				bool bNormalLoaded = TryLoadPoint3(strm, &(pOutFace->normal));
				if (false == bNormalLoaded)
				{
					return false;
				}
				bool bVertIndices_loaded = TryLoadVertexIndices3(&strm, &(pOutFace->vertexIndices));
				if (false == bVertIndices_loaded)
				{
					return false;
				}
				return true;
			}
		}
	}
} // Dv