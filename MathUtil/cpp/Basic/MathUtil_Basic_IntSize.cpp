#include "../../h/Basic/MathUtil_Basic_IntSize.h"

namespace DvUtils
{
	namespace Math
	{
		IntSize LoadIntSize_FromPropertyTree(boost::property_tree::ptree& ptree)
		{
			IntSize res_size;
			boost::property_tree::ptree::const_assoc_iterator it_width = ptree.find("width");
			boost::property_tree::ptree::const_assoc_iterator it_height = ptree.find("height");
			if(it_width != ptree.not_found())
			{
				res_size.width = it_width->second.get_value<UINT>();
			}
			if(it_height != ptree.not_found())
			{
				res_size.height = it_height->second.get_value<UINT>();
			}
			return res_size;
		}
	} // Math
} // DvUtils