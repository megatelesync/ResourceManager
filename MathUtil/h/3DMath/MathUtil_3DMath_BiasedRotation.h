#pragma once

#include "../MathTypes/Orientation/MathUtil_MathTypes_Quaternion.h"
#include "../MathTypes/BLAS/MathUtil_BLAS.h"
//#include "../MathUtil_VectorBaseTypes.h"
//#include "../MathUtil_MatrixBaseTypes.h"

namespace DvUtils
{
	namespace Math
	{
		/**************************************************************
		* TODO:		
		* 1. BiasedRotation<FloatTypeArg>::SetBiasedRotation
		* 1.1. BiasedRotationCache<FloatTypeArg>::SetBiasedRotation +
		* 1.2. CalcMatrix(pOutMatrix, const BiasedRotation<FloatTypeArg>& InBiasedRotation) +
		* 1.2.1. CalcMatrix						(pOutMatrix, const BiasedRotation<FloatTypeArg>& InBiasedRotation) +
		* 1.2.2. CalcSubmatrix33FromQuaternion	(pOutMatrix, const Quaternion<FloatTypeArg>* InQuaternion); +
		* 1.2.3. BiasNormalizedMatrix44			(pOutMatrix, const Vector<FloatTypeArg, 3> InPosition) +		
		* 1.2. BiasedRotationCache<FloatTypeArg>::_CalcDetAndInverseMatrix() +
		* 2. BiasedRotationCache<FloatTypeArg>::SetPosition()+
		* 3. BiasedRotationCache<FloatTypeArg>::SetRot(const QuaternionType& InValue) +
		**************************************************************/

		/***************************************************************
		* Biased rotation:
		* Amalgamates the rotation and bias transformations
		****************************************************************/
		template<class FloatTypeArg>
		class BiasedRotation
		{
		public:
			/********************************************************************
			* Typedefs
			********************************************************************/
			typedef FloatTypeArg											FloatType;
			typedef typename Quaternion<FloatTypeArg>						QuaternionType;
			typedef typename Vec<FloatTypeArg, 3>							Vec3Type;

			/********************************************************************
			* Construction
			********************************************************************/
			// default ctor: initializes the rotation with identity quaternion
			// and position with ZERO
			BiasedRotation();
			BiasedRotation(const Vec3Type& InBias, const QuaternionType& InQuaternion);

			/********************************************************************
			* Getters
			********************************************************************/
			const Vec3Type&									GetBias					()						const { return _bias; }
			const QuaternionType&							GetQuaternion			()						const { return _quaternion; }

			/********************************************************************
			* Setters
			********************************************************************/
			void											SetBias					(const Vec3Type& InValue);
			void											SetQuaternion			(const QuaternionType& InValue);

			/********************************************************************
			* Helper setters
			********************************************************************/
			void											SetBias					(FloatTypeArg InX, FloatTypeArg InY, FloatTypeArg InZ);

		private:
			Vec3Type										_bias;
			QuaternionType									_quaternion;
		};

		// Calculate the 44-matrix for the given biased rotation
		template<class FloatTypeArg>
		void CalcMatrix(SquareMatrix<FloatTypeArg, 4> *pOutMatrix, const BiasedRotation<FloatTypeArg>& InBiasedRotation);			

		/****************************************************************************************
		* Contains the biased rotation its precalculated data.
		* The precalculated data includes the following:
		* 1. "forward-transform" matrix
		* 2. "inverse-transform" matrix
		* 3. "determinant of forward-transform matrix"
		*
		* Construction:
		*		Default ctor initializes the instance with the following state:
		*		1. Position is initialize with ZERO vector
		*		2. Rotation quaternion is initilized with the identity quaternion (0,0,0,W=1)
		****************************************************************************************/
		template<class FloatTypeArg>
		class BiasedRotationCache
		{
		public:
			/**********************************************************************************
			* Typedefs
			**********************************************************************************/
			typedef FloatTypeArg											FloatType;
			typedef typename BiasedRotation<FloatTypeArg>					BiasedRotationType;
			typedef typename SquareMatrix<FloatTypeArg, 4>					MatrixType;
			typedef typename BiasedRotationType::Vec3Type					Vec3Type;
			typedef typename BiasedRotationType::QuaternionType				QuaternionType;

			/**********************************************************************************
			* Getters
			**********************************************************************************/
			const BiasedRotationType&						GetBiasedRotation				()			const			{ return _biased_rotation; }			
			const MatrixType&								GetMatrix						()			const			{ return _matrix; }
			const MatrixType&								GetInverseMatrix				()			const			{ return _inverseMatrix; }
			// Get determinant of the "forward-transform" matrix:
			FloatType										GetDet_Matrix					()			const			{ return _det; }

