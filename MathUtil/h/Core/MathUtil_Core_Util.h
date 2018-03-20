/*******************************************************************************
* MathUtil_Core_Util.h
*
* This header file declares basic utility math functions
* (e.g. square root calculation templates etc.)
*******************************************************************************/

#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdlib>

namespace DvUtils
{
	namespace Math
	{		
		/*********************************************************
		* Utility constants
		* (this constants should have internal linkage,
		* which is by default in C++ for constants)
		*
		* CONSTANT USAGE RULES:
		* 1. If you do not know the concrete type, use the
		* template constant getter functions (Get_Pi, Get_PiOverFour etc.)
		* 2. If you know the concrete type, use the d_*, or flt_*
		*    constants
		*********************************************************/		
		/*********************************************************
		* Utility constants: template functions
		* Use this functions to get a value of a certain constant
		* with the given type provided as the template argument.						
		*********************************************************/
		template<class C>
		C				Get_Pi			();
		
		template<class C>
		C				Get_PiOverFour	();

		template<class C>
		C				Get_PiOverTwo	();

		/*********************************************************
		* Utility constants: double constants
		*********************************************************/			
		constexpr double  d_Pi				= 3.14159265358979323846;
		constexpr double  d_PiOverFour		= d_Pi/4.0;
		constexpr double  d_PiOverTwo		= d_Pi/2.0;

		/*********************************************************
		* Utility constants: float constants
		*********************************************************/		
		constexpr float  flt_Pi				= static_cast<float>(3.14159265358979323846);
		constexpr float  flt_PiOverFour		= flt_Pi/4.0F;
		constexpr float  flt_PiOverTwo		= flt_Pi/2.0F;

		constexpr float RADS_TO_DEGS = 180 / flt_Pi; // multiply by this to convert to degs from rads
		constexpr float DEGS_TO_RADS = flt_Pi / 180; // multiply by this to convert to rads from degs

		/*********************************************************
		* Utility constants: used
		* This section defines constant that already in use
		* (referenced to by the projects)
		*
		* WARNING:
		*	This constants may be marked deprecated in future.
		*********************************************************/	
		// epsilon for geometry-related functions
		constexpr float GeoEps=10E-7F; 

		// Syntax sugar constants and functions
		constexpr double Pi=d_Pi;
		constexpr double PiOver4=d_PiOverFour;
		constexpr double PiOver2=d_PiOverTwo;

		/*********************************************************
		* Utility functions
		*********************************************************/
		/*********************************************************
		* Utility functions: Range checking and clamping
		*********************************************************/
		template<class C> 
		bool	InRange					(C val, C min, C max);

		template<class C>
		bool	InRange_NonInclusive    (C val, C min, C max);

		template<class C>
		bool	AbsUpperBounded              (C val, C MaxBound);

		template<class C>
		bool	AbsUpperBounded_NonInclusive (C val, C MaxBound);

		/*********************************************************
		* Utility functions: Rounding
		*********************************************************/
		template<class C> 
		int		Round					(C val);

		template<class C> 
		C		Clamp					(C val, C min, C max);
		
		/*********************************************************
		* Utility functions: Minimum and maximum
		*********************************************************/
		template<class C> 
		C		Min						(C val1, C val2);

		template<class C> 
		C		Maxim					(C val1, C val2);

		/*********************************************************
		* Utility functions: interpolation
		*********************************************************/
		template<class C> 
		C		Lerp					(C min, C max, C factor);

		/*********************************************************
		* Utility functions: Absolute value
		*********************************************************/
		template<class C> 
		C		Abs						(C val);

		/*********************************************************
		* Utility functions: Logarithm calculation
		*********************************************************/
		template<class C> 
		C		Log						(C val);
		template<class C> 
		C		Log						(C val, C bas);

		/********************************************************
		* Power and Sqrt
		********************************************************/
		template<class C> 
		C		Pow						(C val, int degree);

		template<class C>			
		C		Sqrt					(C val);

		template<class C>
		C		Sqr                     (C val);

		/******************************************************************
		* Trigonometric functions
		******************************************************************/
		template<class C> 
		C		Cos						(C angle);

		template<class C> 
		C		Sin						(C angle);

		template<class C>	
		C		Tan						(C angle);


