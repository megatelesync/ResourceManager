#ifndef _MathUtil_MatrixTypes_h_
#define _MathUtil_MatrixTypes_h_

#define _CRT_SECURE_NO_WARNINGS
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
	   // XMMATRIX is the Row-major matrix.
	   class Matrix
	   {
	   public:
		   Matrix() : _mat(XMMatrixIdentity()) {}
		   Matrix(const XMMATRIX& matIn) : _mat(matIn) {}
		   Matrix(const XMVECTOR& r1, const XMVECTOR& r2, const XMVECTOR& r3, const XMVECTOR& r4)
				{ _mat.r[0] = r1; _mat.r[1] = r2; _mat.r[2] = r3; _mat.r[3] = r4; }
		   Matrix(float m11, float m12, float m13, float m14,
			      float m21, float m22, float m23, float m24,
				  float m31, float m32, float m33, float m34,
				  float m41, float m42, float m43, float m44) : 
				_mat(XMMatrixSet(m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44)) {}
		   const XMMATRIX& ToXM() const { return _mat; }
		   // Identity matrix
		   static const Matrix Identity;
		   // Copying
		   Matrix& operator=(const Matrix& second) 
		   {  
			   assert(&second);
			   _mat = second._mat;
			   return *this;
		   }
		   Matrix(const Matrix& second)
		   {
			   assert(&second);
			   _mat = second._mat;
		   }
		   // comparison, near equality
		   // IsNaN, IsInfinite
		   bool IsGood() const { return !IsNaN() && !IsInfinite(); }
		   bool IsIdentity() const { return XMMatrixIsIdentity(_mat); }
		   bool IsNaN() const { return XMMatrixIsNaN(_mat); }
		   bool IsInfinite() const { return XMMatrixIsInfinite(_mat); }
		   bool operator==(const Matrix& second) const 
		   { 
			   assert(&second); 
			   return XMVector4Equal(_mat.r[0],second._mat.r[0]) && XMVector4Equal(_mat.r[1],second._mat.r[1])
				                     && XMVector4Equal(_mat.r[2],second._mat.r[2]) && XMVector4Equal(_mat.r[3],second._mat.r[3]); }
		   bool operator!=(const Matrix& second) const { assert(&second); return !(*this == second); }
		   // getting components, setting components
		   void Set(const XMVECTOR& r1, const XMVECTOR& r2, const XMVECTOR& r3, const XMVECTOR& r4)
		   {
			    _mat.r[0] = r1; _mat.r[1] = r2; _mat.r[2] = r3; _mat.r[3] = r4;
		   }
		   void Set(float m11, float m12, float m13, float m14,
			        float m21, float m22, float m23, float m24,
			        float m31, float m32, float m33, float m34,
			        float m41, float m42, float m43, float m44) 
		   {
				_mat = XMMatrixSet(m11,m12,m13,m14,m21,m22,m23,m24,m31,m32,m33,m34,m41,m42,m43,m44);
		   }
		   void SetRow(int i, const XMVECTOR& v)
		   {
			   assert(&v);
			   assert(i >= 0);
			   assert(i <= 3);
			   _mat.r[i] = v;
		   }
		   const XMVECTOR& GetRow(int i) const
		   {
			   assert(i >= 0);
			   assert(i <= 3);
			   return _mat.r[i];
		   }		   
		   float GetRowX(int i) const
		   {
			   assert(i >= 0);
			   assert(i <= 3);
			   return XMVectorGetX(_mat.r[i]);
		   }
		   float GetRowYY(int i) const
		   {
			   assert(i >= 0);
			   assert(i <= 3);
			   return XMVectorGetY(_mat.r[i]);
		   }
		   float GetRowZZZ(int i) const
		   {
			   assert(i >= 0);
			   assert(i <= 3);
			   return XMVectorGetZ(_mat.r[i]);
		   }
		   float GetRowWWWW(int i) const
		   {
			   assert(i >= 0);
			   assert(i <= 3);
			   return XMVectorGetW(_mat.r[i]);
		   }
		   // arithmetic
		   Matrix operator*(const Matrix& second) const { return Matrix(XMMatrixMultiply(_mat,second._mat)); } 		   
		   // assign-arithmetic
		   Matrix operator*=(const Matrix& second) { _mat = XMMatrixMultiply(_mat,second._mat); return *this; } 		  
		   // inverse, transpose		   
		   Matrix& Transpose() { _mat = XMMatrixTranspose(_mat); return *this; }
		   Matrix GetTransposed() const { return Matrix(XMMatrixTranspose(_mat)); }
		   // Inverse: compute the inverse (and additionallty, the determinant)
		   // retuns an infinite matrix, if there's no inverse
		   Matrix& Inverse(XMVECTOR *pVectorDetOut = nullptr) { 			   
			   _mat = XMMatrixInverse(pVectorDetOut,_mat); 
			   return *this; 
		   }
		   Matrix GetInversed(XMVECTOR *pVectorDetOut = nullptr) const
		   {			   
			   return Matrix(XMMatrixInverse(pVectorDetOut,_mat));
		   }
		   // determinant as value replicated in each of the vetor's components)
		   XMVECTOR DetVect() const { return XMMatrixDeterminant(_mat); }
		   // determinant as a single floating point value
		   float Det() const { XMVectorGetX(DetVect()); }
		   // decompose
		   bool TryDecompose(XMVECTOR *pOutScale, XMVECTOR *pOutRotQuat, XMVECTOR *pOutTrans) const
		   {
			   assert(pOutScale);
			   assert(pOutRotQuat);
			   assert(pOutTrans);
			   return XMMatrixDecompose(pOutScale,pOutRotQuat,pOutTrans,_mat);
		   }
		   // ToString		   		  
		   std::string ToDebugString() const
		   {
			  char buf[256];
			  sprintf(buf,"{{X=%f Y=%f Z=%f W=%f}\n{X=%f Y=%f Z=%f W=%f}\n{X=%f Y=%f Z=%f W=%f}\n{X=%f Y=%f Z=%f W=%f}}",
				  GetRowX(0),GetRowYY(0),GetRowZZZ(0),GetRowWWWW(0),
				  GetRowX(1),GetRowYY(1),GetRowZZZ(1),GetRowWWWW(1),
				  GetRowX(2),GetRowYY(2),GetRowZZZ(2),GetRowWWWW(2),
				  GetRowX(3),GetRowYY(3),GetRowZZZ(3),GetRowWWWW(3));			  		
			  return std::string(buf);
		   }
		   std::string ToString() const { return ToDebugString(); }
	   private:
		   XMMATRIX _mat;
	   };
   } // namespace Math
} // namespace DvUtils

#endif // _MathUtil_MatrixTypes_h_