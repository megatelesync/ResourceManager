#ifndef _MathUtil_MatrixBaseTypes_h_
#define _MathUtil_MatrixBaseTypes_h_

#define _CRT_SECURE_NO_WARNINGS
#include "MathUtil_Base.h"
#include "MathUtil_VectorBaseTypes.h"
#include <DirectXMath.h>
#include <cassert>
#include <string>
#include <cstdio>
using namespace DirectX;

namespace DvUtils
{
	namespace Math
	{	
	   template<class FloatTypeArg>
	   class Mat44
	   {
	   public:
		   /**********************************************************************************
		   * Typedefs
		   **********************************************************************************/
		   typedef FloatTypeArg					FloatType;
		   typedef Vec4<FloatTypeArg>			VecType;
		   typedef Vec3<FloatTypeArg>			Vec3Type;
		   typedef Mat44<FloatTypeArg>			ThisType;

		   /**********************************************************************************
		   * Ctors
		   **********************************************************************************/
		   inline Mat44						();
		   inline Mat44						(const XMMATRIX& matIn);
		   Mat44							(const XMVECTOR& r1, const XMVECTOR& r2, const XMVECTOR& r3, const XMVECTOR& r4);				
		   Mat44
		   (
				FloatTypeArg m11, FloatTypeArg m12, FloatTypeArg m13, FloatTypeArg m14,
			    FloatTypeArg m21, FloatTypeArg m22, FloatTypeArg m23, FloatTypeArg m24,
				FloatTypeArg m31, FloatTypeArg m32, FloatTypeArg m33, FloatTypeArg m34,
				FloatTypeArg m41, FloatTypeArg m42, FloatTypeArg m43, FloatTypeArg m44
		   );			

		   /***********************************************************************************
		   * Conversion to other forms
		   ***********************************************************************************/
		   const XMMATRIX&				ToXM					() const;

		   FloatTypeArg				   *GetPointer				()       { return _columns[0].GetPointer(); }
		   const FloatTypeArg          *GetPointer				() const { return _columns[0].GetPointer(); }

		   /***********************************************************************************
		   * Copying
		   ***********************************************************************************/
		   // Copying
		   ThisType&					operator=					(const ThisType& second);
		   Mat44													(const ThisType& second);	

		   /***********************************************************************************
		   * Checks		   
		   ***********************************************************************************/
		   bool IsIdentity()		const { return XMMatrixIsIdentity(ToXM()); }

		   // comparison, near equality
		   // IsNaN, IsInfinite
		   bool IsGood()			const { return !IsNaN() && !IsInfinite(); }		   
		   bool IsNaN()				const { return XMMatrixIsNaN(ToXM()); }
		   bool IsInfinite()		const { return XMMatrixIsInfinite(ToXM()); }

		   /************************************************************************************
		   * Equality comparison
		   ************************************************************************************/
		   bool operator==(const ThisType& second) const;
		   bool operator!=(const ThisType& second) const;

		   /************************************************************************************
		   * Getters
		   ************************************************************************************/
		   const VecType&					GetColumn				(int i) const;	
		   VecType							GetRow					(int i) const;		 
		   FloatType						GetRowX					(int i) const;
		   FloatType						GetRowYY				(int i) const;
		   FloatType						GetRowZZZ				(int i) const;
		   FloatType						GetRowWWWW				(int i) const;
		   
		   /************************************************************************************
		   * Setters
		   ************************************************************************************/
		   void						Set
		   (
				const XMVECTOR& r1, 
				const XMVECTOR& r2, 
				const XMVECTOR& r3, 
				const XMVECTOR& r4
		   );
		   void						Set
		   (			
					FloatType m11, FloatType m12, FloatType m13, FloatType m14,
					FloatType m21, FloatType m22, FloatType m23, FloatType m24,
					FloatType m31, FloatType m32, FloatType m33, FloatType m34,
					FloatType m41, FloatType m42, FloatType m43, FloatType m44		   
		   );

		   // Set values of m * n values with a specified distance between component locations in bytes
		   // and specified distance between rows
		   template<class SrcFloatType>
		   void						Set							
		   (
				const SrcFloatType		*pInMatrix, 
				int						 InNumRows,
				int						 InNumColumns,
				uint8_t					 InPerRowStrideInBytes,
				uint8_t					 InPerComponentStepInBytes 		= sizeof(SrcFloatType)
		   );