		/******************************************************************
		* Testing for equality with the given tolerance
		******************************************************************/
		template<class FloatTypeArg>
		bool            IsNearEqual                                     (FloatTypeArg InA, FloatTypeArg InB, FloatTypeArg InTolerance);

		/******************************************************************
		* Determinant calculation
		******************************************************************/
		template<class C> 
		C   Det2x2
		(
			C X1, C Y1,
		    C X2, C Y2
		);
		template<class C> 
		C   Det3x3
		(
			C X1, C Y1, C Z1,
            C X2, C Y2, C Z2,
            C X3, C Y3, C Z3
		);

		template<class C>
		C Det4x4
		(
			const C& In00, const C& In01, const C& In02, const C& In03,
			const C& In10, const C& In11, const C& In12, const C& In13,
			const C& In20, const C& In21, const C& In22, const C& In23,
			const C& In30, const C& In31, const C& In32, const C& In33
		);

		/******************************************************************
		* Geometry functions
		******************************************************************/
		/******************************************************************
		* Geometry functions: Square
		******************************************************************/
		template<class C> 
		C		CircleSquare			(C radius);

		/******************************************************************
		* Geometry functions: Volume
		******************************************************************/
		template<class C> 
		C		PrismVolume				(C baseSquare, C height);

		template<class C> 
		C		BoxVolume				(C width, C height, C depth);

		template<class C> 
		C		CylinderVolume			(C radius, C height);

		template<class C> 
		C		SphereVolume			(C radius);

		/******************************************************************
		* Geometry utils: Wise
		******************************************************************/
		enum class EWise
		{
			None = 0,
			Clockwise,
			CounterClockwise
		};

		/******************************************************************
		* Geometry utils: Axis and plane
		******************************************************************/
		enum class EAxis
		{
			None = 0,
			OX,
			OY,
			OZ
		};

		// EPlane:
		//		This enum is deprecated:
		//		because plane can be fully described with the axis
		//		perpendicular to it:
		enum class EPlane
		{
			None = 0,
			XY,
			XZ,
			YZ
		};	  


		/*********************************************************
		* IMPLEMENTATION
		*********************************************************/
		/*********************************************************
		* IMPLEMENTATION: float constant getters
		*
		* C++ NOTE: These functions are declared with the "inline" 
		* keyword because they are full template specializations,
		* and the full template specialization is not the inline function
		* except the case when it's explicitly declared as inline
		*********************************************************/
		template<>
		inline float Get_Pi<float>()
		{
			return flt_Pi;
		}
		
		template<>
		inline float Get_PiOverFour<float>()
		{
			return flt_PiOverFour;
		}

		template<>
		inline float Get_PiOverTwo<float>()
		{
			return flt_PiOverTwo;
		}

		/*********************************************************
		* IMPLEMENTATION: double constant getters
		*********************************************************/
		template<>
		inline double Get_Pi<double>()
		{
			return d_Pi;
		}
		
		template<>
		inline double Get_PiOverFour<double>()
		{
			return d_PiOverFour;
		}

		template<>
		inline double Get_PiOverTwo<double>()
		{
			return d_PiOverTwo;
		}

		/*********************************************************
		* IMPLEMENTATION: Utility functions: Range checking
		*********************************************************/
		template<class C> bool InRange(C val, C min, C max)
		{
			return min <= val && val <= max;
		}

		template<class C> bool InRange_NonInclusive(C val, C min, C max)
		{
			return min < val && val < max;
		}

		template<class C>
		bool AbsUpperBounded(C val, C MaxBound)
		{
			return InRange(val, -MaxBound, MaxBound);
		}

		template<class C>
		bool AbsUpperBounded_NonInclusive(C val, C MaxBound)
		{
			return InRange_NonInclusive(val, -MaxBound, MaxBound);
		}

		/*********************************************************
		* IMPLEMENTATION: Rounding and clamping
		*********************************************************/
		template<class C> C Clamp(C val, C min, C max)
		{
		    if (max < min)
            {
                typename C t = min;
                min = max;
                max = t;
            }
            if (val < min) { return min; }
            if (val > max) { return max; }
			return val;
		}

		template<class C> int Round(C val)
		{
			return int(floor(val + 0.5f));
		}

		
		/*********************************************************
		* IMPLEMENTATION: Min and max
		*********************************************************/
		template<class C> C Min(C val1, C val2)
		{
			return val1 <= val2 ? val1 : val2;
		}

