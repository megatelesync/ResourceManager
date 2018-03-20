#ifndef _MathUtil_Spline_Master_h_
#define _MathUtil_Spline_Master_h_

#include "../MathTypes/BLAS/MathUtil_BLAS.h"
#include "../Basic//MathUtil_Basic_Algebra.h"

namespace DvUtils
{
	namespace Math	
	{
		// Point of spline in the Ermite form
		// (The spline is in Ermite form if it's expressed with end point coordinates and their tangent vectors)
		struct ErmiteSplinePoint3D
		{
			Vec<float, 3>		point;
			Vec<float, 3>		tangent;

			inline ErmiteSplinePoint3D(const Vec<float, 3>& InPoint, const Vec<float, 3>& InTangent);
		};

		// Get point on the Ermite spline
		// t - parameter (from 0.0F to 1.0F)
		inline Vec<float, 3> CalculateErmiteSplinePoint3D(const ErmiteSplinePoint3D& start, const ErmiteSplinePoint3D& end, float t);






		// Impl
		inline ErmiteSplinePoint3D::ErmiteSplinePoint3D(const Vec<float, 3>& InPoint, const Vec<float, 3>& InTangent)
			: point(InPoint), tangent(InTangent)
		{
		}

		inline Vec<float, 3> CalculateErmiteSplinePoint3D(const ErmiteSplinePoint3D& start, const ErmiteSplinePoint3D& end, float t)
		{
			PowerCache3 t_power_cache;
			t_power_cache.SetValue(t);
			float double_t_pow3 = 2.0F * t_power_cache.GetValuePower3();
			float double_t_pow2 = 2.0F * t_power_cache.GetValuePower2();
			float triple_t_pow2 = double_t_pow2 +  t_power_cache.GetValuePower2();

			float double_t_pow3_triple_t_pow2_delta = double_t_pow3 - triple_t_pow2;
			float start_point_coeff = double_t_pow3_triple_t_pow2_delta + 1.0F;
			float end_point_coeff = -double_t_pow3_triple_t_pow2_delta;
			float start_tangent_coeff = t_power_cache.GetValuePower3() - double_t_pow2 + t_power_cache.GetValue();
			float end_tangent_coeff = t_power_cache.GetValuePower3() - t_power_cache.GetValuePower2();
			Vec<float, 3> result_point = start.point * start_point_coeff + 
								   end.point * end_point_coeff +
								   start.tangent * start_tangent_coeff +
								   end.tangent * end_tangent_coeff;
			return result_point;
		}
	} // Math
} // DvUtils

#endif // _MathUtil_Spline_Master_h_