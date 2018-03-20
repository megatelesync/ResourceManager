#pragma once

#include <DirectXMath.h>

namespace DvUtils
{
	namespace Math
	{				
		/*****************************************************************************
		* ViewProj matrix calculation
		*****************************************************************************/
		// Calculate mult of view and ortho matrix
		inline DirectX::XMMATRIX CalcViewOrthoMatrix
		(
			float InWidth,	float InHeight,
			float InNear,	float InFar,
			float vertical, float horz,
			const DirectX::XMFLOAT3& position
		);

		// Calculate matrix for cancelling out both view and ortho projection
		inline DirectX::XMMATRIX CalcInverseViewOrthoMatrix
		(
			float InWidth,	float InHeight,
			float InNear,	float InFar,
			float vertical, float horz,
			const DirectX::XMFLOAT3& position
		);


		/*****************************************************************************
		* Projection matrix calculation
		*****************************************************************************/
		// coefficients useful for calculation of both ortho and perspective projection matrices
		template<class FloatTypeArg>
		class ProjectionMatrixCache
		{
		public:
			/*******************************************************
			* typedefs
			*******************************************************/
			typedef FloatTypeArg				FloatType;


			/*******************************************************
			* getters
			*******************************************************/
			// Coefficient to be used in the projection matrix as the X scale cofficient
			FloatTypeArg						GetWidthCoeff			()				const		{ return _width_coeff; }
			// Coefficient to be used in the projection matrix as the Y scale cofficient
			FloatTypeArg						GetHeightCoeff			()				const		{ return _height_coeff; }
			// Depth of the projection viewport
			FloatTypeArg						GetDepth				()				const		{ return _depth; }


			/*******************************************************
			* setters
			*******************************************************/
			inline void							Set							(FloatTypeArg InWidthCoef, FloatTypeArg InHeightCoeff, FloatTypeArg InDepth);
			inline void							SetWidthCoeffAndDepth		(FloatTypeArg InWidthCoeff, FloatTypeArg InDepth);
			inline void							SetHeightCoeffAndDepth		(FloatTypeArg InHeightCoeff, FloatTypeArg InDepth);			
			inline void							SetWidthCoeff				(FloatTypeArg InWidthCoeff);
			inline void							SetHeightCoeff				(FloatTypeArg InHeightCoeff);
			inline void							SetDepth					(FloatTypeArg InDepth);


			/*******************************************************
			* constructors
			*******************************************************/
			// default ctor: create matrix with unit width/height coeffs and unit depth
			inline ProjectionMatrixCache() :
				_width_coeff(1.0F), _height_coeff(1.0F), _depth(1.0F) {}
			inline ProjectionMatrixCache
			(
				FloatTypeArg		InWidthCoeff, 
				FloatTypeArg		InHeightCoeff, 
				FloatTypeArg		InDepth
			);

		private:
			FloatTypeArg					_width_coeff;
			FloatTypeArg					_height_coeff;
			FloatTypeArg					_depth;			
		};

		
		// calculate the matrix to perform the orthographic projection
		// (transforms from view space to projection space)
		inline DirectX::XMMATRIX CalcOrthoProjectionMatrix
		(
			float InWidth,
			float InHeight,
			float InNear,
			float InFar
		);
		// calculate the matrix to perform transformation 
		// from projection space to view space
		inline DirectX::XMMATRIX CalcOrthoUnProjectionMatrix
		(
			float InWidth,
			float InHeight,
			float InNear,
			float InFar
		);

