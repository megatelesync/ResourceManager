#pragma once
#include "MathUtil_3DMath_Utils.h"
#include "MathUtil_3DMath_FrustumInfo.h"

namespace DvUtils
{
	namespace Math
	{
		/****************************************************************************************
		* TODO: 
		* 1. impl _Set_HalfFov_and_dependent() (CODED: 22:59/29.09.2016)
		****************************************************************************************/

		// Calculates and stores coefficients useful for calculating
		// the perspective projection matrix
		//
		// WARNING: 
		// If you need a class that represents matrix as SquareMatrix,
		// use the corresponding class that is defined in the following header file:
		// MathTypes/Proj/MathUtil_MathTypes_ProjectionCache_ForwardPerspective.h
		template<class FloatTypeArg>
		class PerspectiveMatrixCache
		{
		public:
			/*******************************************************
			* typedefs
			*******************************************************/
			typedef FloatTypeArg									FloatType;
			typedef ProjectionMatrixCache<FloatTypeArg>				ProjectionMatrixCacheType;


			/*******************************************************
			* getters
			*******************************************************/
			const FrustumInfo&										GetFrustumInfo			()				const		{ return _frustum; }
			// get cache for variables useful for calculating both ortho and perspective proj matrices
			const ProjectionMatrixCacheType&						GetCommon				()				const		{ return _common; }
			// tan of the half FOV (field-of-view) angle
			FloatTypeArg											GetTanHalfFov			()				const		{ return _tan_halfFov; }
			//  Q = depth/ZFar
			FloatTypeArg											GetQ					()				const		{ return _Q; }
			FloatTypeArg											GetQZNear				()				const		{ return _QZNear; }


			/*******************************************************
			* helper getters
			*******************************************************/
			FloatTypeArg											GetZNear								()				const			{ return _frustum.GetZNear(); }
			FloatTypeArg											GetZFar									()				const			{ return _frustum.GetZFar(); }
			FloatTypeArg											GetHalfFov								()				const			{ return _frustum.GetHalfFov(); }				
			FloatTypeArg											GetAspectWidthOverHeight				()				const			{ return _frustum.aspectWithOverHeight; }				

			// Coefficient to be used in the projection matrix as the X scale cofficient
			FloatTypeArg											GetWidthCoeff			()				const		{ return GetCommon().GetWidthCoeff(); }
			// Coefficient to be used in the projection matrix as the Y scale cofficient
			FloatTypeArg											GetHeightCoeff			()				const		{ return GetCommon().GetHeightCoeff(); }
			// Depth of the projection viewport
			FloatTypeArg											GetDepth				()				const		{ return GetCommon().GetDepth(); }


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
			PerspectiveMatrixCache();			
			PerspectiveMatrixCache(const FrustumInfo& InFrustum);			

		private:
			void _Set_frustum_and_dependent							(const FrustumInfo& InNewFrustumValue);
			void _Set_Q_and_dependent								();
			void _Set_QZnear_and_dependent							();
			void _Set_HalfFov_and_dependent							();
			void _Set_WidthCoeff_and_dependent						();
			void _Set_HeightCoeff_and_dependent						();

			void _Set_aspect_width_over_height_and_dependent		(FloatTypeArg InNewAspectWidthOverHeight);			
			void _Set_near_z_and_dependent							(FloatTypeArg InNewNearZ);
			void _Set_far_z_and_dependent							(FloatTypeArg InNewFarZ);

			FrustumInfo									_frustum;
			ProjectionMatrixCacheType				     _common;	
			FloatTypeArg							_tan_halfFov;
			FloatTypeArg								      _Q;			
			FloatTypeArg								 _QZNear;
		};

		template<class FloatTypeArg>
		DirectX::XMMATRIX CalcProjectionMatrix(const PerspectiveMatrixCache<FloatTypeArg>& InCache);


		/*****************************************************************************************
		* Calculation function.
		* WARNING!!! For impl only, use getters instead
		*****************************************************************************************/
		template<class FloatTypeArg>
		FloatTypeArg							Calc_Q				(const PerspectiveMatrixCache<FloatTypeArg>& InCache);

		// precondition: Q and ZNear should be correctly calculated
		template<class FloatTypeArg>
		FloatTypeArg							Calc_QZnear			(const PerspectiveMatrixCache<FloatTypeArg> &Cache);

		template<class FloatTypeArg>
		FloatTypeArg							Calc_Tan_HalfFov	(const PerspectiveMatrixCache<FloatTypeArg>& InCache);

		// precondition: Tan half fov should be correctly evaluated
		template<class FloatTypeArg>
		FloatTypeArg							Calc_WidthCoeff		(const PerspectiveMatrixCache<FloatTypeArg>& InCache);

		// precondition: width coeff should be correctly evaluated
		template<class FloatTypeArg>
		FloatTypeArg							Calc_HeightCoeff	(const PerspectiveMatrixCache<FloatTypeArg>& InCache);
		
		template<class FloatTypeArg>
		void Calc_ProjectionMatrixCache
		(
			typename PerspectiveMatrixCache<FloatTypeArg>::ProjectionMatrixCacheType			*pOutCache,
			const PerspectiveMatrixCache<FloatTypeArg>&											 InCache
		);





		// PerspectiveMatrixCache impl
		template<class FloatTypeArg>
		DirectX::XMMATRIX CalcProjectionMatrix(const PerspectiveMatrixCache<FloatTypeArg>& InCache)
		{
			return CalcPerspectiveProjMatrix
			(
				InCache.GetDepth(), 
				InCache.GetQ(), 
				InCache.GetZNear(), 
				InCache.GetWidthCoeff(),
				InCache.GetHeightCoeff()
			);
		}

