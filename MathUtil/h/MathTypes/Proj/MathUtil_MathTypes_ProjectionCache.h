#pragma once

#include <limits>
#include "MathUtil_MathTypes_ProjectionCache_ForwardPerspective.h"

namespace DvUtils
{
	namespace Math
	{
		/********************************************************************************
		* TODO SUMMARY:
		* 1. All functionality was implemented like in ProjectionCache,
		*    - bugs has been mended (00:55/30.09.2016)
		*	 - the result projection matrix and its inversed are stored as the properties of the class
		*	   (12:11/30.09.2016)
		*	- construction and initialization process changes:
		*     A.Now default ctor does NOT initiaize the class instance at all,
		*	  the Initialize function to be called to initalize the class instance
		*	  (reason: default frustum info ZNear may be 0.0, and it cannot be used)
		*  	 (DONE: 16:12/30.09.2016)
		*	  B.Constructor with argument removed
		*	  (to get rid of ambiguity of what to use - ctor or initialize function)
		*	 (DONE: 16:12/30.09.2016)
		*
		* TODO:
		* 1. Implement the inversed matrix calculation (+ CODED 12:09/30.09.2016)
		*
		* TODO LOG:
		* 1. Implement the ProjectionCache_ForwardPerspective class (+ CODED 23:41/29:09:2016)
		* 2. Implement ProjectionCache class
		* 2.1. Getters												(+ CODED 23:48/29:09:2016)
		* 2.2. Helper getters										(+ CODED 23:48/29:09:2016)
		* 2.3. default ctor (+ CODED: 00:54/30.09.2016)	
		* 2.3.1. _Set_frustum_and_dependent	* (+ CODED: 00:54/30.09.2016)	
		* 2.3.1.1. Repair bug: update all the inversed coeffs (find bug by 'BUG?' keyword)
		* (BUG REPAIRED: 00:53/30.09.2016)
		* 2.4. ctor that takes FrustumInfo (+ CODED: 00:54/30.09.2016)	
		* 2.5. SetFrustum (CODED: 00:54/30.09.2016)	
		* 2.5.1. _Set_frustum_and_dependent	(+ CODED: 00:54/30.09.2016)		
		* 2.6. SetNearZ (+ CODED: 00:09/30:09:2016)
		* 2.6.1. _Set_near_z_and_dependent (+ CODED: 00:09/30:09:2016)
		* 2.6.1.1. _SetInversedQZNear() (+ CODED: 00:09/30:09:2016)
		* 2.7. void SetFarZ	(FloatTypeArg InNewFarZ)	(+ CODED 00:24/30:09:2016)	
		* 2.7.1. _Set_far_z_and_dependent(InNewFarZ) (+ CODED 00:24/30:09:2016)		
		* 2.8. SetHalFov (+ CODED 00:33/30:09:2016)
		* 2.8.1. _Set_HalfFov_and_dependent (+ CODED 00:33/30:09:2016)
		* 2.8.1.1. _SetInversedWidthAndHeightCoeffs (+ CODED 00:33/30:09:2016)
		* 2.8.1.1.1. _SetInversedHeightCoeff (+ CODED 00:33/30:09:2016)
		* 2.9. void SetAspectWidthOverHeight(FloatTypeArg InNewAspecWidthOverHeight) (+ CODED 00:37/30:09:2016)
		* 2.9.1. void _Set_aspect_width_over_height_and_dependent(FloatTypeArg InNewAspectWidthOverHeight) (+ CODED 00:36/30:09:2016)
		********************************************************************************/

