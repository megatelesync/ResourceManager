#ifndef _MathUtil_Geometry_Line_h_
#define _MathUtil_Geometry_Line_h_

// THIS FILE INCLUDES OPERATIONS FOR LINE-POINT, RAY-POINT, LINE_SEGMENT-POINT  DISTANCE CALCULATIONS
#include <cassert>
#include "../MathTypes/BLAS/MathUtil_BLAS.h"
#include "../Core/MathUtil_Core_LinearAlgebraUtils.h"

// TODO:
// Implement closest point on line segment
// Implement bounded ray operations (make, vector, end point calculation etc.)

namespace DvUtils
{
	namespace Math
	{
		// Line, ray or line segment described by the start point and a (non-normalized) vector from start to end
		template<class FloatTypeArg>
		struct DirectedLine
		{
			typedef			FloatTypeArg						FloatType;
			typedef			Vec<FloatTypeArg, 3>				VecType;
			typedef			DirectedLine<FloatTypeArg>			ThisType;

			VecType			start;			
			// length of the vector is equal to the length in the case of the line segment;
			// it's normalized in the case of a line or a ray
			VecType			vector_to_end;  

			DirectedLine					();
			DirectedLine					(const VecType& InStart, const VecType& InVectorToEnd);
		};
		// Make a line segment with specified start and end vertices
		template<class FloatTypeArg>		
		DirectedLine<FloatTypeArg>			MakeLineSegment		(const FloatTypeArg *InStart, const FloatTypeArg *InEnd);

		// Calculate the end point of the line segment described as a directed line structure argument
		template<class FloatTypeArg>		
		FloatTypeArg					   *CalcEnd				(FloatTypeArg *pResultVec, const DirectedLine<FloatTypeArg>& InDirectedLine);

		// Calculate the length of the line segment described as a directed line structure argumet
		template<class FloatTypeArg>
		FloatTypeArg						CalcLength			(const DirectedLine<FloatTypeArg>& InDirectedLine);

		// Bounded ray: Line segment represented by the ray (with normalized direction) and line length
		template<class FloatTypeArg>
		struct BoundedRay
		{
			typedef			FloatTypeArg						FloatType;
			typedef			Vec<FloatTypeArg, 3>				VecType;
			typedef			DirectedLine<FloatTypeArg>			DirectedLineType;
			typedef			BoundedRay<FloatTypeArg>			ThisType;
			
			// ray with normalized direction
			typename DirectedLineType			ray;
			// length of the line segment
			typename FloatTypeArg				length;

			BoundedRay();
			BoundedRay(const DirectedLineType& InRay, FloatTypeArg InLength);
		};
		// Make bounded ray specified by start and end points
		// WARNING: slow (involves square root calculation)
		template<class FloatTypeArg>		
		BoundedRay<FloatTypeArg>			MakeBoundedRaySlow	(const FloatTypeArg *InStart, const FloatTypeArg *InEnd);
		// Calculate the vector from start to the end of the bounded ray
		template<class FloatTypeArg>		
		FloatTypeArg					   *CalcStartToEnd		(FloatTypeArg *pResultVec, const BoundedRay<FloatTypeArg>& InBoundedRay);
		// Calculate the end point of the bounded ray
		template<class FloatTypeArg>		
		FloatTypeArg					   *CalcEnd				(FloatTypeArg *pResultVec, const BoundedRay<FloatTypeArg>& InBoundedRay);

		// Directed line segment and its cached length
		template<class FloatTypeArg>
		class DirectedLineSegmentLengthCache
		{
		public:
			typedef					FloatTypeArg											FloatType;
			typedef					DirectedLine<FloatTypeArg>								DirectedLineType;
			typedef					DirectedLineSegmentLengthCache<FloatTypeArg>			ThisType;			

			DirectedLineSegmentLengthCache();
			DirectedLineSegmentLengthCache(const DirectedLineType& InLineParams);

			const DirectedLineType&							GetParams()				const			{ return _params; }
			FloatType										GetLength()				const			{ return _length; }			

		private:
			// line main parameters (that fully describe the line)
			DirectedLineType		_params;
			// precomputed length of the line segment
			FloatType				_length;
		};		


