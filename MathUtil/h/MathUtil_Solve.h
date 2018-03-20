#ifndef _MathUtil_Solve_h_
#define _MathUtil_Solve_h_

#include <utility>
#include "Core/MathUtil_Core_LinearAlgebraUtils.h"
#include "MathUtil_Plane.h"

namespace DvUtils
{
   namespace Math
   {	
	   // -------------------------------------------------------------------------------------------
	  // Solving linear equations
	  // --------------------------------------------------------------------------------------------	 

	  float   Det3x3         (const Plane& first, const Plane& second, const Plane& third);	          
      bool    Solve3x3       (const Plane& first, const Plane& second, const Plane& third, Vec<float, 3> *pResultOut);

	  // Solve a system 2x2 of equations:
	  // K_0_0 * x + K_0_1 * y = B_0
	  // K_1_0 * x + K_1_1 * y = B_1
	  
	  template<class FloatTypeArg>
	  FloatTypeArg *Solve2x2 
	  (
			FloatTypeArg *pRes,
			FloatTypeArg K_0_0, FloatTypeArg K_0_1, 
			FloatTypeArg K_1_0, FloatTypeArg K_1_1,
			FloatTypeArg B_0, FloatTypeArg B_1
	  );

	  // Find determinant of matrix of the following form:
	  //			|ax ay 1|
	  // ORIENT2D = |bx by 1|   =  |ax-cx		ay-cy|
	  //			|cx cy 1|	   |bx-cx		by-cy|
	  //
	  // Practical applications (see the RealTime collision detection book_:
	  // 1. Determine the clockwisity of the 2D triangle based on vertices (ax, ay); (bx, by); (cx; cy)
	  // (ORIENT2D > 0 => ABC is oriented counter-clockwise)
	  // Arguments:
	  // pInA, pInB, pInC pointers to 2D vectors
	  template<class FloatType> FloatType DetOrient2D(const FloatType *pInA, const FloatType *pInB, const FloatType *pInC);
	  // Find determinant of matrix of the following form:
	  //
	  // ORIENT3D = |ax ay az 1| 
	  //			|bx by bz 1| = |ax-dx	ay-dy	az-dz|
	  //			|cx cy cz 1|   |bx-dx	by-dy	bz-dz|  = (A-D) * ((B-D) cross (C-D))
	  //			|dx dy dz 1|   |cx-dx	cy-dy	cz-dz|
	  //
	  // Practical applications:
	  // 1. ORIENT3D > 0 => D lies below the (A,B,C) triangle;
	  //	(A,B,C triangle is clockwise looking from point D)
	  //    ORIENT3D = 0 => all points are coplanar;
	  template<class FloatType> FloatType DetOrient3D(const FloatType *pInA, const FloatType *pInB, const FloatType *pInC, const FloatType *pInD);




	  // Impl
	  template<class FloatTypeArg>
	  FloatTypeArg *Solve2x2 
	  (
			FloatTypeArg *pRes,
			FloatTypeArg K_0_0, FloatTypeArg K_0_1, 
			FloatTypeArg K_1_0, FloatTypeArg K_1_1,
			FloatTypeArg B_0, FloatTypeArg B_1
	  )
	  {
		  assert(pRes != nullptr);
		  // 2 mults; 1 sub
		  typename FloatTypeArg det = Det2x2(K_0_0, K_0_1, K_1_0, K_1_1);
		  // 2 mults; 1 sub
		  typename FloatTypeArg det_x = Det2x2(B_0, K_0_1, B_1, K_1_1);
		  // 2 mults; 1 sub
		  typename FloatTypeArg det_y = Det2x2(K_0_0, B_0, K_1_0, B_1);		  
		  // 1 div
		  pRes[0] = det_x / det;
		  // 1 div
		  pRes[1] = det_y / det;
		  // total: 3 * 2 = 6 mults; 2 divs; 3 * 1 = 3 subs; = 6+2+3=11 ops
		  return pRes;
	  }

	  template<class FloatType> FloatType DetOrient2D(const FloatType *pInA, const FloatType *pInB, const FloatType *pInC)
	  {
		  assert(pInA);
		  assert(pInB);
		  assert(pInC);
		  FloatType ax_sub_cx = pInA[0] - pInC[0];
		  FloatType bx_sub_cx = pInB[0] - pInC[0];
		  FloatType ay_sub_cy = pInA[1] - pInC[1];
		  FloatType by_sub_cy = pInB[1] - pInC[1];
		  return Det2x2(ax_sub_cx, ay_sub_cy, bx_sub_cx, by_sub_cy);
	  }

	  template<class FloatType> 
	  FloatType DetOrient3D(const FloatType *pInA, const FloatType *pInB, const FloatType *pInC, const FloatType *pInD)
	  {
		  FloatType ax_sub_dx = pInA[0] - pInD[0];
		  FloatType bx_sub_dx = pInB[0] - pInD[0];
		  FloatType cx_sub_dx = pInC[0] - pInD[0];

		  FloatType ay_sub_dy = pInA[1] - pInD[1];
		  FloatType by_sub_dy = pInB[1] - pInD[1];
		  FloatType cy_sub_dy = pInC[1] - pInD[1];

		  FloatType az_sub_dz = pInA[2] - pInD[2];
		  FloatType bz_sub_dz = pInB[2] - pInD[2];
		  FloatType cz_sub_dz = pInC[2] - pInD[2];

		  return Det3x3(ax_sub_dx, ay_sub_dy, az_sub_dz, bx_sub_dx, by_sub_dy, bz_sub_dz, cx_sub_dx, cy_sub_dy, cz_sub_dz);
	  }
   } // namespace Math
} // namespace DvUtils

#endif // _MathUtil_Solve_h_
