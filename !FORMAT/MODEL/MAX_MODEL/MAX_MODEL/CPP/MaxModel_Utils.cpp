#include "../PrivateH/MatModel_Utils_Private.h"

namespace Dv
{
	namespace MaxModel
	{
		boost::optional<Error> TryLoadAndCheckHeader(std::ifstream *pStrm, FileHeader *pOutHeader, const FileHeader *pInReferenceHeader)
		{
			bool bHeaderLoaded = TryLoadHeader(pStrm, pOutHeader);
			if (false == bHeaderLoaded)
			{
				return Error_HeaderNotFound();
			}

			if (pOutHeader->signature != pInReferenceHeader->signature)
			{
				return Error_WrongSignature(pOutHeader->signature);
			}

			if (false == AreVersionsEqual(&(pOutHeader->version), &(pInReferenceHeader->version)))
			{
				return Error_WrongVersion(&(pOutHeader->version));
			}
			return boost::optional<Error>();
		}

		bool TryLoadVersion(std::ifstream& strm, Version *pOutVersion)
		{
			return Dv::Util::TryLoadByteArray(&strm, &((*pOutVersion)[0]), 4);
		}

		bool TryLoadHeader(std::ifstream *pStrm, FileHeader *pOutHeader)
		{
			assert(pStrm);
			assert(pOutHeader);

			bool bSignatureLoaded = Dv::Util::TryLoadString_ZeroTerminated(pStrm, &(pOutHeader->signature));
			if (false == bSignatureLoaded)
			{
				return false;
			}

			bool bVersionLoaded = TryLoadVersion(*pStrm, &(pOutHeader->version));
			if (false == bVersionLoaded)
			{
				return false;
			}

			return true;
		}

		bool TryLoadPoint3(std::ifstream& strm, Point3 *pOutPoint3)
		{
			assert(pOutPoint3);
			float point3_components[3];
			void *pOutBuf = point3_components;
			strm.read(static_cast<char *>(pOutBuf), MAXMODEL_POINT3_SIZE);
			if (strm.fail())
			{
				return false;
			}
			float X = point3_components[0];
			float Y = point3_components[1];
			float Z = point3_components[2];
			DvUtils::Math::Reset3(pOutPoint3, X, Y, Z);
			return true;
		}

		bool TryLoadPoint4(std::ifstream& strm, Point4 *pOutPoint4)
		{
			assert(pOutPoint4);
			float point4_components[4];
			void *pOutBuf = point4_components;
			strm.read(static_cast<char *>(pOutBuf), MAXMODEL_POINT4_SIZE);
			if (strm.fail())
			{
				return false;
			}
			float X = point4_components[0];
			float Y = point4_components[1];
			float Z = point4_components[2];
			float W = point4_components[3];
			DvUtils::Math::Reset4(pOutPoint4, X, Y, Z, W);
			return true;
		}

		bool TryLoadVertexIndices3(std::ifstream *pStrm, VertIndices3 *pOutIndices3)
		{
			assert(pStrm);
			assert(pOutIndices3);
			auto pIndex_0 = &((*pOutIndices3)[0]);
			auto pIndex_1 = &((*pOutIndices3)[1]);
			auto pIndex_2 = &((*pOutIndices3)[2]);
			bool bFirstLoaded = Dv::Util::TryLoadUInt32(pStrm, pIndex_0);
			if (false == bFirstLoaded)
			{
				return false;
			}

			bool bSecondLoaded = Dv::Util::TryLoadUInt32(pStrm, pIndex_1);
			if (false == bSecondLoaded)
			{
				return false;
			}

			bool bThirdLoaded = Dv::Util::TryLoadUInt32(pStrm, pIndex_2);
			if (false == bThirdLoaded)
			{
				return false;
			}

			return true;
		}

		bool AreVersionsEqual(const Version *pFirst, const Version *pSecond)
		{
			bool bA = (*pFirst)[0] == (*pSecond)[0];
			bool bB = (*pFirst)[1] == (*pSecond)[1];
			bool bC = (*pFirst)[2] == (*pSecond)[2];
			bool bD = (*pFirst)[3] == (*pSecond)[3];
			bool bEqual = bA && bB && bC && bD;
			return bEqual;
		}
	}
}