#ifndef _MathUtil_Rotator_h_
#define _MathUtil_Rotator_h_

#include "MathUtil_RotatorTypes.h"

namespace DvUtils
{
   namespace Math
   {
	  Rotator Lerp(const Rotator& min, const Rotator& maxim, float factor);
	  // Rotation
      Vector3 Direction(const Vector3& baseDir, const Rotator& rotation);
      Vector3 Direction(const Vector3& baseDir, const Euler& rotation);
   } // namespace Math
} // namespace DvUtils

#endif // _MathUtil_Rotator_h_