		// Calculate the perspective projection matrix
		inline DirectX::XMMATRIX CalcPerspectiveProjMatrix
		(
			float ZFar, float ZNear,
			float halfFov,
			float aspectWidthOverHeight
		);
		inline DirectX::XMMATRIX CalcPerspectiveUnProjMatrix
		(
			float ZFar, float ZNear,
			float halfFov,
			float aspectWidthOverHeight
		);
		inline DirectX::XMMATRIX CalcPerspectiveProjMatrix
		(
			float depth, 
			float Q,
			float QZnear,
			float widthCoeff,
			float heightCoeff
		);
		inline DirectX::XMMATRIX CalcPerspectiveUnProjMatrix_UsingCoeff
		(
			float inv_ZNear,
			float inv_QZnear,
			float inv_widthCoeff,
			float inv_heightCeoff			
		);
		// Set the width coeff of the perspective or ortho projection matrix
		inline void SetProjMatrix_WidthCoeff							(DirectX::XMMATRIX *pOutMatrix, float InNewCoeff);
		inline void SetProjMatrix_HeightCoeff							(DirectX::XMMATRIX *pOutMatrix, float InNewCoeff);	
		// Set coefficient in [3][3] (Z-scale coefficient) only
		// (usable both for ortho or perspective projection matrix)
		inline void SetProjMatrix_ZScaleCoeff							(DirectX::XMMATRIX *pOutMatrix, float InNewValue);

		// Set coefficient in [3][4] (Z-bias coefficient) only
		// (usable both for ortho or perspective projection matrix)
		inline void SetProjMatrix_ZBiasCoeff
		(
			DirectX::XMMATRIX		*pOutMatrix, 
			float					 InNewValue			
		);

		// Update the z-scale coefficient (known as Q) of the perspective projection matrix
		// and all the dependent elements
		// (usable for the perspective projection matrix only)
		inline void SetPerspectiveProjMatrix_ZScaleCoeffAndDependent	
		(
			DirectX::XMMATRIX		*pOutMatrix, 
			float					 InNewZScaleCoeffQ,
			float					 InNewQZNear
		);


		/*****************************************************************************
		* Texturing 
		*****************************************************************************/
		// matrix for conversion from NDC space to texture space	
		// properties:
		// -z and w coordinates remain untouched
		inline DirectX::XMMATRIX GetNDCtoTexMatrix();


		/******************************************************************************
		* Affine matrices
		******************************************************************************/
		inline DirectX::XMMATRIX RotationMatrixX(float angle);
		inline DirectX::XMMATRIX RotationMatrixY(float angle);
		inline DirectX::XMMATRIX RotationMatrixZ(float angle);

		inline DirectX::XMMATRIX RotationMatrixXVertZHorz(float vertical, float horz);
		inline DirectX::XMMATRIX RotationMatrixXVertYHorz(float vertical, float horz);

		inline DirectX::XMMATRIX CalcAffineMatrix(float vertical, float horz, const DirectX::XMFLOAT3& bias);
		inline DirectX::XMMATRIX CalcInverseAffineMatrix(float vertical, float horz, const DirectX::XMFLOAT3& bias);
		




		// Impl
		inline DirectX::XMMATRIX CalcPerspectiveProjMatrix
		(
			float ZFar, float ZNear,
			float halfFov,
			float aspectWidthOverHeight
		)
		{			
			float depth							= ZFar-ZNear;
			float Q								= depth/ZFar;
			float QZnear						= Q * ZNear;
			float tan_halfFOV					= std::tan(halfFov);
			float widthCoeff					= 1.0F/tan_halfFOV;
			float heightCoeff					= widthCoeff * aspectWidthOverHeight;

			return CalcPerspectiveProjMatrix
			(
				depth, 
				Q, QZnear,
				widthCoeff,heightCoeff
			);
		}

		inline DirectX::XMMATRIX CalcPerspectiveUnProjMatrix
		(
			float ZFar, float ZNear,
			float halfFov,
			float aspectWidthOverHeight
		)
		{
			DirectX::XMMATRIX mat = CalcPerspectiveProjMatrix(ZFar, ZNear, halfFov, aspectWidthOverHeight);
			return DirectX::XMMatrixInverse(nullptr, mat);
		}