		//-----------------------------------------------------------------------------------------------
		// CLOSEST POINT ON LINE SEGMENT
		//-----------------------------------------------------------------------------------------------
		template<class FloatTypeArg>
		FloatTypeArg *ClosestPointFromPointOnLineSegment3D
		(
			FloatTypeArg *pResultPoint,
			const FloatTypeArg *pLineStart, const FloatTypeArg *pVectorStartToEnd,
			const FloatTypeArg *pNormalizedLineDirection, 
			const FloatTypeArg *InPoint
		);
		// ----------------------------------------------------------------------------------------------
		// CLOSEST POINT ON RAY
		//-----------------------------------------------------------------------------------------------
		template<class FloatTypeArg>
		FloatTypeArg *ClosestPointFromPointOnRay3D
		(
			FloatTypeArg *pResultPoint,
			const FloatTypeArg *pRayStart, const FloatTypeArg *pNormalizedRayDirection, 
			const FloatTypeArg *pPoint
		);

		// ----------------------------------------------------------------------------------------------
		// CLOSEST POINT ON LINE
		//-----------------------------------------------------------------------------------------------
		// Computes some point on line that is closest to a given point
		// Arguments:
		// pResultPoint: point to 3d vector where to put the final result
		// pLineStart: 3d vector, start point of the line
		// pNormalizedLineDirection: 3d vector, precomputed normalized line direction
		// pPoint: 3d vector, point to compute closest point to
		template<class FloatTypeArg>
		FloatTypeArg *ClosestPointFromPointOnLine3D
		(
			FloatTypeArg *pResultPoint,
			const FloatTypeArg *pLineStart, const FloatTypeArg *pNormalizedLineDirection,
			const FloatTypeArg *pPoint
		);

		// -----------------------------------------------------------------------------------------------
		// DISTANCE TO LINE SEGMENT
		// -----------------------------------------------------------------------------------------------
		// Compute squared distance from a line segment to a point
		// Line segment to be specified with a line start point and a vector from start of the line to end of the line
		// Arguments:
		//		pLineStart:  start of the line segment
		//		pLineVector: vector from start of the line segment to end of the line segment
		//		pPoint:		 point to compute squared distance to
		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToLineSegment3D(const FloatTypeArg *pLineStart, const FloatTypeArg *pLineVector, const FloatTypeArg *pPoint);

		//-----------------------------------------------------------------------------------------------
		// COORDINATE ALONG AXIS
		//-----------------------------------------------------------------------------------------------
		// Compute coordinate of a given point in the coordinate system of single 3d axis
		// and a given normalized ort vector
		// Math: (Point  - Origin) dot (normalized ort vector)
		// Arguments:
		//		pOrigin: pointer to the coordinate system origin 3d vector
		//		pNormaliedOrtVec: normalized ort vector of a given coordinate system
		//		pPoint: point to calculate the coordinate of
		// Result:
		//		The coordinate of a given point on a given coordinate axis
		template<class FloatTypeArg>
		FloatTypeArg ComputeCoordinateOnAxis3D
		(
			const FloatTypeArg *pOrigin, const FloatTypeArg *pNormalizedOrtVector, 
			const FloatTypeArg *pPoint			
		);

		template<class FloatTypeArg>
		FloatTypeArg ComputeCoordinateOnAxis3D
		(
			const FloatTypeArg *pOrigin, const FloatTypeArg *pNormalizedOrtVector, 
			const FloatTypeArg *pPoint,
			uint8_t originComponentByteStride = sizeof(FloatTypeArg),
			uint8_t normalizedOrtVectorComponentByteStride = sizeof(FloatTypeArg),
			uint8_t pointComponentByteStride = sizeof(FloatTypeArg)
		);

		// -----------------------------------------------------------------------------------------------
		// DISTANCE TO RAY
		// -----------------------------------------------------------------------------------------------
		// Compute squared distance from a ray to a point
		// ray to be specified with a nomalized direction vector and a start point
		// Arguments:
		//		pRayStart:				 start of the ray
		//		pNormalizedRayDirection: normalized direction of the ray
		//		pPoint:					 point to compute squared distance to
		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToNormalizedRay3D(const FloatTypeArg *pRayStart, const FloatTypeArg *pNormalizedRayDirection, const FloatTypeArg *pPoint);

