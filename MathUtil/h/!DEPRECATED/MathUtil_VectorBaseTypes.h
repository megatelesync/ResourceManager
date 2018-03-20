#ifndef _MathUtil_VectorBaseTypes_h_
#define _MathUtil_VectorBaseTypes_h_

#define _CRT_SECURE_NO_WARNINGS
#include "MathUtil_Base.h"
#include <DirectXMath.h>
#include <cassert>
#include <string>
#include <cstdio>
#include "Core/MathUtil_Core_LinearAlgebraUtils.h"
#include <boost/format.hpp>
#include <boost/property_tree/ptree.hpp>
#include "MathTypes/BLAS/MathUtil_MathTypes_Vec4.h"

using namespace DirectX;

namespace DvUtils
{
	namespace Math
	{
	  template<class FloatTypeArg>
	  class Vec3
	  {
	  public:
		  typedef				FloatTypeArg				FloatType;
		  typedef				Vec3<FloatType>				ThisType;

		  inline				Vec3						();
		  inline				Vec3						(const XMVECTOR& vecIn);
		  inline				Vec3						(FloatType x, FloatType y, FloatType z);

		  // Initialize vector from memory of 3 values with a specified distance between component locations in bytes
		  template<class SrcFloatType>
		  inline				Vec3						(const SrcFloatType *pInVector, uint8_t InPerComponentStepInBytes = sizeof(SrcFloatType));

		  inline XMVECTOR		ToFloatXM					()									const;		  
		  inline XMFLOAT4		ToFloatVecXM				()									const;	
		  inline XMFLOAT3		ToFloat3XM					()									const;

		  // copying
		  inline ThisType&		operator=					(const ThisType& second);
		  inline Vec3										(const ThisType& second);

		  // comparison, near equality 
		  inline bool			IzNearEqual					(ThisType& second, FloatType eps)	const;
		  inline bool			IsZero						()									const;
		  inline bool			operator==					(const ThisType& second)			const;
		  inline bool			operator!=					(const ThisType& second)			const;

		  // getting components, setting components
		  ThisType&				Set							(const XMVECTOR& v);
		  ThisType&				Set							(FloatType x, FloatType y, FloatType z);

		  // Initialize vector from memory of 3 values with a specified distance between component locations in bytes
		  template<class SrcFloatType>
		  void					Set							(const SrcFloatType *pInVector, uint8_t InPerComponentStepInBytes = sizeof(SrcFloatType));

		  void					SetX						(FloatTypeArg NewX);
		  void					SetYY						(FloatTypeArg NewYY);
		  void					SetZZZ						(FloatTypeArg NewZZZ);

		  FloatType				X							()									const				{ return _x; }
		  FloatType				YY							()									const				{ return _y; } 
		  FloatType				ZZZ							()									const				{ return _z; }	

		  // convert to array of floating point values
		  const FloatType	   *ToFloatArr					()									const				{ return &_x; }
		  FloatType			   *ToFloatArr					()														{ return &_x; }

		  // length, square length, normalize
		  inline ThisType&		Normalize					();
		  inline ThisType		GetNormalized				()									const;
		  inline FloatType		SqrLength					()									const;
		  inline FloatType		Length						()									const;		  

		  // dot, cross
		  inline FloatType		DotWith						(const ThisType& second)			const;
		  inline ThisType		CrossWith					(const ThisType& second)			const;

		  // arithmetic
		  inline ThisType		operator-					()									const;
		  inline ThisType		operator+					(const ThisType& second)			const;
		  inline ThisType		operator-					(const ThisType& second)			const;
		  inline ThisType		operator*					(FloatType factor)					const;
		  inline ThisType		operator/					(FloatType factor)					const;

		  // assign-arithmetic
		  inline ThisType&		Negate						();
		  inline ThisType&		operator+=					(const ThisType& second);
		  inline ThisType&		operator-=					(const ThisType& second);
		  inline ThisType&		operator*=					(FloatType factor);
		  inline ThisType&		operator/=					(FloatType factor);

		  // ToString
		  inline std::string	ToDebugString				()									const;
		  inline std::string	ToString					()									const;

