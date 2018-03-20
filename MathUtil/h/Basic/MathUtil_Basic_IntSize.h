#ifndef _MathUtil_Basic_IntSize_h_
#define _MathUtil_Basic_IntSize_h_

#include <Windows.h>
#include <boost/property_tree/ptree.hpp>

namespace DvUtils
{
	namespace Math
	{
		/*********************************************************************************************************
		* Size with integer width and height
		*********************************************************************************************************/
		struct IntSize
		{
			/*********************************************************************************************************
			* GETTERS
			*********************************************************************************************************/
			UINT                                  width;
			UINT                                  height;

			/*********************************************************************************************************
			* HELPER GETTERS
			*********************************************************************************************************/
			inline double                         GetAspect                       () const                               { return static_cast<double>(width)/height; }

			/*********************************************************************************************************
			* CONSTRUCTORS
			*********************************************************************************************************/
			inline IntSize() : width(0), height(0) {}
			inline IntSize(UINT widthIn, UINT heightIn) : width(widthIn), height(heightIn) {}
		};
		/*********************************************************************************************************
		* EQUALITY OPERATIONS
		*********************************************************************************************************/
		inline bool operator==                      (const IntSize& first, const IntSize& second)                { return first.width == second.width && first.height == second.height; }
		inline bool operator!=                      (const IntSize& first, const IntSize& second)               { return !operator==(first, second); }

		/*********************************************************************************************************
		* LOADING FROM PROPERTY TREE
		*********************************************************************************************************/
		IntSize LoadIntSize_FromPropertyTree        (boost::property_tree::ptree& ptree);

		/*********************************************************************************************************
		* SETTING
		*********************************************************************************************************/
		inline void	SetSize                         (IntSize *pIntSize, UINT widthIn, UINT heightIn);
		inline void SetUnitSize                     (IntSize *pIntSize);

		/*********************************************************************************************************
		* HASHING
		*********************************************************************************************************/
		template<size_t maxDim> size_t hash         (const IntSize& sz);
		template<size_t maxDim> class IntSizeHashType;

		template<size_t maxDim> size_t hash(const IntSize& sz)
		{
			return sz.width * maxDim + sz.height;
		}

		template<size_t maxDim> class IntSizeHashType
		{
		public:
			inline size_t operator() (const IntSize& sz) const { return hash<maxDim>(sz); }
		};

		/*********************************************************************************************************
		* IntSize IMPLEMENTATION
		*********************************************************************************************************/
		inline void	SetSize		(IntSize *pIntSize, UINT widthIn, UINT heightIn)
		{
			pIntSize->width = widthIn;
			pIntSize->height = heightIn;
		}

		inline void SetUnitSize	(IntSize *pIntSize)
		{
			SetSize(pIntSize, 1.0F, 1.0F);
		}
	};
} // DvUtils

#endif // _MathUtil_Basic_IntSize_h_