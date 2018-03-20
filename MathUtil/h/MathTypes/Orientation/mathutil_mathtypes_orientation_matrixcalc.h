#include "MathUtil_MathTypes_Quaternion.h"
//#include "../MathUtil_MatrixBaseTypes.h"

namespace DvUtils
{
	namespace Math
	{
		// Calculate matrix based on the given quaternion
		//template<class FloatTypeArg>
		//void CalcMatrix(Mat44<FloatTypeArg> *pOutMatrix, const Quaternion<FloatTypeArg>& InQuaternion);


		/*********************************************************************************************************
		* IMPLEMENTATION
		*********************************************************************************************************/
		/*
		template<class FloatTypeArg>
		void CalcMatrix(Mat44<FloatTypeArg> *pOutMatrix, const Quaternion<FloatTypeArg>& InQuaternion)
		{
			assert(pOutMatrix != nullptr);
			assert(&InQuaternion != nullptr);
			FloatTypeArg[3][3] matrix;
			Quaternion_To_Matrix33(matrix, InQuaternion.GetQuat());
			pOutMatrix->Set(pOutMatrix->GetPointer(), 3, 3, 3 * sizeof(FloatTypeArg));			
		}
		*/
	} // Math
} // DvUtils