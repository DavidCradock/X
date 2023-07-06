#pragma once
#include "PCH.h"

namespace X
{
	// Uncomment this next line to set CVector3r to use doubles instead of floats
	#define PRECISION_DOUBLE

	#ifdef PRECISION_DOUBLE

	// Sets real type as type double
	typedef double real;

	// Defines the highest value for the real number.
	#define REAL_MAX DBL_MAX

	// Defines the precision of the square root operator.
	#define real_sqrt sqrt

	// Defines the precision of the absolute magnitude operator.
	#define real_abs fabs

	// Defines the precision of the sine operator.
	#define real_sin sin

	// Defines the precision of the cosine operator.
	#define real_cos cos

	// Defines the precision of the tangent operator.
	#define real_tan tan

	// Defines the precision of the exponent operator.
	#define real_exp exp

	// Defines the precision of the power operator.
	#define real_pow pow

	// Defines the precision of the floating point modulo operator.
	#define real_fmod fmod

	// Defines PI to 15 digits
	#define R_PI 3.14159265358979

	#else

	// Sets real type as type float
	typedef float real;

	// Defines the highest value for the real number.
	#define REAL_MAX FLT_MAX

	// Defines the precision of the square root operator.
	#define real_sqrt sqrtf

	// Defines the precision of the absolute magnitude operator.
	#define real_abs fabsf

	// Defines the precision of the sine operator.
	#define real_sin sinf

	// Defines the precision of the cosine operator.
	#define real_cos cosf

	// Defines the precision of the tangent operator.
	#define real_tan tanf

	// Defines the precision of the exponent operator.
	#define real_exp expf

	// Defines the precision of the power operator.
	#define real_pow powf

	// Defines the precision of the floating point modulo operator.
	#define real_fmod fmodf

	// Defines PI to 6 digits
	#define R_PI 3.141592f

	#endif

	

}