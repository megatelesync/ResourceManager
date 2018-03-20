#pragma once

#include "../../Core/MathUtil_Core_LinearAlgebraUtils.h"
#include "../BLAS/MathUtil_BLAS.h"

namespace DvUtils
{
	namespace Math
	{
		/*******************************************************************************
		* TODO:
		* 1. CalcSubmatrix33FromQuaternion +
		* 1.1. Quaternion<FloatTypeArg>::GetComponentStride() +
		* 1.2. GetPointer(Quaternion<>) +
		* 1.2.1. GetArray() : ArrayType +
		* 1.2.1.1. typedef std::array<FloatTypeArg, 4> ArrayType +
		* 1.2.1.1.1. Replace _quat with _arr +
		*******************************************************************************/
		/********************************************************************************
		* 3D Quaternion
		*
		* Implemented using the quaternion functions from the core linear algebra utils
		*
		* Interface:		
		*	Operation functions:
		*		All functions except getters are provided as the non-member functions.	
		*	Initialization:
		*		Primary construction functions are implemented as the real constructors.
		*		To initialize the quaternion from the axis and rotation angle,
		*		you actually need to call the non-member setter function.
		********************************************************************************/
		template<class FloatTypeArg> class Quaternion
		{
		public:
			/**********************************************************************
			* Typedefs
			**********************************************************************/
			using ThisType                                                   = Quaternion<FloatTypeArg>;

			using FloatType                                                  = FloatTypeArg;

			// Array type: w has index 3
			using ArrayType                                                  = std::array<FloatType, 4>;

			using Vec3Type                                                   = Vec<FloatTypeArg, 3>;
			using Vec4Type                                                   = Vec<FloatTypeArg, 4>;

			/**********************************************************************
			* ctors
			**********************************************************************/
			// Default ctor: constructs the positive identity quaternion.
			// ([0.0F, 0.0F, 0.0F], 1)
			Quaternion();
			// Constructs quaternion from the given array with the given stride between elements.
			Quaternion(const FloatTypeArg *pInQuatArr, size_t quatArrStride = sizeof(FloatTypeArg));
			// Constructs quaternion from the vector and scalar components
			Quaternion
			(
				const FloatTypeArg& InVecX,
				const FloatTypeArg& InVecY,
				const FloatTypeArg& InVecZ,
				const FloatTypeArg& InW
			);

			/**********************************************************************
			* static factory functions
			**********************************************************************/
			// WARNIGN!!! Axis must be normalized
			static ThisType            MakeFromAxisAngle(Vec3Type& Axis, float RadAngle);
			static ThisType            MakeFromAxisAngle(float AxisX, float AxisYY, float AxisZZZ, float RadAngle);

			// Make quaternion from euler Pitch, Yaw, Roll (in radians!!!)
			static ThisType            MakeFromPitchYawRoll(float Pitch, float Yaw, float Roll);
			static ThisType            MakeFromPitchYawRollDegs(float Pitch, float Yaw, float Roll);
			// X: Pitch; Y: Yaw; Z: Roll (in radians!!!)
			static ThisType            MakeFromPitchYawRoll(const Vec3Type& InPitchYawRoll);
			static ThisType            MakeFromPitchYawRollDegs(const Vec3Type& InPitchYawRoll);

			/**********************************************************************
			* static functions
			**********************************************************************/
			// Size of one component (in bytes)
			static size_t				GetComponentSize		()											{ return sizeof(FloatType); }
			// Stride of one component (in bytes)
			static size_t				GetComponentStride		()											{ return GetComponentSize(); }

			/**********************************************************************
			* Getters
			**********************************************************************/
			const ArrayType&			GetArray				()		const								{ return _arr; }
			ArrayType&					GetArray				()											{ return _arr; }
			
			/**********************************************************************
			* Member getters
			**********************************************************************/
			FloatTypeArg                            GetX                            () const            { return _arr[0]; }
			FloatTypeArg                            GetYY                           () const            { return _arr[1]; }	
			FloatTypeArg                            GetZZZ                          () const            { return _arr[2]; }	
			FloatTypeArg                            GetWWWW                         () const            { return _arr[3]; }	

			// Get the {X,Y,Z} vector
			Vec3Type                                GetVec                          () const            { return Vec3Type(GetX(), GetYY(), GetZZZ()); }


			/**********************************************************************
			* Conversion to vectors
			**********************************************************************/
			Vec4Type                                ToVec                           () const            { return Vec4Type(GetX(), GetYY(), GetZZZ(), GetWWWW()); }

			/*********************************************************************
			* Conversion to Axis/Angle representation
			**********************************************************************/
			void                                    ToAxisAndAngle                  (Vec3Type* pOutAxis, float* pOutAngleRads) const;

			// Angle in radians
			FloatTypeArg                            GetAngle                        () const;

			// Rotation axis: (1,0,0) for identity quaternion
			Vec3Type                                GetRotationAxis                 () const;

			/*********************************************************************
			* Rotating vectors
			**********************************************************************/
			Vec3Type                                RotateVector                    (const Vec3Type& InVec) const;
			Vec3Type                                UnrotateVector                  (const Vec3Type& InVec) const;

			/*********************************************************************
			* Local-system vectors of system rotated with this quaternion
			**********************************************************************/
			Vec3Type                                GetForwardVector                () const;
			Vec3Type                                GetUpVector                     () const;
			Vec3Type                                GetRightVector                  () const;

			/**********************************************************************
			* Setters
			**********************************************************************/
			void                                    SetX                            (FloatTypeArg InValue)            { _arr[0] = InValue; }
			void                                    SetYY                           (FloatTypeArg InValue)            { _arr[1] = InValue; }
			void                                    SetZZZ                          (FloatTypeArg InValue)            { _arr[2] = InValue; }	
			void                                    SetWWWW                         (FloatTypeArg InValue)            { _arr[3] = InValue; }

			void                                    SetVec                          (const Vec3Type& InVec);
			void                                    SetVec                          (FloatTypeArg InX, FloatTypeArg InYY, FloatTypeArg InZZZ);
			void                                    Set                             (FloatTypeArg InX, FloatTypeArg InYY, FloatTypeArg InZZZ, FloatTypeArg InWWWW);
			void                                    Set                             (const Vec4Type& InVec);	

			/**********************************************************************
			* Magnitude
			*
			* Equal to X^2 + Y^2 + Z^2 + W^2
			*
			* For unit quaternions is equal to one;
			* For identity quaternions is equal to zero;
			**********************************************************************/
			FloatTypeArg                            Magnitude                       () const { return Sqr(SqrMagnitude()); }
			FloatTypeArg                            SqrMagnitude                    () const { return GetX() * GetX() + GetYY() * GetYY() + GetZZZ() * GetZZZ() + GetWWWW() * GetWWWW(); }

			/**********************************************************************
			* Identity
			**********************************************************************/
			bool                                    IsIdentity                      (float Tolerance = GeoEps) const { return (Abs(GetX()) < Tolerance) && (Abs(GetYY()) < Tolerance) && (Abs(GetZZZ()) < Tolerance) && (Abs(GetWWWW()) < Tolerance); }
			void                                    Normalize                       ();
			ThisType                                GetNormalized                   () const;
			 
			/**********************************************************************
			* Conjugate
			*
			* Mathematically: vector {X,Y,Z} part is negated;
			* Geometrically: represents angular displacement in opposite direction
			**********************************************************************/
			void                                    Conjugate                       ()              { SetVec(-GetX(), -GetYY(), -GetZZZ()); }
			ThisType                                GetConjugated                   () const        { return ThisType(-_arr[0], _arr[1], -_arr[2], _arr[3]); }

			/**********************************************************************
			* Inverse
			*
			* Equal to conjugate divided by magnitude.
			**********************************************************************/
			void                                    Inverse                         ()              { Conjugate(); *this /= Magnitude(); }
			ThisType                                GetInversed                     () const        { return GetConjugate()/Magnitude(); }

		private:	
			ArrayType					_arr;
		};

