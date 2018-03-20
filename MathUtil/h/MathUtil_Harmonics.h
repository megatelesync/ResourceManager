#ifndef _MathUtil_Harmonics_h_
#define _MathUtil_Harmonics_h_

#include "MathUtil_Vector3.h"
#include <vector>

namespace DvUtils
{
	namespace Math
	{
		// Calculate the sinusoid function: y = amplitude * sin(frequency * arg + phase)		
		inline float		CalcSinusoid			(float amplitude, float frequency, float phase, float arg);		
		inline float		CalcCosinusoid			(float amplitude, float frequency, float phase, float arg);
		// Calculate only sinus or cosinus of the sinusoid (without multipliying with the amplitude)
		inline float		CalcSinusoidSin			(float frequency, float phase, float arg);
		inline float		CalcSinusoidCos			(float frequency, float phase, float arg);
		// Calculate the sinusoid argument = frequency * arg + phase
		inline float		CalcSinArg				(float frequency, float phase, float arg);
		// Calculate wave path along its direction given a point X, Y
		inline float		CalcWavePath			(const Vector3& normalized_direction, float X, float Y);

		struct SinHarmonic
		{
			// length of the wave (distance between crests)
			float wave_length;
			// amplitude of the wave
			float amplitude;

			inline SinHarmonic();
			inline SinHarmonic(float wave_length_in, float amplitude_in);
		};
		inline float CalcFrequency		(float wave_length);
		// Get speed described as phase constant.
		// Inputs:
		//		speed - speed in units of the wave_length
		inline float GetPhaseSpeed		(float wave_length, float speed);

		struct WaveHarmonic
		{			
			SinHarmonic				harmonic;
			// normalized direction of the wave propagation
			Vector3					normalized_direction;
			// phase speed
			float					phase_speed;

			inline WaveHarmonic();
			inline WaveHarmonic(const SinHarmonic& harmonicIn, const Vector3& normalized_direction_in, float phase_speed_in);
			inline WaveHarmonic(float wave_length_in, float amplitude_in, const Vector3& normalized_direction_in, float phase_speed_in);
		};		

		// Current time and position
		struct WaveContext
		{
			float T;
			float X;
			float Y;

			inline void SetPosition(float X_in, float Y_in);

			inline WaveContext();
			inline WaveContext(float T_in, float X_in, float Y_in);
		};

		// Cached harmonic parameters for a given time and position
		class WaveHarmonicCache
		{
		public:
			inline WaveHarmonicCache();
			inline WaveHarmonicCache(const WaveHarmonic& harmonic_in);

			inline void SetHarmonic(const WaveHarmonic& harmonic_in);
			
			// Result of the sinusoid function
			float							GetSinusoidResult					()		const		{ return _sinusoid_result; }
			float							GetCosinusoidResult					()		const		{ return _cosinusoid_result; }			
			float							GetSinusoidSin						()		const		{ return _sinusoid_sin; }						
			// Argument of the result trigonomic function (sinus or cosinus)
			float							GetTrigArgument						()		const		{ return _trig_argument; }

			const WaveHarmonic&				GetWaveHarmonic						()		const		{ return _harmonic; }			
			float							GetPhaseZero						()		const		{ return _phase0; }	
			float							GetPhasePath						()		const		{ return _phase_path; }
			float							GetPhaseSpeedAlongDirectionX		()		const		{ return _phase_speed_along_direction_x; }
			float							GetPhaseSpeedAlongDirectionY		()		const		{ return _phase_speed_along_direction_y; }
			float							GetFrequency						()		const		{ return _frequency; }

			const WaveContext&				GetContext							()		const		{ return _context; }

			// Update context and automatically recalculate the cached values, if necessary			
			inline void SetContext		(const WaveContext& newContext);
			inline void SetPosition		(float X_in, float Y_in);
			inline void SetX			(float X_in);
			inline void SetY			(float Y_in);
			inline void SetTime         (float T_in);

			// Just set the time variable to the new value without an update
			inline void SetTime_NoUpdate         (float T_in);

		private:
			WaveHarmonic			   _harmonic;
			float					   _cosinusoid_result; // cached result of the sinusoid function
			float					   _sinusoid_result; // cached result of the sinusoid function
			float					   _sinusoid_sin; // cached value of the sinus of trig argument			
			float					   _trig_argument; // argument of the sinus or cosinus					
			float					   _frequency; // frequency of the wave
			float					   _phase0; // precalculated phase zero
			float					   _phase_path; // _phase_path_dot_x + _phase_path_dot_y
			float					   _phase_path_dot_x; // = _phase_speed_along_direction_x * x			
			float					   _phase_path_dot_y; // = _phase_speed_along_direction_y * y			
			float					   _phase_speed_along_direction_x; //  = 
			float					   _phase_speed_along_direction_y; //  = _direction.y * _frequency
			WaveContext				   _context;

