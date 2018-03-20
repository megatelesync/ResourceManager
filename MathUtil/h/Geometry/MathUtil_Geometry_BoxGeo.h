#ifndef _MathUtil_Geometry_BoxGeo_h_
#define _MathUtil_Geometry_BoxGeo_h_

#include "../3DMath/MathUtil_3DMath_Utils.h"

namespace DvUtils
{
	namespace Math
	{
		// Box for viewport representation
		template<class FloatTypeArg>
		class ViewportBox
		{
		public:
			/*********************************************************************************
			* typedefs
			*********************************************************************************/
			typedef FloatTypeArg			FloatType;			


			/*********************************************************************************
			* public getters
			*********************************************************************************/
			FloatType						GetWidth			()							const					{ return width; }
			FloatType						GetHeight			()							const					{ return height; }
			FloatType						GetNear				()							const					{ return near; }
			FloatType						GetFar				()							const					{ return far; }


			/*********************************************************************************
			* ctors
			*********************************************************************************/
			// default ctor: create a box of unit length with near z==0
			inline ViewportBox() : 
				_width(1.0F), _height(1.0F), _near(0.0F), _far(1.0F) {}
			inline ViewportBox(FloatTypeArg InWidth, FloatTypeArg InHeight, FloatTypeArg InNear = 0.0F, FloatTypeArg InFar = 1.0F);

		private:
			/*********************************************************************************
			* public variables
			*********************************************************************************/
			FloatType						_width;
			FloatType						_height;
			FloatType						_near;
			FloatType						_far;
		};

		template<class FloatTypeArg>
		DirectX::XMMATRIX CalcProjectionMatrix(const ViewportBox<FloatTypeArg>& InViewportBox);		





		// IMPL
		template<class FloatTypeArg>
		ViewportBox<FloatTypeArg>::ViewportBox
		(
			FloatTypeArg InWidth, FloatTypeArg InHeight,
			FloatTypeArg InNear, FloatTypeArg InFar
		) : _width(InWifth), _height(InHeight), _near(InNear), _far(InFar) {}


		template<class FloatTypeArg>
		DirectX::XMMATRIX CalcProjectionMatrix(const ViewportBox<FloatTypeArg>& InViewportBox)
		{
			return CalcOrthoProjectionMatrix
			(
				InViewportBox.GetWidth(), InViewportBox.GetHeight(), 
				InViewportBox.GetNear(), InViewportBox.GetFar()
			);
		}
	} // Math
} // Dv

#endif // _MathUtil_Geometry_BoxGeo_h_