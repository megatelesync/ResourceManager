#include "MathUtil_Vector3types.h"
#include "MathUtil_Vector3DoubleTypes.h"
#include "MathUtil_DoubleVector3.h"
#include "MathUtil_VectorBaseTypes.h"
#include <iostream>

using namespace DvUtils::Math;

void test_float_vector3();
void test_double_vector3();

int main(int arc, char *argv[])
{
	char c;
	test_double_vector3();
	std::cin >> c;
	DvUtils::Math::Vector3::X1Y1.ToFloatVecXM();
}

void test_float_vector3()
{
	Vector3 vec;	
	Vector3 otherVec(1.0F,2.0F,3.0F);	
	vec += otherVec;
	Vector3 res = vec + otherVec;
}

void test_double_vector3()
{
	DvUtils::Math::DoubleVector3 defaultVec;
	DvUtils::Math::DoubleVector3 zVec (0.0F, 0.0F, 1.0F);
	std::cout << defaultVec.ToDebugString() << std::endl;	
	std::cout << zVec.ToDebugString() << std::endl;
}