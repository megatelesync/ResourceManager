#ifndef _MathUtil_CollisionDetection_volume_Sphere_h_
#define _MathUtil_CollisionDetection_volume_Sphere_h_

#include "../../MathTypes/BLAS/MathUtil_BLAS.h"
#include "../../Geometry/MathUtil_Geometry_Sphere.h"
#include "MathUtil_CollisionDetection_volume_utils.h"

#include <boost/assert.hpp>

namespace DvUtils
{
	namespace Math
	{
		// Sphere
		template<class FloatTypeArg> struct Sphere
		{			
			typedef FloatTypeArg                                                    FloatType;
			typedef Vec<FloatTypeArg, 3>			                        VecType;

			VecType									center;
			float									radius;

			inline Sphere();
			inline Sphere(const VecType& InCenter, float InRadius);			
		};

		/*****************************************************************************************************
		* Equality testing
		*****************************************************************************************************/
		template<class FloatTypeArg>
		bool IsNearEqual(const Sphere<FloatTypeArg>& InA, const Sphere<FloatTypeArg>& InB, FloatTypeArg InTolerance = GeoEps);

		/*****************************************************************************************************
		* Volume computation
		*****************************************************************************************************/
		template<class FloatTypeArg>
		FloatTypeArg GetVolume(const Sphere<FloatTypeArg>& InSphere);

		/*****************************************************************************************************
		* Transformation
		*****************************************************************************************************/
		template<class FloatTypeArg>
		Sphere<FloatTypeArg> GetTransformed_NoScale(const Sphere<FloatTypeArg>& InSphere, const SquareMatrix<FloatTypeArg, 4>& InMatrix);

		template<class FloatTypeArg>
		Sphere<FloatTypeArg> GetTransformed_NoScale(const Sphere<FloatTypeArg>& InSphere, const SquareMatrix<FloatTypeArg, 3>& InMatrix);

		template<class FloatTypeArg>
		Sphere<FloatTypeArg>& Transform_NoScale(Sphere<FloatTypeArg>* pOutSphere, const SquareMatrix<FloatTypeArg, 4>& InMatrix);

		template<class FloatTypeArg>
		Sphere<FloatTypeArg>& Transform_NoScale(Sphere<FloatTypeArg>* pOutSphere, const SquareMatrix<FloatTypeArg, 3>& InMatrix);

		/*****************************************************************************************************
		* Combining bounding volumes
		*****************************************************************************************************/
		template<class FloatTypeArg>
		Sphere<FloatTypeArg>& CombineVolumes(Sphere<FloatTypeArg>* pDestSphere, const Sphere<FloatTypeArg>& InSphereToInclude);	

		template<class FloatTypeArg>
		Sphere<FloatTypeArg> GetCombinedVolumes(const Sphere<FloatTypeArg>& InSphere, const Sphere<FloatTypeArg>& InSphereToInclude);

		/*****************************************************************************************************
		* Distance computation
		*****************************************************************************************************/
		// Compute the distance between the spheres (not between their centers)	
		// (if it's less than zero, the spheres are overlapped)		
		template<class FloatTypeArg> 
		FloatTypeArg GetSphereSphereSqrDistance(const Sphere<FloatTypeArg>& InFirst, const Sphere<FloatTypeArg>& InSecond);

		/*****************************************************************************************************
		* Intersection testing
		*****************************************************************************************************/
		template<class FloatTypeArg>
		bool AreSpheresIntersecting(const Sphere<FloatTypeArg>& InA, const Sphere<FloatTypeArg>& InB, FloatTypeArg InTolerance = InGeoEps);

		/*****************************************************************************************************
		* Checking if inside
		*****************************************************************************************************/
		template<class FloatTypeArg>
		bool IsPointInsideSphere(const Vec<FloatTypeArg, 3>& InPoint, const Sphere<FloatTypeArg>& InSphere, FloatTypeArg InTolerance = InGeoEps);

		template<class FloatTypeArg>
		bool IsSphereInsideSphere(const Sphere<FloatTypeArg>& InInnerSphere, const Sphere<FloatTypeArg>& InSphere, FloatTypeArg InTolerance = InGeoEps);


		/*****************************************************************************************************
		* Global function implementation - near equality
		*****************************************************************************************************/
		template<class FloatTypeArg>
		bool IsNearEqual(const Sphere<FloatTypeArg>& InA, const Sphere<FloatTypeArg>& InB, FloatTypeArg InTolerance)
		{
			return IsNearEqual(InA.center, InB.center, InTolerance) && IsNearEqual(InA.radius, InB.radius, InTolerance);
		}		

		/*****************************************************************************************************
		* Global function implementation - volume computation
		*****************************************************************************************************/
		template<class FloatTypeArg>
		FloatTypeArg GetVolume(const Sphere<FloatTypeArg>& InSphere)
		{
			return SphereVolume(InSphere.radius);
		}


		/*****************************************************************************************************
		* Global function implementation - transformation
		*****************************************************************************************************/
		template<class FloatTypeArg>
		Sphere<FloatTypeArg>& Transform_NoScale(Sphere<FloatTypeArg>* pOutSphere, const SquareMatrix<FloatTypeArg, 4>& InMatrix)
		{
			BOOST_ASSERT(pOutSphere);
			pOutSphere->center *= InMatrix;
			return *pOutSphere;
		}

