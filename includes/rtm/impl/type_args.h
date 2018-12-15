#pragma once

////////////////////////////////////////////////////////////////////////////////
// The MIT License (MIT)
//
// Copyright (c) 2018 Nicholas Frechette & Realtime Math contributors
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

#include "rtm/types.h"

namespace rtm
{
	//////////////////////////////////////////////////////////////////////////
	// Register passing typedefs
	//////////////////////////////////////////////////////////////////////////



#if defined(RTM_USE_VECTORCALL)
	// On x64 with __vectorcall, the first 6x vector4 arguments can be passed by value in a register, everything else afterwards is passed by const&
	using vector4f_arg0 = const vector4f;
	using vector4f_arg1 = const vector4f;
	using vector4f_arg2 = const vector4f;
	using vector4f_arg3 = const vector4f;
	using vector4f_arg4 = const vector4f;
	using vector4f_arg5 = const vector4f;
	using vector4f_argn = const vector4f&;

	using quatf_arg0 = const quatf;
	using quatf_arg1 = const quatf;
	using quatf_arg2 = const quatf;
	using quatf_arg3 = const quatf;
	using quatf_arg4 = const quatf;
	using quatf_arg5 = const quatf;
	using quatf_argn = const quatf&;

	// With __vectorcall, vector aggregates are also passed by register
	using qvvf_arg0 = const qvvf;
	using qvvf_arg1 = const qvvf;
	using qvvf_argn = const qvvf&;

	using matrix3x4f_arg0 = const matrix3x4f;
	using matrix3x4f_argn = const matrix3x4f&;
#elif defined(RTM_NEON_INTRINSICS)
	// On ARM NEON, the first 4x vector4 arguments can be passed by value in a register, everything else afterwards is passed by const&
	using vector4f_arg0 = const vector4f;
	using vector4f_arg1 = const vector4f;
	using vector4f_arg2 = const vector4f;
	using vector4f_arg3 = const vector4f;
	using vector4f_arg4 = const vector4f&;
	using vector4f_arg5 = const vector4f&;
	using vector4f_argn = const vector4f&;

	using quatf_arg0 = const quatf;
	using quatf_arg1 = const quatf;
	using quatf_arg2 = const quatf;
	using quatf_arg3 = const quatf;
	using quatf_arg4 = const quatf&;
	using quatf_arg5 = const quatf&;
	using quatf_argn = const quatf&;

	using qvvf_arg0 = const qvvf&;
	using qvvf_arg1 = const qvvf&;
	using qvvf_argn = const qvvf&;

	using matrix3x4f_arg0 = const matrix3x4f&;
	using matrix3x4f_argn = const matrix3x4f&;
#else
	// On every other platform, everything is passed by const&
	using vector4f_arg0 = const vector4f&;
	using vector4f_arg1 = const vector4f&;
	using vector4f_arg2 = const vector4f&;
	using vector4f_arg3 = const vector4f&;
	using vector4f_arg4 = const vector4f&;
	using vector4f_arg5 = const vector4f&;
	using vector4f_argn = const vector4f&;

	using quatf_arg0 = const quatf&;
	using quatf_arg1 = const quatf&;
	using quatf_arg2 = const quatf&;
	using quatf_arg3 = const quatf&;
	using quatf_arg4 = const quatf&;
	using quatf_arg5 = const quatf&;
	using quatf_argn = const quatf&;

	using qvvf_arg0 = const qvvf&;
	using qvvf_arg1 = const qvvf&;
	using qvvf_argn = const qvvf&;

	using matrix3x4f_arg0 = const matrix3x4f&;
	using matrix3x4f_argn = const matrix3x4f&;
#endif
}