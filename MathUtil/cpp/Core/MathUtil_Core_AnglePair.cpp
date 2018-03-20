#include "../../h/Core/MathUtil_Core_AnglePair.h"

namespace DvUtils
{
	namespace Math
	{
		AnglePair LoadAnglePair_FromPropertyTree(const boost::property_tree::ptree& ptree)
		{
			AnglePair res_pair;
			boost::property_tree::ptree::const_assoc_iterator it_horz = ptree.find("horz");
			boost::property_tree::ptree::const_assoc_iterator it_vertical = ptree.find("vertical");
			if(it_horz != ptree.not_found())
			{
				res_pair.horz = it_horz->second.get_value<float>();
			}
			if(it_vertical != ptree.not_found())
			{
				res_pair.vertical = it_vertical->second.get_value<float>();
			}
			return res_pair;
		}
	}
} // DvUtils
