#pragma once

// orthographic projection matrix

#include "../Geometry/MathUtil_Geometry_BoxGeo.h"
#include <DirectXMath.h>

namespace DvUtils
{
	namespace Math
	{
		class OrthoMatrix
		{
		public:
			/***************************************************************************************
			* typedefs
			***************************************************************************************/
			typedef float								  FloatType;
			typedef ViewportBox<FloatType>			ViewportBoxType;


			/***************************************************************************************
			* ctors
			***************************************************************************************/
			// default ctor: create ortho matrix for default view (which is unit viewport)
			inline OrthoMatrix();
			inline OrthoMatrix(const ViewportBoxType& InViewportBox);
			inline OrthoMatrix(FloatType InWidth, FloatType InHeight, FloatType InNear = 0.0F, FloatType InFar = 1.0F);


			/***************************************************************************************
			* updating
			***************************************************************************************/
			inline void									SetViewportBox			(const ViewportBoxType& InViewportBox);

			/***************************************************************************************
			* getters
			***************************************************************************************/
			const ViewportBoxType&						GetViewportBox			()				const			{ return _viewportBox; }
			const DirectX::XMMATRIX&					GetMatrix				()				const			{ return _matrix; }


			/***************************************************************************************
			* helper getters
			***************************************************************************************/
			FloatType									GetNear					()				const			{ return _viewportBox.GetNear(); }
			FloatType									GetFar					()				const			{ return _viewportBox.GetFar(); }
			FloatType									GetWidth				()				const			{ return _viewportBox.GetWidth(); }
			FloatType									GetHeight				()				const			{ return _viewportBox.GetHeight(); }


		private:
			// update matrix from the viewport box
			inline void						_UpdateMatrix();

			ViewportBoxType					_viewportBox;
			DirectX::XMMATRIX					 _matrix;
		};
		inline DirectX::XMMATRIX	CalculateInverseMatrix(const OrthoMatrix& InOrthoMatrix);





		// OrthoMatrix impl
		inline DirectX::XMMATRIX CalculateInverseMatrix(const OrthoMatrix& InOrthoMatrix)
		{
			return CalcOrthoUnProjectionMatrix
			(
				InOrthoMatrix.GetWidth(), InOrthoMatrix.GetHeight(), 
				InOrthoMatrix.GetNear(), InOrthoMatrix.GetFar()
			);
		}

		inline OrthoMatrix::OrthoMatrix()
		{
			_UpdateMatrix();
		}

		inline OrthoMatrix::OrthoMatrix(const ViewportBoxType& InViewportBox) :
			_viewportBox(InViewportBox)
		{
			_UpdateMatrix();
		}
		inline OrthoMatrix::OrthoMatrix
		(
			FloatType InWidth, FloatType InHeight, 
			FloatType InNear, FloatType InFar
		) : _viewportBox(ViewportBoxType(InWidth, InHeight, InNear, InFar))
		{
			_UpdateMatrix();
		}

		inline void	OrthoMatrix::SetViewportBox(const ViewportBoxType& InViewportBox)
		{
			_viewportBox = InViewportBox;
			_UpdateMatrix();
		}

		inline void OrthoMatrix::_UpdateMatrix()
		{
			_matrix = CalcProjectionMatrix(_viewportBox);
		}
	} // dx11
} // Dv