		/********************************************************************************
		* ProjectionCache
		*
		* WARNING! This class supports only perspective projection, as for now.
		*		
		* The result forward projection matrix has the following form:
		* |widthCoeff,				0,					0,					0|
		* |0						heightCoeff,		0,					0|
		* |0,						0,					Q,					1|
		* |0,						0,				   -QZNear,				0|
		*
		* The result inversed projection matrix has the following form:
		* |1/widthCoeff,			0,					0,					0|
		* |0						1/heightCoeff,		0,					0|
		* |0,						0,					0,		  -1/(QZnear)|
		* |0,						0,					1,			  1/Znear|
		*		
		* WARNING!!!
		* This class is designed as the version of the ReversiblePerspectiveMatrixCache
		* that works with the SquareMatrix instead of the XMMATRIX.
		*
		* Class responsibilities:
		*	Calculates and stores coefficients useful for calculating
		* both the perspective projection matrix and the inversed perspective projection matrix
		*
		* 1. Construction initialization		
		* 1.1. constructors
		* 1.1.1. Default ctor
		* WARNING!!! Does NOT init the class instance,
		* the Initialize operation to be called to init the class instance!
		*
		* 2. Initialization
		* Initializes the state of application.		
		* WARNING!!! 
		* 1) ZNear must be greater than ZERO 
		* 2) ZFar must be greater than ZNear
		* 3) Aspect must be greater than ZERO
		* (because we need to compute the inverse projection matrix)!
		********************************************************************************/
		template<class FloatTypeArg>
		class ProjectionCache
		{
		public:
			/*******************************************************
			* typedefs
			*******************************************************/
			typedef FloatTypeArg											FloatType;

			typedef typename ProjectionCache_ForwardPerspective<FloatType>  ForwardPerspectiveCacheType;
			// Common coeff cache type 
			// (corresponds to the PerspectiveMatrixCache::ProjectionMatrixCacheType typedef)
			typedef typename ProjectionMatrixCache<FloatType>				CommonCoeffCacheType;
			typedef typename ForwardPerspectiveCacheType::MatrixType		MatrixType;
			
			/******************************************************
			* Construction and initialization
			******************************************************/
			// Default ctor: 
			// WARNING!!! Does NOT initialize the class instance,
			// the Initialize function must be called
			ProjectionCache();			

			// Initialize the class instance
			// WARNING: ZNear must be GREATER than ZERO
			// (It's a necessary condition to make the projection matrix inversible)
			void Initialize(const FrustumInfo& InFrustum);

			/*******************************************************
			* getters
			*******************************************************/
			// Get forward-transform projection matrix
			const MatrixType&										GetMatrix						()				const		{ assert(_bInitialized); return _cacheForward.GetMatrix(); }
			// Get inversed projection matrix
			const MatrixType&										GetInversedMatrix				()				const		{ assert(_bInitialized); return _inversedMatrix; }


			const ForwardPerspectiveCacheType&						GetCache_ForwardPerspective		()				const		{ assert(_bInitialized); return _cacheForward; }			
			FloatTypeArg											GetInversedZNear				()				const		{ assert(_bInitialized); return _inversed_ZNear; }
			FloatTypeArg											GetInversedQZNear				()				const		{ assert(_bInitialized); return _inversed_QZNear; }
			FloatTypeArg											GetInversedWidthCoeff			()				const		{ assert(_bInitialized); return _inversed_WidthCoeff; }
			FloatTypeArg											GetInversedHeightCoeff			()				const		{ assert(_bInitialized); return _inversed_HeightCoeff; }

			/*******************************************************
			* helper getters
			*******************************************************/
			const FrustumInfo&										GetFrustumInfo							()				const			{ assert(_bInitialized); return _cacheForward.GetFrustumInfo(); }
			// get cache for variables useful for calculating both ortho and perspective proj matrices
			const CommonCoeffCacheType&								GetCommon								()				const			{ assert(_bInitialized); return _cacheForward.GetCommon(); }
			// tan of the half FOV (field-of-view) angle
			FloatTypeArg											GetTanHalfFov							()				const			{ assert(_bInitialized); return _cacheForward.GetHalfFov(); }
			//  Q = depth/ZFar
			FloatTypeArg											GetQ									()				const			{ assert(_bInitialized); return _cacheForward.GetQ(); }
			FloatTypeArg											GetQZNear								()				const			{ assert(_bInitialized); return _cacheForward.GetQZNear(); }

			FloatTypeArg											GetZNear								()				const			{ assert(_bInitialized); return _cacheForward.GetZNear(); }
			FloatTypeArg											GetZFar									()				const			{ assert(_bInitialized); return _cacheForward.GetZFar(); }
			FloatTypeArg											GetHalfFov								()				const			{ assert(_bInitialized); return _cacheForward.GetHalfFov(); }				
			FloatTypeArg											GetAspectWidthOverHeight				()				const			{ assert(_bInitialized); return _cacheForward.GetAspectWidthOverHeight(); }				

