#ifndef _MathUtil_VectorMatrixMult_h_
#define _MathUtil_VectorMatrixMult_h_

#include "MathUtil_Vector3types.h"
#include "MathUtil_Matrix.h"

namespace DvUtils
{
   namespace Math
   {
	  inline Vector3		operator*		(const Vector3& v, const Matrix& m);
	  inline Vector3&		operator*=		(Vector3& v, const Matrix& m);


      inline Vector3 operator*(const Vector3& v, const Matrix& m)
      {
         assert(&v);
         assert(&m);
         return Vector3(XMVector3Transform(v.ToFloatXM(),m.ToXM()));
      }

      inline Vector3& operator*=(Vector3& v, const Matrix& m)
      {
         assert(&v);
         assert(&m);
         v.Set(XMVector3Transform(v.ToFloatXM(),m.ToXM()));
         return v;
      }
   } // namespace Math
} // namespace DvUtils

#endif _MathUtil_VectorMatrixMult_h_