		template<class Strm, class FloatTypeArg>
		Strm& operator<<(Strm& strm, const Quaternion<FloatTypeArg>& Quat)
		{
			return strm << "{X=" << Quat.GetX() << "; Y=" << Quat.GetYY() << "; Z=" << Quat.GetZZZ() << "; W=" << Quat.GetWWWW() << "}" << std::endl;
		}

		template<class FloatTypeArg>
		std::string ToString(const Quaternion<FloatTypeArg>& Quat)
		{
			return std::string("{X=") + std::to_string(Quat.GetX()) + 
				   std::string("; Y=") +  std::to_string(Quat.GetYY()) +
				   std::string("; Z=") + std::to_string(Quat.GetZZZ()) + 
				   std::string("; W=") + std::to_string(Quat.GetWWWW()) + 
				   std::string("}");
		}


		// Calculate matrix from the given quaternion
		// Warning: Only the 3x3 submatrix is updated,
		// the rest of the components are not touched!
		template<class FloatTypeArg> void CalcSubmatrix33FromQuaternion
		(
			SquareMatrix<FloatTypeArg, 4>			*pOutMatrix,
			const Quaternion<FloatTypeArg>&			InQuaternion
		);

		template<class FloatTypeArg> void CalcMatrix33FromQuaternion
		(
			SquareMatrix<FloatTypeArg, 3>			*pOutMatrix,
			const Quaternion<FloatTypeArg>&			InQuaternion
		);

