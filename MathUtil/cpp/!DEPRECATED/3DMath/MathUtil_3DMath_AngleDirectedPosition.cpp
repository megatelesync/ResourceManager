#include "../../h/3DMath/MathUtil_3DMath_AngleDirectedPosition.h"
#include "../../h/MathUtil_VectorBaseTypes.h"

namespace DvUtils
{
	namespace Math
	{
		AngleDirectedPosition LoadAngleDirectedPosition_FromPropertyTree(const boost::property_tree::ptree& ptree)
		{
			AngleDirectedPosition res_dir_pos;
			boost::property_tree::ptree::const_assoc_iterator it_direction = ptree.find("direction");
			boost::property_tree::ptree::const_assoc_iterator it_position = ptree.find("position");
			if(it_direction != ptree.not_found())
			{
				res_dir_pos.direction = LoadAnglePair_FromPropertyTree(it_direction->second);
			}
			if(it_position != ptree.not_found())
			{				
				Vec3<float> pos_vec = LoadVec3_FromPropertyTree<float>(it_position->second);
				res_dir_pos.position = pos_vec.ToFloat3XM();
			}
			return res_dir_pos;
		}
	} // math
} // DvUtils