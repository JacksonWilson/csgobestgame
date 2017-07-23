#include "Math.hpp"

namespace se
{
	void Math::SinCos(float radians, float *sine, float *cosine)
	{
		*sine = sin(radians);
		*cosine = cos(radians);
	}

	void Math::VectorAngles(const Vector &forward, QAngle &angles)
	{
		float yaw, pitch;
		if (forward.y == 0.f && forward.x == 0.f)
		{
			pitch = (forward.z > 0.0f) ? 270.0f : 90.0f; // Pitch (up/down)
			yaw = 0.f;  //yaw left/right
		}
		else
		{
			pitch = atan2(-forward.z, forward.Length2D()) * -180 / M_PI;
			yaw = atan2(forward.y, forward.x) * 180 / M_PI;

			if (yaw > 90.f)
				yaw -= 180.f;
			else if (yaw < 90.f)
				yaw += 180.f;
			else if (yaw == 90.f)
				yaw = 0.f;
		}

		angles.x = pitch;
		angles.y = yaw;
		angles.z = 0.f;
	}

	void Math::AngleVectors(const QAngle &angles, Vector *forward)
	{
		float sp, sy, cp, cy;

		SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
		SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);

		forward->x = cp  *cy;
		forward->y = cp  *sy;
		forward->z = -sp;
	}

	void Math::AngleVectors(const QAngle &angles, Vector *forward, Vector *right, Vector *up)
	{
		float sr, sp, sy, cr, cp, cy;

		SinCos(DEG2RAD(angles[YAW]), &sy, &cy);
		SinCos(DEG2RAD(angles[PITCH]), &sp, &cp);
		SinCos(DEG2RAD(angles[ROLL]), &sr, &cr);

		if (forward)
		{
			forward->x = cp  *cy;
			forward->y = cp  *sy;
			forward->z = -sp;
		}

		if (right)
		{
			right->x = (-1 * sr  *sp  *cy + -1 * cr  *-sy);
			right->y = (-1 * sr  *sp  *sy + -1 * cr  *cy);
			right->z = -1 * sr  *cp;
		}

		if (up)
		{
			up->x = (cr  *sp  *cy + -sr  *-sy);
			up->y = (cr  *sp*sy + -sr  *cy);
			up->z = cr  *cp;
		}
	}

	float Math::GetFov(const QAngle& viewAngle, const QAngle& aimAngle)
	{
		Vector ang, aim;

		AngleVectors(viewAngle, &aim);
		AngleVectors(aimAngle, &ang);

		return RAD2DEG(acos(aim.Dot(ang) / aim.LengthSqr()));
	}
	QAngle Math::CalcAngle(Vector src, Vector dst)
	{
		QAngle angles;
		Vector delta = src - dst;
		VectorAngles(delta, angles);
		delta = delta.Normalized();
		return angles;
	}

	float Math::VectorDistance(Vector v1, Vector v2)
	{
		return FASTSQRT(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2) + pow(v1.z - v2.z, 2));
	}

	void Math::VectorTransform(Vector& in1, matrix3x4a_t& in2, Vector &out)
	{
		out.x = in1.Dot(in2.m_flMatVal[0]) + in2.m_flMatVal[0][3];
		out.y = in1.Dot(in2.m_flMatVal[1]) + in2.m_flMatVal[1][3];
		out.z = in1.Dot(in2.m_flMatVal[2]) + in2.m_flMatVal[2][3];
	}

	float Math::GetDelta(float hspeed, float maxspeed, float airaccelerate)
	{
		float term = (30.f - (airaccelerate  *maxspeed / 66.f)) / hspeed;

		return (term < 1.0f && term > -1.0f) ? acos(term) : 0.f;
	}

	float Math::RandFloat(float M, float N)
	{
		return (float)(M + (rand() / (RAND_MAX / (N - M))));
	}

	Vector Math::ExtrapolateTick(Vector p0, Vector v0, IGlobalVarsBase *globals)
	{
		return p0 + (v0 * globals->interval_per_tick);
	}
}