		// -----------------------------------------------------------------------------------------------
		// DISTANCE TO LINE
		// -----------------------------------------------------------------------------------------------
		// line specified with a nomalized direction vector and a start point
		// arguments:
		//		pLineDirection: direction of the line (must be normalized!)
		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToNormalizedDirectedLine3D(const FloatTypeArg *pLineStart, const FloatTypeArg *pNormalizedLineDirection, const FloatTypeArg *pPoint);

		// line specified with a nomalized direction vector
		// arguments:
		//		pVectorPointToStart: Point - line start vector
		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToNormalizedDirectedLine3D(const FloatTypeArg *pNormalizedLineDirection, const FloatTypeArg *pVectorPointToStart);

		// Compute square distance to line given 2 dot products:
		// formula: AC * AC - (AC * AB) ^ 2,
		// where A - line start; B - line end; C - point to find sqr distance to
		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToNormalizedLine3D(FloatTypeArg startToPointSqrDist, FloatTypeArg startToPointDotNormalizedLineDirection);

		// Compute square distance to line given 3 dot products		
		// formula: AC * AC - (((AC * AB) ^ 2) / (AB * AB))
		// where A - line start; B - line end; C - point to find sqr distance to
		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToLine3D(FloatTypeArg startToPointSqrDist, FloatTypeArg startToPointDotLineDirection, FloatTypeArg vectorStartToEndDotvectorStartToEnd);





		// IMPL
		template<class FloatTypeArg>
		FloatTypeArg *ClosestPointFromPointOnRay3D
		(
			FloatTypeArg *pResultPoint,
			const FloatTypeArg *pRayStart, const FloatTypeArg *pNormalizedRayDirection, 
			const FloatTypeArg *pPoint
		)
		{
			assert(pResultPoint != nullptr);
			assert(pRayStart != nullptr);
			assert(pNormalizedRayDirection != nullptr);			
			assert(pPoint != nullptr);
			// 5 adds; 3 mults
			typename FloatTypeArg coordinate_coeff = ComputeCoordinateOnAxis3D(pRayStart, pNormalizedRayDirection, pPoint);
			if(coordinate_coeff <= 0) 
			{
				return pRayStart; 
			}
			// 3 adds; 3 mults
			Vec3LengthLerp(pResultPoint, pLineStart, pVectorStartToEnd, coordinate_coeff);
			// total: 8 adds; 6 mults; total = 14 operations (excluding conditional logics)
			return pResultPoint;
		}

		template<class FloatTypeArg>
		FloatTypeArg *ClosestPointFromPointOnLine3D
		(
			FloatTypeArg *pResultPoint,
			const FloatTypeArg *pLineStart, const FloatTypeArg *pNormalizedLineDirection, 
			const FloatTypeArg *pPoint
		)
		{
			assert(pResultPoint != nullptr);
			assert(pLineStart != nullptr);
			assert(pNormalizedLineDirection != nullptr);			
			assert(pPoint != nullptr);
			// 5 adds; 3 mults
			typename FloatTypeArg coordinate_coeff = ComputeCoordinateOnAxis3D(pLineStart, pNormalizedLineDirection, pPoint);
			// 3 adds; 3 mults
			Vec3LengthLerp(pResultPoint, pLineStart, pNormalizedLineDirection, coordinate_coeff);
			// total: 8 adds; 6 mults; total = 14 operations
			return pResultPoint;
		}

