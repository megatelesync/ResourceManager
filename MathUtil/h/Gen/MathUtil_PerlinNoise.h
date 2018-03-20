#pragma once

#include "../MathTypes/BLAS/MathUtil_BLAS.h"

#include <vector>
#include <cassert>

namespace DvUtils
{
	namespace Math
	{
		class PermutationTable
		{
		public:
			using IndexType                                 = uint8_t;

			// By default generates permutation table
			PermutationTable();

			enum { ETableSize = 256 };

			uint8_t                                        GetAt(IndexType InIndex) const                              { return _table[InIndex % ETableSize]; }

			void                                           SetAt(IndexType InIndex, IndexType InValue)                 { assert(InIndex < ETableSize); _table[InIndex] = InValue; }

		private:
			uint8_t                                        _table[ETableSize];
		};

		struct PerlinNoiseParams
		{
			// Permutation table
			PermutationTable                                permutationTable;
			int                                             permutationTableOffsetForSeed = 0;
			int                                             octaves = 1;
			double                                          persistence = 0.5;
		};

		double octavePerlin3D(double x, double y, double z, const PerlinNoiseParams *pInParams);

		// returns noise value in range [0.0F;1.0F]
		double perlin3D(double x, double y, double z, const PerlinNoiseParams *pInParams);
	}
}