		   void SetRow(int i, const XMVECTOR& v);		   		   	   
		   void SetColumn(int i, const VecType& newVec);
		   
		   /*********************************************************************************
		   * Multiplication
		   *********************************************************************************/
		   // arithmetic
		   ThisType operator*(const ThisType& second) const;
		   // assign-arithmetic
		   ThisType operator*=(const ThisType& second);

		   /*********************************************************************************
		   * Transposition and inverse
		   *********************************************************************************/
		   ThisType				GetTransposed			()											const;
		   ThisType				GetInversed				(XMVECTOR *pVectorDetOut = nullptr)			const;

		   ThisType&			Transpose				();		   
		   // Inverse: compute the inverse (and additionallty, the determinant)
		   // retuns an infinite matrix, if there's no inverse
		   ThisType&			Inverse					(XMVECTOR *pVectorDetOut = nullptr);
		   		   
		   // determinant as value replicated in each of the vetor's components)
		   XMVECTOR				DetVect					() const;
		   // determinant as a single floating point value
		   FloatType			Det						() const;
		   
		   /************************************************************************************************
		   * Matrix decomposition
		   ************************************************************************************************/
		   bool					TryDecompose			(XMVECTOR *pOutScale, XMVECTOR *pOutRotQuat, XMVECTOR *pOutTrans) const;

		   /************************************************************************************************
		   * Conversion to string
		   ************************************************************************************************/
		   // ToString		   		  
		   std::string			ToDebugString			() const;		   
		   std::string			ToString				() const;

	   private:
		   VecType _columns[4];
	   };	   

	   /********************************************************************************************************
	   * Set operations
	   ********************************************************************************************************/
	   // Set only the X,Y and Z components of the given row
	   template<class FloatTypeArg>
	   void SetRow_XYZ
	   (
			const Mat44<FloatTypeArg> *pOutMatrix, 
			int InRowIndex, 
			FloatTypeArg InX, FloatTypeArg InY, FloatTypeArg InZ
	   );

	   /********************************************************************************************************
	   * Transformation operations
	   ********************************************************************************************************/
	   template<class FloatTypeArg>
	   void Matrix_PerformBias(Mat44<FloatTypeArg> *pOutMatrix, const Vec3<FloatTypeArg>& InBias);

	   // ------------------------------------------------------ IMPLEMENTATION -------------------------------------------
	   template<class FloatTypeArg>
	   void SetRow_XYZ
	   (
			const Mat44<FloatTypeArg> *pOutMatrix, 
			int InRowIndex, 
			FloatTypeArg InX, FloatTypeArg InY, FloatTypeArg InZ
	   )
	   {
		   assert(InRowIndex <= 3);
		   assert(pOutMatrix != nullptr);
		   pOutMatrix->Set
	   }

	   template<class FloatTypeArg>
	   void Matrix_PerformBias(Mat44<FloatTypeArg> *pOutMatrix, const Vec3<FloatTypeArg>& InBias)
	   {
		   assert(pOutMatrix != nullptr);
		   assert(&InBias != nullptr);
		   SetRow_XYZ(pOutMatrix, 3, InBias.X, InBias.YY, InBias.ZZZ);
	   }

	   template<class FloatTypeArg>
	   Mat44<FloatTypeArg>::Mat44() 
	   {
		   _columns[0] = VecType(1.0F, 0.0F, 0.0F, 0.0F);
		   _columns[1] = VecType(0.0F, 1.0F, 0.0F, 0.0F);
		   _columns[2] = VecType(0.0F, 0.0F, 1.0F, 0.0F);
		   _columns[3] = VecType(0.0F, 0.0F, 0.0F, 1.0F);
	   }