		inline DirectX::XMMATRIX CalcPerspectiveProjMatrix
		(
			float depth, 
			float Q,
			float QZnear,
			float widthCoeff,
			float heightCoeff
		)
		{
			DirectX::XMMATRIX res;

			DirectX::XMFLOAT4 rowZero		(widthCoeff,		0.0F,				0.0F,			0.0F);
			DirectX::XMFLOAT4 rowOne		(0.0F,				heightCoeff,		0.0F,			0.0F);
			DirectX::XMFLOAT4 rowTwo		(0.0F,				0.0F,				Q,				1.0F);
			DirectX::XMFLOAT4 rowThree		(0.0F,				0.0F,				-QZnear,		0.0F);
			res.r[0] = DirectX::XMLoadFloat4(&rowZero);
			res.r[1] = DirectX::XMLoadFloat4(&rowOne);
			res.r[2] = DirectX::XMLoadFloat4(&rowTwo);
			res.r[3] = DirectX::XMLoadFloat4(&rowThree);
			return res;
		}

		inline DirectX::XMMATRIX CalcOrthoProjectionMatrix
		(
			float InWidth,
			float InHeight,
			float InNear,
			float InFar
		)
		{
			DirectX::XMMATRIX result_matrix;

			float depth								= InFar - InNear;
			float neg_depth							= -depth;

			float two_over_width					= 2.0F/InWidth;
			float two_over_height					= 2.0F/InHeight;
			float two_over_depth					= 2.0F/depth;
			float near_over_neg_depth				= InNear/neg_depth;

			result_matrix = DirectX::XMMatrixSet
			(
				two_over_width,				   0.0F,			  0.0F,			0.0F,
				0.0F,				two_over_height,			  0.0F,			0.0F,
				0.0F,						   0.0F,    two_over_depth,			0.0F,
				0.0F,						   0.0F,	near_over_neg_depth,    1.0F
			);

			return result_matrix;
		}

		inline DirectX::XMMATRIX CalcOrthoUnProjectionMatrix
		(
			float InWidth,
			float InHeight,
			float InNear,
			float InFar
		)
		{
			float depth								= (InFar - InNear);

			float width_over_two					= InWidth / 2.0F;
			float height_over_two					= InHeight / 2.0F;
			float depth_over_two					= depth / 2.0F;
			float neg_near_over_two					= -InNear/2.0F;

			DirectX::XMMATRIX result_matrix;

			result_matrix = DirectX::XMMatrixSet
			(
				width_over_two,				   0.0F,			  0.0F,						  0.0F,
				0.0F,				height_over_two,			  0.0F,						  0.0F,
				0.0F,						   0.0F,    depth_over_two,			-neg_near_over_two,
				0.0F,						   0.0F,			  0.0F,						  1.0F
			);

			return result_matrix;
		}

		inline void SetProjMatrix_WidthCoeff(DirectX::XMMATRIX *pOutMatrix, float InNewCoeff)
		{
			assert(pOutMatrix != nullptr);
			pOutMatrix->r[0] = DirectX::XMVectorSetX(pOutMatrix->r[0], InNewCoeff);
		}

		inline void SetProjMatrix_HeightCoeff(DirectX::XMMATRIX *pOutMatrix, float InNewCoeff)
		{
			assert(pOutMatrix != nullptr);
			pOutMatrix->r[1] = DirectX::XMVectorSetY(pOutMatrix->r[1], InNewCoeff);
		}
		
		inline void SetProjMatrix_ZScaleCoeff(DirectX::XMMATRIX *pOutMatrix, float InNewValue)
		{
			assert(pOutMatrix != nullptr);
			pOutMatrix->r[2] = DirectX::XMVectorSetZ(pOutMatrix->r[2], InNewValue);
		}
		
		inline void SetProjMatrix_ZBiasCoeff
		(
			DirectX::XMMATRIX		*pOutMatrix, 
			float					 InNewValue			
		)
		{
			assert(pOutMatrix != nullptr);
			pOutMatrix->r[3] = DirectX::XMVectorSetZ(pOutMatrix->r[4], InNewValue);
		}
		
		inline void SetPerspectiveProjMatrix_ZScaleCoeffAndDependent	
		(
			DirectX::XMMATRIX		*pOutMatrix, 
			float					 InNewZScaleCoeffQ,
			float					 InNewQZNear
		)
		{
			assert(pOutMatrix != nullptr);
			SetProjMatrix_ZScaleCoeff	(pOutMatrix, InNewZScaleCoeffQ);
			SetProjMatrix_ZBiasCoeff	(pOutMatrix, InNewQZNear);
		}