		/************************************************************************************
		* Low-level access
		************************************************************************************/		
		template<class FloatTypeArg>
		const FloatTypeArg *GetPointer(const Quaternion<FloatTypeArg> *pInQuat);

		// Get pointer to the first component of the given vector
		template<class FloatTypeArg>
		FloatTypeArg *GetPointer(Quaternion<FloatTypeArg> *pInQuat);

		// getters for the coordinates of the vector part of the quaternion.
		template<class FloatTypeArg>
		const FloatTypeArg			*GetXPointer			(const Quaternion<FloatTypeArg>& InQuat);

		template<class FloatTypeArg>
		FloatTypeArg				*GetXPointer			(Quaternion<FloatTypeArg>& InQuat);

		template<class FloatTypeArg>
		const FloatTypeArg			*GetYYPointer			(const Quaternion<FloatTypeArg>& InQuat);

		template<class FloatTypeArg>
		FloatTypeArg				*GetYYPointer			(Quaternion<FloatTypeArg>& InQuat);

		template<class FloatTypeArg>
		const FloatTypeArg			*GetZZZPointer			(const Quaternion<FloatTypeArg>& InQuat);

		template<class FloatTypeArg>
		FloatTypeArg				*GetZZZPointer			(Quaternion<FloatTypeArg>& InQuat);

		// Const getters for the W (scalar) part of the quaternion.
		template<class FloatTypeArg>
		const FloatTypeArg			*GetWWWWPointer			(const Quaternion<FloatTypeArg>& InQuat);

		template<class FloatTypeArg>
		FloatTypeArg				*GetWWWWPointer			(Quaternion<FloatTypeArg>& InQuat);


		/************************************************************************************
		* Component read-only access functions
		************************************************************************************/				
		template<class FloatTypeArg>
		const FloatTypeArg& GetX
		(
			const Quaternion<FloatTypeArg>&			InQuat
		);

		template<class FloatTypeArg>
		const FloatTypeArg& GetYY
		(
			const Quaternion<FloatTypeArg>&			InQuat
		);

		template<class FloatTypeArg>
		const FloatTypeArg& GetZZZ
		(
			const Quaternion<FloatTypeArg>&			InQuat
		);

		template<class FloatTypeArg>
		const FloatTypeArg& GetWWWW
		(
			const Quaternion<FloatTypeArg>&			InQuat
		);

		/*******************************************************************************************************
		* Quaternion cross (i.e. multiplication) functions
		*
		* WARNING!!! This functions multiply quaternions in official order,
		* meaning that to concatenate two rotations, you must multiply quaternions in INVERSE order!!!.
		*******************************************************************************************************/	
		template<class FloatTypeArg>
		Quaternion<FloatTypeArg> operator*(const Quaternion<FloatTypeArg>& left, const Quaternion<FloatTypeArg>& right);

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>& operator*=(Quaternion<FloatTypeArg>& left, const Quaternion<FloatTypeArg>& right);

		/******************************************************************************************************
		* Multiplication and division by scalar
		*******************************************************************************************************/
		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>             operator*                  (const Quaternion<FloatTypeArg>& left, FloatTypeArg scalar);

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>             operator*                  (FloatTypeArg scalar, const Quaternion<FloatTypeArg>& right);

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>&            operator*=                 (const Quaternion<FloatTypeArg>& left, FloatTypeArg scalar);

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>             operator/                  (const Quaternion<FloatTypeArg>& left, FloatTypeArg scalar);

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>&            operator/=                 (const Quaternion<FloatTypeArg>& left, FloatTypeArg scalar);