			/**********************************************************************************
			* Setters
			**********************************************************************************/
			void											SetBiasedRotation				(const BiasedRotationType& InValue);
			void											SetPosition						(const Vec3Type& InValue);
			void											SetRot							(const QuaternionType& InValue);

		private:
			// calculate det and both of the matrices ("forward-transform" and "inverse-transform" matrices)
			// (the calculatable values are to be calculated based on the _biased_rotation state)
			void											 _CalcDetAndBothMatrices();
			// calculate det and inverse matrix based on the calculated "forward-transform" matrix 
			// (the "forward-transform" matrix is stored in the field named "_matrix");
			void											_CalcDetAndInverseMatrix();

			BiasedRotationType								_biased_rotation;
			// "forward-transform" matrix
			MatrixType										_matrix;
			// "inverse-transform" matrix
			MatrixType										_inverseMatrix;
			// Determinant of the "forward-transform" matrix
			FloatTypeArg									_det;
		};

		/*****************************************************************************************
		* BiasedRotationCache IMPLEMENTATION
		*****************************************************************************************/	
		template<class FloatTypeArg>
		void BiasedRotationCache<FloatTypeArg>::SetBiasedRotation(const BiasedRotationType& InValue)
		{
			_biased_rotation = InValue;
			_CalcDetAndBothMatrices();
		}

		template<class FloatTypeArg>
		void BiasedRotationCache<FloatTypeArg>::SetPosition(const Vec3Type& InValue)
		{
			assert(&InValue);
			_biased_rotation.SetBias(InValue);
			// OPTIMIZE HINT: Here we could update only bias of the forward matrix
			// and the "inverse-transform" matrix and det
			_CalcDetAndBothMatrices();
		}

		template<class FloatTypeArg>
		void BiasedRotationCache<FloatTypeArg>::SetRot(const QuaternionType& InValue)
		{
			assert(&InValue);
			_biased_rotation.SetQuaternion(InValue);
			// OPTIMIZE HINT: Here we could update only 3x3 rotation submatrix
			// of the forward matrix and the "inverse-transform" matrix and det
			_CalcDetAndBothMatrices();
		}

		template<class FloatTypeArg>
		void BiasedRotationCache<FloatTypeArg>::_CalcDetAndBothMatrices()
		{
			CalcMatrix(&_matrix, _biased_rotation);
			_CalcDetAndInverseMatrix();
		}

		template<class FloatTypeArg>
		void BiasedRotationCache<FloatTypeArg>::_CalcDetAndInverseMatrix()
		{
			_det = Det(_matrix);
			InverseCopy_UsePrecomputedDet(&_inverseMatrix, &_matrix, _det);
		}

		/*****************************************************************************************
		* BiasedRotation FUNCTIONS IMPLEMENTATION
		*****************************************************************************************/		
		template<class FloatTypeArg>
		void CalcMatrix(SquareMatrix<FloatTypeArg, 4> *pOutMatrix, const BiasedRotation<FloatTypeArg>& InBiasedRotation)
		{
			assert(pOutMatrix);
			assert(&InBiasedRotation);			
			// 1. calculate rotation matrix from quaternion
			CalcSubmatrix33FromQuaternion(pOutMatrix, InBiasedRotation.GetQuaternion());
			// 2. bias matrix			
			// WARNING!!! We Should NOT bias in opposite direction, NO minus should be here!!!
			BiasNormalizedMatrix44(pOutMatrix, InBiasedRotation.GetBias());
		}

		/*****************************************************************************************
		* BiasedRotation IMPLEMENTATION
		*****************************************************************************************/		
		template<class FloatTypeArg>
		BiasedRotation<FloatTypeArg>::BiasedRotation()
		{
		}

		template<class FloatTypeArg>
		BiasedRotation<FloatTypeArg>::BiasedRotation(const Vec3Type& InBias, const QuaternionType& InQuaternion) :
			_bias(InBias),
			_quaternion(InQuaternion)
		{
		}			

		/********************************************************************
		* Setters
		********************************************************************/
		
		template<class FloatTypeArg>
		void BiasedRotation<FloatTypeArg>::SetBias(const Vec3Type& InValue)
		{
			_bias = InValue;
		}		

		
		template<class FloatTypeArg>
		void BiasedRotation<FloatTypeArg>::SetQuaternion(const QuaternionType& InValue)
		{
			_quaternion = InValue;
		}
		
		template<class FloatTypeArg>
		void BiasedRotation<FloatTypeArg>::SetBias(FloatTypeArg InX, FloatTypeArg InY, FloatTypeArg InZ)
		{
			_bias.Set(InX, InY, InZ);
		}		
	} // Math
} // DvUtils