#ifndef _MathUtil_Vector3DoubleTypes_h_
#define _MathUtil_Vector3DoubleTypes_h_

#define _CRT_SECURE_NO_WARNINGS
#include "MathUtil_VectorBaseTypes.h"
#include "MathUtil_Base.h"
#include <DirectXMath.h>
#include <cassert>
#include <string>
#include <cstdio>
using namespace DirectX;

namespace DvUtils
{
   namespace Math
   {
	  // forward declarations
	  class DoubleVector3;

	  class DoubleVector3 : public Vec3<double>
	  {
	  public:
		  inline DoubleVector3() : Vec3() {}
		  inline DoubleVector3(const XMVECTOR& vecIn) : Vec3(vecIn) {}
		  inline DoubleVector3(double x, double y, double z) : Vec3(x,y,z) {}
		  inline DoubleVector3(const Vec3<double>& other) : Vec3(other) {}
		  
		  // Zero vector
		  static const DoubleVector3 Zero;		  
	  };

	  typedef Vec3Pair<double> DoubleVectorPair;	 
   } // namespace Math
} // namespace DvUtils

#endif // _MathUtil_Vector3DoubleTypes_