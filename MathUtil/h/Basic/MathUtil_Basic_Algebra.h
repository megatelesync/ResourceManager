#ifndef _MathUtil_Basic_Algebra_h_
#define _MathUtil_Basic_Algebra_h_

namespace DvUtils
{
	namespace Math
	{
		class PowerCache2
		{
		public:
			inline PowerCache2();

			float			GetValue				()						const								{ return _value; }
			float			GetValuePower2			()						const								{ return _valuePow2; }

			void			SetValue				(float InValue);			

		private:			
			float _valuePow2;
			float _value;
		};

		class PowerCache3
		{
		public:
			inline PowerCache3();

			float							GetValue				()						const					{ return _power2Cache.GetValue(); }
			float							GetValuePower2			()						const					{ return _power2Cache.GetValuePower2(); }
			float							GetValuePower3			()						const					{ return _valuePow3; }
			const PowerCache2&				GetPower2Cache			()						const					{ return _power2Cache; } 			

			void							SetValue				(float InValue);		

		private:
			PowerCache2  _power2Cache;
			float		 _valuePow3;
		};

		struct LinearFunction2D
		{
			float coeff[3];

			inline LinearFunction2D();
			inline LinearFunction2D(float InCoeff2, float InCoeff1, float InCoeff0);
		};
		inline float CalculateLinearFunction2D(const LinearFunction2D& function, const PowerCache2& t);

		struct LinearFunction3D
		{
			// coefficients for each power (array = power);
			float coeff[4];			

			inline LinearFunction3D();
			inline LinearFunction3D(float InCoeff3, float InCoeff2, float InCoeff1, float InCoeff0);
		};
		// Calculate value of the linear function at the given point t
		inline float CalculateLinearFunction3D(const LinearFunction3D& function, const PowerCache3& t);
		



		// Impl LinearFunction3D
		inline LinearFunction3D::LinearFunction3D() 
		{
			coeff[0] = coeff[1] = coeff[2] = coeff[3] = 0.0F;
		}

		inline LinearFunction3D::LinearFunction3D(float InCoeff3, float InCoeff2, float InCoeff1, float InCoeff0)
		{
			coeff[0] = InCoeff0;
			coeff[1] = InCoeff1;
			coeff[2] = InCoeff2;
			coeff[3] = InCoeff3;
		}				
		inline float CalculateLinearFunction3D(const LinearFunction3D& function, const PowerCache3& t)
		{
			return function.coeff[0] + 
				   function.coeff[1] * t.GetValue() + 
				   function.coeff[2] * t.GetValuePower2() + 
				   function.coeff[3] * t.GetValuePower3();
		}		

		// Impl LinearFunction2D
		inline LinearFunction2D::LinearFunction2D()
		{
			coeff[0] = coeff[1] = coeff[2] = 0.0F;
		}

		inline LinearFunction2D::LinearFunction2D(float InCoeff2, float InCoeff1, float InCoeff0)
		{
			coeff[0] = InCoeff0;
			coeff[1] = InCoeff1;
			coeff[2] = InCoeff2;			
		}
		inline float CalculateLinearFunction2D(const LinearFunction2D& function, const PowerCache2& t)
		{
			return function.coeff[0] + 
				   function.coeff[1] * t.GetValue() + 
				   function.coeff[2] * t.GetValuePower2();				   
		}


		// PowerCache2 impl
		inline PowerCache2::PowerCache2() : _value(0.0F), _valuePow2(0.0F) {}
		inline void	PowerCache2::SetValue(float InValue)
		{
			_value = InValue;
			_valuePow2 = InValue * InValue;
		}

		// PowerCache3 impl
		inline PowerCache3::PowerCache3() : _valuePow3(0.0F) {} 

		inline void PowerCache3::SetValue(float InValue)
		{
			_power2Cache.SetValue(InValue);
			_valuePow3 = _power2Cache.GetValuePower2() * InValue;
		}
	} // Math
} // DvUtils

#endif // _MathUtil_Basic_Algebra_h_