	   template<class FloatTypeArg>
	   Mat44<FloatTypeArg>::Mat44(const XMMATRIX& matIn)
	   {
		   _columns[0] = VecType(XMVectorGetX(matIn.r[0]), XMVectorGetX(matIn.r[1]), XMVectorGetX(matIn.r[2]), XMVectorGetX(matIn.r[3]));
		   _columns[1] = VecType(XMVectorGetY(matIn.r[0]), XMVectorGetY(matIn.r[1]), XMVectorGetY(matIn.r[2]), XMVectorGetY(matIn.r[3]));
		   _columns[2] = VecType(XMVectorGetZ(matIn.r[0]), XMVectorGetZ(matIn.r[1]), XMVectorGetZ(matIn.r[2]), XMVectorGetZ(matIn.r[3]));
		   _columns[3] = VecType(XMVectorGetW(matIn.r[0]), XMVectorGetW(matIn.r[1]), XMVectorGetW(matIn.r[2]), XMVectorGetW(matIn.r[3]));
	   }

	   template<class FloatTypeArg>
	   Mat44<FloatTypeArg>::Mat44(const XMVECTOR& r1, const XMVECTOR& r2, const XMVECTOR& r3, const XMVECTOR& r4)
	   {
		   _columns[0] = VecType(XMVectorGetX(r1), XMVectorGetX(r2), XMVectorGetX(r3), XMVectorGetX(r4));
		   _columns[1] = VecType(XMVectorGetY(r1), XMVectorGetY(r2), XMVectorGetY(r3), XMVectorGetY(r4));
		   _columns[2] = VecType(XMVectorGetZ(r1), XMVectorGetZ(r2), XMVectorGetZ(r3), XMVectorGetZ(r4));
		   _columns[3] = VecType(XMVectorGetW(r1), XMVectorGetW(r2), XMVectorGetW(r3), XMVectorGetW(r4));
	   }

	   template<class FloatTypeArg>
	   Mat44<FloatTypeArg>::Mat44
	   (
			FloatTypeArg m11, FloatTypeArg m12, FloatTypeArg m13, FloatTypeArg m14,
			FloatTypeArg m21, FloatTypeArg m22, FloatTypeArg m23, FloatTypeArg m24,
			FloatTypeArg m31, FloatTypeArg m32, FloatTypeArg m33, FloatTypeArg m34,
			FloatTypeArg m41, FloatTypeArg m42, FloatTypeArg m43, FloatTypeArg m44
	   )
	   {
		   _columns[0] = VecType(m11, m21, m31, m41);
		   _columns[1] = VecType(m12, m22, m32, m42);
		   _columns[2] = VecType(m13, m23, m33, m43);
		   _columns[3] = VecType(m14, m24, m34, m44);
	   }

	   template<class FloatTypeArg>
	   const XMMATRIX& Mat44<FloatTypeArg>::ToXM() const
	   {
		   XMMatrix res;
		   res.r[0] = XMVectorSet(_columns[0].GetX(), _columns[1].GetX(), _columns[2].GetX(), _columns[3].GetX());
		   res.r[1] = XMVectorSet(_columns[0].GetYY(), _columns[1].GetYY(), _columns[2].GetYY(), _columns[3].GetYY());
		   res.r[2] = XMVectorSet(_columns[0].GetZZZ(), _columns[1].GetZZZ(), _columns[2].GetZZZ(), _columns[3].GetZZZ());
		   res.r[3] = XMVectorSet(_columns[0].GetWWWW(), _columns[1].GetWWWW(), _columns[2].GetWWWW(), _columns[3].GetWWWW());
		   return res;
	   }

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::ThisType& Mat44<FloatTypeArg>::operator=(const ThisType& second) 
	   {  
			assert(&second);
			SetColumn(0,second.GetColumn(0));
			SetColumn(1,second.GetColumn(1));
			SetColumn(2,second.GetColumn(2));
			SetColumn(3,second.GetColumn(3));
			return *this;
	   }

	   template<class FloatTypeArg>
	   Mat44<FloatTypeArg>::Mat44(const ThisType& second)
	   {
			assert(&second);
			_columns[0] = second._columns[0];
			_columns[1] = second._colums[1];
			_columns[2] = second._columns[2];
			_columns[3] = second._columns[3];
	   }

	   template<class FloatTypeArg>
	   bool Mat44<FloatTypeArg>::operator==(const ThisType& second) const 
	   { 
			assert(&second); 
			return _columns[0] == second._columns[0] &&
				   _columns[1] == second._columns[1] &&
				   _columns[2] == second._columns[2] &&
				   _columns[3] == second._columns[3];
	   }