			inline void					   _RecalculateWave();
			// recalculate only the PhasePathDotX variable (not variables that depend on it)
			inline void					   _RecalculatePhasePathDotX_Variable();
			inline void					   _RecalculatePhasePathDotY_Variable();
			inline void					   _RecalculatePhasePath_Variable();
			inline void					   _RecalculatePhaseZero_Variable();
			inline void					   _RecalculateTrigArgument_Variable();
			inline void					   _RecalculateSinusoidSin_Variable();
			inline void					   _RecalculateSinusoid_Variable();
			inline void					   _RecalculateCosinusoid_Variable();

			// Recalculate all values that depend on the position x or y variable
			// WARNING: the _phase_path_dot_x and _phase_path_dot_y not recalculated
			// during the call to this function
			inline void					   _Recalculate_Values_That_Depend_On_Position();
			// Recalculate all values that depend on time
			inline void					   _Recalculate_Values_That_Depend_On_Time();
			// Recalculate all values that depend on the sinusoid argument
			inline void					   _Recalculate_Values_That_Depend_On_Argument();
		};

		// Calculate the sinus partial derivative by X or Y variable using given cached data
		inline float			CalcSinusoidPartialDerivByX		(const WaveHarmonicCache& cache);
		inline float			CalcSinusoidPartialDerivByY		(const WaveHarmonicCache& cache);					

		// Vector for storing harmonics
		typedef std::vector<WaveHarmonicCache> WaveHarmonicCacheVector;

		// Calculate the wave height and derivatives at current context
		inline float CalculateWaveHeight	      (WaveHarmonicCacheVector *pHarmonics);
		inline float CalculateWavePartialDerivByX (WaveHarmonicCacheVector *pHarmonics);
		inline float CalculateWavePartialDerivByY (WaveHarmonicCacheVector *pHarmonics);

		// Sets a new position for all harmonic caches in the vector
		inline void SetPosition (WaveHarmonicCacheVector *pHarmonics, float X, float Y);
		inline void SetX		(WaveHarmonicCacheVector *pHarmonics, float X);
		inline void SetY		(WaveHarmonicCacheVector *pHarmonics, float Y);
		// Sets a new time for all harmonic caches in the vector 
		inline void SetTime		(WaveHarmonicCacheVector *pHarmonics, float T);

		struct ComputedHeightMapPoint
		{
			float height; // value of the height function
			float height_derive_by_x; // partial derivative of the height function by dx
			float height_derive_by_y; // partial derivative of the height function by dy

			inline ComputedHeightMapPoint();			
			inline ComputedHeightMapPoint(float height_in, float height_derive_by_x_in, float height_derive_by_y_in);
		};

		// Compute the height and derivatives for the given height map point at the current context
		inline ComputedHeightMapPoint ComputeHeightMapPoint(WaveHarmonicCacheVector *pHarmonics);





		// ComputedHeightMapPoint functions impl
		inline ComputedHeightMapPoint ComputeHeightMapPoint(WaveHarmonicCacheVector *pHarmonics)
		{
			float height = CalculateWaveHeight(pHarmonics);
			float height_deriv_by_dx = CalculateWavePartialDerivByX(pHarmonics);
			float height_deriv_by_dy = CalculateWavePartialDerivByY(pHarmonics);
			ComputedHeightMapPoint res_point (height, height_deriv_by_dx, height_deriv_by_dy);
			return res_point;
		}

		// ComputedHeightMapPoint impl
		inline ComputedHeightMapPoint::ComputedHeightMapPoint() :
			height(0.0F), height_derive_by_x(0.0F), height_derive_by_y(0.0F)
		{
		}

		inline ComputedHeightMapPoint::ComputedHeightMapPoint(float height_in, float height_derive_by_x_in, float height_derive_by_y_in) :
			height(height_in), height_derive_by_x(height_derive_by_x_in), height_derive_by_y(height_derive_by_y_in)
		{
		}

		// WaveHarmonicsCacheVector functions impl	
		inline float CalculateWaveHeight(WaveHarmonicCacheVector *pHarmonics)
		{
			float res_height = 0.0F;
			std::for_each
			(
				pHarmonics->cbegin(), pHarmonics->cend(),
				[&res_height](const WaveHarmonicCache& harmonic)
				{
					res_height += harmonic.GetSinusoidResult();
				}
			);	
			return res_height;
		}

		inline float CalculateWavePartialDerivByX(WaveHarmonicCacheVector *pHarmonics)
		{
			float res_HeightPartialDerivByX = 0.0F;
			std::for_each
			(
				pHarmonics->cbegin(), pHarmonics->cend(),
				[&res_HeightPartialDerivByX](const WaveHarmonicCache& harmonic)
				{
					res_HeightPartialDerivByX += CalcSinusoidPartialDerivByX(harmonic);
				}
			);	
			return res_HeightPartialDerivByX;
		}

