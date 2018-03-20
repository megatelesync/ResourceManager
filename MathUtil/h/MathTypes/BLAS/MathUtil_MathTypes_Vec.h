#pragma once

#include <array>
#include "../../Core/MathUtil_Core_LinearAlgebraUtils.h"

namespace DvUtils
{
	namespace Math
	{		
		/**************************************************************************
		* Generalized vector class		
		*
		* Description
		*		This class is the inheritable wrapper over the std::array.
		*		The reasons to have this class:
		*			- one should not inherit from the std::array;
		*			- std::array have no template argument typedefs		
		*
		* Template arguments:
		*		FloatTypeArg - type of the component
		*		NumElemsArg  - number of the elements of the vector
		*		
		*
		* Tester:
		*		1) See the MathUtilDemo tester
		*		2) See the MathUtil tester in the TestDemo/Vector/MathUtil_Vector.cpp
		*
		* TODO GOALS:
		* 1. Add the assert guard to all functions
		* 2. Add the extra vector operations (see the extra operations sections 
		*    in the IMPLEMENTATION)
		* 3. Add the constructors from vector and constructors from array of the same size
		* 4. Operations for conversion to the XM-types.
		* 5. Add constants
		* 5.1. ort  vectors (x, y, z)
		*
		* IMPLEMENTATION:
		* 1. Member functions
		* 1.1. Construction
		* 1.1.1. Default construction (DEMONSTRATED: 17.05.2016/13:59)
		* 1.1.2. Construction from explicit arguments
		* 1.1.2.1. TO BE IMPL: depends on the Reset function implementation
		* 1.1.3. Construct from the given std::array		(DEMONSTRATED: 20.05.2016/16:29)		
		* 1.1.4. Construct from the given src vector		(DEMONSTRATED: 20.05.2016/16:42)		
		* 1.1.5. Construct from memory						(DEMONSTRATED: 22.05.2016/12:55)
		* 1.1.6. Construct from range
		* 1.1.7. Copy construction and assignment
		* 1.1.7.1. Copy construction
		* 1.1.7.2. Assignment
		* 2. Non-member functions
		* 2.1. Setters
		* 2.1.1. Reset function
		* 2.1.1.1. To be implemented
		* 3.1. SetComponent functions:
		* 3.1.1. SetX (DEMOSTRATED: 23.05.2016/8:59)
		* 3.1.2. SetYY (DEMOSTRATED: 23.05.2016/8:59)
		* 3.1.3. SetZZZ (DEMOSTRATED: 23.05.2016/8:59)
		* 3.1.4. SetWWWW (DEMOSTRATED: 23.05.2016/8:59)
		* 3.2. SetPart functions
		* 3.2.1. SetPart2 (DEMOSTRATED: 23.05.2016/9:15)
		* 3.2.2. SetPart3 (DEMOSTRATED: 23.05.2016/9:15)
		* 3.2.3. SetPart4 (DEMOSTRATED: 23.05.2016/9:15)
		* 2.2. Compare functions
		* 2.2.1. operator== (DEMONSTRATED: 22.05.2016/12:00)
		* 2.2.2. operator!=  (DEMONSTRATED: 22.05.2016/12:00)
		* 2.2.3. IsZero (DEMONSTRATED: 22.05.2016/12:00)
		* 2.2.4. IsNearEqual (DEMONSTRATED: 22.05.2016/12:00)
		* 3. Math functions
		* 3.1. Simple arithmetic operations
		* 3.1.1. Multiplying vector by scalar
		* 3.1.1.1. Assign multiply by scalar:
		* 3.1.1.1.1. Assign multiply by int scalar			(DEMONSTRATED:20.05.2016/14:25)
		* 3.1.1.1.2. Assign multiply by float scalar		(DEMONSTRATED:20.05.2016/14:25)
		* 3.1.1.1.3. Assign multiply by double scalar		(DEMONSTRATED:20.05.2016/14:25)
		* 3.1.1.2. Multiply by scalar:
		* 3.1.1.2.1. Multiply by scalar right
		* 3.1.1.2.1.1. (DEMONSTRATED: 20.05.2016/14:37)
		* 3.1.1.2.2. Multiply by scalar left
		* 3.1.1.2.2.1 (DEMONSTRATED: 20.05.2016/14:48)
		* 3.1.1.2.1.2. 
		* 3.1.2. Dividing vector by scalar
		* 3.1.2.1 Assign divide vector by scalar
		* 3.1.2.1.1. Assign divide by int scalar			(DEMONSTRATED:20.05.2016/14:25)
		* 3.1.2.1.2. Assign divide by float scalar		(DEMONSTRATED:20.05.2016/14:25)
		* 3.1.2.1.3. Assign divide by double scalar		(DEMONSTRATED:20.05.2016/14:25)
		* 3.1.2.2. Divide by scalar	(DEMONSTRATED: 20.05.2016/14:53)		
		* 3.1.3. Negating vector: (DEMONSTRATED: 20.05.2016/14:48)		
		* 3.1.4. Arithmetic operations on vectors of different size
		* 3.1.4.1. Adding:
		* 3.1.4.1.1. Adding from memory to (DEMONSTRATED: 23.05.2016/9:59)
		* 3.1.4.1.2. Adding from vector to
		* 3.1.4.1.3. Setting adding from memory (DEMONSTRATED: 23.05.2016/11:14)
		* 3.1.4.2. Subtracting:
		* 3.1.4.2.1. Subtracting from memory from (DEMONSTRATED: 23.05.2016/11:36)
		* 3.1.4.2.2. Subtracting from vector from
		* 3.1.4.2.3. Setting subtracting from memory from (DEMONSTRATED: 23.05.2016/11:48)

		
		* 3.2. Other operations
		* 3.2.1. SqrLength (DEMONSTRATED: 20.05.2016/13:59)
		* 3.2.2. Length  (DEMONSTRATED: 20.05.2016/13:59)
		* 3.2.3. Normalize (DEMONSTRATED: 22.0.5.2016/12:13)
		* 3.2.4. Dot:
		* 3.2.4.1. Dot for vectors of different types but the same size 
		*		   (DEMONSTRATED: 20.05.2016/13:41)
		* 3.2.4.2. Dot2 (+CODED)
		* 3.2.4.3. Dot3 (+CODED)
		* 3.2.4.4. Dot4 (+CODED)
		* 3.3. New operations:
		* 3.4. Cross3 (DEMONSTRATED: 23.05.2016/20:36)
		* 4. Access functions
		* 4.1. Getters
		* 4.1.1. GetX					(DEMONSTRATED: 23.05.2016/9:06)
		* 4.1.2. GetYY					(DEMONSTRATED: 23.05.2016/9:06)
		* 4.1.3. GetZZZ					(DEMONSTRATED: 23.05.2016/9:06)
		* 4.1.4. GetWWWW				(DEMONSTRATED: 23.05.2016/9:06)
		* 5. Extra operations:
		* 5.1. Set to n
		* 5.2. Swizzle				
		**************************************************************************/
		template<class FloatTypeArg, size_t NumElemsArg>
		class Vec
		{
		public:
			/************************************************************************************
			* Typedefs
			************************************************************************************/
			typedef FloatTypeArg									FloatType;
			typedef std::array<FloatTypeArg, NumElemsArg>			ArrayType;
			typedef Vec<FloatType, NumElemsArg>						ThisType;

