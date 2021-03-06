////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Copyright (c) 2019 Nicholas Frechette & Realtime Math contributors
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include <benchmark/benchmark.h>

#include <rtm/packing/quatf.h>

using namespace rtm;

inline quatf RTM_SIMD_CALL quat_from_positive_w_scalar(vector4f_arg0 input) RTM_NO_EXCEPT
{
	// Operation order is important here, due to rounding, ((1.0 - (X*X)) - Y*Y) - Z*Z is more accurate than 1.0 - dot3(xyz, xyz)
	float w_squared = ((1.0F - vector_get_x(input) * vector_get_x(input)) - vector_get_y(input) * vector_get_y(input)) - vector_get_z(input) * vector_get_z(input);
	// w_squared can be negative either due to rounding or due to quantization imprecision, we take the absolute value
	// to ensure the resulting quaternion is always normalized with a positive W component
	float w = scalar_sqrt(scalar_abs(w_squared));
	return quat_set_w(vector_to_quat(input), w);
}

#if defined(RTM_SSE4_INTRINSICS)
inline quatf RTM_SIMD_CALL quat_from_positive_w_sse4_andnot(vector4f_arg0 input) RTM_NO_EXCEPT
{
	__m128 x2y2z2 = _mm_mul_ps(input, input);
	__m128 one = _mm_set_ss(1.0F);
	__m128 w_squared = _mm_sub_ss(_mm_sub_ss(_mm_sub_ss(one, x2y2z2), _mm_shuffle_ps(x2y2z2, x2y2z2, _MM_SHUFFLE(1, 1, 1, 1))), _mm_shuffle_ps(x2y2z2, x2y2z2, _MM_SHUFFLE(2, 2, 2, 2)));
	w_squared = _mm_andnot_ps(_mm_set_ss(-0.0F), w_squared);
	__m128 w = _mm_sqrt_ss(w_squared);
	return _mm_insert_ps(input, w, 0x30);
}

inline quatf RTM_SIMD_CALL quat_from_positive_w_sse4_and(vector4f_arg0 input) RTM_NO_EXCEPT
{
#if defined(_MSC_VER)
	constexpr __m128i masks = { 0xFFU, 0xFFU, 0xFFU, 0x7FU, 0xFFU, 0xFFU, 0xFFU, 0x7FU, 0xFFU, 0xFFU, 0xFFU, 0x7FU, 0xFFU, 0xFFU, 0xFFU, 0x7FU };
#else
	constexpr __m128i masks = { 0x7FFFFFFF7FFFFFFFULL, 0x7FFFFFFF7FFFFFFFULL };
#endif

	__m128 x2y2z2 = _mm_mul_ps(input, input);
	__m128 one = _mm_set_ss(1.0F);
	__m128 w_squared = _mm_sub_ss(_mm_sub_ss(_mm_sub_ss(one, x2y2z2), _mm_shuffle_ps(x2y2z2, x2y2z2, _MM_SHUFFLE(1, 1, 1, 1))), _mm_shuffle_ps(x2y2z2, x2y2z2, _MM_SHUFFLE(2, 2, 2, 2)));
	w_squared = _mm_and_ps(w_squared, _mm_castsi128_ps(masks));
	__m128 w = _mm_sqrt_ss(w_squared);
	return _mm_insert_ps(input, w, 0x30);
}
#endif

#if defined(RTM_SSE2_INTRINSICS)
inline quatf RTM_SIMD_CALL quat_from_positive_w_sse2_and(vector4f_arg0 input) RTM_NO_EXCEPT
{
#if defined(_MSC_VER)
	constexpr __m128i masks = { 0xFFU, 0xFFU, 0xFFU, 0x7FU, 0xFFU, 0xFFU, 0xFFU, 0x7FU, 0xFFU, 0xFFU, 0xFFU, 0x7FU, 0xFFU, 0xFFU, 0xFFU, 0x7FU };
#else
	constexpr __m128i masks = { 0x7FFFFFFF7FFFFFFFULL, 0x7FFFFFFF7FFFFFFFULL };
#endif

	__m128 x2y2z2 = _mm_mul_ps(input, input);
	__m128 one = _mm_set_ss(1.0F);
	__m128 w_squared = _mm_sub_ss(_mm_sub_ss(_mm_sub_ss(one, x2y2z2), _mm_shuffle_ps(x2y2z2, x2y2z2, _MM_SHUFFLE(1, 1, 1, 1))), _mm_shuffle_ps(x2y2z2, x2y2z2, _MM_SHUFFLE(2, 2, 2, 2)));
	w_squared = _mm_and_ps(w_squared, _mm_castsi128_ps(masks));
	__m128 w = _mm_sqrt_ss(w_squared);
	__m128 input_wyzx = _mm_shuffle_ps(input, input, _MM_SHUFFLE(0, 2, 1, 3));
	__m128 result_wyzx = _mm_move_ss(input_wyzx, w);
	return _mm_shuffle_ps(result_wyzx, result_wyzx, _MM_SHUFFLE(0, 2, 1, 3));
}