		inline float CalculateWavePartialDerivByY(WaveHarmonicCacheVector *pHarmonics)
		{
			float res_HeightPartialDerivByY = 0.0F;
			std::for_each
			(
				pHarmonics->cbegin(), pHarmonics->cend(),
				[&res_HeightPartialDerivByY](const WaveHarmonicCache& harmonic)
				{
					res_HeightPartialDerivByY += CalcSinusoidPartialDerivByY(harmonic);
				}
			);	
			return res_HeightPartialDerivByY;
		}

		inline void SetPosition (WaveHarmonicCacheVector *pHarmonics, float X, float Y)
		{
			std::for_each
			(
				pHarmonics->begin(), pHarmonics->end(),
				[X,Y](WaveHarmonicCache& harmonic)
				{
					harmonic.SetPosition(X, Y);
				}
			);	
		}

		inline void SetX		(WaveHarmonicCacheVector *pHarmonics, float X)
		{
			std::for_each
			(
				pHarmonics->begin(), pHarmonics->end(),
				[X](WaveHarmonicCache& harmonic)
				{
					harmonic.SetX(X);
				}
			);	
		}

		inline void SetY		(WaveHarmonicCacheVector *pHarmonics, float Y)
		{
			std::for_each
			(
				pHarmonics->begin(), pHarmonics->end(),
				[Y](WaveHarmonicCache& harmonic)
				{
					harmonic.SetY(Y);
				}
			);	
		}
		
		inline void SetTime		(WaveHarmonicCacheVector *pHarmonics, float T)
		{
			std::for_each
			(
				pHarmonics->begin(), pHarmonics->end(),
				[T](WaveHarmonicCache& harmonic)
				{
					harmonic.SetTime(T);
				}
			);			
		}

		// WaveHarmonicCache functions impl
		inline float CalcSinusoidPartialDerivByX(const WaveHarmonicCache& cache)
		{
			return cache.GetPhaseSpeedAlongDirectionX() * cache.GetCosinusoidResult();
		}

		inline float CalcSinusoidPartialDerivByY(const WaveHarmonicCache& cache)
		{
			return cache.GetPhaseSpeedAlongDirectionY() * cache.GetCosinusoidResult();
		}

		inline float CalcSinusoid(float amplitude, float frequency, float phase, float arg)
		{
			float sin_arg = CalcSinArg(frequency, phase, arg);
			return amplitude * sin(sin_arg);
		}

		inline float CalcCosinusoid(float amplitude, float frequency, float phase, float arg)
		{
			float sin_arg = CalcSinArg(frequency, phase, arg);
			return amplitude * cos(sin_arg);
		}

		inline float CalcSinusoidSin(float frequency, float phase, float arg)
		{
			float sin_arg = CalcSinArg(frequency, phase, arg);
			return sin(sin_arg);
		}

		inline float CalcSinusoidCos(float frequency, float phase, float arg)
		{
			float sin_arg = CalcSinArg(frequency, phase, arg);
			return cos(sin_arg);
		}

		inline float CalcSinArg(float frequency, float phase, float arg)
		{
			return frequency * arg + phase;
		}

		inline float CalcWavePath(const Vector3& normalized_direction, float X, float Y)
		{
			return normalized_direction.X() * X + normalized_direction.YY() * Y;
		}

		// SinWave functions impl
		inline WaveHarmonicCache::WaveHarmonicCache()
		{
		}

		inline WaveHarmonicCache::WaveHarmonicCache(const WaveHarmonic& harmonic_in)			
		{
			SetHarmonic(harmonic_in);
		}

		inline void WaveHarmonicCache::SetHarmonic(const WaveHarmonic& harmonic_in)
		{
			_harmonic = harmonic_in;
			_frequency = CalcFrequency(harmonic_in.harmonic.wave_length);
			_phase_speed_along_direction_x = harmonic_in.normalized_direction.X() * _frequency;
			_phase_speed_along_direction_y = harmonic_in.normalized_direction.YY() * _frequency;
			_RecalculateWave();
		}

		inline void WaveHarmonicCache::_RecalculateWave()
		{
			_RecalculatePhasePathDotX_Variable();
			_RecalculatePhasePathDotY_Variable();
			_RecalculatePhasePath_Variable();
			_RecalculatePhaseZero_Variable();
			_Recalculate_Values_That_Depend_On_Argument();
		}

		inline void WaveHarmonicCache::_RecalculatePhasePathDotX_Variable()
		{
			_phase_path_dot_x = _phase_speed_along_direction_x * _context.X;
		}

		inline void WaveHarmonicCache::_RecalculatePhasePathDotY_Variable()
		{
			_phase_path_dot_y = _phase_speed_along_direction_y * _context.Y;
		}