		/******************************************************************************************************
		* Vector by quaternion multiplication
		*******************************************************************************************************/
		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3>                 operator*                  (const Vec<FloatTypeArg, 3>& InVec, const Quaternion<FloatTypeArg>& InQuat);

		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3>&                operator*=                 (Vec<FloatTypeArg, 3>& InVec, const Quaternion<FloatTypeArg>& InQuat);

		/******************************************************************************************************
		* Difference
		*******************************************************************************************************/


		/************************************************************************************
		* Component set functions
		************************************************************************************/		
		template<class FloatTypeArg>
		void SetX
		(
			Quaternion<FloatTypeArg>						*pQuat,
			FloatTypeArg									InValue
		);

		template<class FloatTypeArg>
		void SetYY
		(
			Quaternion<FloatTypeArg>						*pQuat,
			FloatTypeArg									InValue
		);

		template<class FloatTypeArg>
		void SetZZZ
		(
			Quaternion<FloatTypeArg>						*pQuat,
			FloatTypeArg									InValue
		);

		template<class FloatTypeArg>
		void SetWWWW
		(
			Quaternion<FloatTypeArg>						*pQuat,
			FloatTypeArg									InValue
		);



		/****************************************************************************
		* GLOBAL FUNCTION IMPLEMENTATION
		****************************************************************************/
		template<class FloatTypeArg> void CalcSubmatrix33FromQuaternion
		(
			SquareMatrix<FloatTypeArg, 4>			*pOutMatrix,
			const Quaternion<FloatTypeArg>&			InQuaternion
		)
		{
			assert(pOutMatrix != nullptr);
			assert(&InQuaternion);

			// We delibarately swapped the strides between columns and rows
			// of the destination matrix, because the SquareMatrix is column-major
			size_t strideInBytes_betweenMatrixColumns = pOutMatrix->GetColumnSize();
			size_t strideInBytes_betweenMatrixRows    = pOutMatrix->GetComponentSize();
			Quaternion_To_Matrix33
			(
				GetPointer(*pOutMatrix), 
				GetPointer(&InQuaternion),
				strideInBytes_betweenMatrixColumns,
				strideInBytes_betweenMatrixRows,
				InQuaternion.GetComponentStride()
			);
		}

		template<class FloatTypeArg> void CalcMatrix33FromQuaternion
		(
			SquareMatrix<FloatTypeArg, 3>			*pOutMatrix,
			const Quaternion<FloatTypeArg>&			InQuaternion
		)
		{
			assert(pOutMatrix != nullptr);
			assert(&InQuaternion);

			// We delibarately swapped the strides between columns and rows
			// of the destination matrix, because the SquareMatrix is column-major
			size_t strideInBytes_betweenMatrixColumns = pOutMatrix->GetColumnSize();
			size_t strideInBytes_betweenMatrixRows    = pOutMatrix->GetComponentSize();

			Quaternion_To_Matrix33
			(
				GetPointer(*pOutMatrix), 
				GetPointer(&InQuaternion),
				strideInBytes_betweenMatrixColumns,
				strideInBytes_betweenMatrixRows,
				InQuaternion.GetComponentStride()
			);
		}

		template<class FloatTypeArg>
		const FloatTypeArg *GetPointer(const Quaternion<FloatTypeArg> *pInQuat)
		{
			assert(pInQuat != nullptr);
			return pInQuat->GetArray().data();
		}

		template<class FloatTypeArg>
		FloatTypeArg *GetPointer(Quaternion<FloatTypeArg> *pInQuat)
		{
			assert(pInQuat != nullptr);
			return pInQuat->GetArray().data();
		}

		template<class FloatTypeArg>
		const FloatTypeArg *GetXPointer(const Quaternion<FloatTypeArg>& InQuat)
		{
			assert(&InQuat != nullptr);
			return &(InQuat.GetArray[0]);
		}

		template<class FloatTypeArg>
		FloatTypeArg *GetXPointer(Quaternion<FloatTypeArg>& InQuat)
		{
			assert(&InQuat != nullptr);
			return &(InQuat.GetArray[0]);
		}

		template<class FloatTypeArg>
		const FloatTypeArg *GetYYPointer(const Quaternion<FloatTypeArg>& InQuat)
		{
			assert(&InQuat != nullptr);
			return &(InQuat.GetArray[1])
		}

