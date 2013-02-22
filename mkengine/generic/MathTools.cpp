// mkengine 
// by morukutsu (morukutsu@hotmail.com)

#include "MathTools.h"
#include <assert.h>

void Vec_normalize(vec3_t v)
{
	float mag = sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
	if(mag > 0.0f)
	{
		v[0] = v[0]/mag;
		v[1] = v[1]/mag;
		v[2] = v[2]/mag;
	}
}

void Quat_computeW (quat4_t q)
{
	float t = 1.0f - (q[X] * q[X]) - (q[Y] * q[Y]) - (q[Z] * q[Z]);

	if (t < 0.0f)
		q[W] = 0.0f;
	else
		q[W] = -sqrt (t);
}

void Quat_rotatePoint (const quat4_t q, const vec3_t in, vec3_t out)
{
	quat4_t tmp, inv, final;

	inv[X] = -q[X]; inv[Y] = -q[Y];
	inv[Z] = -q[Z]; inv[W] =  q[W];

	Quat_normalize (inv);

	Quat_multVec (q, in, tmp);
	Quat_multQuat (tmp, inv, final);

	out[X] = final[X];
	out[Y] = final[Y];
	out[Z] = final[Z];
}

void Quat_multQuat (const quat4_t qa, const quat4_t qb, quat4_t out)
{
	out[W] = (qa[W] * qb[W]) - (qa[X] * qb[X]) - (qa[Y] * qb[Y]) - (qa[Z] * qb[Z]);
	out[X] = (qa[X] * qb[W]) + (qa[W] * qb[X]) + (qa[Y] * qb[Z]) - (qa[Z] * qb[Y]);
	out[Y] = (qa[Y] * qb[W]) + (qa[W] * qb[Y]) + (qa[Z] * qb[X]) - (qa[X] * qb[Z]);
	out[Z] = (qa[Z] * qb[W]) + (qa[W] * qb[Z]) + (qa[X] * qb[Y]) - (qa[Y] * qb[X]);
}

void Quat_normalize (quat4_t q)
{
	/* compute magnitude of the quaternion */
	float mag = sqrt ((q[X] * q[X]) + (q[Y] * q[Y]) + (q[Z] * q[Z]) + (q[W] * q[W]));

	/* check for bogus length, to protect against divide by zero */
	if (mag > 0.0f)
	{
		/* normalize it */
		float oneOverMag = 1.0f / mag;

		q[X] *= oneOverMag;
		q[Y] *= oneOverMag;
		q[Z] *= oneOverMag;
		q[W] *= oneOverMag;
	}
}

void Quat_inverse (quat4_t q, quat4_t out)
{
	quat4_t conjugate;
	conjugate[W] =  q[W];
	conjugate[X] = -q[X];
	conjugate[Y] = -q[Y];
	conjugate[Z] = -q[Z];

	Quat_normalize(conjugate);

	out[W] = conjugate[W];
	out[X] = conjugate[X];
	out[Y] = conjugate[Y];
	out[Z] = conjugate[Z];
}

void Quat_multVec (const quat4_t q, const vec3_t v, quat4_t out)
{
	out[W] = - (q[X] * v[X]) - (q[Y] * v[Y]) - (q[Z] * v[Z]);
	out[X] =   (q[W] * v[X]) + (q[Y] * v[Z]) - (q[Z] * v[Y]);
	out[Y] =   (q[W] * v[Y]) + (q[Z] * v[X]) - (q[X] * v[Z]);
	out[Z] =   (q[W] * v[Z]) + (q[X] * v[Y]) - (q[Y] * v[X]);
}

