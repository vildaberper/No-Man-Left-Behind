#pragma once

#include "MathHelper.h"

class Vector{
public:
	float x;
	float y;

	Vector(const float& x, const float& y){
		Vector::x = x;
		Vector::y = y;
	}

	Vector(){
		Vector::x = 0;
		Vector::y = 0;
	}

	~Vector(){ }

	const float length(){
		return math::distance(x, y, 0, 0);
	}

	const float angle(){
		return math::angleTo(0, 0, x, y);
	}

	const Vector norm(){
		return (*this) / length();
	}

	void operator()(const float& x, const float& y){
		Vector::x = x;
		Vector::y = y;
	}

	float& operator[](const unsigned char& i){
		return i == 0 ? x : y;
	}

	const Vector operator*(const float& f){
		return Vector(x * f, y * f);
	}

	void operator*=(const float& f){
		Vector::x = x * f;
		Vector::y = y * f;
	}

	const Vector operator/(const float& f){
		return Vector(x / f, y / f);
	}

	void operator/=(const float& f){
		Vector::x = x / f;
		Vector::y = y / f;
	}
};