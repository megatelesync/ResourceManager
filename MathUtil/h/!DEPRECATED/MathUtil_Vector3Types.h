#ifndef _MathUtil_Vector3Types_h_
#define _MathUtil_Vector3Types_h_

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
	  class Matrix;
	  class Vector3;
		  
	  class Vector3 : public Vec3<float>
	  {
	  public:
		  Vector3() : Vec3() {}
		  Vector3(const XMVECTOR& vecIn) : Vec3(vecIn) { assert(&vecIn); }
		  Vector3(float x, float y, float z) : Vec3(x, y, z) {}
		  Vector3(const Vec3<float>& other) : Vec3(other) {}
		  // Zero vector
		  static const Vector3 Zero;	  
		  // Axis vectors
		  static const Vector3 XX;
		  static const Vector3 Y;
		  static const Vector3 Z;

		  static const Vector3 CounterX;
		  static const Vector3 CounterY;
		  static const Vector3 CounterZ;
		  
		  // vectors 2D values
		  static const Vector3 X1Y1;
		  static const Vector3 X1Z1;
		  static const Vector3 Y1Z1;

		  static const Vector3 XCounter1Y1;
		  static const Vector3 X1YCounter1;
		  static const Vector3 XCounter1YCounter1;
		  static const Vector3 XCounter1Z1;
		  static const Vector3 X1ZCounter1;		  
		  static const Vector3 XCounter1ZCounter1;
		  static const Vector3 YCounter1Z1;
		  static const Vector3 Y1ZCounter1;
		  static const Vector3 YCounter1ZCounter1;
	  };

	  typedef Vec3Pair<float> VectorPair;
   } // namespace Math
} // namespace DvUtils

#endif // _MathUtil_Vector3Types_