		template<class FloatTypeArg>
		Sphere<FloatTypeArg>& Transform_NoScale(Sphere<FloatTypeArg>* pOutSphere, const SquareMatrix<FloatTypeArg, 3>& InMatrix)
		{
			BOOST_ASSERT(pOutSphere);
			pOutSphere->center *= InMatrix;
			return *pOutSphere;
		}

		template<class FloatTypeArg>
		Sphere<FloatTypeArg> GetTransformed_NoScale(const Sphere<FloatTypeArg>& InSphere, const SquareMatrix<FloatTypeArg, 4>& InMatrix)
		{
			return Sphere<FloatTypeArg>(InSphere.center * InMatrix, InSphere.radius);
		}

		template<class FloatTypeArg>
		Sphere<FloatTypeArg> GetTransformed_NoScale(const Sphere<FloatTypeArg>& InSphere, const SquareMatrix<FloatTypeArg, 3>& InMatrix)
		{
			return Sphere<FloatTypeArg>(InSphere.center * InMatrix, InSphere.radius);
		}

		/*****************************************************************************************************
		* Combining bounding volumes implementation
		*****************************************************************************************************/
		template<class FloatTypeArg>
		Sphere<FloatTypeArg>& CombineVolumes(Sphere<FloatTypeArg>* pDestSphere, const Sphere<FloatTypeArg>& InSphereToInclude)
		{
			BOOST_ASSERT(pDestSphere);

			const auto vec_center_to_other_center           = InSphereToInclude->center - pDestSphere->center;
			const auto vec_center_to_other_center_length    = Length(vec_center_to_other_center);
			const auto unit_vec_center_to_other_center      = vec_center_to_other_center/ vec_center_to_other_center_length;

			const auto end_point                            = pDestSphere->center - unit_vec_center_to_other_center * pDestSphere->radius;
			const auto end_point_other                      = InSphereToInclude.center + unit_vec_center_to_other_center * InSphereToInclude.radius;			

			const auto new_origin                           = (end_point + end_point_other)/2;
			const auto new_rad                              = (vec_center_to_other_center_length + pDestSphere->radius + InSphereToInclude.radius)/2;

			pDestSphere->origin              = new_origin;
			pDestSphere->radius              = new_rad;
			return *pDestSphere;
		}

		template<class FloatTypeArg>
		Sphere<FloatTypeArg> GetCombinedVolumes(const Sphere<FloatTypeArg>& InSphere, const Sphere<FloatTypeArg>& InSphereToInclude)
		{
			Sphere<FloatTypeArg> res_sphere = InSphere;
			CombineVolumes(&res_sphere, InSphereToInclude);
			return res_sphere;
		}

		/*****************************************************************************************************
		* Intersection testing
		*****************************************************************************************************/
		template<class FloatTypeArg>
		bool AreSpheresIntersecting(const Sphere<FloatTypeArg>& InA, const Sphere<FloatTypeArg>& InB, FloatTypeArg InTolerance)
		{
			return SqrLength(InB.center - InA.center) <= Sqr(InA.radius + InB.radius + InTolerance);
		}

		/*****************************************************************************************************
		* Checking if inside
		*****************************************************************************************************/
		template<class FloatTypeArg>
		bool IsPointInsideSphere(const Vec<FloatTypeArg, 3>& InPoint, const Sphere<FloatTypeArg>& InSphere, FloatTypeArg InTolerance)
		{
			return SqrDistance(InPoint, InSphere.center) <= Sqrt(InSphere.radius + InTolerance);
		}

		template<class FloatTypeArg>
		bool IsSphereInsideSphere(const Sphere<FloatTypeArg>& InInnerSphere, const Sphere<FloatTypeArg>& InSphere, FloatTypeArg InTolerance)
		{
			return SqrDistance(InSphere.center, InInnerSphere.center) <= Sqr(InSphere.radius  - InInnerSphere.radius + InTolerance);
		}

		/*****************************************************************************************************
		* Class implementation
		*****************************************************************************************************/
		// Sphere operations
		template<class FloatTypeArg> 
		FloatTypeArg GetSphereSphereSqrDistance
		(
			const Sphere<FloatTypeArg>& InFirst, const Sphere<FloatTypeArg>& InSecond
		)
		{
			// 3 adds:
			typename VecType center_radius_vector = InFirst.center - InSecond.center;
			// 3 mults + 2 adds:
			typename FloatTypeArg sqr_distance = center_radius_vector.Dot(center_radius_vector);
			// 1 add
			typename FloatTypeArg radius_sum = InFirst.radius + InSecond.radius;
			// 1 mult
			typename FloatTypeArg sqr_radius_sum = radius_sum * radius_sum;
			return sqr_distance - sqr_radius_sum;
			// totally: adds: (3+2+1)=6; mults: (3+1)=4;
		}

		// Sphere impl
		template<class FloatTypeArg> 
		Sphere<FloatTypeArg>::Sphere()
		{
		}

		template<class FloatTypeArg> 
		Sphere<FloatTypeArg>::Sphere(const VecType& InCenter, float InRadius) :
			center(InCenter), radius(InRadius)
		{
		}
	} // Math
} // Dv

#endif // _MathUtil_CollisionDetection_volume_Sphere_h_