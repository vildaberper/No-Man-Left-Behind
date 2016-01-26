#pragma once

#include <math.h>

#define PI 3.14159265f

namespace math{
	// Positive distance between f0 and f1
	static const float interv(const float& f0, const float& f1){
		return f0 > f1 ? f0 - f1 : f1 - f0;
	}

	// Distance 
	static const float distance(const float& x0, const float& y0, const float& x1, const float& y1){
		return sqrt(pow(x1 - x0, 2) + pow(y1 - y0, 2));
	}

	// Angle from point 0 to point 1
	static const float angleTo(const float& x0, const float& y0, const float& x1, const float& y1){
		return atan2(y1 - y0, x1 - x0) + (y1 - y0 < 0 ? 2.0f * PI : 0);
	}

	// Radians to degrees
	static const float toDeg(const float& rad){
		return rad / PI * 180.0f;
	}
}