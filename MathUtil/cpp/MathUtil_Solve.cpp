#include "../h/MathUtil_Solve.h"
#include <cfloat>

namespace DvUtils
{
   namespace Math
   {
	   float Det3x3 (const Plane& first, const Plane& second, const Plane& third)
	   {
		   assert(&first);
		   assert(&second);
		   assert(&third);
		   float detValue = first.A() * (second.BB() * third.CCC() - third.BB() * second.CCC())
                          - first.BB() * (second.A() * third.CCC() - third.A() * second.CCC())
                          + first.CCC() * (second.A() * third.BB() - third.A() * second.BB());
            return detValue;
	   }
	   bool Solve3x3 (const Plane& first, const Plane& second, const Plane& third, Vec<float, 3> *pResultOut)
	   {
		   assert(&first);
		   assert(&second);
		   assert(&third);
           float detValue  = Det3x3(first,second,third);
           if (Abs(detValue) < FLT_EPSILON) { *pResultOut = Vec<float, 3>(0,0,0);  return false; }
           *pResultOut = Vec<float, 3>(Det3x3(first.DDDD(),first.BB(),first.CCC(),
                                        second.DDDD(),second.BB(),second.CCC(),
                                        third.DDDD(),third.BB(),third.CCC()) / detValue, 
                                 Det3x3(first.A(),first.DDDD(),first.CCC(),
                                        second.A(),second.DDDD(),second.CCC(),
                                        third.A(),third.DDDD(),third.CCC()) / detValue, 
                                 Det3x3(first.A(),first.BB(),first.DDDD(),
                                        second.A(),second.BB(),second.DDDD(),
                                        third.A(),third.BB(),third.DDDD()) / detValue);
           return true;
	   }
   } // namespace Math
} // namespace DvUtil