#pragma once
#include "MathUtil_3DMath_PerspectiveMatrixCache.h"

namespace DvUtils
{
	namespace Math
	{
		/*******************************************************************************
		* TODO:
		* 1. _Set_frustum_and_dependent(const FrustumInfo& InNewFrustumValue)
		* should update all inversed coeffs (BUG?)
		*******************************************************************************/

		// Calculates and stores coefficients useful for calculating
		// both the perspective projection matrix and the inversed perspective projection matrix
		//
		// WARNING: 
		// If you use the SquareMatrix class to represent matrices
		// (instead of the XMMATRIX), then you should use the 
		// corresponding class in the MathTypes/Proj/MathUtil_MathTypes_ProjectionCache.h		
		template<class FloatTypeArg>
		class ReversiblePerspectiveMatrixCache
		{
		public:
			/*******************************************************
			* typedefs
			*******************************************************/
			typedef FloatTypeArg									FloatType;
			typedef PerspectiveMatrixCache<FloatTypeArg>			PerspectiveMatrixCacheType;
			typedef ProjectionMatrixCache<FloatTypeArg>				ProjectionMatrixCacheType;


			/*******************************************************
			* getters
			*******************************************************/
			const PerspectiveMatrixCacheType&						GetPerspectiveMatrixCache		()				const		{ return _matrixCache; }			
			FloatTypeArg											GetInversedZNear				()				const		{ return _inversed_ZNear; }
			FloatTypeArg											GetInversedQZNear				()				const		{ return _inversed_QZNear; }
			FloatTypeArg											GetInversedWidthCoeff			()				const		{ return _inversed_WidthCoeff; }
			FloatTypeArg											GetInversedHeightCoeff			()				const		{ return _inversed_HeightCoeff; }


			/*******************************************************
			* helper getters
			*******************************************************/
			const FrustumInfo&										GetFrustumInfo							()				const			{ return _matrixCache.GetFrustumInfo(); }
			// get cache for variables useful for calculating both ortho and perspective proj matrices
			const ProjectionMatrixCacheType&						GetCommon								()				const			{ return _matrixCache.GetCommon(); }
			// tan of the half FOV (field-of-view) angle
			FloatTypeArg											GetTanHalfFov							()				const			{ return _matrixCache.GetHalfFov(); }
			//  Q = depth/ZFar
			FloatTypeArg											GetQ									()				const			{ return _matrixCache.GetQ(); }
			FloatTypeArg											GetQZNear								()				const			{ return _matrixCache.GetQZNear(); }

			FloatTypeArg											GetZNear								()				const			{ return _matrixCache.GetZNear(); }
			FloatTypeArg											GetZFar									()				const			{ return _matrixCache.GetZFar(); }
			FloatTypeArg											GetHalfFov								()				const			{ return _matrixCache.GetHalfFov(); }				
			FloatTypeArg											GetAspectWidthOverHeight				()				const			{ return _matrixCache.GetAspectWidthOverHeight(); }				

			// Coefficient to be used in the projection matrix as the X scale cofficient
			FloatTypeArg											GetWidthCoeff			()				const		{ return _matrixCache.GetWidthCoeff(); }
			// Coefficient to be used in the projection matrix as the Y scale cofficient
			FloatTypeArg											GetHeightCoeff			()				const		{ return _matrixCache.GetHeightCoeff(); }
			// Depth of the projection viewport
			FloatTypeArg											GetDepth				()				const		{ return _matrixCache.GetDepth(); }


			/*******************************************************
			* setters
			*******************************************************/
			void SetFrustum								(const FrustumInfo&		InNewFrustumValue);
			void SetNearZ								(FloatTypeArg			InNewNearZ);
			void SetFarZ								(FloatTypeArg			InNewFarZ);
			void SetHalfFov								(FloatTypeArg			InNewHalfFov);
			void SetAspectWidthOverHeight				(FloatTypeArg			InNewAspecWidthOverHeight)


			/*******************************************************
			* ctors
			*******************************************************/
			// default ctor: create matrix for the default frustum
			ReversiblePerspectiveMatrixCache();
			ReversiblePerspectiveMatrixCache(const FrustumInfo& InFrustum);


		private:			
			PerspectiveMatrixCacheType								_matrixCache;
			FloatTypeArg											_inversed_ZNear;
			FloatTypeArg											_inversed_QZNear;
			FloatTypeArg											_inversed_WidthCoeff;
			FloatTypeArg											_inversed_HeightCoeff;

			void _Set_frustum_and_dependent							(const FrustumInfo& InNewFrustumValue);
			void _Set_HalfFov_and_dependent							(FloatTypeArg InNewHalfFovValue);
			void _Set_WidthCoeff_and_dependent						();
			void _Set_HeightCoeff_and_dependent						();

			void _Set_aspect_width_over_height_and_dependent		(FloatTypeArg InNewAspectWidthOverHeight);
			void _Set_near_z_and_dependent							(FloatTypeArg InNewNearZ);
			void _Set_far_z_and_dependent							(FloatTypeArg InNewFarZ);

