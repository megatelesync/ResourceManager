#ifndef _MathUtil_Geometry_Tri_h_
#define _MathUtil_Geometry_Tri_h_

#include <array>

namespace DvUtils
{
	namespace Math	
	{		
		// Precomputed triangle data
		// Designed for the barycentric cordinate computation algorithms
		template<class FloatTypeArg>
		struct TrianglePrecomputedData2D
		{
			typedef				FloatTypeArg									FloatType;
			typedef				std::array<FloatTypeArg, 2>						VecType2D;
			typedef				TrianglePrecomputedData2D<FloatTypeArg>			ThisType;

			// Precomputed vector from point A to point B
			typename			VecType2D			vector_A_B;
			// Precomputed Vector from point A to point C
			typename			VecType2D			vector_A_C;

			typename			FloatTypeArg		AB_dot_AB;
			typename			FloatTypeArg		AC_dot_AC;
			typename			FloatTypeArg		AB_dot_AC;			
		};
	} // Math
} // DvUtils

#endif // _MathUtil_Geometry_Tri_h_