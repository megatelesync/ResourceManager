#ifndef _MathUtil_PlaneTypes_h_
#define _MathUtil_PlaneTypes_h_

#define _CRT_SECURE_NO_WARNINGS
#include "MathUtil_Base.h"
#include <DirectXMath.h>
#include <cassert>
#include <cstdio>
#include <string>
using namespace DirectX;

namespace DvUtils
{
   namespace Math
   {
	  class Plane
	  {
	  public:
		  typedef float FloatType;

		  Plane() : _vec(XMVectorZero()) {}
		  Plane(const XMVECTOR& vecIn) : _vec(vecIn) { assert(&vecIn); }
		  Plane(float a, float b, float c, float d) : _vec(XMVectorSet(a,b,c,d)) {}
		  const XMVECTOR& ToXM() const { return _vec; }
		  // copying
		  Plane& operator=(const Plane& second) { assert(&second); _vec = second._vec; return *this; }
		  Plane(const Plane& second) { assert(&second); _vec = second._vec; }
		  // comparison, near equality
		  // is plane good (contains only numeric values,
		  // does not contain infinity numbers and NaNs)
		  bool IsGood() const { return !IsNaN() && !IsInfinite(); }
		  bool IsNaN() const { return XMPlaneIsNaN(_vec); }
		  bool IsInfinite() const { return XMPlaneIsInfinite(_vec); }
		  bool IzNearEqual(Plane& second, float eps) const { assert(&second);  return XMPlaneNearEqual(_vec,second._vec,XMVectorReplicate(eps)); }
		  bool operator==(const Plane& second) const { assert(&second); return XMPlaneEqual(_vec,second._vec); }
		  bool operator!=(const Plane& second) const { assert(&second); return !(*this == second); }
		  // getting components, setting components
		  Plane& Set(float a, float b, float c, float d) { _vec = XMVectorSet(a,b,c,d); return *this; }
		  float A() const { return XMVectorGetX(_vec); }
		  float BB() const { return XMVectorGetY(_vec); } 
		  float CCC() const { return XMVectorGetZ(_vec); }
		  float DDDD() const { return XMVectorGetW(_vec); }
		  const XMVECTOR& ToXMVector() const { return _vec; };
		  // return pointer to the first element of the floating point array
		  const FloatType *ToFloatArr() const { return static_cast<const FloatType*>(&_vec.m128_f32[0]); }
		  // ToString
		  std::string ToDebugString() const
		  {
			  char buf[100];
			  sprintf(buf,"{A=%f B=%f C=%f D=%f}",A(),BB(),CCC(),DDDD());			  		
			  return std::string(buf);
		  }
		  std::string ToString() const { return ToDebugString(); }		  
		  // Normalize
		  Plane& Normalize() { _vec = XMPlaneNormalize(_vec); return *this; }
		  Plane GetNormalized() { return Plane(XMPlaneNormalize(_vec)); }
	  private:
		  XMVECTOR _vec;
	  };

	  Plane PlaneXY(float Z);
	  Plane PlaneYZ(float X);
	  Plane PlaneXZ(float Y);
	  Plane CounterPlaneXY(float Z);
	  Plane CounterPlaneYZ(float X);
	  Plane CounterPlaneXZ(float Y);
   } // namespace Math
} // namespace DvUtils

#endif // _MathUtil_PlaneTypes_h_