			// Coefficient to be used in the projection matrix as the X scale cofficient
			FloatTypeArg											GetWidthCoeff			()				const		{ assert(_bInitialized); return _cacheForward.GetWidthCoeff(); }
			// Coefficient to be used in the projection matrix as the Y scale cofficient
			FloatTypeArg											GetHeightCoeff			()				const		{ assert(_bInitialized); return _cacheForward.GetHeightCoeff(); }
			// Depth of the projection viewport
			FloatTypeArg											GetDepth				()				const		{ assert(_bInitialized); return _cacheForward.GetDepth(); }

			/*******************************************************
			* setters
			*******************************************************/
			void SetFrustum								(const FrustumInfo&		InNewFrustumValue);
			void SetNearZ								(FloatTypeArg			InNewNearZ);
			void SetFarZ								(FloatTypeArg			InNewFarZ);
			void SetHalfFov								(FloatTypeArg			InNewHalfFov);
			void SetAspectWidthOverHeight				(FloatTypeArg			InNewAspecWidthOverHeight);

		private:
			void _Set_frustum_and_dependent							(const FrustumInfo& InNewFrustumValue);
			void _Set_near_z_and_dependent							(FloatTypeArg InNewNearZ);
			void _Set_far_z_and_dependent							(FloatTypeArg InNewFarZ);
			void _Set_HalfFov_and_dependent							(FloatTypeArg InNewHalfFov);
			void _Set_aspect_width_over_height_and_dependent		(FloatTypeArg InNewAspectWidthOverHeight);
			
			void _SetInversedQZNear									();
			void _SetInversedWidthAndHeightCoeffs					();
			void _SetInversedHeightCoeff							();		
			void _SetInversedZNear									();

			void _Assert_FrustumInfoValid							(const FrustumInfo& InFrustum);

			// Cache for storing the forward matrix
			MatrixType												_inversedMatrix;
			ForwardPerspectiveCacheType								_cacheForward;
			FloatTypeArg											_inversed_ZNear;
			FloatTypeArg											_inversed_QZNear;
			FloatTypeArg											_inversed_WidthCoeff;
			FloatTypeArg											_inversed_HeightCoeff;

			// helper field for asserts
			bool										 _bInitialized;
		};

		/*************************************************************************************
		* WARNING!!! This functions are for implementation only purposes!
		**************************************************************************************/
		template<class FloatTypeArg>
		void UpdateSquareMatrix_WithInverseProjection 
		(
			typename ProjectionCache<FloatTypeArg>::MatrixType				*pOutMatrix,
			const ProjectionCache<FloatTypeArg>&							 InCache
		);

		/*************************************************************************************
		* IMPLEMENTATION
		**************************************************************************************/
		template<class FloatTypeArg>
		void UpdateSquareMatrix_WithInverseProjection 
		(
			typename ProjectionCache<FloatTypeArg>::MatrixType				*pOutMatrix,
			const ProjectionCache<FloatTypeArg>&							 InCache
		)
		{
			assert(pOutMatrix);
			assert(&InCache);
			*pOutMatrix = CalcSquareMatrixInverse_PerspectiveProjection
			(
				InCache.GetInversedZNear(),
				InCache.GetInversedQZNear(),
				InCache.GetInversedWidthCoeff(),
				InCache.GetInversedHeightCoeff()
			);
		}

