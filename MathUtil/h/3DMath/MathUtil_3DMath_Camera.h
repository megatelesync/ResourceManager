#pragma once

#include "MathUtil_3DMath_FrustumInfo.h"
#include "MathUtil_3DMath_AngleDirectedPosition.h"

namespace DvUtils
{
	namespace Math
	{
		// TODO:
		// PerspectiveProjectionMatrix TODO:
		// 1. Add the operations for updating aspect etc.

		// Contains a calculated view matrix supplied with 
		// camera info information (position, frustum info etc.)
		class ViewMat
		{
		public:
			// default ctor: initialize matrix with no transformations (identity matrix)
			inline ViewMat();
			inline ViewMat(const DvUtils::Math::AngleDirectedPosition& InDirectedPosition);

			// getters
			const DirectX::XMMATRIX&							GetMatrix				()				const			{ return _matrix; }
			const DvUtils::Math::AngleDirectedPosition&			GetDirectedPosition		()				const			{ return _dirPos; }

			// helper getters
			const AnglePair&									GetDirection			()				const			{ return _dirPos.direction; }
			const DirectX::XMFLOAT3&							GetPosition				()				const			{ return _dirPos.position; }
			float 												GetHorzAngle			()				const			{ return GetDirection().horz; }
			float 												GetVerticalAngle		()				const			{ return GetDirection().vertical; }

			// setters
			inline void SetDirectedPosition						(const DvUtils::Math::AngleDirectedPosition& InNewValue);			

		private:
			void										_UpdateMatrix();

			DvUtils::Math::AngleDirectedPosition		_dirPos;
			_declspec(align(16)) DirectX::XMMATRIX		_matrix;
		};
		inline DirectX::XMMATRIX					CalcInverseMatrix				(const ViewMat& InMatrix);


		// Contains a calculated projection matrix supplied with 
		// camera perspective projection frustum info
		class PerspectiveProjectionMatrix
		{
		public:
			/************************************************************************************
			* ctors
			*************************************************************************************/
			// default ctor: initialize as default (and valid) frustum info and corresponding matrix
			inline												PerspectiveProjectionMatrix		();
			inline												PerspectiveProjectionMatrix		(const DvUtils::Math::FrustumInfo& InFrustumInfo);


			/************************************************************************************
			* getters
			*************************************************************************************/
			const DirectX::XMMATRIX&							GetMatrix						()				const			{ return _matrix; }
			const DvUtils::Math::FrustumInfo&					GetFrustumInfo					()				const			{ return _frustumInfo; }


			/************************************************************************************
			* helper getters
			*************************************************************************************/
			float												GetAspectWidthOverHeight		()				const			{ return _frustumInfo.aspectWidthOverHeight; }
			float												GetZNear						()				const			{ return _frustumInfo.GetZNear(); }
			float												GetZFar							()				const			{ return _frustumInfo.GetZFar(); }
			float												GetHalfFov						()				const			{ return _frustumInfo.GetHalfFov(); }	


			/************************************************************************************
			* setters
			*************************************************************************************/
			void												SetFrustumInfo					(const DvUtils::Math::FrustumInfo& InNewValue);

		private:
			// Update matrix according to the state of the frustum info
			void												_UpdateMatrix();

			DvUtils::Math::FrustumInfo							_frustumInfo;
			_declspec(align(16)) DirectX::XMMATRIX				_matrix;
		};
		inline DirectX::XMMATRIX					CalcInverseMatrix				(const PerspectiveProjectionMatrix& InMatrix);



		// Contains a calculated view-projection matrix supplied with 
		// camera information (position, frustum info etc.)
		class ViewProjMatrix
		{
		public:
			/************************************************************************************
			* ctors
			*************************************************************************************/
			// default ctor: view and projection matrix are default (and valid) matrices
			inline ViewProjMatrix				();
			inline ViewProjMatrix				(const ViewMat& InViewMat, const PerspectiveProjectionMatrix& InProjMatrix);
			inline ViewProjMatrix				(const DvUtils::Math::AngleDirectedPosition& InAngleDirectedPosition, DvUtils::Math::FrustumInfo& InFrustumInfo);			


			/************************************************************************************
			* getters
			*************************************************************************************/
			const ViewMat&									GetView							()				const		{ return _matrixView; }
			const PerspectiveProjectionMatrix&				GetProjection					()				const		{ return _matrixProjection; }
			// Get the resulting view-projection matrix
			const DirectX::XMMATRIX&						GetMatrix						()				const		{ return _matrix; }


			/************************************************************************************
			* helper getters
			*************************************************************************************/
			const AnglePair&								GetDirection					()				const			{ return _matrixView.GetDirection(); }
			const DirectX::XMFLOAT3&						GetPosition						()				const			{ return _matrixView.GetPosition(); }
			float 											GetHorzAngle					()				const			{ return GetDirection().horz; }
			float 											GetVerticalAngle				()				const			{ return GetDirection().vertical; }

			float											GetAspectWidthOverHeight		()				const			{ return _matrixProjection.GetAspectWidthOverHeight(); }
			float											GetZNear						()				const			{ return _matrixProjection.GetZNear(); }
			float											GetZFar							()				const			{ return _matrixProjection.GetZFar(); }
			float											GetHalfFov						()				const			{ return _matrixProjection.GetHalfFov(); }	