		template<class FloatTypeArg>
		FloatTypeArg *GetYYPointer(Quaternion<FloatTypeArg>& InQuat)
		{
			assert(&InQuat != nullptr);
			return &(InQuat.GetArray[1])
		}

		template<class FloatTypeArg>
		const FloatTypeArg *GetZZZPointer(const Quaternion<FloatTypeArg>& InQuat)
		{
			assert(&InQuat != nullptr);
			return &(InQuat.GetArray[2])
		}

		template<class FloatTypeArg>
		FloatTypeArg *GetZZZPointer(Quaternion<FloatTypeArg>& InQuat)
		{
			assert(&InQuat != nullptr);
			return &(InQuat.GetArray[2])
		}
		
		template<class FloatTypeArg>
		const FloatTypeArg *GetWWWWPointer(const Quaternion<FloatTypeArg>& InQuat)
		{
			assert(&InQuat != nullptr);
			return &(InQuat.GetArray[3])
		}

		template<class FloatTypeArg>
		FloatTypeArg *GetWWWWPointer(Quaternion<FloatTypeArg>& InQuat)
		{
			assert(&InQuat != nullptr);
			return &(InQuat.GetArray[3])
		}

		template<class FloatTypeArg>
		const FloatTypeArg& GetX
		(
			const Quaternion<FloatTypeArg>&			InQuat
		)
		{
			return *GetXPointer(InQuat);
		}

		template<class FloatTypeArg>
		const FloatTypeArg& GetYY
		(
			const Quaternion<FloatTypeArg>&			InQuat
		)
		{
			return *GetYYPointer(InQuat);
		}

		template<class FloatTypeArg>
		const FloatTypeArg& GetZZZ
		(
			const Quaternion<FloatTypeArg>&			InQuat
		)
		{
			return *GetZZZPointer(InQuat);
		}

		template<class FloatTypeArg>
		const FloatTypeArg& GetWWWW
		(
			const Quaternion<FloatTypeArg>&			InQuat
		)
		{
			return *GetWWWWPointer(InQuat);
		}

		template<class FloatTypeArg>
		void SetX
		(
			Quaternion<FloatTypeArg>						*pQuat,
			FloatTypeArg									InValue
		)
		{
			*GetXPointer(*pQuat) = InValue;
		}

		template<class FloatTypeArg>
		void SetYY
		(
			Quaternion<FloatTypeArg>						*pQuat,
			FloatTypeArg									InValue
		)
		{
			*GetYYPointer(*pQuat) = InValue;
		}

		template<class FloatTypeArg>
		void SetZZZ
		(
			Quaternion<FloatTypeArg>						*pQuat,
			FloatTypeArg									InValue
		)
		{
			*GetZZZPointer(*pQuat) = InValue;
		}

		template<class FloatTypeArg>
		void SetWWWW
		(
			Quaternion<FloatTypeArg>						*pQuat,
			FloatTypeArg									InValue
		)
		{
			*GetWWWWPointer(*pQuat) = InValue;
		}

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg> operator*(const Quaternion<FloatTypeArg>& left, const Quaternion<FloatTypeArg>& right)
		{
			FloatTypeArg                                       vecDot                  = Dot(left.GetVec(), right.GetVec());

			// WARNING!!! Here (as it is offical formula) we cross LEFT by RIGHT (no matter what Math primer says)!!!
			// Everywhere the order of cross is V1 to V2 oficcially!!!
			Quaternion<FloatTypeArg>::Vec3Type                 vecCrossReversed        = Cross(left.GetVec(), right.GetVec());

			// WARNING!!! note that in result w vector dot product is subtracted!!!
			FloatTypeArg                                       result_w        = left.GetWWWW() * right.GetWWWW() - vecDot;
			Quaternion<FloatTypeArg>::Vec3Type                 result_vec      = left.GetWWWW() * right.GetVec() + left.GetVec() * right.GetWWWW() + vecCrossReversed;

			return Quaternion<FloatTypeArg>{result_vec.GetX(), result_vec.GetYY(), result_vec.GetZZZ(), result_w};
		}

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>& operator*=(Quaternion<FloatTypeArg>& left, const Quaternion<FloatTypeArg>& right)
		{
			*this = left * right;
			return *this;
		}

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg> operator*(const Quaternion<FloatTypeArg>& left, FloatTypeArg scalar)
		{
			return Quaternion<FloatTypeArg>(left.GetX() * scalar, left.GetYY() * scalar, left.GetZZZ() * scalar, left.GetWWWW() * scalar);
		}

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg> operator*(FloatTypeArg scalar, const Quaternion<FloatTypeArg>& right)
		{
			return right * scalar;
		}

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>& operator*=(const Quaternion<FloatTypeArg>& left, FloatTypeArg scalar)
		{
			Set(left.GetX() * scalar, left.GetYY() * scalar, left.GetZZZ() * scalar, left.GetWWWW() * scalar);
			return *this;
		}

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg> operator/(const Quaternion<FloatTypeArg>& left, FloatTypeArg scalar)
		{
			return Quaternion<FloatTypeArg>(left.GetX() / scalar, left.GetYY() / scalar, left.GetZZZ() / scalar, left.GetWWWW() / scalar);
		}

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>& operator/=(const Quaternion<FloatTypeArg>& left, FloatTypeArg scalar)
		{
			Set(left.GetX() / scalar, left.GetYY() / scalar, left.GetZZZ() / scalar, left.GetWWWW() / scalar);
			return *this;
		}

		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3> operator*(const Vec<FloatTypeArg, 3>& InVec, const Quaternion<FloatTypeArg>& InQuat)
		{
			return InQuat.RotateVector(InVec);
		}

