// THIS HEADER FILE IS DEPRECATED,
// USE Core/MathUtil_Core_Util.h HEADER FILE INSTEAD

/**************************************************************************
* MathUtil_Base.h
*
* TODO:
* 1. Move all the functionality to the Core/MathUtil_Core_Util.h header file. +
* 2. Mark this file as deprecated +
*
* TODO SCHEME:
* 1. Resolve the Det3x3 conflict +
* 2. Move the PiOver2 constant to the destination file +
*    (think of prepending each constant with a type-prefix)
* 
* PROBLEMS:
* 1. Det3x3 function conflict
* 2. PiOver2 constant conflict:
*    there're two constants - double and float
*
* UPDATES:
* 1. Throwing away the FloatRange, IntRange etc.
* 2. Throwing away overloads for integer arguments
* (there's no reason to double it - similar functionality 
* is available as a subset of the float format)
* 3. Int3, int 2 etc. classes look useless -
*    better to define them as the std::array based typedefs
**************************************************************************/

#pragma once

#include "Core/MathUtil_Core_Util.h"

namespace DvUtils
{
   namespace Math
   {                                               
	  /*
	  struct Int3
	  {
		  int value[3];

		  inline Int3();
		  inline Int3(int InFirst, int InSecond, int InThird);
	  };	  

	  struct Int2
	  {
		  int value[2];

		  inline Int2();
		  inline Int2(int InFirst, int InSecond);
	  };	 

	  // --------------------------------------------------------- IMPLEMENTATION -------------------------------------------------------------


	  // Int3 impl
	  inline Int3::Int3() {}
	  inline Int3::Int3(int InFirst, int InSecond, int InThird)
	  {
		  value[0] = InFirst;
		  value[1] = InSecond;
		  value[2] = InThird;
	  }

	  // Int2 impl
	  inline Int2::Int2() {}
	  inline Int2::Int2(int InFirst, int InSecond)
	  {
		  value[0] = InFirst;
		  value[1] = InSecond;		  
	  }
	  */
   } // namespace Math
} // namespace DvUtils