	  private:
		  FloatType _x, _y, _z;
	  };
	  template<class FloatTypeArg>
	  Vec3<FloatTypeArg> LoadVec3_FromPropertyTree(const boost::property_tree::ptree& ptree);

	  // Make the vector have absolute coordinates
	  template<class FloatTypeArg>	  
	  void Absolutize(Vec3<FloatTypeArg> *pVec);

	  template<class FloatTypeArg>
	  class Vec4
	  {
	  public:
		  typedef							FloatTypeArg						FloatType;
		  typedef							Vec4<FloatType>						ThisType;
		  typedef							Vec3<FloatType>						Vec3Type;

		  inline							Vec4								();
		  inline							Vec4								(const XMVECTOR& vecIn);
		  inline							Vec4								(FloatType x, FloatType y, FloatType z, FloatType w);
		  inline							Vec4								(const Vec3<FloatTypeArg>& vec3, FloatTypeArg w = 1.0F);

		  // Initialize vector from memory of 4 values with a specified distance between component locations in bytes
		  template<class SrcFloatType>
		  inline							Vec4								(const SrcFloatType *pInVector, uint8_t InPerComponentStepInBytes = sizeof(SrcFloatType));

		  /*******************************************************************************************************
		  * Conversion to other forms
		  *******************************************************************************************************/
		  inline XMVECTOR					ToFloatXM							()				const;		  
		  inline XMFLOAT4					ToFloatVecXM						()				const;

		  FloatTypeArg					   *GetPointer							()						{ return &_x; }
		  const FloatTypeArg			   *GetPointer							()				const	{ return &_x; }

		  // copying
		  inline ThisType& operator=(const ThisType& second);
		  inline Vec4(const ThisType& second);
		  // comparison, near equality 
		  inline bool IsNearEqual(ThisType& second, FloatType eps) const;
		  inline bool IsZero() const;
		  inline bool IsZeroXYZ() const;
		  inline bool operator==(const ThisType& second) const;
		  inline bool operator!=(const ThisType& second) const;
		  // getting components, setting components		  
		  ThisType& Set(const Vec3Type& other, FloatType w = 1.0F);
		  ThisType& Set(FloatType x, FloatType y, FloatType z);
		  ThisType& Set(FloatType x, FloatType y, FloatType z, FloatType w);

		  void					SetX						(FloatTypeArg NewX);
		  void					SetYY						(FloatTypeArg NewYY);
		  void					SetZZZ						(FloatTypeArg NewZZZ);
		  void					SetWWWW						(FloatTypeArg NewWWWW);

		  FloatType X() const { return _x; }
		  FloatType YY() const { return _y; } 
		  FloatType ZZZ() const { return _z; }		  
		  FloatType WWWW() const { return _w; }		  
		  // length, square length, normalize
		  inline ThisType& Normalize();
		  inline ThisType GetNormalized() const;
		  inline FloatType SqrLength() const;
		  inline FloatType Length() const;
		  // dot, cross
		  inline FloatType DotWith(const ThisType& second) const;
		  inline FloatType DotXYZWith(const ThisType& second) const;
		  inline ThisType CrossXYZWith(const ThisType& second) const;
		  // arithmetic
		  inline ThisType operator-() const;
		  inline ThisType operator+(const ThisType& second) const;
		  inline ThisType operator-(const ThisType& second) const;
		  inline ThisType operator*(FloatType factor) const;
		  inline ThisType operator/(FloatType factor) const;
		  // assign-arithmetic
		  inline ThisType& Negate();
		  inline ThisType& NegateXYZ();
		  inline ThisType& operator+=(const ThisType& second);
		  inline ThisType& operator-=(const ThisType& second);
		  inline ThisType& operator*=(FloatType factor);
		  inline ThisType& operator/=(FloatType factor);
		  // ToString
		  inline std::string ToDebugString() const;
		  inline std::string ToString() const;

	  private:
		  FloatType _x, _y, _z, _w;
	  };
	  template<class FloatTypeArg>
	  Vec4<FloatTypeArg> LoadVec4_FromPropertyTree(const boost::property_tree::ptree& ptree);

