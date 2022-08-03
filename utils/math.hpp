#pragma once

#include "..\includes.hpp"
#include "..\sdk\math\VMatrix.hpp"
#include "m128.h"

#include <DirectXMath.h>

#define RAD2DEG(x) DirectX::XMConvertToDegrees(x)
#define DEG2RAD(x) DirectX::XMConvertToRadians(x)

namespace math {
	void normalize_angles(Vector & angles);
	float angle_distance(float firstangle, float secondangle);
	void normalize_angle(QAngle & angles);
	void normalize_vector(Vector & vector);
	float normalize_yaw(float f);
	float normalize_pitch(float pitch);
	float vector_normalize(Vector& v);
	void clamp_angles(Vector & angles);
	void vector_transform(const Vector& in1, const matrix3x4_t & in2, Vector & out);
	Vector calculate_angle(const Vector& src, const Vector& dst);
	float get_fov(const Vector& viewangles, const Vector& aim_angle);
	void angle_vectors(const Vector& angles, Vector& forward);
	void angle_vectors(const Vector& angles, Vector* forward, Vector* right, Vector* up);
	void vector_angles(const Vector& forward, Vector& angles);
	void vector_angles(const Vector& forward, Vector& up, Vector& angles);
	void VectorMAInline(const float* start, float scale, const float* direction, float* dest);
	void VectorMAInline(const Vector& start, float scale, const Vector& direction, Vector& dest);
	void VectorMA(const Vector& start, float scale, const Vector& direction, Vector& dest);
	void VectorMA(const float * start, float scale, const float *direction, float *dest);
	Vector cross_product(const Vector & a, const Vector & b);
	float dot_product(const Vector & a, const Vector & b);
	bool world_to_screen(const Vector &origin, Vector &screen);
	void SmoothAngle(Vector& From, Vector& To, float Percent);
	void rotate_triangle(std::array<Vector2D, 3>& points, float rotation);
	float random_float(float min, float max);
	int random_int(int min, int max);
	void random_seed(int seed);
	float segment_to_segment(const Vector& s1, const Vector& s2, const Vector& k1, const Vector& k2);
	bool intersect_line_with_bb(Vector& start, Vector& end, Vector& min, Vector& max);
	Vector vector_rotate(const Vector& in1, const matrix3x4_t& in2);
	Vector vector_rotate(const Vector& in1, const Vector& in2);
	matrix3x4_t angle_matrix(const Vector& angles);
	void angle_matrix(const Vector& ang, const Vector& pos, matrix3x4_t& out);
	void vector_rotate(const Vector& in1, const matrix3x4_t& in2, Vector& out);
	Vector matrix_get_origin(const matrix3x4_t& src);
	void matrix_set_origin(const Vector& pos, matrix3x4_t& matrix);
	void fast_rsqrt(float a, float* out);
	float fast_vec_normalize(Vector& vec);
	void matrix_copy(const matrix3x4_t& in, matrix3x4_t& out);
	void concat_transforms(const matrix3x4_t& in1, const matrix3x4_t& in2, matrix3x4_t& out);
	double fast_sin(double x);
	float simple_spline(float value);
	float simple_spline_remap_val_clamped(float value, float a, float b, float c, float d);
	float direction_difference(const Vector& first, const Vector& second);


	__forceinline static Vector matrix_angles(matrix3x4_t& matrix)
	{
		float forward[3];
		float left[3];
		float up[3];

		forward[0] = matrix[0][0];
		forward[1] = matrix[1][0];
		forward[2] = matrix[2][0];

		left[0] = matrix[0][1];
		left[1] = matrix[1][1];
		left[2] = matrix[2][1];

		up[2] = matrix[2][2];

		m128 a1;
		a1.f[0] = forward[0] * forward[0] + forward[1] * forward[1];
		auto calc = sqrt_ps(a1.v);
		const auto xy_dist = reinterpret_cast<const m128*>(&calc)->f[0];

		if (xy_dist > 0.001f)
			return Vector(RAD2DEG(atan2f(-forward[2], xy_dist)),
				RAD2DEG(atan2f(forward[1], forward[0])),
				RAD2DEG(atan2f(left[2], up[2])));

		return Vector(RAD2DEG(atan2f(-forward[2], xy_dist)),
			RAD2DEG(atan2f(-left[0], left[1])),
			0);
	}


	__forceinline static matrix3x4_t multiply_matrix(matrix3x4_t in1, matrix3x4_t in2)
	{
		matrix3x4_t result{};
		result[0][0] = in1[0][0] * in2[0][0] + in1[0][1] * in2[1][0] +
			in1[0][2] * in2[2][0];
		result[0][1] = in1[0][0] * in2[0][1] + in1[0][1] * in2[1][1] +
			in1[0][2] * in2[2][1];
		result[0][2] = in1[0][0] * in2[0][2] + in1[0][1] * in2[1][2] +
			in1[0][2] * in2[2][2];
		result[0][3] = in1[0][0] * in2[0][3] + in1[0][1] * in2[1][3] +
			in1[0][2] * in2[2][3] + in1[0][3];
		result[1][0] = in1[1][0] * in2[0][0] + in1[1][1] * in2[1][0] +
			in1[1][2] * in2[2][0];
		result[1][1] = in1[1][0] * in2[0][1] + in1[1][1] * in2[1][1] +
			in1[1][2] * in2[2][1];
		result[1][2] = in1[1][0] * in2[0][2] + in1[1][1] * in2[1][2] +
			in1[1][2] * in2[2][2];
		result[1][3] = in1[1][0] * in2[0][3] + in1[1][1] * in2[1][3] +
			in1[1][2] * in2[2][3] + in1[1][3];
		result[2][0] = in1[2][0] * in2[0][0] + in1[2][1] * in2[1][0] +
			in1[2][2] * in2[2][0];
		result[2][1] = in1[2][0] * in2[0][1] + in1[2][1] * in2[1][1] +
			in1[2][2] * in2[2][1];
		result[2][2] = in1[2][0] * in2[0][2] + in1[2][1] * in2[1][2] +
			in1[2][2] * in2[2][2];
		result[2][3] = in1[2][0] * in2[0][3] + in1[2][1] * in2[1][3] +
			in1[2][2] * in2[2][3] + in1[2][3];
		return result;
	}
	__forceinline static Vector matrix_position(matrix3x4_t& matrix)
	{
		return Vector(matrix[0][3], matrix[1][3], matrix[2][3]);
	}
	template<class T, class U>
	static T clamp(const T& in, const U& low, const U& high)
	{
		if (in <= low)
			return low;

		if (in >= high)
			return high;

		return in;
	}

	template <typename t>
	static t interpolate(const t& t1, const t& t2, float progress)
	{
		if (t1 == t2)
			return t1;

		return t2 * progress + t1 * (1.0f - progress);
	}

	template <typename t>
	static t lerp(const t& t1, const t& t2, float progress)
	{
		return t1 + (t2 - t1) * progress;
	}

	template <typename t>
	static t lerp2(float progress, const t& t1, const t& t2)
	{
		return t1 + (t2 - t1) * progress;
	}
}