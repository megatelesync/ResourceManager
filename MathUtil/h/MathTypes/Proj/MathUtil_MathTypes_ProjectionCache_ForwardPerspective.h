#pragma once

// instead of "MathUtil_3DMath_FrustumInfo.h",
// also include header for SquareMatrix:
#include "MathUtil_MathTypes_Util.h" 
#include "../../3DMath/MathUtil_3DMath_FrustumInfo.h" 
#include <limits.h>

namespace DvUtils
{
	namespace Math
	{
		/******************************************************
		* TODO SUMMARY:
		* 1. All functionality implemented in the PerspectiveMatrixCache prototype 
		* is implemented 23:36/29.09.2016
		*
		* TODO Bugs and deficiences:
		* 1. Mend bug in _Set_WidthCoeff_and_dependent(): (MENDED: 15:31/30.09.2016)
		* 1.1. No CommonCoeffCacheType::SetWidthCoeff coeff, only the CommonCoeffCacheType::SetWidthCoeffAndDepth
		* 2. Mend bug in _Set_HeightCoeff_and_dependent() (MENDED: 15:31/30.09.2016)
		* 2.1. Similar bug
		* 3. Mend bug: update the depth in the common cache when (MENDED: 15:19/30.09.2016)
		*    a) frustum is updated
		*    b) Znear updated
		*	 c) Zfar updated
		* 4. Design issue: (MENDED: 15:19/30.09.2016)
		*    4.1. Default constructing with default FrustumInfo that inits Znear as ZERO
		*    is error-prone because it makes the inverse matrix incomputible 
		*    (due to devide by zero in expression 1/ZNear).
		*	 4.1.1. Solution:
		*		4.1.1.1. Make default ctor that makes NO initialization (dot it well-documented!)
		*		4.1.1.2. Provide initialize function that takes the FrustumInfo as argument
		*				 and performs all the initialization
		*
		* TODO HISTORY TWO:
		* 1. SetFrustum(const FrustumInfo&	InNewFrustumValue)  (+ CODED: 23:18/29.09.2016)
		* 2. SetNearZ(FloatTypeArg InNewNearZ) (+ CODED: 23:21/29.09.2016)
		* 2.1. _Set_near_z_and_dependent(InNewNearZ) (+ CODED: 23:21/29.09.2016)
		* 3. SetFarZ(FloatTypeArg InNewFarZ) (+ CODED: 23:24/29.09.2016)
		* 3.1. _Set_far_z_and_dependent(InNewFarZ) (+ CODED: 23:24/29.09.2016)
		* 4. SetHalfFov(FloatTypeArg InNewHalfFov) (+ CODED: 23:27/29.09.2016)
		* 4.1. _Set_half_FOV_and_dependent (+ CODED: 23:27/29.09.2016)
		* 5. SetAspectWidthOverHeight(FloatTypeArg InNewAspecWidthOverHeight) (+ CODED: 23:32/29.09.2016)
		* 5.1. _Set_aspect_width_over_height_and_dependent (+ CODED: 23:32/29.09.2016)
		*
		* TODO HISTORY ONE:
		* 1. default ctor (+ CODED: 23:12/29.09.2016)
		* 1.1. _Set_frustum_and_dependent (+ CODED: 23:11/29.09.2016)
		* 1.1.1. Calc_ProjectionMatrixCache
		* 1.1.1.1. CalcSquareMatrix_PerspectiveProjection +
		* 1.1.1.1.1. See TODO in MathUtil_MathTypes_Util.h
		* 1.1.1.2. Getters: GetDepth(), GetQ() etc. +
		* 1.1.1.3. Helper getters +
		* 1.1.2. _Set_Q_and_dependent +
		* 1.1.2.1. Calc_Q +
		* 1.1.2.2. _Set_QZnear_and_dependent(); +
		* 1.1.2.2.1. Calc_QZnear() +		
		* 1.1.3. _Set_HalfFov_and_dependent (+ CODED: 23:10/29.09.2016)
		* 1.1.3.1. Calc_Tan_HalfFov (+ CODED: 23:09/29.09.2016)
		* 1.1.3.2. _Set_WidthCoeff_and_dependent (+ CODED: 23:10/29.09.2016)
		* 1.1.3.2.1. Calc_WidthCoeff (+ CODED: 23:09/29.09.2016)
		* 1.1.3.2.2. _Set_HeightCoeff_and_dependent (+ CODED: 23:10/29.09.2016)
		* 1.1.3.2.2.1. Calc_HeightCoeff (+ CODED: 23:09/29.09.2016)
		* 2. ctor with argument of type "const FrustumInfo& InFrustum" (+ CODED: 23:14/29.09.2016)			
		******************************************************/

