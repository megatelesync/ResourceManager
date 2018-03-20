#pragma once

namespace DvUtils
{
	namespace Math
	{
		template<class FloatTypeArg>
		class Euler
		{
		public:
			/*********************************************************************
			* Ctors
			**********************************************************************/
			// Default ctor: creates the zero euler angle.
			Euler();
			// Ctor: initialize from array of floats with a given stride
			Euler(const FloatTypeArg *pInArr, size_t StrideInBytes = sizeof(FloatTypeArg));			


			/*********************************************************************
			* Getters			
			**********************************************************************/
			const FloatTypeArg			*GetArr		()			const				{ return _euler; }
			FloatTypeArg				*GetArr		()								{ return _euler; }

			const FloatTypeArg			&GetPitch	()			const				{ return _euler[0]; };
			const FloatTypeArg			&GetYaw		()			const				{ return _euler[1]; }
			const FloatTypeArg			&GetRoll	()			const				{ return _euler[2]; }


			/*********************************************************************
			* Setters
			**********************************************************************/
			void						SetPitch	(const FloatTypeArg& InValue)			{ _euler[0] = InValue; }
			void						SetYaw		(const FloatTypeArg& InValue)			{ _euler[1] = InValue; }
			void						SetRoll		(const FloatTypeArg& InValue)			{ _euler[2] = InValue; }

		private:
			// element index corresponds to the axis index
			// element [0] is pitch
			// element [1] is yaw (heading)
			// element [2] is roll (bank)
			FloatTypeArg		_euler[3];
		};


		/*****************************************************************************
		* IMPLEMENTATION
		*****************************************************************************/
		template<class FloatTypeArg>
		Euler<FloatTypeArg>::Euler()
		{
		}

		template<class FloatTypeArg>
		Euler<FloatTypeArg>::Euler(const FloatTypeArg *pInArr, size_t StrideInBytes)
		{
			Copy_n(_euler, pInArr, StrideInBytes);
		}
	} // Math
} // DvUtils