			void _SetInversedWidthAndHeightCoeffs					();
			void _SetInversedHeightCoeff							();
			void _SetInversedQZNear									();
		};

		// Calculate the projection matrix
		template<class FloatTypeArg>
		DirectX::XMMATRIX CalcProjectionMatrix(const ReversiblePerspectiveMatrixCache<FloatTypeArg>& InCache);

		// Calculate the inversed projection matrix
		template<class FloatTypeArg>
		DirectX::XMMATRIX CalcInversedProjectionMatrix(const ReversiblePerspectiveMatrixCache<FloatTypeArg>& InCache);





		// IMPL
		template<class FloatTypeArg>
		DirectX::XMMATRIX CalcProjectionMatrix(const ReversiblePerspectiveMatrixCache<FloatTypeArg>& InCache)
		{
			return CalcProjectionMatrix(InCache.GetPerspectiveMatrixCache());
		}

		template<class FloatTypeArg>
		DirectX::XMMATRIX CalcInversedProjectionMatrix(const ReversiblePerspectiveMatrixCache<FloatTypeArg>& InCache)
		{
			return CalcPerspectiveUnProjMatrix_UsingCoeff
			(
				InCache.GetInversedZNear(),
				InCache.GetInversedQZNear(),
				InCache.GetInversedWidthCoeff(),
				InCache.GetInversedHeightCoeff()
			);
		}

		template<class FloatTypeArg>
		ReversiblePerspectiveMatrixCache<FloatTypeArg>::ReversiblePerspectiveMatrixCache() :
			_inversed_ZNear(1.0F),
			_inversed_QZNear(1.0F),
			_inversed_WidthCoeff(1.0F),
			_inversed_HeightCoeff(1.0F)
		{
			_Set_frustum_and_dependent(FrustumInfo());
		}

		template<class FloatTypeArg>
		ReversiblePerspectiveMatrixCache<FloatTypeArg>::ReversiblePerspectiveMatrixCache(const FrustumInfo& InFrustum) :
			_inversed_ZNear(1.0F),
			_inversed_QZNear(1.0F),
			_inversed_WidthCoeff(1.0F),
			_inversed_HeightCoeff(1.0F)
		{
			_Set_frustum_and_dependent(InNewFrustumValue);
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::SetFrustum(const FrustumInfo& InNewFrustumValue)
		{
			_Set_frustum_and_dependent(InNewFrustumValue);
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::SetNearZ(FloatTypeArg InNewNearZ)
		{
			_Set_near_z_and_dependent(InNewNearZ);
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::SetFarZ(FloatTypeArg nNewFarZ)
		{
			_Set_far_z_and_dependent(nNewFarZ);
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::SetHalfFov(FloatTypeArg InNewHalfFov)
		{
			_Set_HalfFov_and_dependent(InNewHalfFov);
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::SetAspectWidthOverHeight(FloatTypeArg InNewAspecWidthOverHeight)
		{
			_Set_aspect_width_over_height_and_dependent(InNewAspecWidthOverHeight);
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::_Set_frustum_and_dependent(const FrustumInfo& InNewFrustumValue)
		{
			_matrixCache.SetFrustum(InNewFrustumValue);
			// BUG?: Why no update of the inverse coeffs?
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::_Set_HalfFov_and_dependent(FloatTypeArg InNewHalfFovValue)
		{
			_matrixCache.SetHalfFov(InNewHalfFovValue);
			_SetInversedWidthAndHeightCoeffs();
		}		

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::_SetInversedWidthAndHeightCoeffs()
		{
			_inversed_WidthCoeff = 1.0F/_matrixCache.GetWidthCoeff();
			_SetInversedHeightCoeff();
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::_SetInversedHeightCoeff()
		{
			_inversed_HeightCoeff = 1.0F/_matrixCache.GetHeightCoeff();
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::_Set_aspect_width_over_height_and_dependent(FloatTypeArg InNewAspectWidthOverHeight)
		{
			_matrixCache.SetAspectWidthOverHeight(InNewAspectWidthOverHeight);
			_SetInversedHeightCoeff();
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::_Set_near_z_and_dependent(FloatTypeArg InNewNearZ)
		{
			_matrixCache.SetNearZ(InNewNearZ);
			_inversed_ZNear = 1.0F/InNewNearZ;
			_SetInversedQZNear();
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::_Set_far_z_and_dependent(FloatTypeArg InNewFarZ)
		{
			_matrixCache.SetFarZ(InNewFarZ);
			// BUG? Why to recompute QZNear when the inversed ZFar updated?
			// No, it's not bug:
			// COMMENT: The Q is dependent on the NewFarZ, 
			// and Q is automatically recomputed when the new far z is updated.
			_SetInversedQZNear();
		}

		template<class FloatTypeArg>
		void ReversiblePerspectiveMatrixCache<FloatTypeArg>::_SetInversedQZNear()
		{
			_inversed_QZNear = 1.0F/GetQZNear();
		}
	} // Math
} // DvUtils