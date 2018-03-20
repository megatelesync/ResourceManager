#ifndef _MathUtil_RotatorToMatrix_h_
#define _MathUtil_RotatorToMatrix_h_

#include "MathUtil_Matrix.h"
#include "MathUtil_Rotator.h"
#include <DirectXMath.h>
#include <cassert>
using namespace DirectX;

namespace DvUtils
{
   namespace Math
   {
      inline Matrix QuatToMatrix(const Quat& quat) { assert(&quat); return Matrix(XMMatrixRotationQuaternion(quat.ToXM())); }
	  inline Matrix RotatorToMatrix(const Rotator& rotator) { assert(&rotator); return QuatToMatrix(rotator.quat);}
	  inline Matrix EulerToMatrix(const Euler& euler) { assert(&euler); return XMMatrixRotationRollPitchYaw(euler.pitch,euler.yaw,euler.roll); }
   } // namespace Math
} // namespace DvUtils

#endif // _MathUtil_RotatorToMatrix_h_