void Quat_slerp(const quat4_t qa, const quat4_t qb, float t, quat4_t out)
{
	/* Check for out-of range parameter and return edge points if so */
	if(t <= 0.0)
	{
		memcpy(out, qa, sizeof(quat4_t));
		return;
	}

	if(t >= 1.0)
	{
		memcpy (out, qb, sizeof (quat4_t));
		return;
	}

	/* Compute "cosine of angle between quaternions" using dot product */
	float cosOmega = Quat_dotProduct (qa, qb);

	/* If negative dot, use -q1.  Two quaternions q and -q
     represent the same rotation, but may produce
     different slerp.  We chose q or -q to rotate using
     the acute angle. */
	float q1w = qb[W];
	float q1x = qb[X];
	float q1y = qb[Y];
	float q1z = qb[Z];

	if(cosOmega < 0.0f)
    {
		q1w = -q1w;
		q1x = -q1x;
		q1y = -q1y;
		q1z = -q1z;
		cosOmega = -cosOmega;
    }

	/* We should have two unit quaternions, so dot should be <= 1.0 */
	//assert (cosOmega < 1.1f);

	/* Compute interpolation fraction, checking for quaternions
		almost exactly the same */
	float k0, k1;

	if(cosOmega > 0.9999f)
    {
		/* Very close - just use linear interpolation,
		which will protect againt a divide by zero */

		k0 = 1.0f - t;
		k1 = t;
    }
	else
    {
		/* Compute the sin of the angle using the
		 trig identity sin^2(omega) + cos^2(omega) = 1 */
		float sinOmega = sqrt(1.0f - (cosOmega * cosOmega));

		/* Compute the angle from its sin and cosine */
		float omega = atan2(sinOmega, cosOmega);

		/* Compute inverse of denominator, so we only have
		to divide once */
		float oneOverSinOmega = 1.0f / sinOmega;

		/* Compute interpolation parameters */
		k0 = sin ((1.0f - t) * omega) * oneOverSinOmega;
		k1 = sin (t * omega) * oneOverSinOmega;
    }

	/* Interpolate and return new quaternion */
	out[W] = (k0 * qa[3]) + (k1 * q1w);
	out[X] = (k0 * qa[0]) + (k1 * q1x);
	out[Y] = (k0 * qa[1]) + (k1 * q1y);
	out[Z] = (k0 * qa[2]) + (k1 * q1z);
}

float Quat_dotProduct(const quat4_t qa, const quat4_t qb)
{
	return ((qa[X] * qb[X]) + (qa[Y] * qb[Y]) + (qa[Z] * qb[Z]) + (qa[W] * qb[W]));
}

//Autres maths
float Determinant4f(const float m[16])
{
	return
	m[12]*m[9]*m[6]*m[3]-
	m[8]*m[13]*m[6]*m[3]-
	m[12]*m[5]*m[10]*m[3]+
	m[4]*m[13]*m[10]*m[3]+
	m[8]*m[5]*m[14]*m[3]-
	m[4]*m[9]*m[14]*m[3]-
	m[12]*m[9]*m[2]*m[7]+
	m[8]*m[13]*m[2]*m[7]+
	m[12]*m[1]*m[10]*m[7]-
	m[0]*m[13]*m[10]*m[7]-
	m[8]*m[1]*m[14]*m[7]+
	m[0]*m[9]*m[14]*m[7]+
	m[12]*m[5]*m[2]*m[11]-
	m[4]*m[13]*m[2]*m[11]-
	m[12]*m[1]*m[6]*m[11]+
	m[0]*m[13]*m[6]*m[11]+
	m[4]*m[1]*m[14]*m[11]-
	m[0]*m[5]*m[14]*m[11]-
	m[8]*m[5]*m[2]*m[15]+
	m[4]*m[9]*m[2]*m[15]+
	m[8]*m[1]*m[6]*m[15]-
	m[0]*m[9]*m[6]*m[15]-
	m[4]*m[1]*m[10]*m[15]+
	m[0]*m[5]*m[10]*m[15];
}