		template<class FloatTypeArg>
		PerspectiveMatrixCache<FloatTypeArg>::PerspectiveMatrixCache() :
			_Q(1.0F), _QZNear(1.0F), _tan_halfFov(1.0F)
		{
			_Set_frustum_and_dependent(FrustumInfo());
		}

		template<class FloatTypeArg>
		PerspectiveMatrixCache<FloatTypeArg>::PerspectiveMatrixCache(const FrustumInfo& InFrustum) :
			_Q(1.0F), _QZNear(1.0F), _tan_halfFov(1.0F)
		{
			_Set_frustum_and_dependent(InFrustum);
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::SetFrustum(const FrustumInfo& InNewFrustumValue)
		{
			_Set_frustum_and_dependent(InFrustum);
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::SetNearZ(FloatTypeArg InNewNearZ)
		{
			_Set_near_z_and_dependent(InNewNearZ);
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::SetFarZ(FloatTypeArg InNewFarZ)
		{
			_Set_far_z_and_dependent(InNewFarZ);
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::SetHalfFov(FloatTypeArg InNewHalfFov)
		{
			_Set_half_FOV_and_dependent(InNewHalfFov);
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::SetAspectWidthOverHeight(FloatTypeArg InNewAspecWidthOverHeight)
		{
			_Set_aspect_width_over_height_and_dependent(InNewAspecWidthOverHeight);
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::_Set_frustum_and_dependent(const FrustumInfo& InNewFrustumValue)
		{
			_frustum = InNewFrustumValue;
			Calc_ProjectionMatrixCache(&_common, *this);
			_Set_Q_and_dependent();
			_Set_HalfFov_and_dependent();			
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::_Set_Q_and_dependent()
		{
			_Q = Calc_Q(*this);
			_Set_QZnear_and_dependent();
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::_Set_QZnear_and_dependent()
		{
			_QZnear = Calc_QZnear(*this);
			// COMMENT: no variables depend on QZnear,
			// that's why no update here
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::_Set_HalfFov_and_dependent()
		{
			_tan_halfFov = Calc_Tan_HalfFov(*this);
			_Set_WidthCoeff_and_dependent();
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::_Set_WidthCoeff_and_dependent()
		{
			_width_coeff = Calc_WidthCoeff(*this);
			_Set_HeightCoeff_and_dependent();
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::_Set_HeightCoeff_and_dependent()
		{
			_height_coeff = Calc_HeightCoeff(*this);
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::_Set_aspect_width_over_height_and_dependent(FloatTypeArg InNewAspectWidthOverHeight)
		{
			_frustum.aspectWidthOverHeight = InNewAspectWidthOverHeight;
			_Set_HeightCoeff_and_dependent();
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::_Set_half_FOV_and_dependent(FloatTypeArg InNewHalfFov)
		{
			_frustum.geo.halfFov = InNewHalfFov;
			_Set_tan_HalfFov_and_dependent()
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::_Set_near_z_and_dependent(FloatTypeArg InNewNearZ)
		{
			_frustum.geo.zNear = InNewNearZ;		
			_Set_QZnear_and_dependent();
		}

		template<class FloatTypeArg>
		void PerspectiveMatrixCache<FloatTypeArg>::_Set_far_z_and_dependent(FloatTypeArg InNewFarZ)
		{
			_frustum.geo.zFar = InNewFarZ;
			_Set_Q_and_dependent();
		}

		template<class FloatTypeArg>
		FloatTypeArg Calc_Q(const PerspectiveMatrixCache<FloatTypeArg>& InCache)
		{
			return InCache.GetDepth()/InCache.GetZFar();
		}

		template<class FloatTypeArg>
		FloatTypeArg Calc_QZnear(const PerspectiveMatrixCache<FloatTypeArg> &Cache)
		{
			return Cache.GetQ() * Cache.GetZNear();
		}

		template<class FloatTypeArg>
		FloatTypeArg Calc_Tan_HalfFov(const PerspectiveMatrixCache<FloatTypeArg>& InCache)
		{
			return tan(InCache.GetHalfFov());
		}

		template<class FloatTypeArg>
		FloatTypeArg Calc_WidthCoeff(const PerspectiveMatrixCache<FloatTypeArg>& InCache)
		{
			return 1.0F/InCache.GetTanHalfFov();
		}

		template<class FloatTypeArg>
		FloatTypeArg Calc_HeightCoeff(const PerspectiveMatrixCache<FloatTypeArg>& InCache)
		{
			return InCacheGetAspectWidthOverHeight() * InCache.GetWidthCoeff();
		}

		template<class FloatTypeArg>
		void Calc_ProjectionMatrixCache
		(
			typename PerspectiveMatrixCache<FloatTypeArg>::ProjectionMatrixCacheType			*pOutCache,
			const PerspectiveMatrixCache<FloatTypeArg>											&InCache
		)
		{
			assert(pOutCache != nullptr);
			typename FloatTypeArg width_coeff		= InCache.GetWidthCoeff();
			typename FloatTypeArg height_coeff		= InCache.GetHeightCoeff();
			FloatTypeArg depth_coeff				= FrustumDepth(InCache.GetFrustum());
			pOutCache->Set(width_coeff, depth_coeff, height_coeff);
		}
	} // Math
} // DvUtils