		template<class FloatTypeArg>
		FloatTypeArg ComputeCoordinateOnAxis3D
		(
			const FloatTypeArg *pOrigin, const FloatTypeArg *pNormalizedOrtVector, 
			const FloatTypeArg *pPoint			
		)
		{
			typename FloatTypeArg vector_start_to_point[3];
			// 3 subs
			SetSubtract3(vector_start_to_point, pPoint, pOrigin);
			// 2 adds; 3 mults
			typename FloatTypeArg res_coodinate = Dot3(pNormalizedOrtVector, vector_start_to_point);
			// total: 3+2=5 adds; 3 mults = 8 operations
			return res_coodinate;
		}

		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToLineSegment3D(const FloatTypeArg *pLineStart, const FloatTypeArg *pLineVector, const FloatTypeArg *pPoint)
		{
			// square distance from line start to point: 2 adds, 3 mults
			typename FloatTypeArg sqrDistStartToPoint = SqrDist3(pVectorPointToStart);
			// square distance of direction: 2 adds, 3 mults			
			typename FloatTypeArg sqrDistDir = SqrDist3(pNormalizedLineDirection);
			// direction from start to point dot direction of line: 2 adds, 3 mults
			typename FloatTypeArg dir_Start_to_Point_DOT_direction = Dot3(pVectorPointToStart, pNormalizedLineDirection);									
			if(dir_Start_to_Point_DOT_direction <= 0)
			{
				return sqrDistStartToPoint;
			}
			if(dir_Start_to_point_DOT_direction >= sqrDistDir)
			{
				typename FloatTypeArg line_end[3];
				SetAdd3(line_end, pLineStart, pLineVector);
				typename FloatTypeArg dir_End_to_Point[3];
				SetSubtract3(dir_End_to_Point, pPoint, line_end);
				typename FloatTypeArg sqrDist_End_to_Point = dot3(dir_End_to_Point, dir_End_to_Point);				
				return sqrDist_End_to_Point;
			}
			typename FloatTypeArg res_squared_distance =  SqrDistanceToNormalizedLine3D(sqrDistStartToPoint, sqr_dir_Start_to_Point_DOT_direction);
			return res_squared_distance;
		}

		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToNormalizedRay3D(const FloatTypeArg *pLineStart, const FloatTypeArg *pNormalizedLineDirection, const FloatTypeArg *pPoint)
		{
			// square distance from line start to point: 2 adds, 3 mults
			FloatTypeArg sqrDistStartToPoint = SqrDist3(pVectorPointToStart);
			// square distance of direction: 2 adds, 3 mults			
			FloatTypeArg sqrDistDir = SqrDist3(pNormalizedLineDirection);
			// direction from start to point dot direction of line: 2 adds, 3 mults
			FloatTypeArg dir_Start_to_Point_DOT_direction = Dot3(pVectorPointToStart, pNormalizedLineDirection);									
			if(dir_Start_to_Point_DOT_direction <= 0)
			{
				return sqrDistStartToPoint;
			}
			FloatTypeArg res_squared_distance =  SqrDistanceToNormalizedLine3D(sqrDistStartToPoint, sqr_dir_Start_to_Point_DOT_direction);
			return res_squared_distance;
		}

		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToNormalizedDirectedLine3D(const FloatTypeArg *pLineStart, const FloatTypeArg *pNormalizedLineDirection, const FloatTypeArg *pPoint)
		{
			assert(pLineStart != nullptr);
			assert(pNormalizedLineDirection != nullptr);
			assert(pPoint != nullptr);
			// direction (non-normalized) from line start to point: 3 adds
			FloatTypeArg dir_Start_to_Point[3];
			SetSubtract3(dir_Start_to_Point, pPoint, pLineStart);
			// square distance from line start to point: 2 adds, 3 mults
			FloatTypeArg sqrDistStartToPoint = SqrDist3(dir_Start_to_Point);			
			// the result squared distance: 5 adds; 7 mults			
			FloatTypeArg res_squared_distance = SqrDistanceToNormalizedDirectedLine3D(pNormalizedLineDirection, dir_Start_to_Point);
			// totals: 5 + 2 + 3 = 10 adds; 7 + 3 = 10 mults;
			return res_squared_distance;
		}

		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToNormalizedDirectedLine3D(const FloatTypeArg *pNormalizedLineDirection, const FloatTypeArg *pVectorPointToStart)
		{			
			assert(pNormalizedLineDirection != nullptr);
			assert(pVectorPointToStart != nullptr);
			// square distance from line start to point: 2 adds, 3 mults
			FloatTypeArg sqrDistStartToPoint = SqrDist3(pVectorPointToStart);
			// square distance of direction: 2 adds, 3 mults			
			FloatTypeArg sqrDistDir = SqrDist3(pNormalizedLineDirection);
			// direction from start to point dot direction of line: 2 adds, 3 mults
			FloatTypeArg dir_Start_to_Point_DOT_direction = Dot3(pVectorPointToStart, pNormalizedLineDirection);			
			
			// 1 add; 1 adds
			FloatTypeArg res_squared_distance =  SqrDistanceToNormalizedLine3D(sqrDistStartToPoint, sqr_dir_Start_to_Point_DOT_direction);
			// total: adds: 1+2+2=5; mults: 3+3+1=7; 12 operations
			return res_squared_distance;
		}

		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToNormalizedLine3D(FloatTypeArg startToPointSqrDist, FloatTypeArg startToPointDotNormalizedLineDirection)
		{
			// squared direction from start to point dot direction of line: 1 mult			
			FloatTypeArg sqr_dir_Start_to_Point_DOT_direction = startToPointDotNormalizedLineDirection * startToPointDotNormalizedLineDirection;
			// the result squared distance: 1 add
			FloatTypeArg res_squared_distance = startToPointSqrDist - sqr_dir_Start_to_Point_DOT_direction;
			// total: adds: 1 mults: 1; = 2 operations
			return res_squared_distance;
		}

