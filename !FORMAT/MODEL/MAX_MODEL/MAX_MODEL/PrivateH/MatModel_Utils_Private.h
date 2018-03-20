#pragma once

#include "File/Util_File.h"
#include "CoreUtil/CoreUtil_Assert.h"

#include "../H/MaxModel_Utils.h"

namespace Dv
{
	namespace MaxModel
	{
		/***********************************************************************************************
		* Loading utils
		***********************************************************************************************/
		boost::optional<Error>        TryLoadAndCheckHeader       (std::ifstream *pStrm, FileHeader *pOutHeader, const FileHeader *pInReferenceHeader);

		// Load header without checking version and/or signature
		bool                          TryLoadHeader               (std::ifstream *pStrm, FileHeader *pOutHeader);
		bool                          TryLoadVersion              (std::ifstream& strm, Version *pOutVersion);
		// Returns false is loading point3 failed 
		bool                          TryLoadPoint3               (std::ifstream& strm, Point3 *pOutPoint3);
		bool                          TryLoadPoint4               (std::ifstream& strm, Point4 *pOutPoint3);
		bool                          TryLoadVertexIndices3       (std::ifstream *pStrm, VertIndices3 *pOutIndices3);

		/***********************************************************************************************
		* Working with versions
		***********************************************************************************************/
		bool                          AreVersionsEqual            (const Version *pFirst, const Version *pSecond);

		/***************************************************************************************************************************
		* constants
		***************************************************************************************************************************/
		const size_t MAXMODEL_POINT3_SIZE = 4 * 3;
		const size_t MAXMODEL_POINT4_SIZE = 4 * 4;
	}
}