	  template<class FloatTypeArg>
	  struct Vec3Pair
      {
			typedef		Vec3Pair<FloatTypeArg>		ThisType;
			typedef		Vec3<FloatTypeArg>			Vec3Type;

            Vec3<FloatTypeArg> first;
            Vec3<FloatTypeArg> second;

            Vec3Pair(const Vec3Type& firstIn, const Vec3Type& secondIn) : first(firstIn), second(secondIn)
            {
				assert(&firstIn);
				assert(&secondIn);                
            }       
	  };

	  /// -------------------------------------------------IMPLEMENTATION Vec4 --------------------------------------------
	  template<class FloatTypeArg>
	  Vec4<FloatTypeArg>::Vec4() : 
		_x(0.0), _y(0.0), _z(0.0), _w(0.0) {}

	  template<class FloatTypeArg>
	  Vec4<FloatTypeArg>::Vec4(const XMVECTOR& vecIn) 
		  : _x(XMVectorGetX(vecIn)), _y(XMVectorGetY(vecIn)), _z(XMVectorGetZ(vecIn)), _w(XMVectorGetW(vecIn)) { assert(&vecIn); }

	  template<class FloatTypeArg>
	  Vec4<FloatTypeArg>::Vec4(FloatType x, FloatType y, FloatType z, FloatType w) 
		  : _x(x), _y(y), _z(z), _w(w) {}

	  template<class FloatTypeArg>
	  template<class SrcFloatType>	  
	  Vec4<FloatTypeArg>::Vec4(const SrcFloatType *pInVector, uint8_t InPerComponentStepInBytes)
	  {
			_x = static_cast<FloatTypeArg>(*Util::AddPtr(pInVector, InPerComponentStepInBytes * 0));
			_y = static_cast<FloatTypeArg>(*Util::AddPtr(pInVector, InPerComponentStepInBytes * 1));
			_z = static_cast<FloatTypeArg>(*Util::AddPtr(pInVector, InPerComponentStepInBytes * 2));
			_w = static_cast<FloatTypeArg>(*Util::AddPtr(pInVector, InPerComponentStepInBytes * 3));
	  }

	  template<class FloatTypeArg>
	  void Vec4<FloatTypeArg>::SetX(FloatTypeArg NewX)
	  {
		  _x = NewX;
	  }

	  template<class FloatTypeArg>
	  void Vec4<FloatTypeArg>::SetYY(FloatTypeArg NewYY)
	  {
		  _y = NewYY;
	  }

	  template<class FloatTypeArg>
	  void Vec4<FloatTypeArg>::SetZZZ(FloatTypeArg NewZZZ)
	  {
		  _z = NewZZZ;
	  }

	  template<class FloatTypeArg>
	  void Vec4<FloatTypeArg>::SetWWWW(FloatTypeArg NewWWWW)
	  {
		  _w = NewWWWW;
	  }

	  template<class FloatTypeArg>
	  XMVECTOR Vec4<FloatTypeArg>::ToFloatXM() const 
	  { 
		  return XMVectorSet((FloatType)_x, (FloatType)_y, (FloatType)_z, (FloatType)_w);	  
	  }

	  template<class FloatTypeArg>
	  XMFLOAT4 Vec4<FloatTypeArg>::ToFloatVecXM() const
	  {
		  return XMFLOAT4(_x, _y, _z, _w);
	  }

	  template<class FloatTypeArg>
	  Vec4<FloatTypeArg>::Vec4(const Vec3<FloatTypeArg>& vec3, FloatTypeArg w) :
		  _x(vec3.GetX()), _y(vec3.GetYY()), _z(vec3.GetZZZ()), _w(w)
	  {
	  }

	  template<class FloatTypeArg>
	  bool Vec4<FloatTypeArg>::IsNearEqual(ThisType& second, FloatType eps) const
	  {
		  return abs(_x-second._x) <= eps && abs(_y-second._y) <= eps && abs(_z-second._z) <= eps && abs(_w-second._w); 	
	  }

	  template<class FloatTypeArg>
	  bool Vec4<FloatTypeArg>::IsZero() const
	  {
		  return IsZeroXYZ() && _w == 0.0;  	  
	  }