		template<class FloatTypeArg>
		FloatTypeArg SqrDistanceToLine3D(FloatTypeArg startToPointSqrDist, FloatTypeArg startToPointDotLineDirection, FloatTypeArg vectorStartToEndDotvectorStartToEnd)
		{
			// squared direction from start to point dot direction of line: 1 mult			
			FloatTypeArg sqr_dir_Start_to_Point_DOT_direction = startToPointDotNormalizedLineDirection * startToPointDotNormalizedLineDirection;
			// the result squared distance: 1 add; 1 div
			FloatTypeArg res_squared_distance = startToPointSqrDist - sqr_dir_Start_to_Point_DOT_direction / vectorStartToEndDotvectorStartToEnd;
			// total: divs: 1; adds: 1 mults: 1; = 3 operations
			return res_squared_distance;
		}     



		// DirectedLine operations impl
		// Make a line segment with specified start and end vertices
		template<class FloatTypeArg>		
		DirectedLine<FloatTypeArg> MakeLineSegment(const FloatTypeArg *InStart, const FloatTypeArg *InEnd)
		{	
			assert(InStart != nullptr);
			assert(InEnd != nullptr);
			typename FloatTypeArg startToEnd[3];
			SetSubtract3(startToEnd, InEnd, InStart);
			DirectedLine<FloatTypeArg> result_line(Vec3<FloatTypeArg>(InStartVector), Vec3<FloatTypeArg>(startToEnd));
			return result_line;
		}

		template<class FloatTypeArg>		
		FloatTypeArg *CalcEnd(FloatTypeArg *pResultVec, const DirectedLine<FloatTypeArg>& InDirectedLine)
		{			
			assert(pResultVec != nullptr);
			assert(&InDirectedLine != nullptr);
			SetAdd3(pResultVec, InDirectedLine.start, InDirectedLine.vector_to_end);
			return Vec3<FloatTypeArg>(end_point);
		}

		// Calculate the length of the line segment described as a directed line structure argumet
		template<class FloatTypeArg>
		FloatTypeArg CalcLength(const DirectedLine<FloatTypeArg>& InDirectedLine)
		{
			assert(&InDirectedLine != nullptr);
			return SqrDist3(InDirectedLine.vector_to_end);
		}

		// BoundedRay impl
		template<class FloatTypeArg>
		BoundedRay<FloatTypeArg>::BoundedRay()
		{
		}

		template<class FloatTypeArg>
		BoundedRay<FloatTypeArg>::BoundedRay(const DirectedLineType& InRay, FloatTypeArg InLength) :
			ray(InRay), length(InLength)
		{			
		}

		// DirectedLine impl
		template<class FloatTypeArg>
		DirectedLine<FloatTypeArg>::DirectedLine()
		{
		}

		template<class FloatTypeArg>
		DirectedLine<FloatTypeArg>::DirectedLine(const VecType& InStart, const VecType& InVectorToEnd) :
			start(InStart),
			vector_to_end(InVectorToEnd)
		{
		}
	} // Math
} // DvUtils

#endif // _MathUtil_Geometry_Line_h_