	   template<class FloatTypeArg>
	   bool Mat44<FloatTypeArg>::operator!=(const ThisType& second) const 
	   { 
		   assert(&second); 
		   return !(*this == second); 
	   }

	   template<class FloatTypeArg>
	   void Mat44<FloatTypeArg>::Set(const XMVECTOR& r1, const XMVECTOR& r2, const XMVECTOR& r3, const XMVECTOR& r4)
	   {
		   _columns[0] = VecType(XMVectorGetX(r1), XMVectorGetX(r2), XMVectorGetX(r3), XMVectorGetX(r4));
		   _columns[1] = VecType(XMVectorGetY(r1), XMVectorGetY(r2), XMVectorGetY(r3), XMVectorGetY(r4));
		   _columns[2] = VecType(XMVectorGetZ(r1), XMVectorGetZ(r2), XMVectorGetZ(r3), XMVectorGetZ(r4));
		   _columns[3] = VecType(XMVectorGetW(r1), XMVectorGetW(r2), XMVectorGetW(r3), XMVectorGetW(r4));
	   }

	   template<class FloatTypeArg>
	   void Mat44<FloatTypeArg>::Set
	   (
			FloatType m11, FloatType m12, FloatType m13, FloatType m14,
			FloatType m21, FloatType m22, FloatType m23, FloatType m24,
			FloatType m31, FloatType m32, FloatType m33, FloatType m34,
			FloatType m41, FloatType m42, FloatType m43, FloatType m44
	   )
	   {
		   _columns[0] = VecType(m11, m21, m31, m41);
		   _columns[1] = VecType(m12, m22, m32, m42);
		   _columns[2] = VecType(m13, m23, m33, m43);
		   _columns[3] = VecType(m14, m24, m34, m44);
	   }
	   
	   template<class FloatTypeArg>
	   template<class SrcFloatType>
	   void Mat44<FloatTypeArg>::Set							
	   (
			const SrcFloatType		*pInMatrix, 
			int						 InNumRows,
			int						 InNumColumns,
			uint8_t					 InPerRowStrideInBytes,
			uint8_t					 InPerComponentStepInBytes				
	   )
	   {
		   assert(pInMatrix != nullptr);
		   assert(InNumRows > 0);
		   assert(InNumColumns > 0);
		   assert(InNumRows <= 4);
		   assert(InNumColumns <= 4);		   

		   SetIdentityMatrix
		   (
				GetPointer(), 4, 
				4 * sizeof(FloatTypeArg), sizeof(FloatTypeArg)
		   );		   
		   CopyCastMatrix_Transposed
		   (
				GetPointer(), 
				pInMatrix, 
				InNumRows, InNumColumns, 
				sizeof(FloatTypeArg) * 4,
				InPerRowStrideInBytes,				
				sizeof(FloatTypeArg),
				InPerComponentStepInBytes
		   );			   
	   }

	   template<class FloatTypeArg>
	   void Mat44<FloatTypeArg>::SetRow(int i, const XMVECTOR& v)	  
	   {
			assert(&v);
			assert(i >= 0);
			assert(i <= 3);
			_columns[0].Set(i) = XMVectorGetX(v);
			_columns[1].Set(i) = XMVectorGetY(v);
			_columns[2].Set(i) = XMVectorGetZ(v);
			_columns[3].Set(i) = XMVectorGetW(v);
	   }

	   template<class FloatTypeArg>
	   const typename Mat44<FloatTypeArg>::VecType& Mat44<FloatTypeArg>::GetColumn(int i) const
	   {
		   assert(i >= 0);
		   assert(i <= 3);
		   return _columns[i];
	   }

	   template<class FloatTypeArg>
	   void Mat44<FloatTypeArg>::SetColumn(int i, const VecType& newVec)
	   {
		   assert(i >= 0);
		   assert(i <= 3);
		   _columns[i] = newVec;
	   }

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::VecType Mat44<FloatTypeArg>::GetRow(int i) const
	   {
			assert(i >= 0);
			assert(i <= 3);
			return VecType
			(
				_columns[0].GetElement(i), 
				_columns[1].GetElement(i), 
				_columns[2].GetElement(i), 
				_columns[3].GetElement(i)
			);
	   }	