	  template<class FloatTypeArg>
	  bool Vec4<FloatTypeArg>::IsZeroXYZ() const
	  {
		  return _x == 0.0 && _y == 0.0 && _z == 0.0;  
	  }

	  template<class FloatTypeArg>
	  bool Vec4<FloatTypeArg>::operator==(const ThisType& second) const
	  {
		  assert(&second); 
		  return _x == second._x && _y == second._y && _z == second._z && _w == second._w; 
	  }

	  template<class FloatTypeArg>
	  bool Vec4<FloatTypeArg>::operator!=(const ThisType& second) const
	  {
		  assert(&second); 
		  return !(*this == second); 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType& Vec4<FloatTypeArg>::Set(const Vec3Type& other, FloatType w = 1.0F)
	  {
		  Set(other.x, other.y, other.z);
		  _w = w;
		  return *this;
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType& Vec4<FloatTypeArg>::Set(FloatType x, FloatType y, FloatType z) 
	  { 
		  _x = x; 
		  _y = y; 
		  _z = z; 
		  return *this; 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType& Vec4<FloatTypeArg>::Normalize()
	  {
		  FloatType length = Length();
		  *this /= length;
		  return *this; 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType Vec4<FloatTypeArg>::GetNormalized() const
	  {
		  ThisType res(*this);
		  res.Normalize();
		  return res; 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::FloatType Vec4<FloatTypeArg>::SqrLength() const
	  {
		  return _x*_x + _y*_y + _z *_z + _w*w;
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::FloatType Vec4<FloatTypeArg>::Length() const
	  {
		  return Sqrt(SqrLength());
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::FloatType Vec4<FloatTypeArg>::DotXYZWith(const ThisType& second) const
	  {
		  assert(&second); 
		  return _x*second._x + _y*second._y + _z*second._z; 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::FloatType Vec4<FloatTypeArg>::DotWith(const ThisType& second) const
	  {
		  assert(&second); 
		  return _x*second._x + _y*second._y + _z*second._z + _w*second._w; 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType Vec4<FloatTypeArg>::CrossXYZWith(const ThisType& second) const
	  {
		  assert(&second); 
		  FloatType x = _y * second._z - second._y * _z;
		  FloatType y = -(_x * second._z - second._x * _z);
		  FloatType z = _x * second._y - second._x * _y;
		  return ThisType(x,y,z,1);
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType Vec4<FloatTypeArg>::operator-() const
	  {
		  return ThisType(-_x, -_y, -_z, -w); 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType Vec4<FloatTypeArg>::operator+(const ThisType& second) const
	  {
		  assert(&second); 
		  return ThisType(_x+second._x, _y+second._y, _z+second._z, _w+second._w);	  
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType Vec4<FloatTypeArg>::operator-(const ThisType& second) const
	  {
		  assert(&second); 
		  return ThisType(_x-second._x,_y-second._y,_z-second._z, _w-second._w); 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType Vec4<FloatTypeArg>::operator*(FloatType factor) const
	  {
		  return ThisType(_x*factor,_y*factor,_z*factor, _w*factor); 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType Vec4<FloatTypeArg>::operator/(FloatType factor) const
	  {
		  return ThisType(_x/factor, _y/factor, _z/factor, _w/factor); 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType& Vec4<FloatTypeArg>::Negate()
	  {
		  NegateXYZ();
		  _w = -_w;
		  return *this;
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType& Vec4<FloatTypeArg>::NegateXYZ()
	  {
		  _x = -_x; 
		  _y = -_y; 
		  _z = -_z; 
		  return *this; 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType& Vec4<FloatTypeArg>::operator+=(const ThisType& second)
  	  {
		  _x += second._x;
		  _y += second._y; 
		  _z += second._z; 
		  _w += second._w;
		  return *this; 
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType& Vec4<FloatTypeArg>::operator-=(const ThisType& second)
	  {
		  _x -= second._x;
		  _y -= second._y; 
		  _z -= second._z; 
		  _w -= second._w;
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType& Vec4<FloatTypeArg>::operator*=(FloatType factor)
	  {
		  _x *= factor;
		  _y *= factor;
		  _z *= factor;
		  _w *= factor;
	  }

	  template<class FloatTypeArg>
	  typename Vec4<FloatTypeArg>::ThisType& Vec4<FloatTypeArg>::operator/=(FloatType factor)
	  {
		  _x /= factor;
		  _y /= factor;
		  _z /= factor;
		  _w /= factor;
	  }

	  template<class FloatTypeArg>
	  std::string Vec4<FloatTypeArg>::ToDebugString() const
	  {
			return (boost::format("{X=%1% Y=%2% Z=%3% W=%4%}") % X() % YY() % ZZZ() % WWWW()).str();
	  }

	  template<class FloatTypeArg>
	  std::string Vec4<FloatTypeArg>::ToString() const
	  {						
			return (boost::format("{X=%1% Y=%2% Z=%3% W=%4%}") % X() % YY() % ZZZ() % WWWW()).str();			  			  
	  }

	  // Impl Vec3 operations
	  template<class FloatTypeArg>	  
	  void Absolutize(Vec3<FloatTypeArg> *pVec)
	  {
		  FloatTypeArg newX = Abs(pVec->X());
		  FloatTypeArg newY = Abs(pVec->Y());
		  FloatTypeArg newZ = Abs(pVec->Z());
		  pVec->Set(newX, newY, newZ);
	  }

	  /// ------------------------------------------------- IMPLEMENTATION Vec 3-------------------------------------------
	  template<class FloatTypeArg>
	  Vec3<FloatTypeArg>::Vec3() : 
		_x(0.0), _y(0.0), _z(0.0) {}

	  template<class FloatTypeArg>
	  Vec3<FloatTypeArg>::Vec3(const XMVECTOR& vecIn) 
		  : _x(XMVectorGetX(vecIn)), _y(XMVectorGetY(vecIn)), _z(XMVectorGetZ(vecIn)) { assert(&vecIn); }

	  template<class FloatTypeArg>
	  Vec3<FloatTypeArg>::Vec3(FloatType x, FloatType y, FloatType z) 
		  : _x(x), _y(y), _z(z){}

	  // Initialize vector from memory of 3 values with a specified distance between component locations in bytes
	  template<class FloatTypeArg>
	  template<class SrcFloatType>	  
	  Vec3<FloatTypeArg>::Vec3(const SrcFloatType *pInVector, uint8_t InPerComponentStepBytes)
	  {
		  Set(pInVector, InPerComponentStepBytes);
      }

	  template<class FloatTypeArg>
	  template<class SrcFloatType>	  
	  void Vec3<FloatTypeArg>::Set
	  (
	      const SrcFloatType *pInVector, 
		  uint8_t InPerComponentStepInBytes
	  )
	  {
		  _x = static_cast<FloatTypeArg>(*Util::AddPtr(pInVector, InPerComponentStepBytes * 0));
		  _y = static_cast<FloatTypeArg>(*Util::AddPtr(pInVector, InPerComponentStepBytes * 1));
		  _z = static_cast<FloatTypeArg>(*Util::AddPtr(pInVector, InPerComponentStepBytes * 2));
	  }

	  template<class FloatTypeArg>
	  XMVECTOR Vec3<FloatTypeArg>::ToFloatXM() const 
	  { 
		  return XMVectorSet((FloatType)_x, (FloatType)_y, (FloatType)_z, 1.0f);	  
	  }

	  template<class FloatTypeArg>
	  XMFLOAT4 Vec3<FloatTypeArg>::ToFloatVecXM() const
	  {
		  return XMFLOAT4((FloatType)_x, (FloatType)_y, (FloatType)_z, 1.0f);	  
	  }

	  template<class FloatTypeArg>
	  XMFLOAT3 Vec3<FloatTypeArg>::ToFloat3XM() const
	  {
		  return XMFLOAT3((FloatType)_x, (FloatType)_y, (FloatType)_z);	  
	  }

	  // copying
	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType& Vec3<FloatTypeArg>::operator=(const ThisType& second) 
	  { 
		  assert(&second); 
		  _x = second._x; 
		  _y = second._y; 
		  _z = second._z; 
		  return *this; 
	  }

	  template<class FloatTypeArg>
	  Vec3<FloatTypeArg>::Vec3(const ThisType& second) : 
		_x(second._x), _y(second._y), _z(second._z) 
	  { 
		  assert(&second); 
	  }

	  // comparison, near equality 
	  template<class FloatTypeArg>
	  bool Vec3<FloatTypeArg>::IzNearEqual(ThisType& second, FloatType eps) const 
	  { 
		  assert(&second);  
		  return abs(_x-second._x) <= eps && abs(_y-second._y) <= eps && abs(_z-second._z) <= eps; 	  
	  }

	  template<class FloatTypeArg>
	  bool Vec3<FloatTypeArg>::IsZero() const 
	  { 
		  return _x == 0.0 && _y == 0.0 && _z == 0.0; 
	  }

	  template<class FloatTypeArg>
	  bool Vec3<FloatTypeArg>::operator==(const ThisType& second) const 
	  { 
		  assert(&second); 
		  return _x == second._x && _y == second._y && _z == second._z; 
	  }

	  template<class FloatTypeArg>
	  bool Vec3<FloatTypeArg>::operator!=(const ThisType& second) const 
	  { 
		  assert(&second); 
		  return !(*this == second); 
	  }

	  // length, square length, normalize
	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType& Vec3<FloatTypeArg>::Normalize() 
	  { 
		  FloatType length = Length();
		  *this /= length;
		  return *this; 
	  }

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType Vec3<FloatTypeArg>::GetNormalized() const 
	  { 
		  ThisType res(*this);
		  res.Normalize();
		  return res; 
	  }

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::FloatType Vec3<FloatTypeArg>::SqrLength() const 
	  { 
		  return _x*_x + _y*_y + _z*_z; 
	  }

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::FloatType Vec3<FloatTypeArg>::Length() const 
	  { 
		  return sqrt(SqrLength()); 
	  }

	  // dot, cross
	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType Vec3<FloatTypeArg>::CrossWith(const ThisType& second) const 
	  { 
		  assert(&second); 
		  FloatType x = _y * second._z - second._y * _z;
		  FloatType y = -(_x * second._z - second._x * _z);
		  FloatType z = _x * second._y - second._x * _y;
		  return ThisType(x,y,z); 
	  }

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::FloatType Vec3<FloatTypeArg>::DotWith(const ThisType& second) const 
	  { 
		  assert(&second); 
		  return _x*second._x + _y*second._y + _z*second._z; 
	  }

	  // arithmetic
	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType Vec3<FloatTypeArg>::operator-() const 
	  { 
		  return ThisType(-_x, -_y, -_z); 
	  }

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType Vec3<FloatTypeArg>::operator+(const ThisType& second) const 
	  { 
		  assert(&second); 
		  return ThisType(_x+second._x, _y+second._y, _z+second._z);	  
	  } 

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType Vec3<FloatTypeArg>::operator-(const ThisType& second) const 
	  { 
		  assert(&second); 
		  return ThisType(_x-second._x,_y-second._y,_z-second._z); 
	  } 

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType Vec3<FloatTypeArg>::operator*(FloatType factor) const 
	  { 
		  return ThisType(_x*factor,_y*factor,_z*factor); 
	  } 

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType Vec3<FloatTypeArg>::operator/(FloatType factor) const 
	  { 
		  assert(factor != 0.0);
		  return ThisType(_x/factor,_y/factor,_z/factor); 
	  }	

	  // assign-arithmetic
	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType& Vec3<FloatTypeArg>::Negate() 
	  { 
		  _x = -_x; 
		  _y = -_y; 
		  _z = -_z; 
		  return *this; 
	  }

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType& Vec3<FloatTypeArg>::operator+=(const ThisType& second) 
	  { 
		  assert(&second); 
		  _x += second._x;
		  _y += second._y; 
		  _z += second._z; 
		  return *this; 
	  } 

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType& Vec3<FloatTypeArg>::operator-=(const ThisType& second) 
	  { 
		  assert(&second); 
		  _x -= second._x;
		  _y -= second._y;
		  _z -= second._z; 
		  return *this; 
	  } 

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType& Vec3<FloatTypeArg>::operator*=(FloatType factor) 
	  { 
		  _x*=factor; 
		  _y*=factor; 
		  _z*=factor; 
		  return *this;
	  } 

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType& Vec3<FloatTypeArg>::operator/=(FloatType factor) 
	  { 
		  assert(factor != 0.0);
		  _x/=factor; 
		  _y/=factor; 
		  _z/=factor; 
		  return *this;
	  }

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType& Vec3<FloatTypeArg>::Set(const XMVECTOR& v)
	  {
		  _x = XMVectorGetX(v); _y = XMVectorGetY(v); _z = XMVectorGetZ(v); return *this; 
	  }

	  template<class FloatTypeArg>
	  typename Vec3<FloatTypeArg>::ThisType& Vec3<FloatTypeArg>::Set(FloatType x, FloatType y, FloatType z)
	  {
		  _x = x;
		  _y = y;
		  _z = z;
		  return *this;
	  }

	  template<class FloatTypeArg>
	  void Vec3<FloatTypeArg>::SetX(FloatTypeArg NewX)
	  {
		  _x = NewX;
	  }

	  template<class FloatTypeArg>
	  void Vec3<FloatTypeArg>::SetYY(FloatTypeArg NewYY)
	  {
		  _y = NewYY;
	  }

	  template<class FloatTypeArg>
	  void Vec3<FloatTypeArg>::SetZZZ(FloatTypeArg NewZZZ)
	  {
		  _z = NewZZZ;
	  }


	  // ToString
	  template<class FloatTypeArg>
	  std::string Vec3<FloatTypeArg>::ToDebugString() const
	  {
			return (boost::format("{X=%1% Y=%2% Z=%3% W=%4%}") % X() % YY() % ZZZ()).str();
	  }

	  template<class FloatTypeArg>
	  std::string Vec3<FloatTypeArg>::ToString() const
	  {						
			return (boost::format("{X=%1% Y=%2% Z=%3% W=%4%}") % X() % YY() % ZZZ()).str();			  		
	  }	  

	  template<class FloatTypeArg>
	  Vec3<FloatTypeArg> LoadVec3_FromPropertyTree(const boost::property_tree::ptree& ptree)
	  {
		  typename Vec3<FloatTypeArg> res_vec;
		  boost::property_tree::ptree::const_assoc_iterator it_x = ptree.find("X");
		  boost::property_tree::ptree::const_assoc_iterator it_y = ptree.find("Y");
		  boost::property_tree::ptree::const_assoc_iterator it_z = ptree.find("Z");		  
		  if(it_x != ptree.not_found())
		  {
			  res_vec.SetX(it_x->second.get_value<float>());
		  }
		  if(it_y != ptree.not_found())
		  {
			  res_vec.SetYY(it_y->second.get_value<float>());
		  }
		  if(it_z != ptree.not_found())
		  {
			  res_vec.SetZZZ(it_z->second.get_value<float>());
		  }
		  return res_vec;
	  }

	  template<class FloatTypeArg>
	  Vec4<FloatTypeArg> LoadVec4_FromPropertyTree(const boost::property_tree::ptree& ptree)
	  {
		  typename Vec4<FloatTypeArg> res_vec;
		  boost::property_tree::ptree::const_assoc_iterator it_x = ptree.find("X");
		  boost::property_tree::ptree::const_assoc_iterator it_y = ptree.find("Y");
		  boost::property_tree::ptree::const_assoc_iterator it_z = ptree.find("Z");		  
		  boost::property_tree::ptree::const_assoc_iterator it_w = ptree.find("W");	
		  if(it_x != ptree.not_found())
		  {
			  res_vec.SetX(it_x->second.get_value<float>());
		  }
		  if(it_y != ptree.not_found())
		  {
			  res_vec.SetYY(it_y->second.get_value<float>());
		  }
		  if(it_z != ptree.not_found())
		  {
			  res_vec.SetZZZ(it_z->second.get_value<float>());
		  }
		  if(it_w != ptree.not_found())
		  {
			  res_vec.SetWWWW(it_w->second.get_value<float>());
		  }
		  return res_vec;
	  }
	} // Math
} // Dv

#endif // _MathUtil_VectorBase_h_