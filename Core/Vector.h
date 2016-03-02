#pragma once

#include <vector>

#include "SFMLI.h"
#include "MathHelper.h"

enum Direction{
	YN,
	YP,
	XN,
	XP,
	ZERO
};

class Vector{
public:
	float x;
	float y;

	Vector(const float& x, const float& y){
		Vector::x = x;
		Vector::y = y;
	}

	Vector(const std::vector<float>& fs){
		Vector::x = fs.size() > 0 ? fs[0] : 0.0f;
		Vector::y = fs.size() > 1 ? fs[1] : 0.0f;
	}

	Vector(){
		x = 0;
		y = 0;
	}

	~Vector(){
	
	}

	const std::vector<float> fv(){
		std::vector<float> fs;
		fs.push_back(x);
		fs.push_back(y);
		return fs;
	}

	const float length(){
		return math::distance(x, y, 0, 0);
	}

	const float angle(){
		return math::angleTo(0, 0, x, y);
	}

	const Direction direction(){
		if (length() == 0.0f){
			return ZERO;
		}

		float a = math::toDeg(angle());

		if (a < 45.0f || a > 315.0f){
			return XP;
		}
		else if (a <= 135.0f && a >= 45.0f){
			return YP;
		}
		else if (a > 135.0f && a < 225.0f){
			return XN;
		}
		else{
			return YN;
		}
	}

	const Vector norm(){
		float l = length();
		return (*this) / (l != 0.0f ? l : 1.0f);
	}

	void operator=(const sf::Vector2f& sv){
		x = sv.x;
		y = sv.y;
	}

	void operator=(const sf::Vector2i& iv){
		x = float(iv.x);
		y = float(iv.y);
	}

	bool operator==(const Vector& v){
		return x == v.x && y == v.y;
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
		x = x * f;
		y = y * f;
	}

	const Vector operator/(const float& f){
		return Vector(x / f, y / f);
	}

	void operator/=(const float& f){
		x = x / f;
		y = y / f;
	}

	const Vector operator +(const Vector& v){
		return Vector(x + v.x, y + v.y);
	}

	void operator +=(const Vector& v){
		x += v.x;
		y += v.y;
	}

	const Vector operator -(const Vector& v){
		return Vector(x - v.x, y - v.y);
	}

	void operator -=(const Vector& v){
		x -= v.x;
		y -= v.y;
	}
};