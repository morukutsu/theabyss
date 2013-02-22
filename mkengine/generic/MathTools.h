// mkengine 
// by morukutsu (morukutsu@hotmail.com)


#ifndef MK_MATHTOOLS
#define MK_MATHTOOLS

#include <math.h>
#include <string.h>
#include "Spinor.h"

//Vecteurs
typedef float vec2_t[2];
typedef float vec3_t[3];
typedef float vec5_t[5];


void Vec_normalize(vec3_t v);
bool AlmostEqual2sComplement(float A, float B, int maxUlps);
unsigned int roundUpToNextPowerOfTwo(unsigned int x);

//Quaternion (x, y, z, w)
typedef float quat4_t[4];
enum 
{
	X, Y, Z, W
};

/* Joint */
	struct md5_joint_t
	{
		char name[64];
		int parent;
		vec3_t pos;
		quat4_t orient;
	};

	/* Vertex */
	struct md5_vertex_t
	{
		vec2_t st;

		int start; /* start weight */
		int count; /* weight count */
	};

	/* Triangle */
	struct md5_triangle_t
	{
		int index[3];
	};

	/* Weight */
	struct md5_weight_t
	{
		int joint;
		float bias;

		vec3_t pos;
		vec3_t norm;
	};

	/* Bounding box */
	struct md5_bbox_t
	{
		vec3_t min;
		vec3_t max;
	};

	/* MD5 mesh */
	struct md5_mesh_t
	{
		struct md5_vertex_t *vertices;
		struct md5_triangle_t *triangles;
		struct md5_weight_t *weights;

		int num_verts;
		int num_tris;
		int num_weights;

		char shader[256];
	};

	/* MD5 model structure */
	struct md5_model_t
	{
		struct md5_joint_t *baseSkel;
		struct md5_mesh_t *meshes;

		int num_joints;
		int num_meshes;
	};

	/* Animation data */
	struct md5_anim_t
	{
		int num_frames;
		int num_joints;
		int frameRate;

		struct md5_joint_t **skelFrames;
		struct md5_bbox_t *bboxes;
	};

	/* Animation info */
	struct anim_info_t
	{
		int curr_frame;
		int next_frame;

		double last_time;
		double max_time;
	};

	/* Joint info */
	struct joint_info_t
	{
		char name[64];
		int parent;
		int flags;
		int startIndex;
	};

	/* Base frame joint */
	struct baseframe_joint_t
	{
		vec3_t pos;
		quat4_t orient;
	};

void Quat_computeW (quat4_t q);
void Quat_rotatePoint (const quat4_t q, const vec3_t in, vec3_t out);
void Quat_multQuat (const quat4_t qa, const quat4_t qb, quat4_t out);
void Quat_normalize (quat4_t q);
void Quat_multVec (const quat4_t q, const vec3_t v, quat4_t out);
void Quat_slerp (const quat4_t qa, const quat4_t qb, float t, quat4_t out);
float Quat_dotProduct (const quat4_t qa, const quat4_t qb);
void Quat_inverse (quat4_t q, quat4_t out);

//Autres maths
float Determinant4f(const float m[16]);
bool GenerateInverseMatrix4f(float i[16], const float m[16]);

inline void swapf(float& a, float& b)
{
	float c = a;
	a = b;
	b = c;
}

inline float clampf(float x, float min, float max)
{
	return (x < min)? min : (x > max)? max : x;
}

inline float wrapf(float x, float min, float max)
{
	return (x < min)? (x - min) + max : (x > max)? (x - max) + min : x;
}

inline float Pi()
{
    const float pi = atan(1.0f) * 4.0f;

	return pi;
}

inline float TwoPi()
{
	const float two_pi = 2.0f * Pi();
	return two_pi;
}

inline float RadiansToDegrees(float rad)
{
	const float k = 180.0f / Pi();
	return rad * k;
}

inline float DegreesToRadians(float deg)
{
	const float k = Pi() / 180.0f;
	return deg * k;
}

inline float VarMax(float left, float right)
{
	return (left < right ? right : left);
}

static inline float Lerp(float time, float time_start, float time_end, float value_start, float value_end)
{
	// bounding
	if(time > time_end)
		time = time_end;

	if(time < time_start)
		time = time_start;

	// lerp
	return (value_start + (((time - time_start) * (value_end - value_start)) / (time_end - time_start)));
}

static inline float Slerp2D(float t, float time_start, float time_end, float fromAngle, float toAngle)
{
    mk::Spinor from  =  mk::Spinor(cosf(fromAngle / 2), sinf(fromAngle / 2));
    mk::Spinor toSpinor =  mk::Spinor(cosf(toAngle / 2), sinf(toAngle / 2));

	// bounding
	if(t > time_end)
		t = time_end;

	if(t < time_start)
		t = time_start;

	// slerp
    return  mk::Spinor::Slerp(from, toSpinor, (t - time_start) / (time_end - time_start) ).GetAngle();
}


#endif