	   template<class FloatTypeArg>														 
	   typename Mat44<FloatTypeArg>::FloatType Mat44<FloatTypeArg>::GetRowX(int i) const
	   {
			assert(i >= 0);
			assert(i <= 3);
			return _columns[0].GetElement(i);
	   }

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::FloatType Mat44<FloatTypeArg>::GetRowYY(int i) const
	   {
		   	assert(i >= 0);
			assert(i <= 3);
			return _columns[1].GetElement(i);
	   }

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::FloatType Mat44<FloatTypeArg>::GetRowZZZ(int i) const
	   {
			assert(i >= 0);
			assert(i <= 3);
			return _columns[2].GetElement(i);
	   }

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::FloatType Mat44<FloatTypeArg>::GetRowWWWW(int i) const
	   {
		   	assert(i >= 0);
			assert(i <= 3);
			return _columns[3].GetElement(i);
	   }

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::ThisType Mat44<FloatTypeArg>::operator*(const ThisType& second) const 
	   { 
		   VecType rowOne = GetRow(0).DotWith(second.GetColumn(0));
		   VecType rowTwo = GetRow(1).DowWith(second.GetColumn(1));
		   VecType rowThird = GetRow(2).DotWith(second.GetColumn(2));
		   VecType rowFourth = GetRow(3).DotWith(second.GetColumn(3));
		   return ThisType(rowOne, rowTwo, rowThird, rowFourth); 
	   } 		   
	   // assign-arithmetic
	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::ThisType Mat44<FloatTypeArg>::operator*=(const ThisType& second) 
	   {
		   SetRow(0,GetRow(0).DotWith(second.GetColumn(0)));
		   SetRow(1,GetRow(1).DowWith(second.GetColumn(1)));
		   SetRow(2,GetRow(2).DotWith(second.GetColumn(2)));
		   SetRow(3,GetRow(3).DotWith(second.GetColumn(3)));
		   return *this; 
	   } 

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::ThisType& Mat44<FloatTypeArg>::Transpose()
	   {
		   ThisType transposed = GeTransposed();
		   *this = transposed;
		   return *this;
	   }

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::ThisType Mat44<FloatTypeArg>::GetTransposed() const
	   {
			ThisType res;
			res.SetColumn(0,GetRow(0));
			res.SetColumn(1,GetRow(1));
			res.SetColumn(2,GetRow(2));
			res.SetColumn(3,GetRow(3));
			return res;
	   }

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::ThisType& Mat44<FloatTypeArg>::Inverse(XMVECTOR *pVectorDetOut)
	   { 			   
			Set(XMMatrixInverse(pVectorDetOut,ToXM())); 
			return *this; 
	   }		

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::ThisType Mat44<FloatTypeArg>::GetInversed(XMVECTOR *pVectorDetOut) const
	   {
		   ThisType res = *this;
		   res.Inverse();
		   return res;		   
	   }

	   template<class FloatTypeArg>
	   XMVECTOR Mat44<FloatTypeArg>::DetVect() const 
	   { 
		   return XMMatrixDeterminant(ToXM()); 
	   }

	   template<class FloatTypeArg>
	   typename Mat44<FloatTypeArg>::FloatType Mat44<FloatTypeArg>::Det() const 
	   { 
		   return XMVectorGetX(DetVect()); 
	   }

	   template<class FloatTypeArg>
	   bool Mat44<FloatTypeArg>::TryDecompose(XMVECTOR *pOutScale, XMVECTOR *pOutRotQuat, XMVECTOR *pOutTrans) const
	   {
			assert(pOutScale);
			assert(pOutRotQuat);
			assert(pOutTrans);
			return XMMatrixDecompose(pOutScale,pOutRotQuat,pOutTrans,ToXM());
	   }

	   template<class FloatTypeArg>
	   std::string Mat44<FloatTypeArg>::ToDebugString() const
	   {			  
			return (boost::format("%1%\n %2%\n %3%\n %4%\n") % GetRow(0) % GetRow(1) % GetRow(2) % GetRow(3)).str();
	   }

	   template<class FloatTypeArg>
	   std::string Mat44<FloatTypeArg>::ToString() const 
	   { 
		   return ToDebugString(); 
	   }
	} // Math
} // Dv

#endif // _MathUtil_MatrixBaseTypes_h_