		/******************************************************
		* This class is for caching only the forward matrix
		* (computing the inverse matrix is NOT its responsibility)
		*
		* The result forward projection matrix has the following form:
		* |widthCoeff,				0,					0,					0|
		* |0						heightCoeff,		0,					0|
		* |0,						0,					Q,					1|
		* |0,						0,				   -QZNear,				0|
		*
		* WARNING!
		* This class was designed to provide the perspective matrix cache that 
		* uses the SquareMatrix instead of the XMMATRIX to represent the matrix
		* (as in the class in header 3DMath/MathUtil_3DMath_PerspectiveProjectionMatrixCache.h)		
		*
		* 1. Construction and initialization
		* 1.1. Construction
		* 1.1.1 Default ctor
		* WARNING!!! Default ctor performs NO initialization
		* Use Initialize() to perform the initialization 
		* 1.2. Initialization
		* Initializes the state of the instance 
		* using the given parameters.
		* NOTE: Do NOT pass the 0.0 for ZNear if you wnat the 
		* inversible projection matrix.
		******************************************************/
		template<class FloatTypeArg>
		class ProjectionCache_ForwardPerspective
		{
		public:
			/********************************************************************************
			* typedefs
			********************************************************************************/
			typedef FloatTypeArg											FloatType;

			// Common coeff cache type 
			// (corresponds to the PerspectiveMatrixCache::ProjectionMatrixCacheType typedef)
			typedef typename ProjectionMatrixCache<FloatType>				CommonCoeffCacheType;
			typedef typename SquareMatrix<FloatType, 4>						MatrixType;

			/*******************************************************
			* getters
			*******************************************************/
			const MatrixType&										GetMatrix				()				const		{ assert(_bInitialized); return _matrix; }

			const FrustumInfo&										GetFrustumInfo			()				const		{ assert(_bInitialized); return _frustum; }
			// get cache for variables useful for calculating both ortho and perspective proj matrices
			const CommonCoeffCacheType&								GetCommon				()				const		{ assert(_bInitialized); return _cacheCommon; }
			// tan of the half FOV (field-of-view) angle
			FloatTypeArg											GetTanHalfFov			()				const		{ assert(_bInitialized); return _tan_halfFov; }
			//  Q = depth/ZFar
			FloatTypeArg											GetQ					()				const		{ assert(_bInitialized); return _Q; }
			FloatTypeArg											GetQZNear				()				const		{ assert(_bInitialized); return _QZNear; }

			/*******************************************************
			* helper getters
			*******************************************************/
			FloatTypeArg											GetZNear								()				const			{ assert(_bInitialized); return _frustum.GetZNear(); }
			FloatTypeArg											GetZFar									()				const			{ assert(_bInitialized); return _frustum.GetZFar(); }
			FloatTypeArg											GetHalfFov								()				const			{ assert(_bInitialized); return _frustum.GetHalfFov(); }				
			FloatTypeArg											GetAspectWidthOverHeight				()				const			{ assert(_bInitialized); return _frustum.aspectWidthOverHeight; }				

			// Coefficient to be used in the projection matrix as the X scale cofficient
			FloatTypeArg											GetWidthCoeff			()				const		{ assert(_bInitialized); return GetCommon().GetWidthCoeff(); }
			// Coefficient to be used in the projection matrix as the Y scale cofficient
			FloatTypeArg											GetHeightCoeff			()				const		{ assert(_bInitialized); return GetCommon().GetHeightCoeff(); }
			// Depth of the projection viewport
			FloatTypeArg											GetDepth				()				const		{ assert(_bInitialized); return GetCommon().GetDepth(); }

			/*******************************************************
			* Setters
			********************************************************/
			void SetFrustum								(const FrustumInfo&		InNewFrustumValue);
			void SetNearZ								(FloatTypeArg			InNewNearZ);
			void SetFarZ								(FloatTypeArg			InNewFarZ);
			void SetHalfFov								(FloatTypeArg			InNewHalfFov);
			void SetAspectWidthOverHeight				(FloatTypeArg			InNewAspecWidthOverHeight);

			/*******************************************************
			* construction and initialization
			*******************************************************/
			// default ctor: create matrix for the default frustum
			// does NOT perform ANY initialization
			// (class state is incorrect until you call Initialize())
			ProjectionCache_ForwardPerspective								();			
			