		inline DirectX::XMMATRIX CalcPerspectiveUnProjMatrix_UsingCoeff
		(
			float inv_ZNear,
			float inv_QZnear,
			float inv_widthCoeff,
			float inv_heightCoeff			
		)
		{
			DirectX::XMMATRIX result_matrix;

			result_matrix = DirectX::XMMatrixSet
			(
				inv_widthCoeff,							0.0F,				0.0F,					0.0F,
				0.0F,						inv_heightCoeff,				0.0F,					0.0F,
				0.0F,									0.0F,				0.0F,					inv_QZnear,
				0.0F,									0.0F,				1.0F,					inv_ZNear
			);

			return result_matrix;
		}

		inline DirectX::XMMATRIX GetNDCtoTexMatrix()
		{
			DirectX::XMMATRIX			result_matrix;

			result_matrix = DirectX::XMMatrixSet
			(
				0.5F,				   0.0F,			  0.0F,			0.0F,
				0.0F,				  -0.5F,			  0.0F,			0.0F,
				0.0F,				   0.0F,			  1.0F,			0.0F,
				0.5F,				   0.5F,			  0.0F,		    1.0F
			);

			return result_matrix;
		}

		inline DirectX::XMMATRIX RotationMatrixX(float angle)
		{
			DirectX::XMMATRIX res;			
			float cosAngle = std::cosf(angle);
			float sinAngle = std::sinf(angle);
			DirectX::XMFLOAT4 rowZero			(1.0F,		0.0F,			0.0F,			0.0F);
			DirectX::XMFLOAT4 rowOne			(0.0F,		cosAngle,		-sinAngle,		0.0F);
			DirectX::XMFLOAT4 rowTwo			(0.0F,		sinAngle,		cosAngle,		0.0F);
			DirectX::XMFLOAT4 rowThree			(0.0F,		0.0F,			0.0F,			1.0F);							
			res.r[0] = DirectX::XMLoadFloat4(&rowZero);
			res.r[1] = DirectX::XMLoadFloat4(&rowOne);
			res.r[2] = DirectX::XMLoadFloat4(&rowTwo);
			res.r[3] = DirectX::XMLoadFloat4(&rowThree);
			return res;
		}

		inline DirectX::XMMATRIX RotationMatrixY(float angle)
		{
			DirectX::XMMATRIX res;			
			float cosAngle = std::cosf(angle);
			float sinAngle = std::sinf(angle);
			DirectX::XMFLOAT4 rowZero		(cosAngle,		0.0F,			sinAngle,		0.0F);
			DirectX::XMFLOAT4 rowOne		(0.0F,			1.0F,			0.0F,			0.0F);
			DirectX::XMFLOAT4 rowTwo		(-sinAngle,		0.0F,			cosAngle,		0.0F);
			DirectX::XMFLOAT4 rowThree		(0.0F,			0.0F,			0.0F,			1.0F);							
			res.r[0] = DirectX::XMLoadFloat4(&rowZero);
			res.r[1] = DirectX::XMLoadFloat4(&rowOne);
			res.r[2] = DirectX::XMLoadFloat4(&rowTwo);
			res.r[3] = DirectX::XMLoadFloat4(&rowThree);
			return res;
		}

		inline DirectX::XMMATRIX RotationMatrixZ(float angle)
		{
			DirectX::XMMATRIX res;			
			float cosAngle = std::cosf(angle);
			float sinAngle = std::sinf(angle);
			DirectX::XMFLOAT4 rowZero (cosAngle, -sinAngle, 0.0F, 0.0F);
			DirectX::XMFLOAT4 rowOne (sinAngle, cosAngle, 0.0F, 0.0F);
			DirectX::XMFLOAT4 rowTwo (0.0F, 0.0F, 1.0F, 0.0F);
			DirectX::XMFLOAT4 rowThree (0.0F, 0.0F, 0.0F, 1.0F);							
			res.r[0] = DirectX::XMLoadFloat4(&rowZero);
			res.r[1] = DirectX::XMLoadFloat4(&rowOne);
			res.r[2] = DirectX::XMLoadFloat4(&rowTwo);
			res.r[3] = DirectX::XMLoadFloat4(&rowThree);
			return res;
		}