		/*************************************************************************************
		* member-function IMPLEMENTATION
		**************************************************************************************/
		template<class FloatTypeArg>
		ProjectionCache<FloatTypeArg>::ProjectionCache() :
			// Solution with using std::numeric_limits introduces conflicts with
			// WINDOWS-defined max and min global functions.
			_inversed_ZNear(FLT_MAX),
			_inversed_QZNear(FLT_MAX),
			_inversed_WidthCoeff(FLT_MAX),
			_inversed_HeightCoeff(FLT_MAX),
			_bInitialized(false)
		{
			// COMMENT:
			// No initialization here and it's intended 
			// (see comments in the class description).
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::Initialize(const FrustumInfo& InFrustum)		
		{
			assert(&InFrustum);						
			// COMMENT set initialized flag at the start of the initialization
			// (because some operations used during initialization
			// may check this flag is set)
			_bInitialized = true;
			_cacheForward.Initialize(InFrustum);
			_Assert_FrustumInfoValid(InFrustum);
			_Set_frustum_and_dependent(InFrustum);
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::_Assert_FrustumInfoValid(const FrustumInfo& InFrustum)
		{
			// Z Near must be greater than ZERO (to make the projection matrix inversible)
			assert(InFrustum.geo.zNear > 0);
			assert(InFrustum.geo.zFar > InFrustum.geo.zNear);

			assert(InFrustum.aspectWidthOverHeight > 0);
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::SetFrustum(const FrustumInfo& InNewFrustumValue)
		{
			assert(&InNewFrustumValue);
			assert(_bInitialized);
			_Set_frustum_and_dependent(InNewFrustumValue);
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::SetNearZ(FloatTypeArg InNewNearZ)
		{
			assert(_bInitialized);
			_Set_near_z_and_dependent(InNewNearZ);
			UpdateSquareMatrix_WithInverseProjection(&_inversedMatrix, *this);
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::SetFarZ(FloatTypeArg InNewFarZ)
		{
			assert(_bInitialized);
			_Set_far_z_and_dependent(InNewFarZ);
			UpdateSquareMatrix_WithInverseProjection(&_inversedMatrix, *this);
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::SetHalfFov(FloatTypeArg	InNewHalfFov)
		{
			assert(_bInitialized);
			_Set_HalfFov_and_dependent(InNewHalfFov);
			UpdateSquareMatrix_WithInverseProjection(&_inversedMatrix, *this);
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::SetAspectWidthOverHeight(FloatTypeArg	InNewAspecWidthOverHeight)
		{
			assert(_bInitialized);
			_Set_aspect_width_over_height_and_dependent(InNewAspecWidthOverHeight);
			UpdateSquareMatrix_WithInverseProjection(&_inversedMatrix, *this);
		}
		
		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::_Set_frustum_and_dependent(const FrustumInfo& InNewFrustumValue)
		{			
			assert(&InNewFrustumValue);
			_cacheForward.SetFrustum(InNewFrustumValue);
			//BUG?Why no update of all coeffs?
			_SetInversedZNear();
			_SetInversedQZNear();
			_SetInversedWidthAndHeightCoeffs();
			UpdateSquareMatrix_WithInverseProjection(&_inversedMatrix, *this);
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::_Set_near_z_and_dependent(FloatTypeArg InNewNearZ)
		{
			_cacheForward.SetNearZ(InNewNearZ);
			_SetInversedZNear();
			_SetInversedQZNear();
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::_Set_far_z_and_dependent(FloatTypeArg InNewFarZ)
		{
			_cacheForward.SetFarZ(InNewFarZ);
			// BUG? Why to recompute QZNear when the inversed ZFar updated?
			// No, it's not bug:
			// COMMENT: The Q is dependent on the NewFarZ, 
			// and Q is automatically recomputed when the new far z is updated.
			_SetInversedQZNear();
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::_Set_HalfFov_and_dependent(FloatTypeArg InNewHalfFov)
		{
			_cacheForward.SetHalfFov(InNewHalfFov);
			_SetInversedWidthAndHeightCoeffs();
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::_Set_aspect_width_over_height_and_dependent(FloatTypeArg InNewAspectWidthOverHeight)
		{
			_cacheForward.SetAspectWidthOverHeight(InNewAspectWidthOverHeight);
			_SetInversedHeightCoeff();
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::_SetInversedWidthAndHeightCoeffs()
		{
			_inversed_WidthCoeff = static_cast<FloatTypeArg>(1)/_cacheForward.GetWidthCoeff();
			_SetInversedHeightCoeff();
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::_SetInversedHeightCoeff()
		{
			_inversed_HeightCoeff = static_cast<FloatTypeArg>(1)/_cacheForward.GetHeightCoeff();
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::_SetInversedZNear()
		{
			_inversed_ZNear = 1.0F/GetZNear();
		}

		template<class FloatTypeArg>
		void ProjectionCache<FloatTypeArg>::_SetInversedQZNear()
		{
			_inversed_QZNear = static_cast<FloatTypeArg>(1)/GetQZNear();
		}			
	} // Math
} // DvUtils