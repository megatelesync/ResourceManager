#ifndef _MathUtil_3D_h_
#define _MathUtil_3D_h_

#include "Geometry/MathUtil_Geometry_Line.h"

#include "MathUtil_Vector3.h"
#include "MathUtil_DoubleVector3.h"
#include "MathUtil_Plane.h"
#include "MathUtil_Matrix.h"
#include "MathUtil_Rotator.h"
#include "MathUtil_RotatorToMatrix.h"

#include <DirectXMath.h>
#include <cassert>
using namespace DirectX;

namespace DvUtils
{
	namespace Math
    {					
		struct LineSegment
		{	  
            // fields
            Vector3							start, direction;

            Vector3							end			()				const				{ return start + direction; }
            float							length		()				const				{ return Distance(start,end()); }

            // construction
			inline LineSegment();
			inline LineSegment(const Vector3& startIn, const Vector3& directionIn);            
		};

		Vector3			Project					(const Vector3& P, const Plane& plane);
		Vector3			Project					(const Vector3& P, const LineSegment& ray);
		bool			InsersectRay			(const Plane& plane, const LineSegment& ray, Vector3 *pResultOut);		





		// Line impl
		inline LineSegment::LineSegment() {}
		inline LineSegment::LineSegment(const Vector3& startIn, const Vector3& directionIn) : 
			start(startIn), direction(directionIn) {}       
	} // namespace Math
} // namespace DvUtils

#endif // _MathUtil_3D_h_