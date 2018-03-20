#pragma once

#include "MathUtil_Base.h"
#include "MathUtil_Vector3.h"
#include <DirectXMath.h>
#include <cassert>
using namespace DirectX;

namespace DvUtils
{
   namespace Math
   {
	  class Quat
	  {
	  public:
		  Quat() : _vec(XMQuaternionIdentity()) {} // creates an identity quaternions
		  Quat(float pitch, float yaw, float roll) : _vec(XMQuaternionRotationRollPitchYaw(pitch,yaw,roll)) {}
		  Quat(float x, float y, float z, float w) : _vec(XMVectorSet(x,y,z,w)) {}
		  Quat(const XMVECTOR& vecIn) : _vec(vecIn) {}
		  const XMVECTOR& ToXM() const { return _vec; }
		  // Identity quaternion
		  static const Quat Identity;
		  // copying
		  Quat& operator=(const Quat& second) {  assert(&second); _vec = second._vec; return *this; }
		  Quat(const Quat& second) { assert(&second); _vec = second._vec; }
		  // comparison, near equality
		  bool IsIdentity() const { return XMQuaternionIsIdentity(_vec); }
		  // is quaternion quaternion good (contains only numeric values,
		  // does not contain infinity numbers and NaNs)
		  bool IsGood() const { return !IsNaN() && !IsInfinite(); }
		  bool IsNaN() const { return XMQuaternionIsNaN(_vec); }
		  bool IsInfinite() const { return XMQuaternionIsInfinite(_vec); }
		  bool operator==(const Quat& second) const { assert(&second); return XMQuaternionEqual(_vec,second._vec); }
		  bool operator!=(const Quat& second) const { return !(operator==(second)); }
		  // getting components, setting components
		  Quat& Set(float x, float y, float z) { _vec = XMVectorSet(x,y,z,1.0f); return *this; }
		  float X() const { return XMVectorGetX(_vec); }
		  float YY() const { return XMVectorGetY(_vec); } 
		  float ZZZ() const { return XMVectorGetZ(_vec); }
		  float WWWW() const { assert(XMVectorGetW(_vec) == 1.0F); return XMVectorGetW(_vec); }
		  // length, square length, normalize
		  Quat& Normalize() { _vec = XMQuaternionNormalize(_vec); return *this; }
		  Quat GetNormalized() const { return Quat(XMQuaternionNormalize(_vec)); }
		  float SqrLength() const { return XMVectorGetX(XMQuaternionLengthSq(_vec)); }
		  float Length() const { return XMVectorGetX(XMQuaternionLength(_vec)); } 
		  // arithmetic
		  Quat operator*(const Quat& second) const { return Quat(XMQuaternionMultiply(_vec,second._vec)); }
		  Quat& operator*=(const Quat& second) { _vec = XMQuaternionMultiply(_vec,second._vec); return *this; }
		  Quat GetConjugated() const { return Quat(XMQuaternionConjugate(_vec)); }
		  Quat& Conjugate() { _vec = XMQuaternionConjugate(_vec); return *this; }
		  // inverse
		  Quat GetInversed() const { return Quat(XMQuaternionInverse(_vec)); }
		  Quat& Inverse() { _vec = XMQuaternionInverse(_vec); return *this; }
		  // ToString
		  std::string ToDebugString() const
		  {
			  char buf[256];
			  sprintf_s(buf, 256, "{X=%f Y=%f Z=%f W=%f}",
				  X(),YY(),ZZZ(),WWWW());			  		
			  return std::string(buf);
		  }
		  std::string ToString() const { return ToDebugString(); }
	  private:
		  XMVECTOR _vec;
	  };

	  // Euler struct
      struct Euler
      {
         // fields
         float pitch, yaw, roll;
         // construction
         Euler(float pitchIn, float yawIn, float rollIn) : 
			 pitch(pitchIn), yaw(yawIn), roll(rollIn) {}
      };

	  struct Rotator
	  {
	     Quat quat;

         // operations
         Rotator    GetInversed() { return Rotator(quat.GetInversed()); }
         Rotator    GetMultiplied(const Rotator& second) { return Rotator(quat*second.quat); }

         // construction
         Rotator(const Quat& quatIn) : quat(quatIn) {}
         Rotator(const Euler& eulerIn) : quat(Quat(eulerIn.pitch, eulerIn.yaw, eulerIn.roll)) {}    
         Rotator(float pitchIn, float yawIn, float rollIn) : quat(Quat(pitchIn, yawIn, rollIn)) {}            
	  };
   } // namespace Math
} // namespace DvUtils