#ifndef _MathUtil_Geometry_Cone_h_
#define _MathUtil_Geometry_Cone_h_

#include "../3DMath/MathUtil_3DMath_DirectedPosition.h"

namespace DvUtils
{
	namespace Math	
	{
		// Parameters of the cone extent in the 3d space
		// (excludes info about position and orientation)
		template<class FloatTypeArg>
		struct ConeExtent
		{
			typedef				FloatTypeArg					FloatType;			
			typedef				ConeExtent<FloatTypeArg>		 ThisType;
			
			FloatTypeArg		height;
			FloatTypeArg		rad;

			ConeExtent();
			ConeExtent(const FloatTypeArg InHeight, const FloatTypeArg InRad);
		};

		// Cone in the 3D space
		template<class FloatTypeArg>
		struct Cone
		{
			typedef				FloatTypeArg						FloatType;			
			typedef				ConeExtent<FloatTypeArg>			ConeExtentType;
			typedef				DirPosition<FloatTypeArg>			DirPositionType;
			typedef				Cone<FloatTypeArg>					ThisType;			
			
			ConeExtentType		extent;	
			DirPositionType		dir_position; // direction and position of the cone

			Cone();
			Cone(const ConeExtentType& InExtent, const DirPositionType& InDirPosition);
		};





		// Cone impl
		template<class FloatTypeArg>
		Cone<FloatTypeArg>::Cone()
		{
		}

		template<class FloatTypeArg>
		Cone<FloatTypeArg>::Cone(const ConeExtentType& InExtent, const DirPositionType& InDirPosition) :
			extent(InExtent), dir_position(InDirPosition)
		{
		}

		// ConeBounds impl
		template<class FloatTypeArg>
		ConeExtent<FloatTypeArg>::ConeExtent()
		{
		}

		template<class FloatTypeArg>
		ConeExtent<FloatTypeArg>::ConeExtent(const FloatTypeArg InHeight, const FloatTypeArg InRad) :
			height(InHeight), rad(InRad)
		{
		}
	} // Math
} // DvUtils

#endif // _MathUtil_Geometry_Cone_h_