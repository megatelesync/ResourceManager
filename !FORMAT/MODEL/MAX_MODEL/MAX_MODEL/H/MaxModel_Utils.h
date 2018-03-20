#pragma once

#include <boost/optional.hpp>
#include <cassert>
#include <string>
#include <fstream>
#include <vector>
#include <array>
#include <map>
#include <memory>
#include <MathTypes\BLAS\MathUtil_BLAS.h>

namespace Dv
{
	namespace MaxModel
	{
		// Corresponds to type of Point3 in MaxScript
		using Point3                               = DvUtils::Math::Vec<float, 3>;
		using Point4                               = DvUtils::Math::Vec<float, 4>;
		using VertIndex                            = uint32_t;
		using VertIndices3                         = VertIndex[3];
		// Version: Lower byte is lower version;
		using Version                              = std::array<uint8_t, 4>;
		using MaterialID                           = uint32_t;

		struct FileHeader
		{
			std::string                             signature;
			Version                                 version;

			FileHeader() = default;
			FileHeader(const std::string& InSignature, const Version& InVersion) :
				signature(InSignature),
				version(InVersion)
			{
			}
		};

		/*********************************************************************************************
		* Errors
		*********************************************************************************************/
		// Error of loader
		class Error
		{
		public:
			Error(const std::string& InDescription) : _error(InDescription) {}

		private:
			std::string _error;
		};

		class Error_HeaderNotFound : public Error
		{
		public:
			Error_HeaderNotFound() :
				Error("Header not found")
			{
			}

		private:
		};

		class Error_WrongSignature : public Error
		{
		public:
			Error_WrongSignature(const std::string& InRealSignature) :
				Error("Wrong signature"),
				_realSignature(InRealSignature)
			{
			}

		private:
			std::string _realSignature;
		};

		class Error_WrongVersion : public Error
		{
		public:
			Error_WrongVersion(const Version *pInRealVersion) :
				Error("Wrong version"),
				_realVersion(*pInRealVersion)
			{
			}

		private:
			Version _realVersion;
		};

		class Error_EofWhileReadingVertices : public Error
		{
		public:
			Error_EofWhileReadingVertices(uint32_t InNumVerticesShouldBe, uint32_t InRealNumVertices) :
				Error("Eof while reading vertices"),
				_numVerticesShouldBe(InNumVerticesShouldBe),
				_realNumVertices(InRealNumVertices)
			{
			}

			uint32_t GetNumVerticesShouldBe() const { return _numVerticesShouldBe; }
			uint32_t GetRealNumVertices() const { return _realNumVertices; }

		private:
			uint32_t               _numVerticesShouldBe;
			uint32_t               _realNumVertices;
		};

		class Error_EofWhileReadingFaces : public Error
		{
		public:
			Error_EofWhileReadingFaces(uint32_t InNumFacesShouldBe, uint32_t InRealNumFaces) :
				Error("Eof while reading faces"),
				_numFacesShouldBe(InNumFacesShouldBe),
				_realNumFaces(InRealNumFaces)
			{
			}

			uint32_t GetNumFacesShouldBe() const { return _numFacesShouldBe; }
			uint32_t GetRealNumFaces() const { return _realNumFaces; }

		private:
			uint32_t               _numFacesShouldBe;
			uint32_t               _realNumFaces;
		};

		class Error_EofWhileReadingTextureVertices : public Error
		{
		public:
			Error_EofWhileReadingTextureVertices(uint32_t InNumVerticesShouldBe, uint32_t InRealNumVertices) :
				Error("Eof while reading texture vertices"),
				_numVerticesShouldBe(InNumVerticesShouldBe),
				_realNumVertices(InRealNumVertices)
			{
			}

			uint32_t GetNumVerticesShouldBe() const { return _numVerticesShouldBe; }
			uint32_t GetRealNumVertices() const { return _realNumVertices; }

		private:
			uint32_t               _numVerticesShouldBe;
			uint32_t               _realNumVertices;
		};

		class Error_EofWhileReadingTextureFaces : public Error
		{
		public:
			Error_EofWhileReadingTextureFaces(uint32_t InNumFacesShouldBe, uint32_t InRealNumFaces) :
				Error("Eof while reading faces"),
				_numFacesShouldBe(InNumFacesShouldBe),
				_realNumFaces(InRealNumFaces)
			{
			}

			uint32_t GetNumFacesShouldBe() const { return _numFacesShouldBe; }
			uint32_t GetRealNumFaces() const { return _realNumFaces; }

		private:
			uint32_t               _numFacesShouldBe;
			uint32_t               _realNumFaces;
		};

	}
}