		inline DirectX::XMMATRIX RotationMatrixXVertZHorz(float vertical, float horz)
		{
			DirectX::XMMATRIX rotX		= RotationMatrixX(vertical);
			DirectX::XMMATRIX rotZ		= RotationMatrixZ(horz);
			DirectX::XMMATRIX res		= DirectX::XMMatrixMultiply(rotX,rotZ);
			return res;
		}

		inline DirectX::XMMATRIX RotationMatrixXVertYHorz(float vertical, float horz)
		{
			DirectX::XMMATRIX rotX		= RotationMatrixX(vertical);
			DirectX::XMMATRIX rotY		= RotationMatrixY(horz);
			DirectX::XMMATRIX res		= DirectX::XMMatrixMultiply(rotX,rotY);
			return res;
		}

		inline DirectX::XMMATRIX CalcAffineMatrix(float vertical, float horz, const DirectX::XMFLOAT3& bias)
		{
			DirectX::XMMATRIX resMatrix = RotationMatrixXVertYHorz(vertical, horz);
			DirectX::XMFLOAT4 position4 (bias.x, bias.y, bias.z, 1.0F);
			resMatrix.r[3] = DirectX::XMLoadFloat4(&position4); 
			return resMatrix;
		}

		inline DirectX::XMMATRIX CalcInverseAffineMatrix(float vertical, float horz, const DirectX::XMFLOAT3& bias)
		{
			DirectX::XMMATRIX affineMatrix	= CalcAffineMatrix			(vertical, horz, bias);
			DirectX::XMMATRIX resMatrix		= DirectX::XMMatrixInverse	(nullptr, affineMatrix);
			return resMatrix;
		}

		// ProjectionMatrixCache impl
		template<class FloatTypeArg>
		ProjectionMatrixCache<FloatTypeArg>::ProjectionMatrixCache
		(
			FloatTypeArg		InWidthCoeff, 
			FloatTypeArg		InHeightCoeff, 
			FloatTypeArg		InDepth
		) :
			_width_coeff(InWidthCoeff),
			_height_coeff(InHeightCoeff),
			_depth(InDepth)
		{
		}

		template<class FloatTypeArg>
		void ProjectionMatrixCache<FloatTypeArg>::Set(FloatTypeArg InWidthCoeff, FloatTypeArg InHeightCoeff, FloatTypeArg InDepth)
		{
			_width_coeff = InWidthCoeff;
			_height_coeff = InHeightCoeff;
			_depth = InDepth;
		}

		template<class FloatTypeArg>
		void ProjectionMatrixCache<FloatTypeArg>::SetWidthCoeffAndDepth(FloatTypeArg InWidthCoeff, FloatTypeArg InDepth)
		{
			_width_coeff = InWidthCoeff;
			_depth = InDepth;
		}

		template<class FloatTypeArg>
		void ProjectionMatrixCache<FloatTypeArg>::SetHeightCoeffAndDepth(FloatTypeArg InHeightCoeff, FloatTypeArg InDepth)
		{
			_height_coeff = InHeightCoeff;
			_depth = InDepth;
		}

		template<class FloatTypeArg>
		void ProjectionMatrixCache<FloatTypeArg>::SetWidthCoeff(FloatTypeArg InWidthCoeff)
		{
			_width_coeff = InWidthCoeff;			
		}

		template<class FloatTypeArg>
		void ProjectionMatrixCache<FloatTypeArg>::SetHeightCoeff(FloatTypeArg InHeightCoeff)
		{
			_height_coeff = InHeightCoeff;
		}

		template<class FloatTypeArg>
		void ProjectionMatrixCache<FloatTypeArg>::SetDepth(FloatTypeArg InDepth)
		{
			_depth = InDepth;
		}
	} // Math
} // DvUtils