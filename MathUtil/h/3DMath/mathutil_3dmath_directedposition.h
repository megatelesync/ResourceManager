#ifndef _MathUtil_3DMath_DirectedPosition_h_
#define _MathUtil_3DMath_DirectedPosition_h_

#include "../MathTypes/BLAS/MathUtil_BLAS.h"
#include <boost/property_tree/ptree.hpp>

namespace DvUtils
{
	namespace Math
	{
		// Generalized directed position
		template<class FloatTypeArg>
		struct DirPosition
		{
			typedef							Vec<FloatTypeArg, 3>					VecType;
			typedef							DirPosition<FloatTypeArg>				ThisType;			

			VecType							position;
			VecType							direction;

			DirPosition();
			DirPosition(const VecType& InPosition, const VecType& InDirection);			
		};
		template<class FloatTypeArg>
		DirPosition<FloatTypeArg> LoadDirPosition_FromPropertyTree(const boost::property_tree::ptree& ptree);



		/******************************************************************************************************
		* WARNING: Used for the dx11 constant buffers!
		******************************************************************************************************/
		struct DirectedPosition
		{	
			/******************************************************************************************************
			* Public variables
			******************************************************************************************************/
			_declspec(align(16)) Vec<float, 3> position;
			_declspec(align(16)) Vec<float, 3> direction;


			/******************************************************************************************************
			* ctors
			******************************************************************************************************/
			inline DirectedPosition();
			inline DirectedPosition(const Vec<float, 3>& positionIn, const Vec<float, 3>& directionIn);
		};
		// returns row-major affine matrix for a given directed position
		inline DirectX::XMMATRIX CalcAffineMatrix(const DirectedPosition& directedPosition);
		// returns row-major view matrix for a given directed position
		inline DirectX::XMMATRIX CalcViewMatrix(const DirectedPosition& directedPosition);





		// DirPosition impl
		template<class FloatTypeArg>
		DirPosition<FloatTypeArg>::DirPosition() {}

		template<class FloatTypeArg>
		DirPosition<FloatTypeArg>::DirPosition(const VecType& InPosition, const VecType& InDirection) :
			position(InPosition), direction(InDirection)
		{
		}

		// DirectedPosition impl
		inline DirectedPosition::DirectedPosition() : 
			position(0,0,0), 
			direction(0,0,0) {}
		inline DirectedPosition::DirectedPosition(const Vec<float, 3>& positionIn, const Vec<float, 3>& directionIn) :
			position(positionIn), 
			direction(directionIn) {}

		template<class FloatTypeArg>
		DirPosition<FloatTypeArg> LoadDirPosition_FromPropertyTree(const boost::property_tree::ptree& ptree)
		{
			typename DirPosition<FloatTypeArg> res_dirpos;
			boost::property_tree::ptree::const_assoc_iterator it_position = ptree.find("position");
			boost::property_tree::ptree::const_assoc_iterator it_direction = ptree.find("direction");
			if(it_position != ptree.not_found())
			{
				res_dirpos.position = LoadVec3_FromPropertyTree<FloatTypeArg>(it_position->second);
			}
			if(it_direction != ptree.not_found())
			{
				res_dirpos.direction = LoadVec3_FromPropertyTree<FloatTypeArg>(it_direction->second);
			}
			return res_dirpos;
		}
	} // Math
} // Dv

#endif // _MathUtil_3DMath_DirectedPosition_h_