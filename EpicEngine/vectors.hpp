#pragma once

#ifndef w32vectors
#define w32vectors

struct vec2
{
	

	vec2(const float _x, const float _y) { x = _x; y = _y; }
	vec2(const float v) { x = v; y = v; }
	vec2() { x = 0; y = 0; }

	bool operator==(const vec2 v) {
		return x == v.x && y == v.y;
	}
	void operator+=(const vec2 v) {
		x += v.x;
		y += v.y;
	}
	vec2 operator+(const vec2 v) {
		return vec2(x + v.x, y + v.y);
	}
	void operator-=(const vec2 v) {
		x -= v.x;
		y -= v.y;
	}
	vec2 operator-(const vec2 v) {
		return vec2(x - v.x, y - v.y);
	}
	float& operator[](const size_t huh) {
		return *(float*)((DWORD)&x + (huh * 4));
	}


	float x, y;
};

struct vec4
{


	vec4(const float _x, const float _y, const float _z, const float _w) { x = _x; y = _y; z = _z; w = _w; }
	vec4(const float v) { x = v; y = v; }
	vec4() { x = 0; y = 0; }

	bool operator==(const vec4 v) {
		return x == v.x && y == v.y && z == v.z && w == v.w;
	}
	void operator+=(const vec4 v) {
		x += v.x;
		y += v.y;
		z += v.z;
		w += v.w;
	}
	vec4 operator+(const vec4 v) {
		return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
	}
	void operator-=(const vec4 v) {
		x -= v.x;
		y -= v.y;
		z -= v.z;
		w -= v.w;
	}
	vec4 operator-(const vec4 v) {
		return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
	}
	float& operator[](const size_t huh) {
		return *(float*)((DWORD)&x + (huh * 4));
	}


	float x, y, z, w;
};

#endif