			// Perform the initialization (see the notes in the class description)
			void Initialize													(const FrustumInfo& InFrustum);

		private:
			void _Set_frustum_and_dependent							(const FrustumInfo& InNewFrustumValue);
			void _Set_Q_and_dependent								();
			void _Set_QZnear_and_dependent							();
			void _Set_HalfFov_and_dependent							();
			void _Set_WidthCoeff_and_dependent						();
			void _Set_HeightCoeff_and_dependent						();

			void _Recalculate_Depth									();

			void _Set_near_z_and_dependent							(FloatTypeArg InNewNearZ);
			void _Set_far_z_and_dependent							(FloatTypeArg InNewFarZ);
			void _Set_aspect_width_over_height_and_dependent		(FloatTypeArg InNewAspectWidthOverHeight);


			FrustumInfo									 _frustum;
			CommonCoeffCacheType				     _cacheCommon;	
			FloatTypeArg							 _tan_halfFov;
			FloatTypeArg								       _Q;			
			FloatTypeArg								  _QZNear;
			MatrixType									  _matrix;
			// helper field for asserts
			bool										 _bInitialized;
		};		

		/*****************************************************************************************
		* Calculation function.
		* WARNING!!! For impl only, use getters instead
		*****************************************************************************************/
		template<class FloatTypeArg>
		SquareMatrix<FloatTypeArg, 4>			CalcProjectionMatrix(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache);

		template<class FloatTypeArg>
		FloatTypeArg							Calc_Q				(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache);

		template<class FloatTypeArg>
		FloatTypeArg							Calc_QZnear			(const ProjectionCache_ForwardPerspective<FloatTypeArg> &InCache);

		template<class FloatTypeArg>
		FloatTypeArg							Calc_Tan_HalfFov	(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache);

		// precondition: Tan half fov should be correctly evaluated
		template<class FloatTypeArg>
		FloatTypeArg							Calc_WidthCoeff		(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache);

		// precondition: width coeff should be correctly evaluated
		template<class FloatTypeArg>
		FloatTypeArg							Calc_HeightCoeff	(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache);

		// precondition: Znear and Zfar should be correctly evaluated
		template<class FloatTypeArg>
		FloatTypeArg							Calc_Depth	(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache);


		/********************************************************************************************
		* global function IMPLEMENTATION
		********************************************************************************************/
		template<class FloatTypeArg>
		SquareMatrix<FloatTypeArg, 4> CalcProjectionMatrix(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache)
		{
			assert(&InCache);
			return CalcSquareMatrix_PerspectiveProjection
			(				
				InCache.GetQ(), 
				InCache.GetZNear(), 
				InCache.GetWidthCoeff(),
				InCache.GetHeightCoeff()
			);
		}

		template<class FloatTypeArg>
		FloatTypeArg Calc_Q(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache)
		{
			return InCache.GetDepth()/InCache.GetZFar();
		}

		template<class FloatTypeArg>
		FloatTypeArg Calc_QZnear(const ProjectionCache_ForwardPerspective<FloatTypeArg> &InCache)
		{
			return InCache.GetQ() * InCache.GetZNear();
		}

		template<class FloatTypeArg>
		FloatTypeArg Calc_Tan_HalfFov(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache)
		{
			return tan(InCache.GetHalfFov());
		}
		
		template<class FloatTypeArg>
		FloatTypeArg Calc_WidthCoeff(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache)
		{
			return (static_cast<FloatTypeArg>(1))/InCache.GetTanHalfFov();
		}
		
		template<class FloatTypeArg>
		FloatTypeArg Calc_HeightCoeff(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache)
		{
			return InCache.GetAspectWidthOverHeight() * InCache.GetWidthCoeff();
		}

		template<class FloatTypeArg>
		FloatTypeArg Calc_Depth(const ProjectionCache_ForwardPerspective<FloatTypeArg>& InCache)
		{
			assert(InCache.GetZFar() > InCache.GetZNear());
			return InCache.GetZFar() - InCache.GetZNear();
		}