		template<class C> C Maxim(C val1, C val2)
		{
			return val1 <= val2 ? val2 : val1;
		}

		/*********************************************************
		* IMPLEMENTATION: Utility functions: interpolation
		*********************************************************/
		template<class C> C Lerp(C min, C max, C factor)
		{
			return min + (max - min) * factor;
		}

		template<class C> C Abs(C val)
	    {
			return fabs(val);
	    }

		/*******************************************************************
		* IMPLEMENTATION: Logarithm calculation
		*******************************************************************/
		template<class C> C Log(C val)
		{
			return log(val);
		}

		template<class C> C Log(C val, C bas)
		{
			return log(val)/log(bas);
		}

		/*******************************************************************
		* IMPLEMENTATION: Power and sqrt calculation
		*******************************************************************/
		template<class C> C Sqrt(C val)
		{
			return sqrt(val);
		}

		template<class C> C Sqr(C val)
		{
			return val * val;
		}

		template<class C> C Pow(C val, int degree)
		{
			return pow(val, degree);
		}

		/*******************************************************************
		* IMPLEMENTATION: Trigonometric calculation
		*******************************************************************/
		template<class C> C Cos(C angle)
	    {
			return cos(angle);
	    }

		template<class C> C Sin(C angle)
		{
			return sin(angle);
		}

		template<class C> C Tan(C angle)
		{
			return tan(angle);
		}

		/******************************************************************
		* IMPLEMENTATION: Testing for equality with the given tolerance
		******************************************************************/
		template<class FloatTypeArg>
		bool IsNearEqual(FloatTypeArg InA, FloatTypeArg InB, FloatTypeArg InTolerance)
		{
			return Abs(InA - InB) <= InTolerance;
		}

		/******************************************************************
		* IMPLEMENTATION: Determinant calculation
		******************************************************************/
		template<class C> C   Det2x2
		(
			C X1, C Y1,
			C X2, C Y2
		) 
		{
			return X1 * Y2 - X2 * Y1; 
		}
		template<class C> C   Det3x3
		(
			C X1, C Y1, C Z1,
            C X2, C Y2, C Z2,
            C X3, C Y3, C Z3
		) 
		{
		    typename C detValue = X1 * (Y2 * Z3 - Y3 * Z2)
                           - Y1 * (X2 * Z3 - X3 * Z2)
                           + Z1 * (X2 * Y3 - X3 * Y2);
            return detValue;
		}

		template<class C>
		C Det4x4
		(
			const C& In00, const C& In01, const C& In02, const C& In03,
			const C& In10, const C& In11, const C& In12, const C& In13,
			const C& In20, const C& In21, const C& In22, const C& In23,
			const C& In30, const C& In31, const C& In32, const C& In33
		)
		{
			C det11 = Det3x3
			(
				In11, In12, In13,
				In21, In22, In23,
				In31, In32, In33
			);
			C det12 = Det3x3
			(
				In10, In12, In13,
				In20, In22, In23,
				In30, In32, In33
			);
			C det13 = Det3x3
			(
				In10, In11, In13,
				In20, In21, In23,
				In30, In31, In33
			); 
			C det14 = Det3x3
			(
				In10, In11, In12, 
				In20, In21, In22,
				In30, In31, In32
			);

			C res_dot = In00 * det11 -In01 * det12 + In02 * det13 -In03 * det14;
			return res_dot;
		}

		/*********************************************************
		* IMPLEMENTATION: Geometry functions: Square
		*********************************************************/
		template<class C> C   CircleSquare   (C radius)  
		{
			return radius * radius * Pi; 
		}
		
		/*********************************************************
		* IMPLEMENTATION: Geometry functions: Volume
		*********************************************************/
		template<class C> C   PrismVolume    (C baseSquare, C height)                                          
		{
			return baseSquare * height; 
		}
		template<class C> C   BoxVolume      (C width, C height, C depth)                                      
		{ 
			return width * height * depth; 
		}
		template<class C> C   CylinderVolume (C radius, C height)                                              
		{ 
			return PrismVolume(CircleSquare(radius),height); 
		}
		template<class C> C   SphereVolume   (C radius)  
		{
			return 4.0F/3.0F * Pi * radius * radius * radius; 
		} 
	} // Math
} // DvUtils