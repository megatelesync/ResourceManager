#include "../../h/3DMath/MathUtil_3DMath_Frustum.h"

namespace DvUtils
{
	namespace Math
	{
		FrustumGeoInfo LoadFrustumGeoInfo_FromPropertyTree(const boost::property_tree::ptree& ptree)
		{
			FrustumGeoInfo res_info;
			boost::property_tree::ptree::const_assoc_iterator it_minZ = ptree.find("nearZ");
			boost::property_tree::ptree::const_assoc_iterator it_maximZ = ptree.find("farZ");
			boost::property_tree::ptree::const_assoc_iterator it_halfFOV = ptree.find("halfFOV");
			if(it_minZ != ptree.not_found())
			{
				res_info.zNear = it_minZ->second.get_value<float>();
			}
			if(it_maximZ != ptree.not_found())
			{
				res_info.zFar = it_maximZ->second.get_value<float>();
			}
			if(it_halfFOV != ptree.not_found())
			{
				res_info.halfFov = it_halfFOV->second.get_value<float>();
			}
			return res_info;
		}
	}
} // DV