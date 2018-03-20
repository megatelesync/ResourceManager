#ifndef _MathUtil_VectorBase_h_
#define _MathUtil_VectorBase_h_

#include "MathUtil_VectorBaseTypes.h"

namespace DvUtils
{
	namespace Math
	{
		template<class VecType>
		VecType Lerp(const VecType& min, const VecType& maxim, typename VecType::FloatType factor);

		template<class VecType>
		typename VecType::FloatType UnsignedAngle (const VecType& v1, const VecType& v2);

		template<class VecType>
		typename VecType::FloatType Angle (const VecType& v1, const VecType& v2);

		template<class VecType>
		typename VecType::FloatType Distance(const VecType& P1, const VecType& P2);

		template<class VecType>
		typename VecType::FloatType SignedDistance(const VecType& P1, const VecType& P2);

		// square
		template<class VecType>
		typename VecType::FloatType SignedSquare   (const Vec3Pair<typename VecType::FloatType>& pair)                       { return Det2x2(pair.first.X(), pair.first.YY(), pair.second.X(), pair.second.YY()) / 2.0F; }

		template<class VecType>
		typename VecType::FloatType TriangleSquare (const Vec3Pair<typename VecType::FloatType>& pair)                           { return Abs(SignedSquare(pair)); }

		// triple product ==( v1 dot (v2 cross v3) )
		template<class VecType>
		typename VecType::FloatType TripleProduct(const VecType& P1, const VecType& P2, const VecType& P3) { return P1.DotWith(P2.CrossWith(P3)); }
		// returns vector of unit length
		template<class VecType>
		VecType VecUnit(const VecType& P);

		// returns NaN if axis is None
		template<class VecType>
		typename VecType::FloatType VecElem     (const VecType& P, EAxis axis);

		template<class VecType>
		EAxis   MaxProjAxis (const VecType& P);

		// projecting vectors to planes
		template<class VecType>
		VecType ProjXY(const VecType& P);

		template<class VecType>
		VecType ProjXZ(const VecType& P);

		template<class VecType>
		VecType ProjYZ(const VecType& P);

		// returns Vector3.One if plane is None
		template<class VecType>
		VecType ProjTo(const VecType& P, EPlane plane);

		// sign function (multiplies the corresponding vector component on -1)
		// (WARN: the vector must have all coordinates positive or zero!)
		template<class VecType>
		VecType MinusMask000(const VecType& P);

		template<class VecType>
		VecType MinusMask001(const VecType& P);

		template<class VecType>
		VecType MinusMask010(const VecType& P);

		template<class VecType>
		VecType MinusMask011(const VecType& P);

		template<class VecType>
		VecType MinusMask100(const VecType& P);

		template<class VecType>
		VecType MinusMask101(const VecType& P);

		template<class VecType>
		VecType MinusMask110(const VecType& P);

		template<class VecType>
		VecType MinusMask111(const VecType& P);
	} // Math
} // Dv

#endif // _MathUtil_VectorBase_h_