inline quatf RTM_SIMD_CALL quat_from_positive_w_sse2_and2(vector4f_arg0 input) RTM_NO_EXCEPT
{
#if defined(_MSC_VER)
	constexpr __m128i masks = { 0xFFU, 0xFFU, 0xFFU, 0x7FU, 0xFFU, 0xFFU, 0xFFU, 0x7FU, 0xFFU, 0xFFU, 0xFFU, 0x7FU, 0xFFU, 0xFFU, 0xFFU, 0x7FU };
#else
	constexpr __m128i masks = { 0x7FFFFFFF7FFFFFFFULL, 0x7FFFFFFF7FFFFFFFULL };
#endif

	__m128 x2y2z2 = _mm_mul_ps(input, input);
	__m128 one = _mm_set_ss(1.0F);
	__m128 w_squared = _mm_sub_ss(_mm_sub_ss(_mm_sub_ss(one, x2y2z2), _mm_shuffle_ps(x2y2z2, x2y2z2, _MM_SHUFFLE(1, 1, 1, 1))), _mm_shuffle_ps(x2y2z2, x2y2z2, _MM_SHUFFLE(2, 2, 2, 2)));
	w_squared = _mm_and_ps(w_squared, _mm_castsi128_ps(masks));
	__m128 w = _mm_sqrt_ss(w_squared);
	__m128 zzww = _mm_shuffle_ps(input, w, _MM_SHUFFLE(0, 0, 2, 2));
	return _mm_shuffle_ps(input, zzww, _MM_SHUFFLE(2, 0, 1, 0));
}
#endif

#if defined(RTM_NEON_INTRINSICS)
inline quatf RTM_SIMD_CALL quat_from_positive_w_neon(vector4f_arg0 input) RTM_NO_EXCEPT
{
	float32x4_t x2y2z2 = vmulq_f32(input, input);
	float w_squared = ((1.0F - vgetq_lane_f32(x2y2z2, 0)) - vgetq_lane_f32(x2y2z2, 1)) - vgetq_lane_f32(x2y2z2, 2);
	float w = rtm::scalar_sqrt(rtm::scalar_abs(w_squared));
	return vsetq_lane_f32(w, input, 3);
}
#endif

static void bm_quat_from_positive_w_scalar(benchmark::State& state)
{
	quatf q0 = quat_identity();

	for (auto _ : state)
		benchmark::DoNotOptimize(q0 = quat_from_positive_w_scalar(q0));
}

BENCHMARK(bm_quat_from_positive_w_scalar);

#if defined(RTM_SSE4_INTRINSICS)
static void bm_quat_from_positive_w_sse4_andnot(benchmark::State& state)
{
	quatf q0 = quat_identity();

	for (auto _ : state)
		benchmark::DoNotOptimize(q0 = quat_from_positive_w_sse4_andnot(q0));
}

BENCHMARK(bm_quat_from_positive_w_sse4_andnot);

static void bm_quat_from_positive_w_sse4_and(benchmark::State& state)
{
	quatf q0 = quat_identity();

	for (auto _ : state)
		benchmark::DoNotOptimize(q0 = quat_from_positive_w_sse4_and(q0));
}

BENCHMARK(bm_quat_from_positive_w_sse4_and);
#endif

#if defined(RTM_SSE2_INTRINSICS)
static void bm_quat_from_positive_w_sse2_and(benchmark::State& state)
{
	quatf q0 = quat_identity();

	for (auto _ : state)
		benchmark::DoNotOptimize(q0 = quat_from_positive_w_sse2_and(q0));
}

BENCHMARK(bm_quat_from_positive_w_sse2_and);

static void bm_quat_from_positive_w_sse2_and2(benchmark::State& state)
{
	quatf q0 = quat_identity();

	for (auto _ : state)
		benchmark::DoNotOptimize(q0 = quat_from_positive_w_sse2_and2(q0));
}

BENCHMARK(bm_quat_from_positive_w_sse2_and2);
#endif

#if defined(RTM_NEON_INTRINSICS)
static void bm_quat_from_positive_w_neon(benchmark::State& state)
{
	quatf q0 = quat_identity();

	for (auto _ : state)
		benchmark::DoNotOptimize(q0 = quat_from_positive_w_neon(q0));
}

BENCHMARK(bm_quat_from_positive_w_neon);
#endif
