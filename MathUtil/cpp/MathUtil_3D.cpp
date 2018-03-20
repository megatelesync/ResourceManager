#include "../h/MathUtil_3D.h"

namespace DvUtils
{
   namespace Math
   {
	  // floating-point related constants      

	   const Vector3 Vector3::Zero = Vector3();
	   const DoubleVector3 DoubleVector3::Zero = DoubleVector3();
	   const Quat Quat::Identity = Quat();
	   const Matrix Matrix::Identity = Matrix();

	   // Vector3 typedefs
	   const Vector3 Vector3::XX = Vector3(1.0F, 0.0F, 0.0F);
	   const Vector3 Vector3::Y = Vector3(0.0F, 1.0F, 0.0F);
	   const Vector3 Vector3::Z = Vector3(0.0F, 0.0F, 1.0F);

	   const Vector3 Vector3::CounterX = Vector3(-1.0F, 0.0F, 0.0F);
	   const Vector3 Vector3::CounterY = Vector3(0.0F, -1.0F, 0.0F);
	   const Vector3 Vector3::CounterZ = Vector3(0.0F, 0.0F, -1.0F);
		  
	   // vectors 2D values
	   const Vector3 Vector3::X1Y1 = Vector3(1.0F, 1.0F, 0.0F);
	   const Vector3 Vector3::X1Z1 = Vector3(1.0F, 0.0F, 1.0F);
	   const Vector3 Vector3::Y1Z1 = Vector3(0.0F, 1.0F, 1.0F);

	   const Vector3 Vector3::XCounter1Y1 = Vector3(-1.0F, 1.0F, 0.0F);
	   const Vector3 Vector3::X1YCounter1 = Vector3(0.0F, -1.0F, 0.0F);
	   const Vector3 Vector3::XCounter1YCounter1 = Vector3(-1.0F, -1.0F, 0.0F);
	   const Vector3 Vector3::XCounter1Z1 = Vector3(-1.0F, 0.0F, 1.0F);
	   const Vector3 Vector3::X1ZCounter1 = Vector3(1.0F, 0.0F, -1.0F);	   
	   const Vector3 Vector3::XCounter1ZCounter1 = Vector3(-1.0F, 0.0F, -1.0F);
	   const Vector3 Vector3::YCounter1Z1 = Vector3(0.0F, -1.0F, 1.0F);
	   const Vector3 Vector3::Y1ZCounter1 = Vector3(0.0F, 1.0F, -1.0F);
	   const Vector3 Vector3::YCounter1ZCounter1 = Vector3(0.0F, -1.0F, -1.0F);
   } // namespace Math
} // namespace DvUtils