			/************************************************************************************
			* static functions
			************************************************************************************/
			// Size of one component
			static size_t				GetComponentSize    ();

			/************************************************************************************
			* ctors
			************************************************************************************/
			// default ctor (creates zero vector)
			Vec												();

			// 
			// constructors from a set of component
			//
			// Should only have a constructor from the pair of the components:
			// 1. Providing constructor for creating a vector of one component is ambigious-looking
			//    - should we fill all the components of the vector by replicating the given value or only the first?
			// 2. We should provide the constructors of 3 or 4 components 
			//    Because we do not implement the 3-compoent and 4-component vectors as derived classes.
			Vec												(const FloatTypeArg& InX, const FloatTypeArg& InYY);
			Vec												(const FloatTypeArg& InX, const FloatTypeArg& InYY, const FloatTypeArg& InZZZ);
			Vec												(const FloatTypeArg& InX, const FloatTypeArg& InYY, const FloatTypeArg& InZZZ, const FloatTypeArg& InWWWW);

			// Copy from the given std::array specified number of elems
			template<class InSrcType, size_t InSrcNumComponents>
			Vec												
			(
				const std::array<InSrcType, InSrcNumComponents>&	 InSrcArray, 
				int													 InNumElems				
			);

			// Copy from the given Vec
			template<class InSrcType>
			explicit Vec												
			(
				const Vec<InSrcType, NumElemsArg>&			 InSrcVec				
			);

			// Copy from the given Vec
			template<class InSrcType, size_t InSrcNumComponents>
			Vec												
			(
				const Vec<InSrcType, InSrcNumComponents>&			 InSrcVec, 
				int													 InNumElems				
			);

			// Copy from the given array
			template<class InSrcType>
			Vec												
			(				
				const InSrcType				*pInSrc, 
				int							 InNumElems,				
				size_t					     InByteStride = sizeof(InSrcType)
			);

			// Copy from the given range
			template<class InSrcIteratorType>
			Vec												(const InSrcIteratorType &InStartIterator, const InSrcIteratorType &InEndIterator);

			/************************************************************************************
			* Copying and copy-construction
			************************************************************************************/			
			Vec												(const ThisType& InSrc);
			ThisType&				operator=				(const ThisType& InSrc);			


			/************************************************************************************
			* Num elements
			************************************************************************************/
			size_t					GetNumElems				()						const				{ return NumElemsArg; }						
			
			/************************************************************************************
			* Getters
			************************************************************************************/
			const ArrayType&		GetArray				()						const				{ return _arr; }
			ArrayType&				GetArray				()											{ return _arr; }

			/************************************************************************************
			* Helper getters
			************************************************************************************/
			FloatTypeArg            GetX                    () const                                    { return _arr[0]; }
			FloatTypeArg            GetYY                   () const                                    { return _arr[1]; }
			FloatTypeArg            GetZZZ                  () const                                    { return _arr[2]; }
			FloatTypeArg            GetWWWW                 () const                                    { return _arr[3]; }

			/************************************************************************************
			* Helper setters
			************************************************************************************/
			void                    SetX                    (FloatTypeArg InValue) { _arr[0] = InValue; }
			void                    SetYY                   (FloatTypeArg InValue) { _arr[1] = InValue; }
			void                    SetZZZ                  (FloatTypeArg InValue) { _arr[2] = InValue; }
			void                    SetWWWW                 (FloatTypeArg InValue) { _arr[3] = InValue; }

			void                    Set                     (FloatTypeArg InX, FloatTypeArg InYY);
			void                    Set                     (FloatTypeArg InX, FloatTypeArg InYY, FloatTypeArg InZZZ);
			void                    Set                     (FloatTypeArg InX, FloatTypeArg InYY, FloatTypeArg InZZZ, FloatTypeArg InWWWW);
			
		private:			
			ArrayType				_arr;
		};		

		template<class FloatTypeArg, size_t NumElemsArg>
		void Vec<FloatTypeArg, NumElemsArg>::Set(FloatTypeArg InX, FloatTypeArg InYY)
		{
			_arr[0] = InX;
			_arr[1] = InYY;
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		void Vec<FloatTypeArg, NumElemsArg>::Set(FloatTypeArg InX, FloatTypeArg InYY, FloatTypeArg InZZZ)
		{
			_arr[0] = InX;
			_arr[1] = InYY;
			_arr[2] = InZZZ;
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		void Vec<FloatTypeArg, NumElemsArg>::Set(FloatTypeArg InX, FloatTypeArg InYY, FloatTypeArg InZZZ, FloatTypeArg InWWWW)
		{
			_arr[0] = InX;
			_arr[1] = InYY;
			_arr[2] = InZZZ;
			_arr[3] = InWWWW;
		}

		/************************************************************************************
		* Low-level access
		************************************************************************************/				
		// Get const pointer to the first component of the given vector
		//template<class FloatTypeArg, int NumElemsArg>
		//const FloatTypeArg *GetPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec);

		// DESCRIPTION:
		//		Get pointer to the first component of the given vector
		// WARNING:
		//		This function is commented because ever const reference parameter is incorrectly deduced
		// for this non-const reference function.
		//template<class FloatTypeArg, int NumElemsArg>
		//FloatTypeArg *GetPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec);

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetPointer(const Vec<FloatTypeArg, NumElemsArg> *InVec);

		// Get pointer to the first component of the given vector
		template<class FloatTypeArg, int NumElemsArg>
		FloatTypeArg *GetPointer(Vec<FloatTypeArg, NumElemsArg> *InVec);

		// Get pointer to the given component of the given vector
		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetComponentPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec, int InIndex);

		template<class FloatTypeArg, int NumElemsArg>		
		FloatTypeArg *GetComponentPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec, int InIndex);

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetXPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec);

