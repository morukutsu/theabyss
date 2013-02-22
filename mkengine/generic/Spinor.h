// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#ifndef H_SPINOR
#define H_SPINOR

#include <math.h>

namespace mk 
{
	class Spinor
	{
		public:
			Spinor(float angle)
			{
				_real = cos(angle);
				_complex = sin(angle);
			}

			Spinor(float realpart, float complexpart)
			{
				_complex = complexpart;
				_real = realpart;
			}

			Spinor GetScale(float t)
			{
				Spinor s(_real * t, _complex * t);
				return s;
			}

			Spinor GetInvert()
			{
				Spinor s(_real, -_complex);
				return s.GetScale(s.GetLengthSquared() );
			}

			Spinor GetAdd(Spinor other)
			{
				return Spinor(_real + other._real, _complex + other._complex);
			}

			float GetLength()
			{
				return sqrt(_real * _real + _complex * _complex);
			}

			float GetLengthSquared()
			{
				return (_real * _real + _complex * _complex);
			}

			Spinor GetMultiply(Spinor other)
			{
				return Spinor(_real * other._real - _complex * other._complex, _real * other._complex + _complex * other._real);
			}

			Spinor GetNormalized()
			{
				float length = GetLength();
				return Spinor(_real / length, _complex / length);
			}

			float GetAngle()
			{
				return atan2(_complex, _real) * 2;
			}

			Spinor Lerp(Spinor startVal, Spinor endVal, float t)
			{
				return startVal.GetScale(1 - t).GetAdd(endVal.GetScale(t)).GetNormalized();
			}

			static Spinor Slerp(Spinor from, Spinor dest, float t)
			{
				float tr;
				float tc;
				float omega, cosom, sinom, scale0, scale1;

				//calc cosine
				cosom = from._real * dest._real + from._complex * dest._complex;

				//adjust signs
				if(cosom <0)
				{
					cosom = -cosom;
					tc = -dest._complex;
					tr = -dest._real;
				}
				else
				{
					tc = dest._complex;
					tr = dest._real;
				}

				//coefficients
				if((1 - cosom) > 0.001)  //threshold, use linear interp if too close
				{
					omega = acos(cosom);
					sinom = sin(omega);
					scale0 = sin((1 - t) * omega) / sinom;
					scale1 = sin(t * omega) / sinom;
				}
				else
				{
					scale0 = 1 - t;
					scale1 = t;
				}

				//calc final
				Spinor res = Spinor(0, 0);
				res._complex = scale0 * from._complex + scale1 * tc;
				res._real = scale0 * from._real + scale1 * tr;
				return res;
			}

		private:
			float _real, _complex;
	};
}

#endif