		/********************************************************************************************
		* member-function IMPLEMENTATION
		********************************************************************************************/
		template<class FloatTypeArg>
		ProjectionCache_ForwardPerspective<FloatTypeArg>::ProjectionCache_ForwardPerspective() :
			// solution of using the std::numeric_limits conflicts with min and max 
			// global functions defined in the windows header
			_Q(FLT_MAX), 
			_QZNear(FLT_MAX),
			_tan_halfFov(FLT_MAX),
			_bInitialized(false)
		{
			// COMMENT:
			// NO initialization here and that's intended
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::Initialize(const FrustumInfo& InFrustum)
		{
			assert(&InFrustum);
			// COMMENT set initialized flag at the start of the initialization
			// (because some operations used during initialization
			// may check this flag is set)
			_bInitialized = true; 
			_Set_frustum_and_dependent(InFrustum);
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::SetFrustum(const FrustumInfo& InFrustum)
		{
			assert(_bInitialized); 
			assert(&InFrustum);
			_Set_frustum_and_dependent(InFrustum);
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::SetNearZ(FloatTypeArg InNewNearZ)
		{		
			assert(_bInitialized); 
			_Set_near_z_and_dependent(InNewNearZ);
			_matrix = CalcProjectionMatrix(*this);
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::SetFarZ(FloatTypeArg InNewFarZ)
		{			
			assert(_bInitialized); 
			_Set_far_z_and_dependent(InNewFarZ);
			_matrix = CalcProjectionMatrix(*this);
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::SetHalfFov(FloatTypeArg InNewHalfFov)
		{
			assert(_bInitialized); 
			_frustum.geo.halfFov = InNewHalfFov;
			_Set_HalfFov_and_dependent();
			_matrix = CalcProjectionMatrix(*this);
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::SetAspectWidthOverHeight(FloatTypeArg InNewAspecWidthOverHeight)
		{
			assert(_bInitialized); 
			assert(InNewAspecWidthOverHeight > 0);
			_Set_aspect_width_over_height_and_dependent(InNewAspecWidthOverHeight);
			_matrix = CalcProjectionMatrix(*this);
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::_Set_frustum_and_dependent(const FrustumInfo& InNewFrustumValue)
		{
			_frustum = InNewFrustumValue;
			_Recalculate_Depth();
			_Set_Q_and_dependent();
			_Set_HalfFov_and_dependent();
			_matrix = CalcProjectionMatrix(*this);
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::_Set_Q_and_dependent()
		{
			_Q = Calc_Q(*this);
			_Set_QZnear_and_dependent();			
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::_Set_QZnear_and_dependent()
		{
			_QZNear = Calc_QZnear(*this);
			// COMMENT: no variables depend on QZnear,
			// that's why no update here
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::_Set_HalfFov_and_dependent()
		{			
			_tan_halfFov = Calc_Tan_HalfFov(*this);
			_Set_WidthCoeff_and_dependent();
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::_Set_WidthCoeff_and_dependent()
		{
			// This string was COMMENTED because it looked like a bug and compile error:
			// WAS BUG?: 
			// 1) The _cacheCommon should contain the coeffs
			// 2) There's no field _width_coeff at all;			
			//_width_coeff = Calc_WidthCoeff(*this);
			// To repair the bug, the width_coeff to be calculated as such:
			FloatTypeArg width_coeff = Calc_WidthCoeff(*this);
			_cacheCommon.SetWidthCoeff(width_coeff);

			_Set_HeightCoeff_and_dependent();
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::_Set_HeightCoeff_and_dependent()
		{
			// This string was COMMENTED because it looked like a bug and compile error:
			// WAS BUG?: 
			// 1) The _cacheCommon should contain the coeffs
			// 2) There's no field _height_coeff at all;			
			//_height_coeff = Calc_HeightCoeff(*this);
			// To repair the bug, the height_coeff to be calculated as such:
			FloatTypeArg height_coeff = Calc_HeightCoeff(*this);
			_cacheCommon.SetHeightCoeff(height_coeff);			
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::_Set_near_z_and_dependent(FloatTypeArg InNewNearZ)
		{
			_frustum.geo.zNear = InNewNearZ;		
			_Recalculate_Depth();
			_Set_QZnear_and_dependent();
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::_Set_far_z_and_dependent(FloatTypeArg InNewFarZ)
		{
			_frustum.geo.zFar = InNewFarZ;
			_Recalculate_Depth();
			_Set_Q_and_dependent();
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::_Set_aspect_width_over_height_and_dependent(FloatTypeArg InNewAspectWidthOverHeight)
		{
			_frustum.aspectWidthOverHeight = InNewAspectWidthOverHeight;
			_Set_HeightCoeff_and_dependent();
		}

		template<class FloatTypeArg>
		void ProjectionCache_ForwardPerspective<FloatTypeArg>::_Recalculate_Depth()
		{
			FloatTypeArg new_depth = Calc_Depth(*this);
			_cacheCommon.SetDepth(new_depth);
		}
	} // Math
} // DvUtils