		inline void WaveHarmonicCache::_RecalculatePhasePath_Variable()
		{
			_phase_path = _phase_path_dot_x + _phase_path_dot_y;
		}

		inline void WaveHarmonicCache::_RecalculatePhaseZero_Variable()
		{
			_phase0 = _context.T * _harmonic.phase_speed;
		}

		inline void WaveHarmonicCache::_RecalculateTrigArgument_Variable()
		{
			_trig_argument = _phase_path + _phase0;
		}

		inline void WaveHarmonicCache::_RecalculateSinusoidSin_Variable()
		{
			_sinusoid_sin = sin(_trig_argument);
		}

		inline void WaveHarmonicCache::_RecalculateSinusoid_Variable()
		{
			_sinusoid_result = _harmonic.harmonic.amplitude * _sinusoid_sin;
		}

		inline void WaveHarmonicCache::_RecalculateCosinusoid_Variable()
		{
			_cosinusoid_result = _harmonic.harmonic.amplitude * cos(_trig_argument);
		}

		inline void WaveHarmonicCache::_Recalculate_Values_That_Depend_On_Position()
		{
			_RecalculatePhasePath_Variable();
			_Recalculate_Values_That_Depend_On_Argument();
		}

		inline void WaveHarmonicCache::_Recalculate_Values_That_Depend_On_Time()
		{
			_RecalculatePhaseZero_Variable();
			_Recalculate_Values_That_Depend_On_Argument();
		}

		inline void WaveHarmonicCache::_Recalculate_Values_That_Depend_On_Argument()
		{
			_RecalculateTrigArgument_Variable();
			_RecalculateSinusoidSin_Variable();	
			_RecalculateSinusoid_Variable();
			_RecalculateCosinusoid_Variable();
		}

		inline void WaveHarmonicCache::SetPosition(float X_in, float Y_in)
		{
			_context.SetPosition(X_in, Y_in);
			_RecalculatePhasePathDotX_Variable();
			_RecalculatePhasePathDotY_Variable();
			_Recalculate_Values_That_Depend_On_Position();
		}

		inline void WaveHarmonicCache::SetContext(const WaveContext& newContext)
		{
			_context = newContext;			
			_RecalculateWave();
		}

		inline void WaveHarmonicCache::SetX(float X_in)
		{
			_context.X = X_in;		
			_RecalculatePhasePathDotX_Variable();
			_Recalculate_Values_That_Depend_On_Position();
		}

		inline void WaveHarmonicCache::SetY(float Y_in)
		{
			_context.Y = Y_in;			
			_RecalculatePhasePathDotY_Variable();
			_Recalculate_Values_That_Depend_On_Position();
		}

		inline void WaveHarmonicCache::SetTime(float T_in)
		{
			_context.T = T_in;			
			_Recalculate_Values_That_Depend_On_Time();
		}

		inline void WaveHarmonicCache::SetTime_NoUpdate (float T_in)
		{
			_context.T = T_in;
		}

		inline float CalcFrequency(float wave_length)
		{
			return 2.0F * 3.1415926535897F/wave_length;
		}
		inline float GetPhaseSpeed	(float wave_length, float speed)
		{
			return CalcFrequency(wave_length) * speed;
		}

		// Harmonic impl
		inline WaveHarmonic::WaveHarmonic() :
			harmonic(), normalized_direction(Vector3::Zero), phase_speed(0.0F)
		{
		}

		inline WaveHarmonic::WaveHarmonic(const SinHarmonic& harmonicIn, const Vector3& normalized_direction_in, float phase_speed_in) :
			harmonic(harmonicIn), normalized_direction(normalized_direction_in), phase_speed(phase_speed_in)
		{
		}
		inline WaveHarmonic::WaveHarmonic(float wave_length_in, float amplitude_in, const Vector3& normalized_direction_in, float phase_speed_in) :
			harmonic(SinHarmonic(wave_length_in, amplitude_in)), normalized_direction(normalized_direction_in), phase_speed(phase_speed_in)
		{
		}

		// SinHarmonic impl
		inline SinHarmonic::SinHarmonic() : wave_length(0.0F), amplitude(0.0F) {}
		inline SinHarmonic::SinHarmonic(float wave_length_in, float amplitude_in) :
			wave_length(wave_length_in), amplitude(amplitude_in)
		{ 
		}

		// WaveContext impl
		inline WaveContext::WaveContext() : T(0.0F), X(0.0F), Y(0.0F) {}
		inline WaveContext::WaveContext(float T_in, float X_in, float Y_in) :
			T(T_in), X(X_in), Y(Y_in)
		{
		}
		inline void WaveContext::SetPosition(float X_in, float Y_in)
		{
			X = X_in;
			Y = Y_in;
		}
	} // Math
} // DvUtils

#endif // _MathUtil_Harmonics_h_