bool GenerateInverseMatrix4f(float i[16], const float m[16])
{
	float x=Determinant4f(m);
	if (x==0) return false;

	i[0]= (-m[13]*m[10]*m[7] +m[9]*m[14]*m[7] +m[13]*m[6]*m[11]
	-m[5]*m[14]*m[11] -m[9]*m[6]*m[15] +m[5]*m[10]*m[15])/x;
	i[4]= ( m[12]*m[10]*m[7] -m[8]*m[14]*m[7] -m[12]*m[6]*m[11]
	+m[4]*m[14]*m[11] +m[8]*m[6]*m[15] -m[4]*m[10]*m[15])/x;
	i[8]= (-m[12]*m[9]* m[7] +m[8]*m[13]*m[7] +m[12]*m[5]*m[11]
	-m[4]*m[13]*m[11] -m[8]*m[5]*m[15] +m[4]*m[9]* m[15])/x;
	i[12]=( m[12]*m[9]* m[6] -m[8]*m[13]*m[6] -m[12]*m[5]*m[10]
	+m[4]*m[13]*m[10] +m[8]*m[5]*m[14] -m[4]*m[9]* m[14])/x;
	i[1]= ( m[13]*m[10]*m[3] -m[9]*m[14]*m[3] -m[13]*m[2]*m[11]
	+m[1]*m[14]*m[11] +m[9]*m[2]*m[15] -m[1]*m[10]*m[15])/x;
	i[5]= (-m[12]*m[10]*m[3] +m[8]*m[14]*m[3] +m[12]*m[2]*m[11]
	-m[0]*m[14]*m[11] -m[8]*m[2]*m[15] +m[0]*m[10]*m[15])/x;
	i[9]= ( m[12]*m[9]* m[3] -m[8]*m[13]*m[3] -m[12]*m[1]*m[11]
	+m[0]*m[13]*m[11] +m[8]*m[1]*m[15] -m[0]*m[9]* m[15])/x;
	i[13]=(-m[12]*m[9]* m[2] +m[8]*m[13]*m[2] +m[12]*m[1]*m[10]
	-m[0]*m[13]*m[10] -m[8]*m[1]*m[14] +m[0]*m[9]* m[14])/x;
	i[2]= (-m[13]*m[6]* m[3] +m[5]*m[14]*m[3] +m[13]*m[2]*m[7]
	-m[1]*m[14]*m[7] -m[5]*m[2]*m[15] +m[1]*m[6]* m[15])/x;
	i[6]= ( m[12]*m[6]* m[3] -m[4]*m[14]*m[3] -m[12]*m[2]*m[7]
	+m[0]*m[14]*m[7] +m[4]*m[2]*m[15] -m[0]*m[6]* m[15])/x;
	i[10]=(-m[12]*m[5]* m[3] +m[4]*m[13]*m[3] +m[12]*m[1]*m[7]
	-m[0]*m[13]*m[7] -m[4]*m[1]*m[15] +m[0]*m[5]* m[15])/x;
	i[14]=( m[12]*m[5]* m[2] -m[4]*m[13]*m[2] -m[12]*m[1]*m[6]
	+m[0]*m[13]*m[6] +m[4]*m[1]*m[14] -m[0]*m[5]* m[14])/x;
	i[3]= ( m[9]* m[6]* m[3] -m[5]*m[10]*m[3] -m[9]* m[2]*m[7]
	+m[1]*m[10]*m[7] +m[5]*m[2]*m[11] -m[1]*m[6]* m[11])/x;
	i[7]= (-m[8]* m[6]* m[3] +m[4]*m[10]*m[3] +m[8]* m[2]*m[7]
	-m[0]*m[10]*m[7] -m[4]*m[2]*m[11] +m[0]*m[6]* m[11])/x;
	i[11]=( m[8]* m[5]* m[3] -m[4]*m[9]* m[3] -m[8]* m[1]*m[7]
	+m[0]*m[9]* m[7] +m[4]*m[1]*m[11] -m[0]*m[5]* m[11])/x;
	i[15]=(-m[8]* m[5]* m[2] +m[4]*m[9]* m[2] +m[8]* m[1]*m[6]
	-m[0]*m[9]* m[6] -m[4]*m[1]*m[10] +m[0]*m[5]* m[10])/x;

	return true;
}

// Usable AlmostEqual function
bool AlmostEqual2sComplement(float A, float B, int maxUlps)
{
    // Make sure maxUlps is non-negative and small enough that the
    // default NAN won't compare as equal to anything.
    assert(maxUlps > 0 && maxUlps < 4 * 1024 * 1024);
    int aInt = *(int*)&A;
    // Make aInt lexicographically ordered as a twos-complement int
    if (aInt < 0)
        aInt = 0x80000000 - aInt;
    // Make bInt lexicographically ordered as a twos-complement int
    int bInt = *(int*)&B;
    if (bInt < 0)
        bInt = 0x80000000 - bInt;
    int intDiff = abs(aInt - bInt);
    if (intDiff <= maxUlps)
        return true;
    return false;
}

unsigned int roundUpToNextPowerOfTwo(unsigned int x)
{
	x--;
	x |= x >> 1;  // handle  2 bit numbers
	x |= x >> 2;  // handle  4 bit numbers
	x |= x >> 4;  // handle  8 bit numbers
	x |= x >> 8;  // handle 16 bit numbers
	x |= x >> 16; // handle 32 bit numbers
	x++;
	return x;
}