			/************************************************************************************
			* setters
			*************************************************************************************/
			inline void										SetView							(const ViewMat&	InNewValue);
			inline void										SetProjection					(const PerspectiveProjectionMatrix& InNewValue);
			inline void										SetDirectedPosition				(const DvUtils::Math::AngleDirectedPosition& InNewValue);			
			inline void										SetFrustumInfo					(const DvUtils::Math::FrustumInfo& InNewValue);

		private:
			// Update matrix based on the view and projection matrix
			inline void _UpdateMatrix();

			ViewMat								_matrixView;
			PerspectiveProjectionMatrix			_matrixProjection;
			// resulting view and projection matrix
			_declspec(align(16)) DirectX::XMMATRIX					_matrix;
		};

		/*****************************************************************************
		* matrix calculation
		*****************************************************************************/
		inline DirectX::XMMATRIX					CalcInverseMatrix				(const ViewProjMatrix& InMatrix);


		/*****************************************************************************
		* setting matrix
		*****************************************************************************/
		inline void									SetViewProjMatrix				
		(
			ViewProjMatrix									*pMatrix, 
			const DvUtils::Math::AngleDirectedPosition		&AngleDirectedPosition,
			const FrustumGeoInfo							&InGeoInfo,
			float											InAspectWidthOverHeight
		);





		// ViewProjMatrix impl
		inline DirectX::XMMATRIX CalcInverseMatrix(const ViewProjMatrix& InMatrix)
		{
			return DirectX::XMMatrixInverse(nullptr, InMatrix.GetMatrix());
		}

		inline void SetViewProjMatrix				
		(
			ViewProjMatrix									*pMatrix, 
			const DvUtils::Math::AngleDirectedPosition		&AngleDirectedPosition,
			const FrustumGeoInfo							&InGeoInfo,
			float											InAspectWidthOverHeight
		)
		{
			assert(pMatrix != nullptr);
			pMatrix->SetFrustumInfo(FrustumInfo(InGeoInfo, InAspectWidthOverHeight));
			pMatrix->SetDirectedPosition(AngleDirectedPosition);
		}

		inline ViewProjMatrix::ViewProjMatrix()
		{
			_UpdateMatrix();
		}

		inline ViewProjMatrix::ViewProjMatrix
		(
			const ViewMat&											InViewMat, 
			const PerspectiveProjectionMatrix&						InProjMatrix
		) :
			_matrixView(InViewMat),
			_matrixProjection(InProjMatrix)
		{
			_UpdateMatrix();
		}

		inline ViewProjMatrix::ViewProjMatrix
		(
			const DvUtils::Math::AngleDirectedPosition&				InAngleDirectedPosition, 
			DvUtils::Math::FrustumInfo&								InFrustumInfo
		) :
			_matrixView(InAngleDirectedPosition),
			_matrixProjection(InFrustumInfo)
		{
			_UpdateMatrix();
		}

		inline void ViewProjMatrix::SetView(const ViewMat&	InNewValue)
		{
			_matrixView = InNewValue;
			_UpdateMatrix();
		}

		inline void ViewProjMatrix::SetProjection(const PerspectiveProjectionMatrix& InNewValue)
		{
			_matrixProjection = InNewValue;
			_UpdateMatrix();
		}

		inline void ViewProjMatrix::SetDirectedPosition(const DvUtils::Math::AngleDirectedPosition& InNewValue)
		{			
			_matrixView.SetDirectedPosition(InNewValue);
			_UpdateMatrix();
		}

		inline void ViewProjMatrix::SetFrustumInfo(const DvUtils::Math::FrustumInfo& InNewValue)
		{
			_matrixProjection.SetFrustumInfo(InNewValue);
			_UpdateMatrix();
		}

		inline void ViewProjMatrix::_UpdateMatrix()
		{			
			_matrix = DirectX::XMMatrixMultiply(_matrixView.GetMatrix(), _matrixProjection.GetMatrix());
		}

		// PerspectiveProjectionMatrix impl
		inline DirectX::XMMATRIX CalcInverseMatrix(const PerspectiveProjectionMatrix& InMatrix)
		{
			return DirectX::XMMatrixInverse(nullptr, InMatrix.GetMatrix());
		}

		inline PerspectiveProjectionMatrix::PerspectiveProjectionMatrix()			
		{
			_UpdateMatrix();
		}

		inline void PerspectiveProjectionMatrix::_UpdateMatrix()
		{
			_matrix = CalcProjMatrix(_frustumInfo);
		}

		inline void PerspectiveProjectionMatrix::SetFrustumInfo(const DvUtils::Math::FrustumInfo& InNewValue)
		{
			_frustumInfo = InNewValue;
			_UpdateMatrix();
		}

		// ViewMat impl
		inline DirectX::XMMATRIX CalcInverseMatrix(const ViewMat& InMatrix)
		{
			return DirectX::XMMatrixInverse(nullptr, InMatrix.GetMatrix());
		}

		inline ViewMat::ViewMat()
		{			
			_UpdateMatrix();
		}		

		inline ViewMat::ViewMat(const DvUtils::Math::AngleDirectedPosition& InDirectedPosition)
		{
			_dirPos = InDirectedPosition;
			_UpdateMatrix();
		}

		inline void ViewMat::SetDirectedPosition(const DvUtils::Math::AngleDirectedPosition& InNewValue)
		{
			_dirPos = InNewValue;
			_UpdateMatrix();
		}

		inline void ViewMat::_UpdateMatrix()
		{
			_matrix = CalcViewMatrix(_dirPos);
		}
	} // dx11
} // Dv