		template<class FloatTypeArg>
		Vec<FloatTypeArg, 3>& operator*=(Vec<FloatTypeArg, 3>& InVec, const Quaternion<FloatTypeArg>& InQuat)
		{
			*this = InQuat.RotateVector(InVec);
			return *this;
		}

		/****************************************************************************
		* MEMBER FUNCTION IMPLEMENTATION
		****************************************************************************/
		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>::Quaternion()
		{
			SetQuaternion_AsIdentity(GetPointer(this), GetComponentStride());
		}

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>::Quaternion(const FloatTypeArg *pInQuatArr, size_t quatArrStride)
		{
			CopyVec4_n(GetPointer(this), pQuatArr, sizeof(FloatTypeArg), quatArrSize);
		}

		template<class FloatTypeArg>
		Quaternion<FloatTypeArg>::Quaternion
		(
			const FloatTypeArg& InVecX,
			const FloatTypeArg& InVecY,
			const FloatTypeArg& InVecZ,
			const FloatTypeArg& InW
		)
		{
			Set4(GetPointer(this), InVecX, InVecY, InVecZ, InW, GetComponentStride());
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::ThisType Quaternion<FloatTypeArg>::MakeFromAxisAngle(Vec3Type& Axis, float RadAngle)
		{
			return MakeFromAxisAngle(Axis.GetX(), Axis.GetYY(), Axis.GetZZZ(), RadAngle);
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::ThisType Quaternion<FloatTypeArg>::MakeFromAxisAngle(float AxisX, float AxisYY, float AxisZZZ, float RadAngle)
		{
			const auto SinHalfAngle = sin(RadAngle/2.0F);
			const auto CosHalfAngle = cos(RadAngle/2.0F);
			return Quaternion<FloatTypeArg>{ AxisX * SinHalfAngle, AxisYY * SinHalfAngle, AxisZZZ * SinHalfAngle, CosHalfAngle };
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::ThisType Quaternion<FloatTypeArg>::MakeFromPitchYawRoll(float Pitch, float Yaw, float Roll)
		{
			auto quatPitch = Quaternion<FloatTypeArg>::MakeFromAxisAngle(1.0F, 0.0F, 0.0F, Pitch);
			auto quatYaw = Quaternion<FloatTypeArg>::MakeFromAxisAngle(0.0F, 1.0F, 0.0F, Yaw);
			auto quatRoll = Quaternion<FloatTypeArg>::MakeFromAxisAngle(0.0F, 0.0F, 1.0F, Roll);

			// NOTE: Order of multiplication is reversed (as official quaternion mult formula is used)
			return quatRoll * quatYaw * quatPitch;
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::ThisType Quaternion<FloatTypeArg>::MakeFromPitchYawRollDegs(const Vec3Type& InEuler)
		{
			return MakeFromPitchYawRoll(InEuler * DEGS_TO_RADS);
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::ThisType Quaternion<FloatTypeArg>::MakeFromPitchYawRollDegs(float Pitch, float Yaw, float Roll)
		{
			return MakeFromPitchYawRoll(Pitch * DEGS_TO_RADS, Yaw * DEGS_TO_RADS, Roll * DEGS_TO_RADS);
		}
		
		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::ThisType Quaternion<FloatTypeArg>::MakeFromPitchYawRoll(const Vec3Type& InPitchYawRoll)
		{
			return MakeFromPitchYawRoll(InPitchYawRoll.GetX(), InPitchYawRoll.GetYY(), InPitchYawRoll.GetZZZ());
		}

		template<class FloatTypeArg>
		void Quaternion<FloatTypeArg>::ToAxisAndAngle(Vec3Type* pOutAxis, float* pOutAngleRads) const
		{
			*pOutAxis                   = GetRotationAxis();
			*pOutAngleRads              = GetAngle();
		}

		template<class FloatTypeArg>
		FloatTypeArg Quaternion<FloatTypeArg>::GetAngle() const
		{
			return 2.0F*acos(GetW());
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::Vec3Type Quaternion<FloatTypeArg>::GetRotationAxis() const
		{
			if (IsIdentity())
			{
				return Vec3Type(1.0F, 0.0F, 0.0F);
			}

			// Here we must ensure that we never try to compute square root from the negative number
			const FloatTypeArg SinHalfAngle_Sqr = Maxim(1.0F-GetW()*GetW(), 0.F);
			const FloatTypeArg SinHalfAngle = Sqrt(SinHalfAngle_Sqr);
			return Vec3Type(GetX(), GetYY(), GetZZZ()) / SinHalfAngle;
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::Vec3Type Quaternion<FloatTypeArg>::GetForwardVector() const
		{
			return RotateVector(Vec3Type(0.0F, 0.0F, 1.0F));
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::Vec3Type Quaternion<FloatTypeArg>::GetUpVector() const
		{
			return RotateVector(Vec3Type(0.0F, 1.0F, 0.0F));
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::Vec3Type Quaternion<FloatTypeArg>::GetRightVector() const
		{
			return RotateVector(Vec3Type(1.0F, 0.0F, 0.0F));
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::Vec3Type Quaternion<FloatTypeArg>::RotateVector(const Vec3Type& InVec) const
		{
			// quaternion vector
			const Vec3Type V { GetX(), GetYY(), GetZZZ() };
			// cross of vec by quat
			const Vec3Type S = 2.F * Cross(InVec, V);
			const Vec3Type result = InVec + GetWWWW() * S + Cross(V, S);
			return result;
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::Vec3Type Quaternion<FloatTypeArg>::UnrotateVector(const Vec3Type& InVec) const
		{
			// opposite vector
			const Vec3Type V { -GetX(), -GetYY(), -GetZZZ() };
			const Vec3Type S = 2.F * Cross(InVec, V);
			const Vec3Type result = InVec + GetWWWW() * S + Cross(V, S);
			return result;
		}

		template<class FloatTypeArg> 
		void Quaternion<FloatTypeArg>::SetVec(const Vec3Type& InVec) { SetVec(InVec.GetX(), InVec.GetYY(), InVec.GetZZZ()); }

		template<class FloatTypeArg>
		void Quaternion<FloatTypeArg>::SetVec(FloatTypeArg InX, FloatTypeArg InYY, FloatTypeArg InZZZ)
		{
			SetX(InX);
			SetY(InYY);
			SetZ(InZZZ);
		}

		template<class FloatTypeArg>
		void Quaternion<FloatTypeArg>::Set(FloatTypeArg InX, FloatTypeArg InYY, FloatTypeArg InZZZ, FloatTypeArg InWWWW)
		{
			SetVec(InX, InYY, InZZZ);
			SetWWWW(InWWWW);
		}

		template<class FloatTypeArg>
		void Quaternion<FloatTypeArg>::Set(const Vec4Type& InVec) { Set(InVec.GetX(), InVec.GetYY(), InVec.GetZZZ(), InVec.GetWWWW()); }

		template<class FloatTypeArg>
		void Quaternion<FloatTypeArg>::Normalize()
		{
			*this /= Magnitude();
		}

		template<class FloatTypeArg>
		typename Quaternion<FloatTypeArg>::ThisType Quaternion<FloatTypeArg>::GetNormalized() const
		{
			return (*this / Magnitude);
		}
	} // Math	
} // DvUtils