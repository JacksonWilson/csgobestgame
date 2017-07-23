#pragma once
#include <cmath>
#include "Vector.hpp"
#include "QAngle.hpp"
#include "VMatrix.hpp"
#include "IGlobalVars.hpp"

#define square(x) (x *x)
#define M_PI 3.14159265358979323846f
#define M_RADPI 57.295779513082f
#define M_PI_F ((float)(M_PI))	// Shouldn't collide with anything.
#define RAD2DEG(x) ((float)(x) *(float)(180.f / M_PI_F))
#define DEG2RAD(x) ((float)(x) *(float)(M_PI_F / 180.f))

namespace se
{
	enum
	{
		PITCH = 0,	// up / down
		YAW,		// left / right
		ROLL		// fall over
	};

	class Math
	{
	public:
		static void SinCos(float radians, float *sine, float *cosine);
		static void VectorAngles(const Vector& forward, QAngle &angles);
		static void AngleVectors(const QAngle& angles, Vector *forward);
		static void AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up);
		static QAngle CalcAngle(Vector v1, Vector v2);
		static float GetFov(const QAngle& viewAngle, const QAngle& aimAngle);
		static float VectorDistance(Vector v1, Vector v2);
		static void VectorTransform(Vector& in1, matrix3x4a_t& in2, Vector &out);
		static float GetDelta(float hspeed, float maxspeed, float airaccelerate);
		static Vector ExtrapolateTick(Vector p0, Vector v0, IGlobalVarsBase *globals);

		static float RandFloat(float M, float N);

		// sperg cried about the previous method, 
		//here's not only a faster one but inaccurate as well to trigger more people
		static float FASTSQRT(float x)
		{
			unsigned int i = *(unsigned int*)&x;

			i += 127 << 23;
			// approximation of square root
			i >>= 1;
			return *(float*)&i;
		}
	};
}