// THIS FILE DEFINES:
// - All math constants with EXTERNAL LINKAGE

#define _USE_MATH_DEFINES
#include <cmath>
#include "../h/Core/MathUtil_Core_Master.h"

namespace DvUtils
{
	namespace Math
	{		
		// AnglePair
		const AnglePair AnglePair::DirectionX = AnglePair(0.0F, flt_PiOverTwo);
		const AnglePair AnglePair::DirectionY = AnglePair(flt_PiOverTwo, 0.0F);
		const AnglePair AnglePair::DirectionZ = AnglePair(0.0F,0.0F);
	} // Math
} // DvUtils