		template<class FloatTypeArg, int NumElemsArg>		
		FloatTypeArg *GetXPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec);

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetYYPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec);

		template<class FloatTypeArg, int NumElemsArg>		
		FloatTypeArg *GetYYPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec);

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetZZZPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec);

		template<class FloatTypeArg, int NumElemsArg>		
		FloatTypeArg *GetZZZPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec);

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetWWWWPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec);

		template<class FloatTypeArg, int NumElemsArg>		
		FloatTypeArg *GetWWWWPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec);

		/************************************************************************************
		* String representation
		************************************************************************************/
		template<class FloatTypeArg, int NumElemsArg>
		std::string	ToDebugString				(const Vec<FloatTypeArg, NumElemsArg>&	InVec, int InPrecisionDigs = 6);

		template<class FloatTypeArg, int NumElemsArg>
		std::string	ToString					(const Vec<FloatTypeArg, NumElemsArg>&	InVec, int InPrecisionDigs = 6);

		/************************************************************************************
		* Stream output
		************************************************************************************/
		template<class StrmType, class FloatTypeArg, int NumElemsArg>
		StrmType& operator<<(StrmType& oStrm, const Vec<FloatTypeArg, NumElemsArg>& InVec)
		{
			return oStrm << ToString(InVec);
		}

		/************************************************************************************
		* Copying and setting
		************************************************************************************/
		// Make the given vector elements in range [InFirstElem, InFirstElem + numComponents) zero
		template<class DestFloatTypeArg, size_t DestNumElemsArg>		
		void MkZero_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			int												 numComponents,			
			int												 InFirstElem			= 0
		);

		template<class DestFloatTypeArg, size_t DestNumElemsArg>		
		void MkZero_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec
		);

		// Set the given range of components [InFirstElem, InFirstElem + numComponents) 
		// of the given vector to the given value
		// By default: sets ALL components of the given vector
		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Set_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 componentValue, 
			int												 numComponents			= pVec->GetNumElems(),			
			int												 InFirstElem			= 0
		);		

		// Resets the given vector:
		// Sets the first two components and sets to zero others
		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void Reset2
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,			
			int												 InFirstElem			= 0
		);

		// Resets the given vector:
		// Sets the first three components and sets to zero others
		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void Reset3
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,			
			DestFloatTypeArg								 Z,			
			int												 InFirstElem			= 0
		);

		// Resets the given vector:
		// Sets the first four components and sets to zero others
		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void Reset4
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,			
			DestFloatTypeArg								 Z,			
			DestFloatTypeArg								 W,	
			int												 InFirstElem			= 0
		);

		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void SetPart2
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,			
			int												 InFirstElem			= 0
		);

		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void SetPart3
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,
			DestFloatTypeArg								 Z,					
			int												 InFirstElem			= 0
		);

		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void SetPart4
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,
			DestFloatTypeArg								 Z,
			DestFloatTypeArg								 W,					
			int												 InFirstElem			= 0
		);

		// Copying from the given src array to the given dest vector
		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg, int SrcNumElemsArg>
		void CopyCast_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pDestVec,
			const Vec<SrcFloatTypeArg, SrcNumElemsArg>		&InSrcVec,
			int												 InNumElems					= InSrcArray.size(), 
			int												 InFirstSrcElem				= 0
		);
		
		// Copying from the given src vector to the given dest vector
		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg, int SrcNumElemsArg>
		void CopyCast_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>					*pDestVec,
			const std::array<SrcFloatTypeArg, SrcNumElemsArg>		&InSrcVec,
			int														 InNumElems			= InSrcVec.size(), 
			int														 InFirstSrcElem		= 0
		);

		// Copying from the given memory (provided by the pointer) to the given dest vector
		template<class DestFloatTypeArg, int DestNumElemsArg, class InSrcType, size_t InSrcNumComponents>		
		void CopyCast_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>					*pDestVec,
			const InSrcType											*pInSrc, 
			size_t													 InByteStride        = sizeof(InSrcType)
		);		

		/************************************************************************************
		* Equality testing
		************************************************************************************/
		// Test for strict equality
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t NumElemsArgs>
		bool operator==									
		(
			const Vec<DestFloatTypeArg, NumElemsArgs>&			InDestVec,
			const Vec<SrcFloatTypeArg, NumElemsArgs>&			InSrcVec
		);
		
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t NumElemsArgs>
		bool operator!=									
		(
			const Vec<DestFloatTypeArg, NumElemsArgs>&			InDestVec,
			const Vec<SrcFloatTypeArg, NumElemsArgs>&			InSrcVec
		);

		// Is the given vector STRICTLY zero
		template<class DestFloatTypeArg, size_t DestNumElemsArg>
		bool IsZero
		(
			const Vec<DestFloatTypeArg, DestNumElemsArg>&				InDest 
		);

		// Is the given vector nearly zero
		template<class DestFloatTypeArg, size_t DestNumElemsArg>
		bool IsNearZero
		(
			const Vec<DestFloatTypeArg, DestNumElemsArg>&				InDestVec,
			DestFloatTypeArg											InEps
		);


		// Test for near-equality
		// The epsilon type is taken from the src vec parameter
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t NumElemsArg>
		bool IsNearEqual
		(
			const Vec<SrcFloatTypeArg, NumElemsArg>&				InDest, 
			const Vec<DestFloatTypeArg,NumElemsArg>&				InSrc, 			
			SrcFloatTypeArg											InEps
		);		

		/************************************************************************************
		* Component read-only access functions
		************************************************************************************/
		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg&	GetComponent
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec, 
			int												InIndex
		);		
		
		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg& GetX
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec
		);

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg& GetYY
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec
		);

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg& GetZZZ
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec
		);

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg& GetWWWW
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec
		);

		/************************************************************************************
		* Component set functions
		************************************************************************************/
		template<class FloatTypeArg, int NumElemsArg>
		void SetComponent
		(
			Vec<FloatTypeArg, NumElemsArg>					*pVec, 
			int												InIndex,
			FloatTypeArg									InValue
		);		
		
		template<class FloatTypeArg, int NumElemsArg>
		void SetX
		(
			Vec<FloatTypeArg, NumElemsArg>					*pVec,
			FloatTypeArg									InValue
		);

		template<class FloatTypeArg, int NumElemsArg>
		void SetYY
		(
			Vec<FloatTypeArg, NumElemsArg>				   *pVec,
			FloatTypeArg									InValue
		);

		template<class FloatTypeArg, int NumElemsArg>
		void SetZZZ
		(
			Vec<FloatTypeArg, NumElemsArg>				   *pVec,
			FloatTypeArg									InValue
		);

		template<class FloatTypeArg, int NumElemsArg>
		void SetWWWW
		(
			Vec<FloatTypeArg, NumElemsArg>				   *pVec,
			FloatTypeArg									InValue
		);


		/*************************************************************************************
		* Math functions
		*************************************************************************************/	
		template<class FloatTypeArg, int NumElemsArg>
		FloatTypeArg							SqrLength
		(
			const Vec<FloatTypeArg,NumElemsArg>					&InVec 			
		);

		template<class FloatTypeArg, int NumElemsArg>
		FloatTypeArg							Length
		(
			const Vec<FloatTypeArg,NumElemsArg>					&InVec 			
		);

		template<class FloatTypeArg, int NumElemsArg>
		FloatTypeArg                            SqrDistance
		(
			const Vec<FloatTypeArg, NumElemsArg>&               InA,
			const Vec<FloatTypeArg, NumElemsArg>&               InB
		);

		template<class FloatTypeArg, int NumElemsArg>
		FloatTypeArg                            Distance
		(
			const Vec<FloatTypeArg, NumElemsArg>&               InA,
			const Vec<FloatTypeArg, NumElemsArg>&               InB
		);

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg,NumElemsArg>		   &Normalize
		(
			Vec<FloatTypeArg,NumElemsArg>						&InVec 			
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg, int NumElemsArg>
		DestFloatTypeArg Dot
		(
			const Vec<DestFloatTypeArg,NumElemsArg>					&InVec, 
			const Vec<SrcFloatTypeArg,NumElemsArg>					&InVecTwo
		);


		// Dot2: Takes maximum TWO components from each vector 
		// WARNING!!! Each vector must contain minimum Two components!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot2
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot2
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		);



		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot2
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot2
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot2
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		);


		// Dot3: Takes only three first components from the second vector
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot3
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		);

		// Dot3: Takes only three first components from the second vector
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot3
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		);

		// Dot3: Takes only three first components from the second vector
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot3
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		);

		// Dot3: Takes only three first components from the second vector
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot3
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		);

		// Dot3: Takes only three first components from the second vector
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot3
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		);

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ONE!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wOne
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		);

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ONE!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wOne
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		);

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ONE!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wOne
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wOne
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		);

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ONE!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wOne
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		);

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ZERO!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wZero
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		);

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ZERO!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wZero
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		);

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ONE!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wZero
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		);

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wZero
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		);

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ZERO!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wZero
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		);

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator-					
		(
			const Vec<FloatTypeArg, NumElemsArg>& InVec
		);

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator+					
		(
			const Vec<FloatTypeArg, NumElemsArg>& InFirst,
			const Vec<FloatTypeArg, NumElemsArg>& InSecond
		);

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator-					
		(
			const Vec<FloatTypeArg, NumElemsArg>& InFirst,
			const Vec<FloatTypeArg, NumElemsArg>& InSecond
		);

		template<class FloatTypeArg, class CoeffFloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator*					
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec,
			CoeffFloatTypeArg								InFactor
		);

		template<class FloatTypeArg, class CoeffFloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator*					
		(
			CoeffFloatTypeArg								InFactor,
			const Vec<FloatTypeArg, NumElemsArg>&			InVec			
		);

		template<class FloatTypeArg, class CoeffFloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator/					
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec,
			CoeffFloatTypeArg 								InFactor
		);

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>&		operator+=					
		(
			Vec<FloatTypeArg, NumElemsArg>					  &InVec,
			const Vec<FloatTypeArg, NumElemsArg>	          &InVecTwo
		);

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>&		operator-=					
		(
			Vec<FloatTypeArg, NumElemsArg>					  &InVec,
			const Vec<FloatTypeArg, NumElemsArg>			  &InVecTwo
		);

		template<class FloatTypeArg, class CoeffFloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>&		operator*=					
		(
			Vec<FloatTypeArg, NumElemsArg>					  &InVec,
			CoeffFloatTypeArg								   InFactor
		);

		template<class FloatTypeArg, class CoeffFloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>&		operator/=					
		(
			Vec<FloatTypeArg, NumElemsArg>					  &InVec,
			CoeffFloatTypeArg								   InFactor
		);

		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, DestNumElemsArg>		*AddMemoryVectorTo
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>				*pDestVec,
			const SrcFloatTypeArg								*pSrcVec,
			int													 InNumComponents,
			int													 InStartDestIndex = 0,
			size_t												 srcComponentStride = sizeof(SrcFloatTypeArg)										 
		);

		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, DestNumElemsArg>		*SubtractMemoryVectorFrom
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>				*pDestVec,
			const SrcFloatTypeArg								*pSrcVec,
			int													 InNumComponents,
			int													 InStartDestIndex = 0,
			size_t												 srcComponentStride = sizeof(SrcFloatTypeArg)										 
		);

		// Set the sum of vectors to the given vector 
		// components with indices in range [InStartDestIndex, InStartDestIndex + InNumComponents)
		// (without touching other components)
		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg, class SrcFloatTypeArgTwo>
		Vec<DestFloatTypeArg, DestNumElemsArg>		*SetAddMemoryVector
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>				*pDestVec,
			const SrcFloatTypeArg								*pSrcVecOne,
			const SrcFloatTypeArgTwo							*pSrcVecTwo,
			int													 InNumComponents,
			int													 InStartDestIndex = 0,
			size_t												 srcComponentOneStride = sizeof(SrcFloatTypeArg),									 
			size_t												 srcComponentTwoStride = sizeof(SrcFloatTypeArgTwo)										 
		);

		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg, class SrcFloatTypeArgTwo>
		Vec<DestFloatTypeArg, DestNumElemsArg>		*SetSubtractMemoryVector
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>				*pDestVec,
			const SrcFloatTypeArg								*pSrcVecOne,
			const SrcFloatTypeArgTwo							*pSrcVecTwo,
			int													 InNumComponents,
			int													 InStartDestIndex = 0,
			size_t												 srcComponentOneStride = sizeof(SrcFloatTypeArg),										 
			size_t												 srcComponentTwoStride = sizeof(SrcFloatTypeArgTwo)										 
		);

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>&		Negate						
		(
			Vec<FloatTypeArg, NumElemsArg>					*pVec
		);

		/***********************************************************************************
		* Vec3 functions
		************************************************************************************/
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg,3> Cross
		(
			const Vec<DestFloatTypeArg,3>&		InFirst,
			const Vec<SrcFloatTypeArg,3>&		InSecond
		);

		
		/************************************************************************************
		* IMPLEMENTATION
		************************************************************************************/
		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg&	GetComponent
		(
			const Vec<FloatTypeArg, NumElemsArg>& InVec, 
			int InIndex
		) 
		{
			return InVec.GetArray()[InIndex]; 
		}

		//template<class FloatTypeArg, int NumElemsArg>
		//const FloatTypeArg *GetPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec)
		//{
		//	return InVec.GetArray().data();
		//}

		//template<class FloatTypeArg, int NumElemsArg>
		//FloatTypeArg *GetPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec)
		//{
		//	return InVec.GetArray().data();
		//}

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetPointer(const Vec<FloatTypeArg, NumElemsArg> *InVec)
		{
			assert(InVec != nullptr);
			return InVec->GetArray().data();
		}
		
		template<class FloatTypeArg, int NumElemsArg>
		FloatTypeArg *GetPointer(Vec<FloatTypeArg, NumElemsArg> *InVec)
		{
			assert(InVec != nullptr);
			return InVec->GetArray().data();
		}

		template<class FloatTypeArg, int NumElemsArg>
		std::string	ToDebugString (const Vec<FloatTypeArg, NumElemsArg>&	InVec, int InPrecisionDigs)
		{
			return VectorToDebugString(GetPointer(&InVec), InVec.GetNumElems(), InVec.GetComponentSize(), InPrecisionDigs);
		}

		template<class FloatTypeArg, int NumElemsArg>
		std::string	ToString (const Vec<FloatTypeArg, NumElemsArg>&	InVec, int InPrecisionDigs)
		{
			return VectorToString(GetPointer(&InVec), InVec.GetNumElems(), InVec.GetComponentSize(), InPrecisionDigs);
		}

		template<class DestFloatTypeArg, size_t DestNumElemsArg>		
		void MkZero_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			int												 numComponents,			
			int												 InFirstElem
		)
		{
			Set_all_n(GetPointer(pVec) + InFirstElem * pVec->GetComponentSize(), static_cast<DestFloatTypeArg>(0), numComponents);
		}

		template<class DestFloatTypeArg, size_t DestNumElemsArg>		
		void MkZero_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec
		)
		{			
			MkZero_n(pVec, pVec->GetNumElems(), 0);
		}

		// Set the given range of components [InFirstElem, InFirstElem + numComponents) 
		// of the given vector to the given value
		// By default: sets ALL components of the given vector
		template<class DestFloatTypeArg, int DestNumElemsArg>		
		void Set_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 componentValue, 
			int												 numComponents,	
			int												 InFirstElem
		)
		{
			Set_all_n(GetPointer() + InFirstElem * pVec->GetComponentSize(), 0, numComponents, pVec->GetComponentSize());
		}

		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void SetPart2
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,					
			int												 InFirstElem
		)
		{
			DestFloatTypeArg *pDestStartElem = Util::AddPtr(GetPointer(pVec), InFirstElem * pVec->GetComponentSize());
			Set2(pDestStartElem, X, Y, pVec->GetComponentSize());
		}

		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void SetPart3
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,
			DestFloatTypeArg								 Z,			
			int												 InFirstElem
		)
		{
			DestFloatTypeArg *pDestStartElem = Util::AddPtr(GetPointer(pVec), InFirstElem * pVec->GetComponentSize());
			Set3(pDestStartElem, X, Y, Z, pVec->GetComponentSize());
		}

		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void SetPart4
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,
			DestFloatTypeArg								 Z,
			DestFloatTypeArg								 W,					
			int												 InFirstElem
		)
		{			
			DestFloatTypeArg *pDestStartElem = Util::AddPtr(GetPointer(pVec), InFirstElem * pVec->GetComponentSize());
			Set4(pDestStartElem, X, Y, Z, W, pVec->GetComponentSize());
		}
		
		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void Reset2
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,			
			int												 InFirstElem
		)
		{			
			assert(pVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 2" && (DestNumElemsArg >= 2));
			ResetZeroed2
			(
				GetPointer(pVec), 
				X, Y, 
				pVec->GetNumElems(), 
				pVec->GetComponentSize(), 
				InFirstElem
			);
		}
		
		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void Reset3
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,			
			DestFloatTypeArg								 Z,			
			int												 InFirstElem
		)
		{
			assert(pVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 3" && (DestNumElemsArg >= 3));			
			DestFloatTypeArg *pDestPointer = GetPointer(pVec);
			ResetZeroed3
			(
				pDestPointer, 
				X, Y, Z,
				pVec->GetNumElems(), 
				pVec->GetComponentSize(), 
				InFirstElem
			);
		}
		
		template<class DestFloatTypeArg, int DestNumElemsArg> 
		void Reset4
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pVec,
			DestFloatTypeArg								 X, 
			DestFloatTypeArg								 Y,			
			DestFloatTypeArg								 Z,			
			DestFloatTypeArg								 W,	
			int												 InFirstElem
		)
		{
			assert(pVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 4" && (DestNumElemsArg >= 4));
			ResetZeroed4
			(
				GetPointer(pVec), 
				X, Y, Z, W,
				pVec->GetNumElems(), 
				pVec->GetComponentSize(), 
				InFirstElem
			);
		}

		// Copying from the given src array to the given dest vector
		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg, int SrcNumElemsArg>
		void CopyCast_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>			*pDestVec,
			const Vec<SrcFloatTypeArg, SrcNumElemsArg>		&InSrcVec,
			int												 InNumElems,
			int												 InFirstSrcElem
		)
		{
			CopyCast_n
			(
				GetPointer(pDestVec), 
				GetPointer(&InSrcVec) + InFirstSrcElem * InSrcVec.GetComponentSize(), 
				InNumElems, 
				pDestVec->GetComponentSize(), 
				InSrcVec.GetComponentSize()
			);
		}
		
		// Copying from the given src vector to the given dest vector
		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg, int SrcNumElemsArg>
		void CopyCast_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>					*pDestVec,
			const std::array<SrcFloatTypeArg, SrcNumElemsArg>		&InSrcVec,
			int														 InNumElems,
			int														 InFirstSrcElem
		)
		{
			CopyCast_n
			(
				GetPointer(*pDestVec), 
				InSrcVec.data() + InFirstSrcElem * sizeof(InSrcVec[0]), 
				InNumElems, 
				pDestVec->GetComponentSize(), 
				InSrcVec.GetComponentSize()
			);
		}

		// Copying from the given memory (provided by the pointer) to the given dest vector
		template<class DestFloatTypeArg, int DestNumElemsArg, class InSrcType, size_t InSrcNumComponents>		
		void CopyCast_n
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>					*pDestVec,
			const InSrcType											*pInSrc, 
			size_t													 InByteStride
		)
		{
			CopyCast_n
			(
				GetPointer(*pDestVec), 
				pInSrc, 
				pDestVec->GetNumElems(), 
				pDestVec->GetComponentSize(), 
				pInSrc->GetComponentSize()
			);
		}

		// Test for strict equality
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t NumElemsArgs>
		bool operator==									
		(
			const Vec<DestFloatTypeArg, NumElemsArgs>&			InDestVec,
			const Vec<SrcFloatTypeArg, NumElemsArgs>&			InSrcVec
		)
		{
			return std::equal
			(
				InDestVec.GetArray().cbegin(), 
				InDestVec.GetArray().cend(), 
				InSrcVec.GetArray().cbegin()
			);
		}
		
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t NumElemsArgs>
		bool operator!=									
		(
			const Vec<DestFloatTypeArg, NumElemsArgs>&			InDestVec,
			const Vec<SrcFloatTypeArg, NumElemsArgs>&			InSrcVec
		)
		{
			return !(InDestVec == InSrcVec);
		}

		// Is the given vector STRICTLY zero
		template<class DestFloatTypeArg, size_t DestNumElemsArg>
		bool IsZero
		(
			const Vec<DestFloatTypeArg, DestNumElemsArg>&				InDest 
		)
		{
			for(int i = 0; i < InDest.GetNumElems(); i++)
			{
				if(GetComponent(InDest, i) != 0)
				{
					return false;
				}
			}
			return true;
		}

		// Is the given vector nearly zero
		template<class DestFloatTypeArg, size_t DestNumElemsArg>
		bool IsNearZero
		(
			const Vec<DestFloatTypeArg, DestNumElemsArg>&				InDestVec,
			DestFloatTypeArg											InEps
		)
		{
			for(int i = 0; i < InDestVec.GetNumElems(); i++)
			{
				DestFloatTypeArg component = GetComponent(InDestVec, i);
				if(component > InEps || component < -InEps)
				{
					return false;
				}
			}
			return true;
		}

		// Test for near-equality
		// The epsilon type is taken from the src vec parameter
		template<class DestFloatTypeArg, class SrcFloatTypeArg, size_t NumElemsArg>
		bool IsNearEqual
		(
			const Vec<SrcFloatTypeArg, NumElemsArg>&				InDest, 
			const Vec<DestFloatTypeArg,NumElemsArg>&				InSrc, 			
			SrcFloatTypeArg											InEps
		)
		{
			for(int i = 0; i < InDest.GetNumElems(); i++)
			{
				DestFloatTypeArg dest_component = GetComponent(InDest, i);				
				DestFloatTypeArg src_component = GetComponent(InSrc, i);								
				DestFloatTypeArg dest_minus_src_component = dest_component - src_component;
				if(dest_minus_src_component > InEps || dest_minus_src_component < -InEps)
				{
					return false;
				}
			}
			return true;
		}

		template<class FloatTypeArg, int NumElemsArg>
		void SetComponent
		(
			Vec<FloatTypeArg, NumElemsArg>					*pVec, 
			int												InIndex,
			FloatTypeArg									InValue
		)
		{			
			assert(pVec != nullptr);
			assert(InIndex >= 0);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be > InIndex" && (NumElemsArg > InIndex));
			FloatTypeArg *pComponent = GetComponentPointer(*pVec, InIndex);
			*pComponent = InValue;
		}
		
		template<class FloatTypeArg, int NumElemsArg>
		void SetX
		(
			Vec<FloatTypeArg, NumElemsArg>					*pVec,
			FloatTypeArg									InValue
		)
		{
			assert(pVec != nullptr);			
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 1" && (NumElemsArg >= 1));
			FloatTypeArg *pComponent = GetXPointer(*pVec);
			*pComponent = InValue;
		}

		template<class FloatTypeArg, int NumElemsArg>
		void SetYY
		(			
			Vec<FloatTypeArg, NumElemsArg>				   *pVec,
			FloatTypeArg									InValue
		)
		{
			assert(pVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 2" && (NumElemsArg >= 2));
			FloatTypeArg *pComponent = GetYYPointer(*pVec);
			*pComponent = InValue;
		}

		template<class FloatTypeArg, int NumElemsArg>
		void SetZZZ
		(
			Vec<FloatTypeArg, NumElemsArg>				   *pVec,
			FloatTypeArg									InValue
		)
		{
			assert(pVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 3" && (NumElemsArg >= 3));
			FloatTypeArg *pComponent = GetZZZPointer(*pVec);
			*pComponent = InValue;
		}

		template<class FloatTypeArg, int NumElemsArg>
		void SetWWWW
		(
			Vec<FloatTypeArg, NumElemsArg>				   *pVec,
			FloatTypeArg									InValue
		)
		{
			assert(pVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 4" && (NumElemsArg >= 4));
			FloatTypeArg *pComponent = GetWWWWPointer(*pVec);
			*pComponent = InValue;
		}

		// Get pointer to the given component of the given vector
		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetComponentPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec, int InIndex)
		{			
			assert(&InVec != nullptr);
			assert(InIndex >= 0);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be > InIndex" && (NumElemsArg > InIndex));
			return Util::AddPtr(GetPointer(&InVec), InIndex * InVec.GetComponentSize());
		}

		template<class FloatTypeArg, int NumElemsArg>		
		FloatTypeArg *GetComponentPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec, int InIndex)
		{
			assert(&InVec != nullptr);
			assert(InIndex >= 0);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be > InIndex" && (NumElemsArg > InIndex));
			return Util::AddPtr(GetPointer(&InVec), InIndex * InVec.GetComponentSize());
		}

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetXPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 1" && (NumElemsArg >= 1));
			return GetComponentPointer(InVec, 0);
		}

		template<class FloatTypeArg, int NumElemsArg>		
		FloatTypeArg *GetXPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 1" && (NumElemsArg >= 1));
			return GetComponentPointer(InVec, 0);
		}

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetYYPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 2" && (NumElemsArg >= 2));
			return GetComponentPointer(InVec, 1);
		}

		template<class FloatTypeArg, int NumElemsArg>		
		FloatTypeArg *GetYYPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 2" && (NumElemsArg >= 2));
			return GetComponentPointer(InVec, 1);
		}

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetZZZPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 3" && (NumElemsArg >= 3));
			return GetComponentPointer(InVec, 2);
		}

		template<class FloatTypeArg, int NumElemsArg>		
		FloatTypeArg *GetZZZPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 3" && (NumElemsArg >= 3));
			return GetComponentPointer(InVec, 2);
		}

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg *GetWWWWPointer(const Vec<FloatTypeArg, NumElemsArg>& InVec)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 4" && (NumElemsArg >= 4));
			return GetComponentPointer(InVec, 3);
		}

		template<class FloatTypeArg, int NumElemsArg>		
		FloatTypeArg *GetWWWWPointer(Vec<FloatTypeArg, NumElemsArg>&	InVec)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 4" && (NumElemsArg >= 4));
			return GetComponentPointer(InVec, 3);
		}
		
		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg& GetX
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec
		)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 1" && (NumElemsArg >= 1));
			return *GetXPointer(InVec);
		}

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg& GetYY
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec
		)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 2" && (NumElemsArg >= 2));
			return *GetYYPointer(InVec);
		}

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg& GetZZZ
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec
		)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 3" && (NumElemsArg >= 3));
			return *GetZZZPointer(InVec);
		}

		template<class FloatTypeArg, int NumElemsArg>
		const FloatTypeArg& GetWWWW
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec
		)
		{
			assert(&InVec != nullptr);
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 4" && (NumElemsArg >= 4));
			return *GetWWWWPointer(InVec);
		}


		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator-					
		(
			const Vec<FloatTypeArg, NumElemsArg>& InVec
		)
		{
			assert(&InVec != nullptr);
			Vec<FloatTypeArg, NumElemsArg> res_vec(InVec);
			Negate(&res_vec);
			return res_vec;
		}

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator+					
		(
			const Vec<FloatTypeArg, NumElemsArg>& InFirst,
			const Vec<FloatTypeArg, NumElemsArg>& InSecond
		)
		{
			assert(&InFirst != nullptr);
			assert(&InSecond != nullptr);
			Vec<FloatTypeArg, NumElemsArg> res_vec;
			SetAddMemoryVector
			(
				&res_vec, 
				GetPointer(&InFirst), GetPointer(&InSecond), 
				InFirst.GetNumElems(), 
				0, 
				InFirst.GetComponentSize(), InSecond.GetComponentSize()
			);
			return res_vec;
		}

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator-					
		(
			const Vec<FloatTypeArg, NumElemsArg>& InFirst,
			const Vec<FloatTypeArg, NumElemsArg>& InSecond
		)
		{

			assert(&InFirst != nullptr);
			assert(&InSecond != nullptr);
			Vec<FloatTypeArg, NumElemsArg> res_vec;
			SetSubtractMemoryVector
			(
				&res_vec, 
				GetPointer(&InFirst), GetPointer(&InSecond), 
				InFirst.GetNumElems(), 
				0, 
				InFirst.GetComponentSize(), InSecond.GetComponentSize()
			);
			return res_vec;
		}

		template<class FloatTypeArg, class CoeffFloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator*					
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec,
			CoeffFloatTypeArg								InFactor
		)
		{
			assert(&InVec != nullptr);							
			Vec<FloatTypeArg, NumElemsArg> res_vec(InVec);
			res_vec *= InFactor;			
			return res_vec;
		}

		template<class FloatTypeArg, class CoeffFloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator*					
		(
			CoeffFloatTypeArg								InFactor,
			const Vec<FloatTypeArg, NumElemsArg>&			InVec			
		)
		{
			return operator*(InVec, InFactor);
		}

		template<class FloatTypeArg, class CoeffFloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>		operator/					
		(
			const Vec<FloatTypeArg, NumElemsArg>&			InVec,
			CoeffFloatTypeArg								InFactor
		)
		{
			assert(&InVec != nullptr);	
			Vec<FloatTypeArg, NumElemsArg> res_vec(InVec);
			res_vec /= InFactor;
			return res_vec;
		}

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>&		operator+=					
		(
			Vec<FloatTypeArg, NumElemsArg>					          &InVec,
			const Vec<FloatTypeArg, NumElemsArg>					  &InVecTwo
		)
		{
			assert(&InVec != nullptr);
			assert(&InVecTwo != nullptr);
			AddMemoryVectorTo(&InVec, GetPointer(&InVecTwo), InVec.GetNumElems());
			return InVec;
		}

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>&		operator-=					
		(
			Vec<FloatTypeArg, NumElemsArg>					          &InVec,
			const Vec<FloatTypeArg, NumElemsArg>					  &InVecTwo
		)
		{
			assert(&InVec != nullptr);
			assert(&InVecTwo != nullptr);
			SubtractMemoryVectorFrom(&InVec, GetPointer(&InVecTwo), InVec.GetNumElems());
			return InVec;
		}

		template<class FloatTypeArg, class CoeffFloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>&		operator*=					
		(
			Vec<FloatTypeArg, NumElemsArg>					  &InVec,
			CoeffFloatTypeArg								   InFactor
		)
		{			
			assert(&InVec != nullptr);			
			MultScalar_n(GetPointer(&InVec), InFactor, InVec.GetNumElems(), InVec.GetComponentSize());
			return InVec;
		}

		template<class FloatTypeArg, class CoeffFloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>&		operator/=					
		(
			Vec<FloatTypeArg, NumElemsArg>					  &InVec,
			CoeffFloatTypeArg								   InFactor
		)
		{
			assert(&InVec != nullptr);
			DivScalar_n(GetPointer(&InVec), InFactor, InVec.GetNumElems(), InVec.GetComponentSize());
			return InVec;
		}

		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, DestNumElemsArg>		*AddMemoryVectorTo
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>				*pDestVec,
			const SrcFloatTypeArg								*pSrcVec,
			int													 InNumComponents,
			int													 InStartDestIndex,
			size_t												 srcComponentStride
		)
		{
			assert(pDestVec != nullptr);
			assert(pSrcVec != nullptr);
			assert(InNumComponents >= 0);
			assert(InNumComponents <= DestNumElemsArg);
			assert(InStartDestIndex >= 0);			
			Add_n_To
			(
				GetPointer(pDestVec), pSrcVec, 
				InNumComponents, 
				pDestVec->GetComponentSize(), 
				srcComponentStride
			);
			return pDestVec;
		}

		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg, DestNumElemsArg>		*SubtractMemoryVectorFrom
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>				*pDestVec,
			const SrcFloatTypeArg								*pSrcVec,
			int													 InNumComponents,
			int													 InStartDestIndex,
			size_t												 srcComponentStride
		)
		{
			Subtract_n_from
			(
				GetPointer(pDestVec), pSrcVec, 
				InNumComponents, 
				pDestVec->GetComponentSize(), 
				srcComponentStride
			);
			return pDestVec;
		}

		// Set the sum of vectors to the given vector 
		// components with indices in range [InStartDestIndex, InStartDestIndex + InNumComponents)
		// (without touching other components)
		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg, class SrcFloatTypeArgTwo>
		Vec<DestFloatTypeArg, DestNumElemsArg>		*SetAddMemoryVector
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>				*pDestVec,
			const SrcFloatTypeArg								*pSrcVecOne,
			const SrcFloatTypeArgTwo							*pSrcVecTwo,
			int													 InNumComponents,
			int													 InStartDestIndex,
			size_t												 srcComponentOneStride,									 
			size_t												 srcComponentTwoStride									 
		)
		{
			SetAdd_n
			(
				GetPointer(pDestVec), 
				pSrcVecOne, pSrcVecTwo, 
				InNumComponents, 
				pDestVec->GetComponentSize(),
				srcComponentOneStride, srcComponentTwoStride
			);
			return pDestVec;
		}

		template<class DestFloatTypeArg, int DestNumElemsArg, class SrcFloatTypeArg, class SrcFloatTypeArgTwo>
		Vec<DestFloatTypeArg, DestNumElemsArg>		*SetSubtractMemoryVector
		(
			Vec<DestFloatTypeArg, DestNumElemsArg>				*pDestVec,
			const SrcFloatTypeArg								*pSrcVecOne,
			const SrcFloatTypeArgTwo							*pSrcVecTwo,
			int													 InNumComponents,
			int													 InStartDestIndex,
			size_t												 srcComponentOneStride,
			size_t												 srcComponentTwoStride										 
		)
		{
			SetSubtract_n
			(
				GetPointer(pDestVec), 
				pSrcVecOne, pSrcVecTwo, 
				InNumComponents,
				pDestVec->GetComponentSize(),
				srcComponentOneStride, srcComponentTwoStride
			);
			return pDestVec;
		}

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>&		Negate						
		(
			Vec<FloatTypeArg, NumElemsArg>					*pVec
		)
		{
			Negate_n(GetPointer(pVec), pVec->GetNumElems(), pVec->GetComponentSize());
			return *pVec;
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg, int NumElemsArg>
		DestFloatTypeArg Dot
		(
			const Vec<DestFloatTypeArg,NumElemsArg>					&InVec, 
			const Vec<SrcFloatTypeArg,NumElemsArg>					&InVecTwo
		)
		{
			return Dot_n
			(
				GetPointer(&InVec), GetPointer(&InVecTwo), 
				InVec.GetNumElems(), 
				InVec.GetComponentSize(), InVecTwo.GetComponentSize()
			);
		}	


		// Dot2: Takes maximum TWO components from each vector 
		// WARNING!!! Each vector must contain minimum Two components!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot2
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + GetYY(InVec) * GetYY(InVecTwo);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot2
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + GetYY(InVec) * GetYY(InVecTwo);
		}



		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot2
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + GetYY(InVec) * GetYY(InVecTwo);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot2
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + GetYY(InVec) * GetYY(InVecTwo);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot2
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + GetYY(InVec) * GetYY(InVecTwo);
		}


		// Dot3: Takes only three first components from the second vector
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot3
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + GetYY(InVec) * GetYY(InVecTwo) + GetZZZ(InVec) * GetZZZ(InVecTwo);
		}

		// Dot3: Takes only three first components from the second vector
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot3
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		)
		{
			return Dot2(InVec, InVecTwo);
		}

		// Dot3: Takes only three first components from the second vector
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot3
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		)
		{
			return Dot2(InVec, InVecTwo);
		}

		// Dot3: Takes only three first components from the second vector
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot3
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + GetYY(InVec) * GetYY(InVecTwo) + GetZZZ(InVec) * GetZZZ(InVecTwo);
		}

		// Dot3: Takes only three first components from the second vector
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot3
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + GetYY(InVec) * GetYY(InVecTwo) + GetZZZ(InVec) * GetZZZ(InVecTwo);
		}

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ONE!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wOne
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + 
			       GetYY(InVec) * GetYY(InVecTwo) + 
			       GetZZZ(InVec) * GetZZZ(InVecTwo) +
			       GetWWWW(InVec) * GetWWWW(InVecTwo);
		}

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ONE!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wOne
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + 
			       GetYY(InVec) * GetYY(InVecTwo) + 
			       GetZZZ(InVec) * GetZZZ(InVecTwo) +
			       GetWWWW(InVec) * 1;
		}

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ONE!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wOne
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + 
			       GetYY(InVec) * GetYY(InVecTwo) + 
			       GetZZZ(InVec) * GetZZZ(InVecTwo) +
			       1 * GetWWWW(InVecTwo);
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wOne
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + 
			       GetYY(InVec) * GetYY(InVecTwo) + 
			       GetWWWW(InVec); // WARNING!!! GetWWWW(InVec) is VERY important here!!!
		}

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ONE!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wOne
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		)
		{
			return GetX(InVec) * GetX(InVecTwo) + 
			       GetYY(InVec) * GetYY(InVecTwo) + 
			       GetWWWW(InVecTwo); // WARNING!!! GetWWWW(InVecTwo) is VERY important here!!!	
		}

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ZERO!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wZero
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		)
		{
			return Dot4_wOne(InVec, InVecTwo);
		}

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ZERO!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wZero
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,3>				                &InVecTwo
		)
		{
			return Dot3(InVec, InVecTwo);
		}

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ZERO!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wZero
		(
			const Vec<DestFloatTypeArg,3>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		)
		{
			return Dot3(InVec, InVecTwo)
		}

		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wZero
		(
			const Vec<DestFloatTypeArg,4>					        &InVec, 
			const Vec<SrcFloatTypeArg,2>				                &InVecTwo
		)
		{
			return Dot2(InVec, InVecTwo);
		}

		// Dot4: Takes only Two first components from the second vector
		// WARNING!!! Will treat W as ZERO!!!
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		DestFloatTypeArg Dot4_wZero
		(
			const Vec<DestFloatTypeArg,2>					        &InVec, 
			const Vec<SrcFloatTypeArg,4>				                &InVecTwo
		)
		{
			return Dot2(InVec, InVecTwo); 
		}

		template<class FloatTypeArg, int NumElemsArg>
		FloatTypeArg SqrLength
		(
			const Vec<FloatTypeArg,NumElemsArg>					&InVec 			
		)
		{
			return VecSqrLength_n(GetPointer(&InVec), InVec.GetNumElems(), InVec.GetComponentSize());
		}

		template<class FloatTypeArg, int NumElemsArg>
		FloatTypeArg Length
		(
			const Vec<FloatTypeArg,NumElemsArg>					&InVec 			
		)
		{
			return VecLength_n(GetPointer(&InVec), InVec.GetNumElems(), InVec.GetComponentSize());
		}

		template<class FloatTypeArg, int NumElemsArg>
		Vec<FloatTypeArg,NumElemsArg>							&Normalize
		(
			Vec<FloatTypeArg,NumElemsArg>						&InVec 			
		)
		{
			Normalize_n(GetPointer(&InVec), InVec.GetNumElems(), InVec.GetComponentSize());
			return InVec;
		}

		template<class FloatTypeArg, int NumElemsArg>
		FloatTypeArg                            SqrDistance
		(
			const Vec<FloatTypeArg, NumElemsArg>&               InA,
			const Vec<FloatTypeArg, NumElemsArg>&               InB
		)
		{
			return SqrLength(InB - InA);
		}

		template<class FloatTypeArg, int NumElemsArg>
		FloatTypeArg                            Distance
		(
			const Vec<FloatTypeArg, NumElemsArg>&               InA,
			const Vec<FloatTypeArg, NumElemsArg>&               InB
		)
		{
			return Length(InB - InA);
		}

		/***********************************************************************************
		* Vec3 functions IMPLEMENTATION
		************************************************************************************/
		template<class DestFloatTypeArg, class SrcFloatTypeArg>
		Vec<DestFloatTypeArg,3> Cross
		(
			const Vec<DestFloatTypeArg,3>&		InFirst,
			const Vec<SrcFloatTypeArg,3>&		InSecond
		)
		{
			Vec<DestFloatTypeArg,3> res_vec;
			Cross3
			(
				GetPointer(&res_vec), 
				GetPointer(&InFirst), 
				GetPointer(&InSecond), 
				res_vec.GetComponentSize(),
				InFirst.GetComponentSize(), 
				InSecond.GetComponentSize()
			);
			return res_vec;
		}

		/************************************************************************************
		* Member-function IMPLEMENTATION
		************************************************************************************/
		template<class FloatTypeArg, size_t NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>::Vec()
		{
			MkZero_n(this);
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>::Vec												
		(
			const FloatTypeArg& InX, const FloatTypeArg& InYY
		)
		{
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 2" && (NumElemsArg >= 2));
			Reset2(this, InX, InYY);
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>::Vec(const FloatTypeArg& InX, const FloatTypeArg& InYY, const FloatTypeArg& InZZZ)
		{
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 3" && (NumElemsArg >= 3));
			Reset3(this, InX, InYY, InZZZ);
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>::Vec(const FloatTypeArg& InX, const FloatTypeArg& InYY, const FloatTypeArg& InZZZ, const FloatTypeArg& InWWWW)
		{
			assert("Vec<FloatTypeArg, NumElemsArg>: NumElemsArg must be >= 4" && (NumElemsArg >= 4));
			Reset4(this, InX, InYY, InZZZ, InWWWW);
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		template<class InSrcType, size_t InSrcNumComponents>
		Vec<FloatTypeArg, NumElemsArg>::Vec												
		(
			const std::array<InSrcType, InSrcNumComponents>&	 InSrcArray, 
			int													 InNumElems			
		)
		{			
			CopyCastZeroed_n(GetPointer(this), &(InSrcArray[0]), GetNumElems(), InNumElems, GetComponentSize(), sizeof(InSrcArray[0]));
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		template<class InSrcType>
		Vec<FloatTypeArg, NumElemsArg>::Vec												
		(
			const Vec<InSrcType, NumElemsArg>&			 InSrcVec				
		)
		{
			CopyCast_n(GetPointer(this), GetPointer(&InSrcVec), GetNumElems(), GetComponentSize(), InSrcVec.GetComponentSize());
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		template<class InSrcType, size_t InSrcNumComponents>
		Vec<FloatTypeArg, NumElemsArg>::Vec												
		(
			const Vec<InSrcType, InSrcNumComponents>&			 InSrcVec, 
			int													 InNumElems			
		)
		{
			CopyCastZeroed_n(GetPointer(this), GetPointer(&InSrcVec), GetNumElems(), InNumElems, GetComponentSize(), InSrcVec.GetComponentSize());
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		template<class InSrcType>
		Vec<FloatTypeArg, NumElemsArg>::Vec												
		(
			const InSrcType				*pInSrc, 
			int							 InNumElems,
			size_t						 InByteStride			
		)
		{			
			CopyCastZeroed_n
			(
				GetPointer(this), 
				pInSrc, 
				GetNumElems(), 
				InNumElems, 
				GetComponentSize(),
				InByteStride
			);
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		template<class InSrcIteratorType>
		Vec<FloatTypeArg, NumElemsArg>::Vec												
		(
			const InSrcIteratorType &InStartIterator,
			const InSrcIteratorType &InEndIterator
		)
		{
			std::copy(InStartIterator, InEndIterator, _arr.begin());
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		Vec<FloatTypeArg, NumElemsArg>::Vec	(const ThisType& InSrc)
		{
			CopyCast_n(this, InSrc, GetArray().size());
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		typename Vec<FloatTypeArg, NumElemsArg>::ThisType& Vec<FloatTypeArg, NumElemsArg>::operator=(const ThisType& InSrc)
		{
			CopyCast_n(this, InSrc, GetArray().size());
			return *this;
		}

		template<class FloatTypeArg, size_t NumElemsArg>
		size_t Vec<FloatTypeArg, NumElemsArg>::GetComponentSize()
		{
			return sizeof(FloatTypeArg);
		}
	} // Math
} // DvUtils